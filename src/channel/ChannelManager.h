#pragma once

#include "Channel.h"
#include <string>

class ChannelManager
{

    typedef std::map<std::string, Channel *>::iterator ChannelMapIterator;

  public:
    ChannelManager();
    ~ChannelManager();

    Channel *get(std::string &name) const;
    Channel *create(std::string name, Channel::Type type);
    std::map<std::string, Channel *> getAll() const;
    void remove(std::string name);
    void removeAll();
    void removeClientFromAll(Client *client);
    void broadcast(std::string const &message, Channel *channel);
    bool channelExists(std::string const &name) const;

  private:
    ChannelManager &operator=(ChannelManager const &rhs);
    ChannelManager(ChannelManager const &other);
    std::map<std::string, Channel *> channels_;
};
