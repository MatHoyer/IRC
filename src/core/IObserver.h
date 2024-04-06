#pragma once
#include <sys/epoll.h>

class IObserver
{
  public:
    virtual ~IObserver()
    {
    }
    virtual void update(int fd, int event) = 0;
};
