#include "Server.h"
#include "../buffer/Buffer.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <sys/epoll.h>
#include <sys/poll.h>

Server::Server(std::string port, std::string password) : port_(port), password_(password), listener_(-1)
{
}

Server::~Server()
{
    cManager_.removeAll();
    uManager_.removeAll();
    for (PollFdsIterator it = poller_.begin(); it != poller_.end(); ++it)
        close(it->fd);
    poller_.clear();
    close(listener_);
}

void Server::start()
{

    if ((listener_ = listenForConnection()) < 0)
        throw std::runtime_error("listenForConnection");
    CommandManager commands(&cManager_, &uManager_, password_);
    initListener();
    logger_.log("ircserv listening on port " + port_ + "\n", Logger::INFO);
    while (!stop)
    {
        poll(poller_.data(), poller_.size(), -1);
        for (size_t i = 0; i < poller_.size(); ++i)
        {
            if (poller_[i].revents & POLLERR || poller_[i].revents & POLLHUP)
            {
                removeFromPolling(poller_[i].fd);
                Client *client = uManager_.get(poller_[i].fd);
                if (client)
                {
                    cManager_.removeClientFromAll(client);
                    uManager_.remove(client->getFd());
                }
                close(poller_[i].fd);
                poller_[i].fd = -1;
                poller_[i].events = 0;
                poller_[i].revents = 0;
            }
            else if (poller_[i].revents & POLLIN)
            {
                if (poller_[i].fd == listener_)
                    acceptConnection();
                else if (uManager_.get(poller_[i].fd) && uManager_.get(poller_[i].fd)->shouldDisconnect())
                    continue;
                else
                    recvData(poller_[i], commands);
            }
            else if (poller_[i].revents & POLLOUT)
                sendData(poller_[i]);
        }
        disconnectClients();
    }
}

void Server::setLogger(Logger &logger)
{
    logger_ = logger;
}

void Server::update(int fd, int event)
{
    for (PollFdsIterator it = poller_.begin(); it != poller_.end(); ++it)
    {
        if (it->fd == fd)
        {
            it->events = event;
            return;
        }
    }
}

void *Server::getInAddr_(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &reinterpret_cast<struct sockaddr_in *>(sa)->sin_addr;
    return &reinterpret_cast<struct sockaddr_in6 *>(sa)->sin6_addr;
}

void Server::acceptConnection()
{
    struct sockaddr_storage remAddr;
    socklen_t addrlen = sizeof(remAddr);
    int fd = accept(listener_, (struct sockaddr *)&remAddr, &addrlen);
    if (fd < 0)
        logger_.log(std::string("accept: ") + strerror(errno), Logger::ERROR);
    else
    {
        fcntl(fd, F_SETFL, O_NONBLOCK);
        addToPolling(fd);
        uManager_.create(fd, this);
        logger_.log("new connection\n", Logger::INFO);
    }
}

void Server::addToPolling(int fd)
{
    struct pollfd pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.events = POLLIN | POLLERR | POLLHUP;
    pfd.revents = 0;
    pfd.fd = fd;
    poller_.push_back(pfd);
}

void Server::removeFromPolling(int fd)
{
    logger_.log("connection closed\n", Logger::INFO);
    for (PollFdsIterator it = poller_.begin(); it != poller_.end(); ++it)
    {
        if (it->fd == fd)
        {
            poller_.erase(it);
            return;
        }
    }
}

