#include "../CommandManager.h"

std::string pong(std::string token)
{
    return SERVER_NAME + std::string("PONG " + token);
}
