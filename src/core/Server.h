#pragma once

#include "../buffer/Buffer.h"
#include "../channel/ChannelManager.h"
#include "../client/Client.h"
#include "../client/ClientManager.h"
#include "../commands/CommandManager.h"
#include "../ft_irc.h"
#include "IObserver.h"
#include "Logger.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define MAX_LISTENER 64
#define MAX_EVENTS 512

class Server : public IObserver
{

    typedef std::vector<pollfd>::iterator PollFdsIterator;

  public:
    Server(std::string port, std::string password);
    ~Server();

    void start();
    void setLogger(Logger &logger);
    virtual void update(int fd, int event);

  private:
    Server(Server const &other);
    Server &operator=(Server const &rhs);

    void *getInAddr_(struct sockaddr *sa);
    void initListener();

    int listenForConnection();
    void acceptConnection();

    void removeFromPolling(int fd);
    void addToPolling(int fd);

    void sendData(struct pollfd &event);
    void recvData(struct pollfd &event, CommandManager &commands);

    bool hasCRLF(std::string &buffer);

    void disconnectClients();

    std::string const port_;
    std::string const password_;

    Logger logger_;

    ClientManager uManager_;
    ChannelManager cManager_;

    int epollfd_;
    std::vector<pollfd> poller_;

    int listener_;
};
