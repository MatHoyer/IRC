#include "../CommandManager.h"

std::string cap(Message &msg, Client *user)
{
    (void)msg;
    user->setCapSent(true);
    return "";
}
