#include "../CommandManager.h"
#include <deque>
#include <vector>

bool isErrChar(std::string &nickname)
{
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") !=
        std::string::npos)
        return true;
    return false;
}

std::string nick(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
    std::string ret = "";
    if (msg.params.empty())
        return SERVER_NAME + ERR_NONICKNAMEGIVEN("");
    else if (msg.params.front().length() > NICKLEN)
        msg.params.front().erase(NICKLEN, std::string::npos);
    if (uManager->nickExists(msg.params.front()) || msg.params.front().compare(BOT_NAME) == 0)
        return SERVER_NAME + ERR_NICKNAMEINUSE(msg.params.front(), msg.params.front());
    if (isErrChar(msg.params.front()))
        return SERVER_NAME + ERR_ERRONEUSNICKNAME(user->nick, msg.params.front());
    std::map<std::string, Channel *> channels = cManager->getAll();
    ret = ":" + user->nickmask + " NICK " + msg.params.front();
    std::vector<Client *> alreadyKnow;
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second->isClientOnChannel(user))
            it->second->broadcastUnique(ret, user, alreadyKnow);
    }
    std::vector<Client *> privmsg = user->getPrivmsg();
    for (std::vector<Client *>::iterator it = privmsg.begin(); it != privmsg.end(); it++)
    {
        if (isInAlreadyKnow((*it)->nick, alreadyKnow) == false)
            (*it)->send(ret);
    }
    std::string oldNick(user->nick);
    user->nick = msg.params.front();
    user->updateNickmask();
    for (std::vector<Client *>::iterator it = privmsg.begin(); it != privmsg.end(); it++)
        (*it)->updatePrivmsg(oldNick, user);
    return ret;
}
