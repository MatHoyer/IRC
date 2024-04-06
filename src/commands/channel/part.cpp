#include "../../utils/utils.h"
#include "../CommandManager.h"

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> part(Message &msg, Client *client, ChannelManager *cManager)
{
    std::vector<std::string> ret;
    std::map<std::string, Channel *> channels;
    std::string broadcastMessage;
    std::vector<std::string> userList;
    Channel *channel;

    if (msg.params.empty())
    {
        ret.push_back(SERVER_NAME + ERR_NEEDMOREPARAMS(client->nick, msg.verb));
        return ret;
    }

    channels = cManager->getAll();

    for (std::deque<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        std::string broadcastMessage = ":" + client->nickmask + " PART " + *it;
        if (cManager->channelExists(*it))
        {
            channel = cManager->get(*it);
            if (channel->isClientOnChannel(client))
            {
                channel->removeClient(client);
                if (channel->getClients().empty())
                    cManager->remove(channel->name);
                else
                    channel->broadcast(broadcastMessage, client);
                ret.push_back(broadcastMessage);
            }
            else
                ret.push_back(ERR_NOTONCHANNEL(client->nick, channel->name));
        }
        else
            ret.push_back(ERR_NOSUCHCHANNEL(client->nick, *it));
    }
    return ret;
}
