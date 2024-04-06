#include "ClientManager.h"
#include "Bot.h"

ClientManager::ClientManager()
{
}

ClientManager::~ClientManager()
{
    removeAll();
}

int ClientManager::getClientCount()
{
    return clients_.size();
}

std::map<int, Client *> &ClientManager::getClients()
{
    return clients_;
}

Client *ClientManager::get(int fd)
{
    ClientMapIterator it = clients_.find(fd);
    if (it != clients_.end())
        return it->second;
    return NULL;
}

Client *ClientManager::getByNick(std::string const &nick)
{
    for (std::map<int, Client *>::const_iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->second->nick == nick)
            return it->second;
    }
    return NULL;
}

Client *ClientManager::create(int fd, IObserver *observer)
{
    Client *newClient = new Client(fd, observer);
    clients_[fd] = newClient;
    return clients_[fd];
}

void ClientManager::remove(int fd)
{
    ClientMapIterator it = clients_.find(fd);
    if (it != clients_.end())
    {
        delete it->second;
        clients_.erase(it);
    }
}

void ClientManager::removeAll()
{
    for (ClientMapIterator it = clients_.begin(); it != clients_.end(); ++it)
        delete it->second;
    clients_.clear();
}

bool ClientManager::nickExists(std::string const &nick)
{
    for (ClientMapIterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->second->nick == nick)
            return true;
    }
    return false;
}

bool ClientManager::isSameClient(Client *client, std::string const &nick)
{
    return client->nick == nick;
}
