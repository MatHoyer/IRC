#include "../ft_irc.h"
#include <string>
#include <vector>

std::string const RPL_WELCOME(std::string const &client, std::string const &nickmask)
{
    return "001 " + client + " :Welcome to ft_irc " + nickmask;
}

std::string const RPL_YOURHOST(std::string const &client, std::string const &servname, std::string const &vers)
{
    return "002 " + client + " :Your host is " + servname + ", running version " + vers;
}

std::string const RPL_CREATED(std::string const &client, std::string const &datetime)
{
    return "003 " + client + " :This server was created " + datetime;
}

std::string const RPL_MYINFO(std::string const &client, std::string const &sname, std::string const &vers,
                             std::string const &umodes, std::string const &cmodes)
{
    return "004 " + client + " " + sname + " " + vers + " " + umodes + " " + cmodes;
}

std::string const RPL_ISUPPORT(std::string const &client, std::string const &tokens)
{
    return "005 " + client + tokens + " :are supported by this server";
}

std::string const RPL_YOUREOPER(std::string const &client)
{
    return "381 " + client + " :You are now an IRC operator";
}

////
// CHANNELS
////

std::string const RPL_TOPICWHOTIME(std::string const &client, std::string const &channel, std::string const &nick,
                                   std::string const &time)
{
    return "333 " + client + " " + channel + " " + nick + " " + time;
}

std::string const RPL_NAMREPLY(std::string const &client, std::string const &channel, std::vector<std::string> nicks)
{
    std::string ret = SERVER_NAME + std::string("353 " + client + " = " + channel + " :");
    for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); ++it)
    {
        ret += *it;
        if (it + 1 != nicks.end())
            ret += " ";
    }
    return ret;
}

std::string const RPL_ENDOFNAMES(std::string const &client, std::string const &channel)
{
    return SERVER_NAME + std::string("366 " + client + " " + channel + " :End of /NAMES list");
}

std::string const RPL_MOTD(std::string const &client, std::string const &motd)
{
    return "372 " + client + " :" + motd;
}

std::string const RPL_TOPIC(std::string const &client, std::string const &channel, std::string const &topic)
{
    return "332 " + client + " " + channel + " :" + topic;
}

std::string const RPL_NOTOPIC(std::string const &client, std::string const &channel)
{
    return "331 " + client + " " + channel + " :No topic is set";
}

std::string const RPL_INVITING(std::string const &client, std::string const &nick, std::string const &channel)
{
    return " 341 " + client + " " + nick + " " + channel;
}


//// MODE

std::string const RPL_UMODEIS(std::string const &client, std::string const &umodes)
{
    return "221 " + client + " " + umodes;
}

std::string const RPL_CHANNELMODEIS(std::string const &client, std::string const &channel, std::string const &modes)
{
    return "324 " + client + " " + channel + " " + modes;
}
