#include "../includes/utils.hpp"

extern bool	server;

void	sig_handler(int sig){
    if (sig == SIGINT) {
        server = false;
    }
}

