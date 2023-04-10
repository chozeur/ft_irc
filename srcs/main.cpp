#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "server/server.hpp"
#include "client/client.hpp"
#include "../includes/utils.hpp"

bool	server = true;

std::ofstream ft_irc::log_file;
ft_irc::LogStream ft_irc::cout(std::cout);

int	main(int ac, char **av, char **env){

	ft_irc::log_file.open("server.log", std::ios_base::out | std::ios_base::app);
	ft_irc::cout.rdbuf(ft_irc::log_file.rdbuf());

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

	ft_irc::cout << "----------SERVER STARTED ( " << serv.HRdate() << " )----------" << std::endl;

	while (server)
		serv.run();
	serv.stop();
	return (0);
}
