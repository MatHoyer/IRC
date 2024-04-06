#include "Client.h"

bool isInAlreadyKnow(std::string nickToCherch, std::vector<Client *> alreadyKnow)
{
    for (std::vector<Client *>::iterator itVector = alreadyKnow.begin(); itVector != alreadyKnow.end(); itVector++)
    {
        if ((*itVector)->nick.compare(nickToCherch) == 0)
            return true;
    }
    return false;
}
