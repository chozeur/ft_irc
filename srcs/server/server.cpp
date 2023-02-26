/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 19:38:14 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

ft_irc::server::server(void){}

ft_irc::server::~server(void){}

ft_irc::server::server(server const &src){
	*this = src;
}

ft_irc::server	&ft_irc::server::operator=(server const &src){
	if (this != &src){
		this->_serv_addr = src._serv_addr;
		this->_sock_fd = src._sock_fd;
		this->_conf_map = src._conf_map;
	}
	return (*this);
}

struct sockaddr_in	*ft_irc::server::getServAddr()const{

	return (_serv_addr);
}

int	ft_irc::server::getSockfd()const{

	return (_sock_fd);
}

void	ft_irc::server::setSockfd(int fd){

	_sock_fd = fd;
}

void	ft_irc::server::setConfMap(std::string key, std::string value){
	this->_conf_map[key] = value;
}

std::string	ft_irc::server::getConfMap(std::string key){
	return (this->_conf_map[key]);
}
