#include "ft_irc.h"
#include <cstring>

volatile sig_atomic_t stop = false;

void sighandler(int sig)
{
    (void)sig;
    stop = true;
    std::cout << "\r";
}

void setupsig()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sighandler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}
