#include "../CommandManager.h"

std::string pass(Message &msg, Client *user, std::string &serverPassword)
{
    if (user->isRegistered())
        return SERVER_NAME + ERR_ALREADYREGISTERED(user->nick);
    if (msg.params.empty())
        return SERVER_NAME + ERR_NEEDMOREPARAMS(user->nick, "PASS");
    if (msg.params[0].compare(serverPassword) != 0)
    {
        user->setShouldDisconnect(true);
        return SERVER_NAME + ERR_PASSWDMISMATCH(user->nick);
    }
    user->setPassSent(true);
    user->setShouldDisconnect(false);
    return "";
}
