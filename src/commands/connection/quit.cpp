#include "../CommandManager.h"

std::string quit(Message &msg, Client *user, ChannelManager *chanManager)
{
    std::map<std::string, Channel *> channels = chanManager->getAll();
    std::vector<Client *> alreadyKnow;
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second->isClientOnChannel(user))
            it->second->broadcastUnique(":" + user->nickmask + " QUIT " + msg.trailling, user, alreadyKnow);
    }
    std::vector<Client *> privmsg = user->getPrivmsg();
    for (std::vector<Client *>::iterator it = privmsg.begin(); it != privmsg.end(); it++)
    {
        if (isInAlreadyKnow((*it)->nick, alreadyKnow) == false)
            (*it)->send(":" + user->nickmask + " QUIT " + msg.trailling);
        (*it)->removePrivmsg(user->nick);
        user->removePrivmsg((*it)->nick);
    }
    user->setShouldDisconnect(true);
    return error(msg, user);
}
