#include "../client/Bot.h"
#include "CommandManager.h"

bool doBotThings(Message msg, Client *user, ChannelManager *cManager)
{
    bool isAction = false;
    while (!msg.params.empty())
    {
        if (msg.verb.compare("PRIVMSG") == 0 && *(msg.params.front().begin()) == '#' &&
            msg.trailling.compare("!help") == 0)
        {
            std::string channelName = msg.params.front();
			if (cManager->get(channelName) == NULL)
				return false;
            if (cManager->get(channelName)->isClientOnChannel(user) == false)
                return false;
            Bot bot;
            std::string prefix(":" + bot.nickmask + " PRIVMSG " + user->nickmask + " :");
            std::vector<std::string> botMessages;
            botMessages.push_back(prefix + "Of course I can help you " + user->nick + " !" + CRLF);
            botMessages.push_back(prefix + "CHANNEL COMMAND on irssi for channel: " + channelName + CRLF);
            if (cManager->get(channelName)->isOperator(user))
            {
                    botMessages.push_back(std::string(prefix + "") + CRLF);
                    botMessages.push_back(std::string(prefix + "As an operator you can do:") + CRLF);
                    botMessages.push_back(std::string(prefix + "/mode <mod> => to change the mode of the channel (l i k t o)") + CRLF);
                    botMessages.push_back(std::string(prefix + "/invite <nickToInvite> => to invite someone if the channel is on invite mode") + CRLF);
                    botMessages.push_back(std::string(prefix + "/topic <newTopic> => to set the channel topic") + CRLF);
                    botMessages.push_back(std::string(prefix + "/kick <user> => to kick an user from the channel") + CRLF);
            }
            botMessages.push_back(std::string(prefix + "") + CRLF);
            botMessages.push_back(std::string(prefix + "As a normal user you can do:") + CRLF);
            botMessages.push_back(std::string(prefix + "/topic => to see the channel topic") + CRLF);
            botMessages.push_back(std::string(prefix + "/names => to see users in the channel") + CRLF);
            botMessages.push_back(std::string(prefix + "/part => to leave this channel") + CRLF);
            user->sendMany(botMessages);
            isAction = true;
        }
        msg.params.pop_front();
    }
    return isAction;
}

