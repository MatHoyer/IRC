#pragma once

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
class Logger
{

  public:
    enum LoggerLevel
    {
        INFO,
        WARNING,
        ERROR
    };
    Logger(std::ostream &stream = std::cout);
    ~Logger();
    Logger &operator=(Logger const &rhs);
    Logger(Logger const &other);
    void log(const std::string &message, LoggerLevel level = INFO) const;

  private:
    std::string getLevelStr(LoggerLevel level) const;
    std::string getColor(LoggerLevel level) const;
    std::string getResetColor() const;
    std::string getTimestamp() const;
    std::ostream *stream;
};
