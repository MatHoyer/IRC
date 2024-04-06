#pragma once

#include "../channel/ChannelManager.h"
#include "../client/Client.h"
#include "../client/ClientManager.h"
#include "../ft_irc.h"
#include "../message/Message.h"
#include "../client/Bot.h"
#include <deque>
#include <string>
#include <vector>

class CommandManager
{
  public:
    CommandManager(ChannelManager *cManager, ClientManager *uManager, std::string const &pwd);
    ~CommandManager();

    void doCommands(std::deque<Message> &msgs, Client *sender);
    void sendIsupport(Client *sender);
    void sendMotd(Client *sender);

  private:
    ChannelManager *cManager_;
    ClientManager *uManager_;
    std::string pwd_;
    std::string motd_;
};

std::string cap(Message &msg, Client *client);
std::string pass(Message &msg, Client *client, std::string &password);
std::string nick(Message &msg, Client *client, ClientManager *uManager, ChannelManager *cManager);
std::string user(Message &msg, Client *client);

std::string motd(std::string const &msg, Client *client);
std::string ping(Message &msg, Client *client);
std::string pong(std::string token);
std::string topic(Message &msg, Client *user, ChannelManager *cManager);
std::string invite(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager);
std::vector<std::string> join(Message &msg, Client *client, ChannelManager *cManager);
std::vector<std::string> part(Message &msg, Client *client, ChannelManager *cManager);
std::vector<std::string> privmsg(Message msg, Client *sender, ClientManager *uManager, ChannelManager *cManager);
std::string kick(Message &msg, Client *user, ClientManager *uManager, ChannelManager *cManager);

std::string error(Message &msg, Client *user);
std::string quit(Message &msg, Client *user, ChannelManager *chanManager);
std::vector<std::string> mode(Message &msg, Client *client, ClientManager *cManager, ChannelManager *chManager);
bool doBotThings(Message msg, Client *user, ChannelManager *cManager);

