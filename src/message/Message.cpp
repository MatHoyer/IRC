#include "Message.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <iostream>

std::string recupVerb(std::string &line)
{
    size_t space = line.find(' ');
    std::string verb(line, 0, space);
    if (space == std::string::npos)
        line.erase(0, space);
    else
        line.erase(0, space + 1);
    for (std::string::iterator it = verb.begin(); it != verb.end(); it++)
        *it = std::toupper(*it);
    return verb;
}

std::deque<std::string> recupParams(std::string &line, bool *isEmptyTrailling)
{
    std::deque<std::string> params;
    size_t limiter = line.find_first_of(", :");
    while (limiter != std::string::npos && *(line.begin() + limiter) != ':')
    {
        params.push_back(std::string(line, 0, limiter));
        line.erase(0, limiter + 1);
        limiter = line.find_first_of(", :");
    }
    std::string tmp(line, 0, limiter);
    if (!tmp.empty())
        params.push_back(tmp);
    if (limiter != std::string::npos)
    {
        *isEmptyTrailling = 1;
        line.erase(0, limiter + 1);
    }
    else
    {
        *isEmptyTrailling = 0;
        line.erase(0, limiter);
    }
    return params;
}

std::string recupTrailing(std::string &line)
{
    std::string::iterator begin = line.begin();
    std::string::iterator end = line.end();
    return std::string(begin, end);
}

Message::Message(std::string line)
{
    if (line.size() > 512)
        line.erase(512, std::string::npos);

    verb = recupVerb(line);
    if (!verb.empty())
    {
        params = recupParams(line, &isEmptyTrailling);
        trailling = recupTrailing(line);
    }
    // std::cout << "verb     :" << verb << ":" << std::endl;
    // for (size_t i = 0; i < params.size(); i++)
    //     std::cout << "params" << i << "  :" << params[i] << ":" << std::endl;
    // if (params.empty())
    //     std::cout << "params" << "   :" << "empty" << ":" << std::endl;
    // std::cout << "trailling:" << trailling << ":" << std::endl;
    // std::cout << "=============================================" << std::endl;
}

Message Message::operator=(Message const &other)
{
    if (this == &other)
        return *this;
    this->isEmptyTrailling = other.isEmptyTrailling;
    this->trailling = other.trailling;
    this->params = other.params;
    this->verb = other.verb;
    return *this;
}
