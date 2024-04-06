#pragma once

#include "../message/Message.h"
#include <iostream>
#include <sstream>
#include <string>
#include <deque>

class Buffer
{
  public:
    static void bufferToMessage(std::string &recvBuffer, std::deque<Message> &msgs);

  private:
    Buffer();
    ~Buffer();
};
