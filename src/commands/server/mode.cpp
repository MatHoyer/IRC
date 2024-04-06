#include "../CommandManager.h"
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

struct Mode
{
    char mode;
    bool set;
};

int getMaxClient(std::string const &maxClientStr)
{
    char *endPtr;

    long maxClient = std::strtol(maxClientStr.c_str(), &endPtr, 10);
    if (endPtr == maxClientStr.c_str() || *endPtr != '\0')
        return 0;
    if ((errno == ERANGE &&
         (maxClient == std::numeric_limits<long>::max() || maxClient == std::numeric_limits<long>::min())) ||
        maxClient > std::numeric_limits<int>::max() || maxClient < std::numeric_limits<int>::min())
    {
        return 0;
    }

    if (maxClient < 0)
        maxClient = 0;
    else if (maxClient > 9999)
        maxClient = 9999;

    return static_cast<int>(maxClient);
}

std::vector<Mode> parseModeStr(std::string const &modeStr)
{
    std::vector<Mode> modes;
    bool inMode = false;
    Mode mode;
    for (std::string::const_iterator it = modeStr.begin(); it != modeStr.end(); ++it)
    {
        if (!inMode)
            mode = Mode();
        if (!inMode && (*it == '+' || *it == '-'))
        {
            mode.set = *it == '+';
            inMode = true;
            continue;
        }
        mode.mode = *it;
        modes.push_back(mode);
        inMode = false;
    }

    return modes;
}

std::vector<std::string> handleUserModes(Client *client, ClientManager *cManager, Message &msg)
{
    std::vector<std::string> ret;
    std::string const USER_MODES = "+-io";

    if (!cManager->nickExists(msg.params[0]))
        ret.push_back(SERVER_NAME + ERR_NOSUCHNICK(client->nick, msg.params[0]));
    else if (!cManager->isSameClient(client, msg.params[0]))
        ret.push_back(SERVER_NAME + ERR_USERSDONTMATCH(client->nick));
    else if (msg.params.size() == 1)
        ret.push_back(SERVER_NAME + RPL_UMODEIS(client->nick, client->modesToStr()));
    else
    {
        std::vector<Mode> modes = parseModeStr(msg.params[1]);
        for (std::vector<Mode>::const_iterator it = modes.begin(); it != modes.end(); ++it)
        {
            if (USER_MODES.find(it->mode) == std::string::npos)
            {
                ret.push_back(SERVER_NAME + ERR_UMODEUNKNOWNFLAG(client->nick));
                continue;
            }
            else if (it->mode == 'o' && (msg.params.size() < 2 || msg.params[2].empty()))
                ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
            else if (it->set)
                client->addMode(client->getMode(it->mode));
            else
                client->removeMode(client->getMode(it->mode));
        }
        ret.push_back(SERVER_NAME + RPL_UMODEIS(client->nick, client->modesToStr()));
    }
    return ret;
}

std::vector<std::string> handleChannelModes(ChannelManager *chManager, Client *client, ClientManager *cManager,
                                            Message &msg)
{
    std::string const CHANNEL_MODES = "+-itkol";
    std::vector<std::string> ret;
    bool changed = false;
    if (!chManager->channelExists(msg.params[0]))
        ret.push_back(SERVER_NAME + ERR_NOSUCHCHANNEL(client->nick, msg.params[0]));

    else if (msg.params.size() == 1)
    {
        Channel *chan = chManager->get(msg.params[0]);
        ret.push_back(SERVER_NAME + RPL_CHANNELMODEIS(client->nick, msg.params[0], chan->modesToStr()));
    }
    else
    {
        Channel *chan = chManager->get(msg.params[0]);
        if (!chan->isClientOnChannel(client))
            ret.push_back(SERVER_NAME + ERR_NOTONCHANNEL(client->nick, msg.params[0]));
        else if (!chan->isOperator(client))
        {
            if (msg.params.size() < 3 && msg.params[1] == "b")
                ;
            else
                ret.push_back(SERVER_NAME + ERR_CHANOPRIVSNEEDED(client->nick, msg.params[0]));
        }
        else
        {
            std::vector<Mode> modes = parseModeStr(msg.params[1]);
            for (std::vector<Mode>::const_iterator it = modes.begin(); it != modes.end(); ++it)
            {
                if (CHANNEL_MODES.find(it->mode) == std::string::npos)
                {
                    ret.push_back(SERVER_NAME + ERR_UNKNOWNMODE(client->nick, std::string(1, it->mode)));
                    continue;
                }
                if (it->mode == 'k') // key
                {
                    if (it->set)
                    {
                        if (msg.params.size() < 3 || msg.params[2].empty())
                        {
                            ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
                            continue;
                        }
                        chan->addMode(chan->getMode(it->mode));
                        chan->password = msg.params[2];
                    }
                    else
                    {
                        chan->removeMode(chan->getMode(it->mode));
                        chan->password = "";
                    }
                }
                else if (it->mode == 'o') // operator
                {
                    if (it->set)
                    {
                        if (msg.params.size() < 3 || msg.params[2].empty())
                        {
                            ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
                            continue;
                        }
                        Client *target = cManager->getByNick(msg.params[2]);
                        if (!target)
                        {
                            ret.push_back(SERVER_NAME + ERR_NOSUCHNICK(client->nick, msg.params[2]));
                            continue;
                        }
                        chan->addMode(chan->getMode(it->mode));
                        chan->addOperator(target);
                        target->setRoleInChannel(chan->name, Client::OPERATOR);
                    }
                    else
                    {
                        Client *target = cManager->getByNick(msg.params[2]);
                        if (!target)
                        {
                            ret.push_back(SERVER_NAME + ERR_NOSUCHNICK(client->nick, msg.params[2]));
                            continue;
                        }
                        chan->removeMode(chan->getMode(it->mode));
                        chan->removeOperator(target);
                        target->setRoleInChannel(chan->name, Client::VOICE);
                    }
                }
                else if (it->mode == 'l')
                {
                    if (it->set)
                    {
                        if (msg.params.size() < 3 || msg.params[2].empty() || !std::isdigit(msg.params[2][0]))
                        {
                            ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
                            continue;
                        }
                        chan->addMode(chan->getMode(it->mode));
                        chan->maxClient = getMaxClient(msg.params[2]);
                    }
                    else
                    {
                        chan->removeMode(chan->getMode(it->mode));
                        chan->maxClient = CHANNEL_MAX_USER;
                    }
                }
                else
                {
                    if (it->set)
                        chan->addMode(chan->getMode(it->mode));
                    else
                        chan->removeMode(chan->getMode(it->mode));
                }
                changed = true;
            }
        }
        if (changed)
        {
            std::string extra;
            extra = chan->modesToStr();
            if (extra.size() == 1)
                extra = "";
            if (msg.params.size() > 2)
                extra += " :" + msg.params[2];
            chan->broadcast(SERVER_NAME + RPL_CHANNELMODEIS(client->nick, msg.params[0], extra), client, true);
        }
    }

    return ret;
}

std::vector<std::string> mode(Message &msg, Client *client, ClientManager *cManager, ChannelManager *chManager)
{
    std::vector<std::string> ret;
    if (msg.params.empty() || msg.params[0].empty())
        ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, "MODE"));
    else if (msg.params[0][0] != '#')
        return handleUserModes(client, cManager, msg);
    else if (msg.params[0][0] == '#')
        return handleChannelModes(chManager, client, cManager, msg);
    return ret;
}
