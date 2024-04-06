#include "Buffer.h"

int howManyCRLF(std::string &buffer)
{
    int nbCRLF = 0;
    for (std::string::iterator it = buffer.begin(); it != buffer.end() - 1; ++it)
    {
        if (*it == '\r' && *(it + 1) == '\n')
            nbCRLF++;
    }

    return nbCRLF;
}

void Buffer::bufferToMessage(std::string &recvBuffer, std::deque<Message> &msgs)
{
    int nbCRLF = howManyCRLF(recvBuffer);
    for (int i = 0; i < nbCRLF; i++)
    {
        std::string::iterator nextCRLF = recvBuffer.begin() + recvBuffer.find("\r\n");
        if (nextCRLF != recvBuffer.begin())
        {
            std::string::iterator begin = recvBuffer.begin();
            msgs.push_back(Message(std::string(begin, nextCRLF)));
            recvBuffer.erase(0, nextCRLF - begin + 2);
        }
        else if (nextCRLF == recvBuffer.begin())
            recvBuffer.erase(0, 2);
    }
}
