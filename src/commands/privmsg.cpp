#include "CommandManager.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <sys/socket.h>

std::string getSendStr(Message &msg)
{
    std::string filename = msg.params[1];
    std::string ip = msg.params[2];
    int port = 0;
    if (msg.params.size() > 3)
    {
        port = std::atoi(msg.params[3].c_str());
        if (port < 1024 || port > 65535)
            return "";
        std::stringstream ss;
        ss << ":DCC SEND" << filename << inet_addr(ip.c_str()) << ntohs(port);
        return ss.str();
    }
    return "";
}

std::vector<std::string> privmsg(Message msg, Client *sender, ClientManager *uManager, ChannelManager *cManager)
{
    std::vector<std::string> ret;
    if (msg.params.empty())
    {
        ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(sender->nick, msg.verb));
        return ret;
    }
    while (!msg.params.empty())
    {
        if (msg.trailling.find(":DCC SEND") != std::string::npos)
        {
            std::string sendStr = getSendStr(msg);
            if (sendStr.empty())
            {
                ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(sender->nick, msg.verb));
                return ret;
            }
            sender->send(sendStr);
            msg.params.pop_front();
            continue;
        }
        std::string destName = msg.params.front();
        std::string userMessage = msg.trailling;
        if (*(msg.params.front().begin()) == '#')
        {
            if (cManager->get(destName) == NULL)
                ret.push_back(SERVER_NAME + ERR_NOSUCHCHANNEL(sender->nick, msg.params.front()));
            else if (cManager->get(destName)->isClientOnChannel(sender) == true)
            {
                Channel *channel = cManager->get(destName);
                std::string message = ":" + sender->RolePrefixToString(sender->getRoleInChannel(channel->name)) +
                                      sender->nickmask + " PRIVMSG " + channel->name + " :" + userMessage;
                channel->broadcast(message, sender);
            }
            else
                ret.push_back(SERVER_NAME + ERR_CANNOTSENDTOCHAN(sender->nick, msg.params.front()));
        }
        else
        {
            bool isFind = false;
            std::map<int, Client *> clients = uManager->getClients();
            for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                if (it->second->nick.compare(destName) == 0 && it->second->nick.compare(BOT_NAME))
                {
                    isFind = true;
                    if (sender->nick.compare(BOT_NAME) && sender->isInPrivmsg(it->second->nick) == false)
                        sender->addPrivmsg(it->second);
                    if (sender->nick.compare(BOT_NAME) && it->second->isInPrivmsg(sender->nick) == false)
                        it->second->addPrivmsg(sender);
                    std::string message =
                        ":" + sender->nickmask + " PRIVMSG " + it->second->nickmask + " :" + userMessage;
                    it->second->send(message);
                }
            }
            if (isFind == false)
                ret.push_back(SERVER_NAME + ERR_NOSUCHNICK(sender->nick, destName));
        }
        msg.params.pop_front();
    }
    return ret;
}
