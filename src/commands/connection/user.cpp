#include "../CommandManager.h"

std::string user(Message &msg, Client *user)
{
    if (!user->username.empty() && !user->realname.empty())
        return SERVER_NAME + ERR_ALREADYREGISTERED("");
    if (!msg.params.empty() && !msg.trailling.empty())
    {
        user->username = msg.params[0];
        user->realname = msg.trailling;
        user->updateNickmask();
        return "";
    }
    return SERVER_NAME + ERR_NEEDMOREPARAMS("", msg.verb);
}
