/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/27 00:21:13 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "server/server.hpp"
#include "../includes/init.hpp"
#include "../includes/utils.hpp"


bool	stop = false;

int	main(int ac, char **av, char **env){

	ft_irc::server	serv = ft_irc::server();
	long			port;
	std::string		password = av[2];

	try {
		port = parsing_args(ac, av[1], env);
		serv.init(password, port);
	} catch (std::exception &e){
		return (1);
	}

	signal(SIGINT, sig_handler);
	while (!stop){

		//server.run(); //? server.run() should be called only once..?
	}
	return (0);
}

	// std::cout << serv.getConfMap("port") << std::endl << serv.getConfMap("password") << std::endl;
