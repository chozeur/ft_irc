/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 23:17:42 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/26 23:49:17 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/init.hpp"

long	parsing_args(int ac, char *str, char **env){

	char	*endptr = NULL;
	long	port = 0;

	if (!(*env))
		throw std::runtime_error("Error : no environment variables");
	if (ac != 3)
		throw std::runtime_error("Error : ./ircserv <port> <password>");
	port = strtol(str, &endptr, port);
	if (port < 1 || port > 65535 || *endptr != '\0')
		throw std::runtime_error("Error : port must be a number between 1 and 65535");
	return (port);
}
