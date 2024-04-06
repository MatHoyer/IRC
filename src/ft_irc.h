#pragma once
#include <bits/types/sig_atomic_t.h>
#include <csignal>
#include <iostream>
#include <signal.h>
#include <vector>

#define SERVER_NAME ":ft_irc "

#define BOT_NAME "BOT"

#define MAX_SIZE_SEND_BUFFER 510

// TOKEN PACK 1
#define CHANNELLEN 32
#define NICKLEN 31
#define USERLEN 18
#define CHANNEL_MAX_USER 20
#define CHANNEL_MAX_KEY_SIZE 50
#define CRLF "\r\n"

#define LINE1 " ________  _________         _____  _______      ______  \n"
#define LINE2 "|_   __  ||  _   _  |       |_   _||_   __ \\   .' ___  | \n"
#define LINE3 "  | |_ \\_||_/ | | \\_|         | |    | |__) | / .'   \\_| \n"
#define LINE4 "  |  _|       | |             | |    |  __ /  | |        \n"
#define LINE5 " _| |_       _| |_  _______  _| |_  _| |  \\ \\_\\ `.___.'\\ \n"
#define LINE6 "|_____|     |_____||_______||_____||____| |___|`.____ .' \n"
#define LINE7 "                                   by mhoyer & ebouvier\n"
#define LINE8 "Type /join #<channel> to join a channel\n"
#define LINE9 "Type /nick <nickname> to change your nickname\n"
#define LINE10 "Type /quit to quit the server\n"
#define LINE11 "Thank you for using ft_irc!\n"
extern volatile sig_atomic_t stop;
void setupsig();

// RPL
std::string const RPL_WELCOME(std::string const &client, std::string const &nickmask);
std::string const RPL_YOURHOST(std::string const &client, std::string const &servname, std::string const &vers);
std::string const RPL_CREATED(std::string const &client, std::string const &datetime);
std::string const RPL_MYINFO(std::string const &client, std::string const &sname, std::string const &vers,
                             std::string const &umodes, std::string const &cmodes);
std::string const RPL_ISUPPORT(std::string const &client, std::string const &tokens);
std::string const RPL_YOUREOPER(std::string const &client);
std::string const RPL_MOTD(std::string const &client, std::string const &motd);

// RPL CHANNELS
std::string const RPL_TOPICWHOTIME(std::string const &client, std::string const &channel, std::string const &nick,
                                   std::string const &time);
std::string const RPL_TOPIC(std::string const &client, std::string const &channel, std::string const &topic);
std::string const RPL_NOTOPIC(std::string const &client, std::string const &channel);
std::string const RPL_NAMREPLY(std::string const &client, std::string const &channel, std::vector<std::string> nicks);
std::string const RPL_ENDOFNAMES(std::string const &client, std::string const &channel);
std::string const RPL_CHANNELMODEIS(std::string const &client, std::string const &channel, std::string const &modes);
std::string const RPL_INVITING(std::string const &client, std::string const &nick, std::string const &channel);

// RPL MODE
std::string const RPL_UMODEIS(std::string const &client, std::string const &umodes);

// ERR
std::string const ERR_PASSWDMISMATCH(std::string const &client);
std::string const ERR_ALREADYREGISTERED(std::string const &client);
std::string const ERR_NEEDMOREPARAMS(std::string const &client, std::string const &cmd);
std::string const ERR_NICKNAMEINUSE(std::string const &client, std::string const &nick);
std::string const ERR_NONICKNAMEGIVEN(std::string const &client);
std::string const ERR_ERRONEUSNICKNAME(std::string const &client, std::string const &nick);
std::string const ERR_ERROR(std::string const &reason);
std::string const ERR_UNKNOWNCOMMAND(std::string const &client, std::string const &command);
std::string const ERR_NOOPERHOST(std::string const &client);
std::string const ERR_NOSUCHNICK(std::string const &client, std::string const &nick);
std::string const ERR_USERSDONTMATCH(std::string const &client);

// ERR CHANNELS
std::string const ERR_TOOMANYCHANNELS(std::string const &client, std::string const &channel);
std::string const ERR_NOSUCHCHANNEL(std::string const &client, std::string const &channel);
std::string const ERR_BADCHANNELKEY(std::string const &client, std::string const &channel);
std::string const ERR_BANNEDFROMCHAN(std::string const &client, std::string const &channel);
std::string const ERR_CHANNELISFULL(std::string const &client, std::string const &channel);
std::string const ERR_INVITEONLYCHAN(std::string const &client, std::string const &channel);
std::string const ERR_BADCHANMASK(std::string const &client, std::string const &channel);
std::string const ERR_INVALIDMODEPARAM(std::string const &client, std::string const &target, std::string const &mode,
                                       std::string const &parameter, std::string const &reason);
std::string const ERR_INVALIDKEY(std::string const &client, std::string const &channel);
std::string const ERR_CANNOTSENDTOCHAN(std::string const &client, std::string const &channel);
std::string const ERR_NOTONCHANNEL(std::string const &client, std::string const &channel);
std::string const ERR_USERONCHANNEL(std::string const &client, std::string const &nick, std::string const &channel);

// ERR MODE
std::string const ERR_UMODEUNKNOWNFLAG(std::string const &client);
std::string const ERR_UNKNOWNMODE(std::string const &client, std::string const &modechar);
std::string const ERR_CHANOPRIVSNEEDED(std::string const &client, std::string const &channel);