void Server::sendData(struct pollfd &event)
{
    if (!uManager_.get(event.fd))
        return;
    ssize_t len = uManager_.get(event.fd)->getSendBuffer().size();
    ssize_t bytesSent = ::send(event.fd, uManager_.get(event.fd)->getSendBuffer().c_str(), len, MSG_NOSIGNAL);
    if (bytesSent == -1)
    {
        logger_.log("send: " + std::string(strerror(errno)), Logger::WARNING);
        return;
    }
    if (bytesSent > 0 && len - bytesSent == 0)
    {
        // debug
        std::string s = uManager_.get(event.fd)->getSendBuffer();
        // for (size_t pos = 0; (pos = s.find("\r\n", pos)) != std::string::npos; s.replace(pos, 2, "\\r\\n"), pos += 2)
        // ;
        logger_.log(">" + s, Logger::INFO);
        // end debug
        uManager_.get(event.fd)->getSendBuffer().clear();
        event.events = POLLIN;
    }
    else
        uManager_.get(event.fd)->getSendBuffer().erase(0, bytesSent);
}

void Server::recvData(struct pollfd &event, CommandManager &commands)
{
    if (!uManager_.get(event.fd))
        return;
    char tmpBuff[4096] = {0};
    ssize_t bytesRead = recv(event.fd, tmpBuff, sizeof(tmpBuff), MSG_NOSIGNAL);
    if (bytesRead < 0)
    {
        logger_.log("recv: " + std::string(strerror(errno)), Logger::WARNING);
        return;
    }
    if (bytesRead == 0)
    {
        removeFromPolling(event.fd);
        Client *client = uManager_.get(event.fd);
        if (client)
        {
            cManager_.removeClientFromAll(client);
            uManager_.remove(client->getFd());
        }
        close(event.fd);
        event.fd = -1;
        event.events = 0;
        event.revents = 0;
        return;
    }
    uManager_.get(event.fd)->getRecvBuffer().append(tmpBuff, bytesRead);
    std::string r = uManager_.get(event.fd)->getRecvBuffer();
    logger_.log("<" + r, Logger::INFO);
    if (hasCRLF(uManager_.get(event.fd)->getRecvBuffer()))
    {
        std::deque<Message> msgs;
        Buffer::bufferToMessage(uManager_.get(event.fd)->getRecvBuffer(), msgs);
        commands.doCommands(msgs, uManager_.get(event.fd));
        event.revents = POLLOUT;
    }
}

void Server::initListener()
{
    struct pollfd pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.events = POLLIN;
    pfd.revents = 0;
    pfd.fd = listener_;
    poller_.push_back(pfd);
}

bool Server::hasCRLF(std::string &buffer)
{
    return (buffer.find("\r\n") != std::string::npos);
}

void Server::disconnectClients()
{
    std::vector<int> toRemove;
    for (std::map<int, Client *>::iterator it = uManager_.getClients().begin(); it != uManager_.getClients().end();
         ++it)
    {
        if (it->second->shouldDisconnect())
            toRemove.push_back(it->first);
    }
    for (std::vector<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it)
    {
        std::map<std::string, Channel *> channels = cManager_.getAll();
        for (std::map<std::string, Channel *>::iterator itChan = channels.begin(); itChan != channels.end(); itChan++)
        {
            if (itChan->second->isClientOnChannel(uManager_.get(*it)))
            {
                itChan->second->removeClient(uManager_.get(*it));
                if (itChan->second->getClients().empty())
                    cManager_.remove(itChan->first);
            }
        }
        uManager_.remove(*it);
        removeFromPolling(*it);
    }
}

int Server::listenForConnection()
{
    struct addrinfo hints, *addr, *curr;
    int status;
    int sockfd;
    int yes = 1;
    int no = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, port_.c_str(), &hints, &addr)) != 0)
        throw std::runtime_error("getaddrinfo: " + std::string(strerror(errno)));
    for (curr = addr; curr; curr = curr->ai_next)
    {
        if ((sockfd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol)) < 0)
            continue;

        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
        setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));

        if (bind(sockfd, curr->ai_addr, curr->ai_addrlen) < 0)
        {
            close(sockfd);
            continue;
        }
        break;
    }
    freeaddrinfo(addr);
    if (!curr)
        return -1;

    if (listen(sockfd, MAX_LISTENER) < 0)
        return -1;
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}
