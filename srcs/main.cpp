/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/05 22:08:20 by flcarval         ###   ########.fr       */
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
	ft_irc::server	serv;

	try {
		serv.init(
			av[2] ? av[2] : "no password",
			parsing_args(ac, av, env),
			env);
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}

	/**/
	/*TEST*/
	// char buffer[256];
	// bzero(buffer, 256);
    // struct sockaddr_in cli_addr;
	// socklen_t	clilen = sizeof(cli_addr);
	// listen(serv.getSockfd(), 1);
	// int newsockfd = accept(serv.getSockfd(), (struct sockaddr *) &cli_addr, &clilen);
	// int n = read(newsockfd,buffer,255);
	// if (n < 0) std::cout << "ERROR reading from socket" << std::endl;
	// std::cout << buffer << std::endl;
	/**/

	// signal(SIGINT, sig_handler);
	// while (!stop){

		//server.run(); //? server.run() should be called only once..?
	// }

	serv.run();

	return (0);
}

