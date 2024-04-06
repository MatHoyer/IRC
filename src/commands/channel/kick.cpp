#include "../CommandManager.h"

std::string kick(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
    if (msg.params.size() < 2)
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.verb);
    std::string channelName = msg.params.front();
    msg.params.pop_front();
    std::string nickname = msg.params.front();
    std::string comment = msg.trailling;
    if (comment.empty())
        comment = "for no reason";
    if (cManager->channelExists(channelName) == false)
        return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, channelName);
    Channel *channel = cManager->get(channelName);
    if (channel->isOperator(user) == false)
        return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, channelName);
    if (uManager->nickExists(nickname) == false)
        return SERVER_NAME + ERR_NOSUCHNICK(user->nick, nickname);
    Client *toKick = uManager->getByNick(nickname);
    if (user->nick.compare(nickname) == 0)
        return "";
    if (channel->isClientOnChannel(toKick) == false)
        return SERVER_NAME + ERR_NOTONCHANNEL(nickname, channelName);
    channel->broadcast(":" + user->nickmask + " KICK " + channelName + " " + nickname + " " + comment, user, true);
    channel->removeClient(toKick);
    return "";
}
