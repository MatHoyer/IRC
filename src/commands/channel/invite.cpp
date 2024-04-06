#include "../CommandManager.h"

std::string invite(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager)
{
	if (msg.params.size() < 2)
		return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.verb);
	std::string nickToInvite = msg.params.front();
	msg.params.pop_front();
	std::string channelToInvite = msg.params.front();
	if (cManager->channelExists(channelToInvite) == false)
		return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, channelToInvite);
	Channel *channel = cManager->get(channelToInvite);
	if (channel->isClientOnChannel(user) == false)
		return SERVER_NAME + ERR_NOTONCHANNEL(user->nick, channelToInvite);
	Client *client = uManager->getByNick(nickToInvite);
	if (channel->hasMode(Channel::INVITE_ONLY) == true && channel->isOperator(user) == false)
		return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, channelToInvite);
	if (channel->isClientOnChannel(client))
		return SERVER_NAME + ERR_USERONCHANNEL(user->nick, nickToInvite, channelToInvite);
	if (channel->isOnInviteList(client) == false)
		channel->addInvite(client);
	client->send(SERVER_NAME + RPL_INVITING(user->nick, nickToInvite, channelToInvite));
	return "";
}
