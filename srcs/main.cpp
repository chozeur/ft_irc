#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "server/server.hpp"
#include "client/client.hpp"
#include "../includes/utils.hpp"

bool	server = true;

int	main(int ac, char **av, char **env){

	ft_irc::Server	serv;
	signal(SIGINT, sig_handler);

	try {
		serv.init(
			av[2] ? av[2] : "no password",
			parsing_args(ac, av, env),
			env);
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}

	while (server)
		serv.run();
	serv.stop();
	return (0);
}

