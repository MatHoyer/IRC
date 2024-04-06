#include "../../utils/utils.h"
#include "../CommandManager.h"

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// clang-format off
typedef std::vector<std::pair<std::string, std::string> > PairChannelNameAndPassword;
// clang-format on
static std::string errorCheck(std::string const &channelName, std::string const &nick,
                              std::string const &channelPassword)
{
    if (channelName[0] != '#' && channelName[0] != '&')
        return SERVER_NAME + ERR_NOSUCHCHANNEL(nick, channelName);

    if (channelName.length() <= 1 || channelName.length() > CHANNELLEN)
        return SERVER_NAME + ERR_NOSUCHCHANNEL(nick, channelName);

    if (channelName.find_first_of(", \a") != std::string::npos)
        return SERVER_NAME + ERR_NOSUCHCHANNEL(nick, channelName);

    if (channelPassword.find_first_of(", \a") != std::string::npos)
        return SERVER_NAME + ERR_INVALIDKEY(nick, channelName);
    return "";
}

std::vector<std::string> getUserList(Channel *channel, Client *client)
{
    std::vector<std::string> ret;
    std::map<int, Client *> clients = channel->getClients();
    std::vector<std::string> nicks;
    int i = 0;
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        nicks.push_back(it->second->RolePrefixToString(it->second->getRoleInChannel(channel->name)) + it->second->nick);
        if ((i + 1) % 10 == 0)
        {
            ret.push_back(RPL_NAMREPLY(client->nick, channel->name, nicks));
            nicks.clear();
        }
        i++;
    }
    if (nicks.size() > 0)
        ret.push_back(RPL_NAMREPLY(client->nick, channel->name, nicks));
    return ret;
}

Channel::Type getChannelType(std::string const &channelName)
{
    if (channelName[0] == '#')
        return Channel::REGULAR;
    else
        return Channel::LOCAL;
}

PairChannelNameAndPassword extractNameAndPass(Message &msg)
{
    bool isName = true;
    PairChannelNameAndPassword ret;
    for (std::deque<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        if (*it == "0" || *it == "1")
            continue;
        if (isName)
        {
            ret.push_back(std::make_pair(*it, ""));
            isName = false;
        }
        else
        {
            ret.back().second = *it;
            isName = true;
        }
    }
    return ret;
}

std::vector<std::string> join(Message &msg, Client *client, ChannelManager *cManager)
{
    std::vector<std::string> ret;
    std::map<std::string, Channel *> channels;
    PairChannelNameAndPassword nameAndPassword;
    std::string broadcastMessage, errMessage;
    std::vector<std::string> userList;
    Channel *newChannel, *channel;

    if (msg.params.empty())
    {
        ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, msg.verb));
        return ret;
    }
    nameAndPassword.reserve(msg.params.size());
    channels = cManager->getAll();
    nameAndPassword = extractNameAndPass(msg);
    for (PairChannelNameAndPassword::iterator it = nameAndPassword.begin(); it != nameAndPassword.end(); ++it)
    {
        std::string broadcastMessage = ":" + client->nickmask + " JOIN " + it->first;
        std::string errMessage = errorCheck(it->first, client->nick, it->second);
        if (!errMessage.empty())
        {
            ret.push_back(errMessage);
            continue;
        }
        if (channels.find(it->first) == channels.end()) // create new channel
        {
            newChannel = cManager->create(it->first, getChannelType(it->first));
            if (!it->second.empty())
                newChannel->password = it->second;
            newChannel->addClient(client);
            newChannel->addOperator(client);
            client->setRoleInChannel(newChannel->name, Client::OPERATOR);
            newChannel->broadcast(broadcastMessage, client, true);
            if (!newChannel->topic.empty())
                ret.push_back(RPL_TOPIC(client->nick, newChannel->name, newChannel->topic));
            std::vector<std::string> userList = getUserList(newChannel, client);
            for (std::vector<std::string>::iterator it = userList.begin(); it != userList.end(); ++it)
                ret.push_back(*it);
            ret.push_back(RPL_ENDOFNAMES(client->nick, newChannel->name));
        }
        else // join existing channel
        {
            channel = channels.find(it->first)->second;
            if (!channel)
                ret.push_back(SERVER_NAME + ERR_NOSUCHCHANNEL(client->nick, it->first));
            else if (channel->hasMode(Channel::CLIENT_LIMIT) && channel->getClientCount() >= channel->maxClient)
                ret.push_back(SERVER_NAME + ERR_CHANNELISFULL(client->nick, channel->name));
            else if (channel->hasMode(Channel::INVITE_ONLY) && !channel->isOnInviteList(client))
                ret.push_back(SERVER_NAME + ERR_INVITEONLYCHAN(client->nick, channel->name));
            else if (channel->hasMode(Channel::KEY) && channel->password != it->second)
                ret.push_back(SERVER_NAME + ERR_BADCHANNELKEY(client->nick, channel->name));
            else
            {
                client->setRoleInChannel(channel->name, Client::VOICE);
                channel->addClient(client);
                channel->removeInvite(client);
                channel->broadcast(broadcastMessage, client, true);
                if (!channel->topic.empty())
                    ret.push_back(RPL_TOPIC(client->nick, channel->name, channel->topic));
                std::vector<std::string> userList = getUserList(channel, client);
                for (std::vector<std::string>::iterator it = userList.begin(); it != userList.end(); ++it)
                    ret.push_back(*it);
                ret.push_back(RPL_ENDOFNAMES(client->nick, channel->name));
            }
        }
    }
    return ret;
}
