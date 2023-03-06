/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:16:58 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/06 14:16:05 by flcarval         ###   ########.fr       */
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

	ft_irc::Server	serv;

	try {
		serv.init(
			av[2] ? av[2] : "no password",
			parsing_args(ac, av, env),
			env);
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}

	serv.run();

	try {
		ft_irc::Client	test_client(serv);
		test_client.read();
		std::cout << "Message received: " << test_client.getBuffer() << std::endl;
		test_client.write("Message has been received");
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}


	return (0);
}

