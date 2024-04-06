#pragma once

#include "../ft_irc.h"
#include "Client.h"

class Bot
{
  public:
    Bot();
    ~Bot();

    std::string nick;
    std::string username;
    std::string realname;
    std::string nickmask;

    void updateNickmask();
};
