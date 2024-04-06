#include "Client.h"
#include <utility>
#include <vector>

Client::Client(int fd, IObserver *observer)
    : nick(""), username(""), realname(""), fd_(fd), shouldDisconnect_(false), registered_(false), modes_(0),
      capSent_(false), passSent_(false)
{
    observer_ = observer;
}

Client::~Client()
{
    if (nick.compare(BOT_NAME))
        disconnect();
}

int Client::getFd() const
{
    return fd_;
}

std::string &Client::getSendBuffer()
{
    return sendBuffer_;
}

std::string &Client::getRecvBuffer()
{
    return recvBuffer_;
}

void Client::setSendBuffer(std::string const &sendBuffer)
{
    sendBuffer_ = sendBuffer;
}

void Client::setRecvBuffer(std::string const &recvBuffer)
{
    recvBuffer_ = recvBuffer;
}

void Client::sendMany(std::vector<std::string> messages)
{
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it)
        send(*it);
}

Client::RolePrefix Client::getRoleInChannel(std::string const &channelName)
{
    return channelRoles_[channelName];
}

void Client::setRoleInChannel(std::string const &channelName, RolePrefix role)
{
    channelRoles_[channelName] = role;
}

std::string Client::RolePrefixToString(RolePrefix role)
{
    if (role == OPERATOR)
        return "@";
    else
        return "";
}

Client::UserModes Client::getMode(char mode) const
{
    if (mode == 'i')
        return (INVISIBLE);
    else if (mode == 'o')
        return (OPER);
    else
        return (NOT_SUPPORTED);
}

void Client::addMode(int modes)
{
    modes_ |= modes;
}

void Client::removeMode(int mode)
{
    modes_ &= ~mode;
}

int Client::getModes() const
{
    return modes_;
}

std::string Client::modesToStr()
{
    std::string ret;
    ret += "+";
    if (modes_ & INVISIBLE)
        ret += "i";
    if (modes_ & OPER)
        ret += "o";
    return (ret);
}

void Client::send(std::string message)
{
    if (message.empty())
        return;
    if (message.length() > MAX_SIZE_SEND_BUFFER)
        message.erase(MAX_SIZE_SEND_BUFFER, std::string::npos);
    sendBuffer_.append(message + CRLF);
    notifyObserver(EPOLLOUT);
}

bool Client::shouldDisconnect() const
{
    return shouldDisconnect_ == true;
}

void Client::setShouldDisconnect(bool shouldDisconnect)
{
    shouldDisconnect_ = shouldDisconnect;
}

bool Client::isRegistered() const
{
    return registered_ == true;
}

void Client::setRegistered(bool registered)
{
    registered_ = registered;
}

void Client::setPassSent(bool passSent)
{
    passSent_ = passSent;
}

void Client::setCapSent(bool capSent)
{
    capSent_ = capSent;
}

bool Client::isCapSent() const
{
    return capSent_;
}

void Client::setOp(bool op)
{
    op_ = op;
}

bool Client::isOp() const
{
    return op_;
}

void Client::updateNickmask()
{
    nickmask = nick + "!" + username + "@localhost";
}

std::vector<Client *> Client::getPrivmsg()
{
    return privmsgWith_;
}

void Client::addPrivmsg(Client *newClientPrimsg)
{
    if (nick.compare(BOT_NAME) == 0)
        return;
    privmsgWith_.push_back(newClientPrimsg);
}

void Client::updatePrivmsg(std::string &nickOldClient, Client *updateClient)
{
    for (std::vector<Client *>::iterator it = privmsgWith_.begin(); it != privmsgWith_.end(); it++)
    {
        if ((*it)->nick.compare(nickOldClient) == 0)
            (*it) = updateClient;
    }
}

void Client::removePrivmsg(std::string &nick)
{
    if (nick.compare(BOT_NAME) == 0)
        return;
    for (std::vector<Client *>::iterator it = privmsgWith_.begin(); it != privmsgWith_.end();)
    {
        if ((*it)->nick.compare(nick) == 0)
            it = privmsgWith_.erase(it);
        else
            ++it;
    }
}

bool Client::isInPrivmsg(std::string &nick)
{
    if (nick.compare(BOT_NAME) == 0)
        return false;
    for (std::vector<Client *>::iterator it = privmsgWith_.begin(); it != privmsgWith_.end(); it++)
    {
        if ((*it)->nick.compare(nick) == 0)
            return true;
    }
    return false;
}

bool Client::isPassSent() const
{
    return passSent_ == true;
}

void Client::disconnect()
{
    close(fd_);
    fd_ = -1;
}

Client::Client(Client const &other) : fd_(other.fd_)
{
}

Client &Client::operator=(Client const &rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

bool Client::operator==(Client const &rhs)
{
    return fd_ == rhs.fd_;
}

void Client::notifyObserver(EPOLL_EVENTS event)
{
    if (observer_)
        observer_->update(fd_, event);
}
