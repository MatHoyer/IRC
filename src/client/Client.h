#pragma once
#include "../core/IObserver.h"
#include "../ft_irc.h"

#include <map>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Client
{
  public:
    enum RolePrefix
    {
        OPERATOR = 0,
        VOICE
    };

    enum UserModes
    {
        INVISIBLE = (1 << 0),
        OPER = (1 << 1),
        NOT_SUPPORTED = (1 << 2)
    };

    Client(int fd, IObserver *observer);
    virtual ~Client();

    int getFd() const;

    std::string &getSendBuffer();
    std::string &getRecvBuffer();

    void setSendBuffer(std::string const &sendBuffer);
    void setRecvBuffer(std::string const &recvBuffer);

    void send(std::string message);
    void sendMany(std::vector<std::string> message);

    RolePrefix getRoleInChannel(std::string const &channelName);
    void setRoleInChannel(std::string const &channelName, RolePrefix role);
    std::string RolePrefixToString(RolePrefix role);

    UserModes getMode(char mode) const;
    void addMode(int mode);
    void removeMode(int mode);
    int getModes() const;
    std::string modesToStr();

    bool shouldDisconnect() const;
    void setShouldDisconnect(bool shouldDisconnect);

    bool isRegistered() const;
    void setRegistered(bool registered);

    void setPassSent(bool passSent);
    bool isPassSent() const;

    void setCapSent(bool capSent);
    bool isCapSent() const;

    void setOp(bool op);
    bool isOp() const;

    void updateNickmask();

    std::vector<Client *> getPrivmsg();
    void addPrivmsg(Client *newClientPrimsg);
    void updatePrivmsg(std::string &nickOldClient, Client *updateClient);
    void removePrivmsg(std::string &nick);
    bool isInPrivmsg(std::string &nick);

    void disconnect();

    IObserver *observer_;
    std::string nick;
    std::string username;
    std::string realname;
    std::string nickmask;

  private:
    Client(Client const &other);
    Client &operator=(Client const &rhs);
    bool operator==(Client const &rhs);
    void notifyObserver(EPOLL_EVENTS event);

    int fd_;
    bool shouldDisconnect_;
    bool registered_;
    int modes_;

    bool capSent_;
    bool passSent_;

    bool op_;
    std::map<std::string, RolePrefix> channelRoles_;
    std::vector<Client *> privmsgWith_;

    std::string sendBuffer_;
    std::string recvBuffer_;
};

bool isInAlreadyKnow(std::string toCherch, std::vector<Client *> alreadyKnow);
