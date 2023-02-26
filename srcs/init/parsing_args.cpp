/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 23:17:42 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/26 23:17:43 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.hpp"

long	parsing_args(int ac, char *str, char **env){

	char	*endptr = NULL;
	long	port = 0;

	if (!(*env))
		return (-1);
	if (ac != 3){
		std::cout << "Error : ./ircserv <port> <password>" << std::endl;
		return (-1);
	}

	port = strtol(str, &endptr, port);

	if (port < 1 || port > 65535 || *endptr != '\0'){
		std::cout << "Error : problem with the port" << std::endl;
		return (-1);
	}
	return (port);
}
