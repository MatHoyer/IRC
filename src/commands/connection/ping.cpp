#include "../../ft_irc.h"
#include "../CommandManager.h"

std::string ping(Message &msg, Client *user)
{
    if (msg.params.empty())
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, "PING");
    return pong(msg.params[0]);
}
