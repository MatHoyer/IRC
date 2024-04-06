#include "../CommandManager.h"

std::string topic(Message &msg, Client *user, ChannelManager *cManager)
{
    if (msg.params.empty())
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, msg.verb);
    if (cManager->get(*(msg.params.begin())) == NULL)
        return SERVER_NAME + ERR_NOSUCHCHANNEL(user->nick, *(msg.params.begin()));
    Channel *chan = cManager->get(*(msg.params.begin()));
    if (chan->isClientOnChannel(user) == false)
        return SERVER_NAME + ERR_NOTONCHANNEL(user->nick, chan->name);
    if (msg.trailling.empty())
    {
        if (msg.isEmptyTrailling)
        {
            if (chan->hasMode(Channel::PROTECTED_TOPIC))
            {
                if (chan->isOperator(user))
                    chan->topic = "";
                else
                    return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, chan->name);
            }
            else
                chan->topic = "";
        }
        else
        {
            if (chan->topic.empty())
                return RPL_NOTOPIC(user->nick, chan->name);
            else
                return RPL_TOPIC(user->nick, chan->name, chan->topic);
        }
    }
    else
    {
        if (chan->hasMode(Channel::PROTECTED_TOPIC))
        {
            if (chan->isOperator(user))
                chan->topic = msg.trailling;
            else
                return SERVER_NAME + ERR_CHANOPRIVSNEEDED(user->nick, chan->name);
        }
        else
            chan->topic = msg.trailling;
    }
	chan->broadcast(":" + user->nickmask + " TOPIC " + chan->name + " " + chan->topic, user, true);
    return "";
}
