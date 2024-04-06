#include "Bot.h"

Bot::Bot()
{
    nick = BOT_NAME;
    username = BOT_NAME;
    realname = "ROBOT";
    updateNickmask();
}

Bot::~Bot()
{
}

void Bot::updateNickmask() 
{
    nickmask = nick + "!" + username + "@localhost";
}
