#include "../includes/utils.hpp"

extern bool	stop;

void	sig_handler(int sig){

	if (sig == SIGINT)
		stop = true;
}
