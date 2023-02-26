/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 23:52:44 by flcarval         ###   ########.fr       */
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

std::string	ft_irc::server::getConfMap(std::string key){
	return (this->_conf_map[key]);
}

void	ft_irc::server::setConfMap(std::string key, std::string value){
	this->_conf_map[key] = value;
}

struct sockaddr_in	ft_irc::server::getServAddr()const{
	return (this->_serv_addr);
}

void				ft_irc::server::setServAddr(long port){

	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htonl(port);
}

int	ft_irc::server::getSockfd()const{
	return (this->_sock_fd);
}

void	ft_irc::server::setSockfd(int fd){
	this->_sock_fd = fd;
}

void	ft_irc::server::init(char **av, long port){
	this->setConfMap("port", av[1]);
	this->setConfMap("password", av[2]);
	this->setSockfd(socket(AF_INET, SOCK_STREAM, 0));
	if (this->getSockfd() < 0)
		throw std::runtime_error("Error : socket");
	this->setServAddr(port);
}
