/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:37:49 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "launchConfig/launchConfig.hpp"
#include "server/server.hpp"

bool stop = false;

void	sig_handler(int sig){

	if (sig == SIGINT)
		stop = true;
}

long	parsing_args(int ac, char *str, char **env){

	if (!(*env))
		return (-1);
	if (ac != 3){

		std::cout << "Error : ./ircserv <port> <password>" << std::endl;
		return (-1);
	}
	
	char	*endptr = NULL;
	long port = 0; 
	port = strtol(str, &endptr, port);
	
	if (port < 0 || port > 65535 || *endptr != '\0'){

		std::cout << "Error : problem with the port" << std::endl;
		return (-1);
	}
	return (port);
}

int	main(int ac, char **av, char **env){

	long port = parsing_args(ac, av[1], env);
	if (port == -1)
		return (1);
		
	irc::server serv = irc::server();
	
	signal(SIGINT, sig_handler);
	serv.getConf().setConfig("port", av[1]);
	serv.getConf().setConfig("password", av[2]);
	
	//server is running
	while (!stop){

		//server.run();
	}
	return (0);
}

	// std::cout << serv.getConf().getConfig("port") << std::endl << serv.getConf().getConfig("password") << std::endl;
	