#pragma once

#include "../client/Client.h"
#include <map>
#include <string>

class Channel
{

  public:
    // have to handle i, t, k, o, l
    // i = invite only
    // t = topic settable by channel operator only
    // k = key
    // o = channel operator
    // l = limit
    enum ChannelMode
    {
        CLIENT_LIMIT = (1 << 0),    // +l
        INVITE_ONLY = (1 << 1),     // +i
        KEY = (1 << 2),             // +k
        PROTECTED_TOPIC = (1 << 3), // +t
        OPERATOR = (1 << 4),        // +o
        NONE = (1 << 5)
    };

    // only #
    enum Type
    {
        REGULAR = (1 << 0),
        LOCAL = (1 << 1)
    };

    Channel(std::string name, Type type);
    ~Channel();

    std::string name;
    std::string password;
    std::string topic;
    int maxClient;

    std::map<int, Client *> getClients();
    std::map<int, Client *> getOperators();

    int getMode();
    void setMode(int mode);
    void addMode(int mode);
    bool hasMode(int mode);
    bool hasModes(int modes);
    ChannelMode getMode(char mode) const;
    void removeMode(int mode);
    std::string modesToStr();
    std::string modeToStr();

    int getType();
    void setType(int type);
    void addType(int type);
    bool hasType(int type);

    void addClient(Client *client);
    void removeClient(Client *client);

    void addOperator(Client *client);
    void removeOperator(Client *client);
    bool isOperator(Client *client);

    int getClientCount();

    bool isClientBanned(Client *client);
    void addBan(Client *client);
    void removeBan(Client *client);

    bool isOnInviteList(Client *client);
    void addInvite(Client *client);
    void removeInvite(Client *client);

    void broadcast(std::string const &message, Client *sender, bool sendToSender = false);
    void broadcastUnique(std::string const &message, Client *sender, std::vector<Client *> &alreadyKnow, bool sendToSender = false);

    bool isClientOnChannel(Client *client);

  private:
    int modes_;
    int type_;
    std::map<int, Client *> clients_;
    std::map<int, Client *> operators_;
    std::map<std::string, Client *> ban_;
    std::map<std::string, Client *> inviteList_;
};
