/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/25 19:52:29 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

irc::server::server() {}

irc::server::~server() {}

irc::config	&irc::server::getConf(){

	return (conf);
}

struct sockaddr_in	*irc::server::getServAddr()const{

	return (serv_addr);
}

int	irc::server::getSockfd()const{

	return (sock_fd);
}

void	irc::server::setSockfd(int fd){

	sock_fd = fd;
}
