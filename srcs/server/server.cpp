/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 19:01:38 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

ft_irc::server::server() {}

ft_irc::server::~server() {}

ft_irc::config	&ft_irc::server::getConf(){

	return (this->conf);
}

struct sockaddr_in	*ft_irc::server::getServAddr()const{

	return (serv_addr);
}

int	ft_irc::server::getSockfd()const{

	return (sock_fd);
}

void	ft_irc::server::setSockfd(int fd){

	sock_fd = fd;
}
