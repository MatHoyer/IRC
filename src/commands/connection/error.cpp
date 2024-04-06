#include "../CommandManager.h"

std::string error(Message &msg, Client *user)
{
    (void)user;
    return "ERROR :" + msg.trailling;
}
