#pragma once

#include <map>
#include <sys/epoll.h>
#include <vector>

#include <iostream>
#include <utility>

#include "Client.h"

class ClientManager
{

  public:
    typedef std::map<int, Client *>::iterator ClientMapIterator;
    typedef std::vector<epoll_event>::iterator EventsIterator;

    ClientManager();
    ~ClientManager();
    int getClientCount();
    std::map<int, Client *> &getClients();
    Client *get(int fd);
    Client *getByNick(std::string const &nick);
    Client *create(int fd, IObserver *observer);
    void remove(int fd);
    void removeAll();
    bool nickExists(std::string const &nick);
    bool isSameClient(Client *client, std::string const &nick);

  private:
    ClientManager &operator=(ClientManager const &rhs);
    ClientManager(ClientManager const &other);
    std::map<int, Client *> clients_;
};
