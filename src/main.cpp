#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>

#include "core/Logger.h"
#include "core/Server.h"
#include "ft_irc.h"

struct args
{
    int port;
    std::string portStr;
    std::string password;
};

struct args parseArgs(char *port, char *pwd)
{
    struct args res;
    std::stringstream ss;
    ss << port;
    ss >> res.port;
    ss.clear();
    ss.str("");
    ss << pwd;
    res.password = ss.str();
    res.portStr = std::string(port);
    return res;
}

void printUsageExit()
{
    std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
    ::exit(1);
}

int main(int ac, char **av)
{
    if (ac != 3)
        printUsageExit();
    struct args parsed = parseArgs(av[1], av[2]);
    if (parsed.password.empty() || (parsed.port < 1024 || parsed.port > 65535))
        printUsageExit();
    setupsig();
    Logger logger(std::cout);
    Server ircServ(parsed.portStr, parsed.password);
    ircServ.setLogger(logger);
    try
    {
        ircServ.start();
    }
    catch (std::exception &e)
    {
        logger.log(e.what(), Logger::ERROR);
    }
    return 0;
}
