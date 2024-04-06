#include "CommandManager.h"
#include "../client/ClientManager.h"
#include "../client/Bot.h"

CommandManager::CommandManager(ChannelManager *cManager, ClientManager *uManager, std::string const &pwd)
    : cManager_(cManager), uManager_(uManager), pwd_(pwd)
{
}

CommandManager::~CommandManager()
{
}

std::string toString(int nb)
{
    std::stringstream ss;
    ss << nb;
    std::string str = ss.str();
    return str;
}

void CommandManager::sendIsupport(Client *sender)
{
    std::string buf;
    buf.append(" CHANNELLEN=" + toString(CHANNELLEN));
    buf.append(" NICKLEN=" + toString(NICKLEN));
    buf.append(" USERLEN=" + toString(USERLEN));
    sender->send(SERVER_NAME + RPL_ISUPPORT(sender->nick, buf));
}

void CommandManager::sendMotd(Client *sender)
{
    sender->send(motd(LINE1, sender));
    sender->send(motd(LINE2, sender));
    sender->send(motd(LINE3, sender));
    sender->send(motd(LINE4, sender));
    sender->send(motd(LINE5, sender));
    sender->send(motd(LINE6, sender));
    sender->send(motd(LINE7, sender));
    sender->send(motd(LINE8, sender));
    sender->send(motd(LINE9, sender));
    sender->send(motd(LINE10, sender));
    sender->send(motd(LINE11, sender));
}

void CommandManager::doCommands(std::deque<Message> &msgs, Client *sender)
{
    while (!msgs.empty())
    {
        if (msgs.front().verb.compare("CAP") == 0)
            sender->send(cap(msgs.front(), sender));
        else if (sender->isCapSent() && msgs.front().verb.compare("PASS") == 0)
            sender->send(pass(msgs.front(), sender, pwd_));
        else if (sender->isCapSent() && sender->isPassSent() && msgs.front().verb.compare("NICK") == 0)
            sender->send(nick(msgs.front(), sender, uManager_, cManager_));
        else if (sender->isCapSent() && sender->isPassSent() && msgs.front().verb.compare("USER") == 0)
            sender->send(user(msgs.front(), sender));
        else if (sender->isRegistered())
        {
            bool isAnswerBot = doBotThings(msgs.front(), sender, cManager_);
            if (msgs.front().verb.compare("MOTD") == 0)
                sendMotd(sender);
            else if (msgs.front().verb.compare("PING") == 0)
                sender->send(ping(msgs.front(), sender));
            else if (msgs.front().verb.compare("JOIN") == 0)
                sender->sendMany(join(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("PART") == 0)
                sender->sendMany(part(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("PRIVMSG") == 0 && isAnswerBot == false)
                sender->sendMany(privmsg(msgs.front(), sender, uManager_, cManager_));
            else if (msgs.front().verb.compare("QUIT") == 0)
                sender->send(quit(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("MODE") == 0)
                sender->sendMany(mode(msgs.front(), sender, uManager_, cManager_));
            else if (msgs.front().verb.compare("TOPIC") == 0)
                sender->send(topic(msgs.front(), sender, cManager_));
            else if (msgs.front().verb.compare("INVITE") == 0)
                sender->send(invite(msgs.front(), sender, uManager_, cManager_));
            else if (msgs.front().verb.compare("KICK") == 0)
                sender->send(kick(msgs.front(), sender, uManager_, cManager_));
        }
        else
        {
            sender->setShouldDisconnect(true);
        }
        msgs.pop_front();
        if (!sender->isRegistered() && sender->isCapSent() && sender->isPassSent() && !sender->nick.empty() && !sender->username.empty())
        {
            sender->setRegistered(true);
            sendIsupport(sender);
            sender->send(SERVER_NAME + RPL_WELCOME(sender->nick, sender->nickmask));
            sendMotd(sender);
        }
    }
}
