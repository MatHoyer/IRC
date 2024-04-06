#include "Channel.h"

Channel::Channel(std::string name, Type type) : name(name), password(""), topic(""), modes_(Channel::NONE), type_(type)
{
}

Channel::~Channel()
{
}

std::map<int, Client *> Channel::getClients()
{
    return clients_;
}

std::map<int, Client *> Channel::getOperators()
{
    return operators_;
}

int Channel::getMode()
{
    return modes_;
}

void Channel::setMode(int mode)
{
    modes_ = mode;
}

void Channel::addMode(int mode)
{
    modes_ |= mode;
}

bool Channel::hasMode(int mode)
{
    return modes_ & mode;
}

bool Channel::hasModes(int modes)
{
    return (modes_ & modes) == modes;
}

Channel::ChannelMode Channel::getMode(char mode) const
{
    if (mode == 'l')
        return CLIENT_LIMIT;
    else if (mode == 'i')
        return INVITE_ONLY;
    else if (mode == 'k')
        return KEY;
    else if (mode == 't')
        return PROTECTED_TOPIC;
    else if (mode == 'o')
        return OPERATOR;
    else
        return NONE;
}

void Channel::removeMode(int mode)
{
    modes_ &= ~mode;
}

std::string Channel::modesToStr()
{
    std::string ret;
    if (modes_ == Channel::NONE)
        return ret;
    ret += "+";
    if (hasMode(Channel::CLIENT_LIMIT))
        ret += "l";
    if (hasMode(Channel::INVITE_ONLY))
        ret += "i";
    if (hasMode(Channel::KEY))
        ret += "k";
    if (hasMode(Channel::PROTECTED_TOPIC))
        ret += "t";
    if (hasMode(Channel::OPERATOR))
        ret += "o";
    return ret;
}

int Channel::getType()
{
    return type_;
}

void Channel::setType(int type)
{
    type_ = type;
}

void Channel::addType(int type)
{
    type_ |= type;
}

bool Channel::hasType(int type)
{
    return type_ & type;
}

void Channel::addClient(Client *client)
{
    clients_[client->getFd()] = client;
}

void Channel::removeClient(Client *client)
{
    clients_.erase(client->getFd());
}

void Channel::addOperator(Client *client)
{
    operators_[client->getFd()] = client;
}

void Channel::removeOperator(Client *client)
{
    operators_.erase(client->getFd());
}

bool Channel::isOperator(Client *client)
{
    return operators_.find(client->getFd()) != operators_.end();
}

std::string Channel::modeToStr()
{
    std::string mode;
    mode += "+";
    if (hasMode(Channel::INVITE_ONLY))
        mode += "i";
    if (hasMode(Channel::PROTECTED_TOPIC))
        mode += "t";
    if (hasMode(Channel::KEY))
        mode += "k";
    if (hasMode(Channel::CLIENT_LIMIT))
        mode += "l";
    if (hasMode(Channel::OPERATOR))
        mode += "o";
    return mode;
}

int Channel::getClientCount()
{
    return clients_.size();
}

bool Channel::isClientBanned(Client *client)
{
    return ban_.find(client->nickmask) != ban_.end();
}

void Channel::addBan(Client *client)
{
    ban_[client->nickmask] = client;
}

void Channel::removeBan(Client *client)
{
    ban_.erase(client->nickmask);
}

bool Channel::isOnInviteList(Client *client)
{
    return inviteList_.find(client->nickmask) != inviteList_.end();
}

void Channel::addInvite(Client *client)
{
    inviteList_[client->nickmask] = client;
}

void Channel::removeInvite(Client *client)
{
    inviteList_.erase(client->nickmask);
}

void Channel::broadcast(std::string const &message, Client *sender, bool sendToSender)
{
    for (std::map<int, Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->second != sender || sendToSender)
            it->second->send(message);
    }
}

void Channel::broadcastUnique(std::string const &message, Client *sender, std::vector<Client *> &alreadyKnow,
                              bool sendToSender)
{
    for (std::map<int, Client *>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->second != sender || sendToSender)
        {
            if (isInAlreadyKnow(it->second->nick, alreadyKnow) == false)
            {
                alreadyKnow.push_back(it->second);
                it->second->send(message);
            }
        }
    }
}

bool Channel::isClientOnChannel(Client *client)
{
    return clients_.find(client->getFd()) != clients_.end();
}
