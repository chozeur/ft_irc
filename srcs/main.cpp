/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/01 15:18:39 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <climits>

#include "server/server.hpp"
#include "client/client.hpp"
#include "../includes/utils.hpp"


bool	stop = false;

int	main(int ac, char **av, char **env){

	ft_irc::server	serv = ft_irc::server();
	long			port;
	std::string		password = av[2] ? av[2] : "no password";

	try {
		port = parsing_args(ac, av, env);
		serv.init(password, port);
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	
	/**/
	/*TEST*/
    struct sockaddr_in cli_addr;
	socklen_t	cli_len = sizeof(cli_addr);
	listen(serv.getSockfd(), 1);
	/**/
	
	// signal(SIGINT, sig_handler);
	// while (!stop){

		//server.run(); //? server.run() should be called only once..?
	// }
	return (0);
}

