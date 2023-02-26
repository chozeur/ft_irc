/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 23:32:02 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "server/server.hpp"
#include "../include/init.hpp"
#include "../include/utils.hpp"


bool	stop = false;

int	main(int ac, char **av, char **env){

	ft_irc::server serv = ft_irc::server();
	long port = parsing_args(ac, av[1], env);

	if (port == -1)
		return (1);

	try {
		serv.init(av, port);
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
