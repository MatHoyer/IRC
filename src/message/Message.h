#pragma once

#include <sstream>
#include <string>
#include <deque>

struct Message
{
    Message(std::string line);
    Message operator=(Message const &other);
    std::string verb;
    std::deque<std::string> params;
    std::string trailling;
    bool isEmptyTrailling;
};
