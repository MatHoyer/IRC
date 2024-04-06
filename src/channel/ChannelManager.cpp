#include "ChannelManager.h"
#include "Channel.h"

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
    removeAll();
}

Channel *ChannelManager::get(std::string &name) const
{
    if (channels_.find(name) == channels_.end())
        return NULL;
    return channels_.find(name)->second;
}

Channel *ChannelManager::create(std::string name, Channel::Type type)
{
    Channel *channel = new Channel(name, type);
    channels_[name] = channel;
    return channels_[name];
}

std::map<std::string, Channel *> ChannelManager::getAll() const
{
    return channels_;
}

void ChannelManager::remove(std::string name)
{
    ChannelMapIterator it = channels_.find(name);
    if (it != channels_.end())
    {
        delete it->second;
        channels_.erase(it);
    }
}

void ChannelManager::removeAll()
{
    for (ChannelMapIterator it = channels_.begin(); it != channels_.end(); ++it)
        delete it->second;
    channels_.clear();
}

void ChannelManager::removeClientFromAll(Client *client)
{
    for (ChannelMapIterator it = channels_.begin(); it != channels_.end(); ++it)
        it->second->removeClient(client);
}

void ChannelManager::broadcast(std::string const &message, Channel *channel)
{
    std::map<int, Client *> clients = channel->getClients();
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        it->second->send(message);
}

bool ChannelManager::channelExists(std::string const &name) const
{
    return channels_.find(name) != channels_.end();
}

ChannelManager &ChannelManager::operator=(ChannelManager const &rhs)
{
    if (this != &rhs)
    {
        channels_ = rhs.channels_;
    }
    return *this;
}

ChannelManager::ChannelManager(ChannelManager const &other) : channels_(other.channels_)
{
}
