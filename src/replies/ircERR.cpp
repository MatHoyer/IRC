#include "../ft_irc.h"
#include <string>

std::string const ERR_PASSWDMISMATCH(std::string const &client)
{
    return "464 " + client + " :Password incorrect";
}

std::string const ERR_ALREADYREGISTERED(std::string const &client)
{
    return "462 " + client + " :You may not reregister";
}

std::string const ERR_NEEDMOREPARAMS(std::string const &client, std::string const &cmd)
{
    return "461 " + client + " " + cmd + " :Not enough parameters";
}

std::string const ERR_NICKNAMEINUSE(std::string const &client, std::string const &nick)
{
    return "433 " + client + " " + nick + " :Nickname is already in use";
}

std::string const ERR_NONICKNAMEGIVEN(std::string const &client)
{
    return "431 " + client + " :No nickname given";
}

std::string const ERR_ERRONEUSNICKNAME(std::string const &client, std::string const &nick)
{
    return "432 " + client + " " + nick + " :Erroneus nickname";
}

std::string const ERR_ERROR(std::string const &reason)
{
    return "ERROR :" + reason;
}

std::string const ERR_UNKNOWNCOMMAND(std::string const &client, std::string const &command)
{
    return "421 " + client + " " + command + " :Unknown command";
}

std::string const ERR_NOOPERHOST(std::string const &client)
{
    return "491 " + client + " :No O-lines for your host";
}

std::string const ERR_NOSUCHNICK(std::string const &client, std::string const &nick)
{
    return "401 " + client + " " + nick + " :No such nick/channel";
}

std::string const ERR_USERSDONTMATCH(std::string const &client)
{
    return "502 " + client + " :Cant change mode for other users";
}

////
// CHANNELS
////

std::string const ERR_TOOMANYCHANNELS(std::string const &client, std::string const &channel)
{
    return "405 " + client + " " + channel + " :You have joined too many channels";
}

std::string const ERR_NOSUCHCHANNEL(std::string const &client, std::string const &channel)
{
    return "403 " + client + " " + channel + " :No such channel";
}

std::string const ERR_BADCHANNELKEY(std::string const &client, std::string const &channel)
{
    return "475 " + client + " " + channel + " :Cannot join channel (+k)";
}

std::string const ERR_BANNEDFROMCHAN(std::string const &client, std::string const &channel)
{
    return "474 " + client + " " + channel + " :Cannot join channel (+b)";
}

std::string const ERR_CHANNELISFULL(std::string const &client, std::string const &channel)
{
    return "471 " + client + " " + channel + " :Cannot join channel (+l)";
}

std::string const ERR_INVITEONLYCHAN(std::string const &client, std::string const &channel)
{
    return "473 " + client + " " + channel + " :Cannot join channel (+i)";
}

std::string const ERR_BADCHANMASK(std::string const &client, std::string const &channel)
{
    return "476 " + client + " " + channel + " :Bad Channel Mask";
}

std::string const ERR_INVALIDMODEPARAM(std::string const &client, std::string const &target, std::string const &mode,
                                       std::string const &parameter, std::string const &reason)
{
    return "696 " + client + " " + target + " " + mode + " " + parameter + " :" + reason;
}

std::string const ERR_INVALIDKEY(std::string const &client, std::string const &channel)
{
    return "525 " + client + " " + channel + " :Key is not well-formed";
}

std::string const ERR_CANNOTSENDTOCHAN(std::string const &client, std::string const &channel)
{
    return "404 " + client + " " + channel + " :Cannot send to channel";
}

std::string const ERR_NOTONCHANNEL(std::string const &client, std::string const &channel)
{
    return "442 " + client + " " + channel + " :You're not on that channel";
}

std::string const ERR_USERONCHANNEL(std::string const &client, std::string const &nick, std::string const &channel)
{
    return "443 " + client + " " + nick + " " + channel + " :is already on channel";
}

//// MODES
std::string const ERR_UMODEUNKNOWNFLAG(std::string const &client)
{
    return "501 " + client + " :Unknown MODE flag";
}

std::string const ERR_UNKNOWNMODE(std::string const &client, std::string const &modechar)
{
    return "472 " + client + " " + modechar + " :is unknown mode char to me";
}

std::string const ERR_CHANOPRIVSNEEDED(std::string const &client, std::string const &channel)
{
    return "482 " + client + " " + channel + " :You're not channel operator";
}
