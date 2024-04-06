#include "Logger.h"
#include <iomanip>
#include <ios>
#include <ostream>

Logger::Logger(std::ostream &stream) : stream(&stream)
{
}

Logger::~Logger()
{
}

Logger &Logger::operator=(Logger const &rhs)
{
    if (this != &rhs)
        stream = rhs.stream;
    return *this;
}

Logger::Logger(Logger const &other) : stream(other.stream)
{
}

void Logger::log(const std::string &message, LoggerLevel level) const
{
    std::string levelStr;
    int levelWidth = 8;
    *stream << "[" << getTimestamp() << "] " << getColor(level) << std::left << std::setw(levelWidth)
            << getLevelStr(level) << getResetColor() << message;
}

std::string Logger::getLevelStr(LoggerLevel level) const
{
    std::string levelStr;
    switch (level)
    {
    case INFO:
        levelStr = "INFO";
        break;
    case WARNING:
        levelStr = "WARNING";
        break;
    case ERROR:
        levelStr = "ERROR";
        break;
    default:
        levelStr = "INFO";
        break;
    }
    return levelStr;
}

std::string Logger::getColor(LoggerLevel level) const
{
    std::string color;
    switch (level)
    {
    case INFO:
        color = "\033[0;32m";
        break;
    case WARNING:
        color = "\033[0;33m";
        break;
    case ERROR:
        color = "\033[0;31m";
        break;
    default:
        color = "\033[0;32m";
        break;
    }
    return color;
}

std::string Logger::getResetColor() const
{
    return "\033[0m";
}

std::string Logger::getTimestamp() const
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}
