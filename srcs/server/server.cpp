/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/27 00:34:12 by flcarval         ###   ########.fr       */
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
		this->_port = src._port;
		this->_password = src._password;
	}
	return (*this);
}

long	ft_irc::server::getPort(void) const {
	return (this->_port);
}

void	ft_irc::server::setPort(long port){
	this->_port = port;
}

std::string	ft_irc::server::getPassword(void)const{
	return (this->_password);
}

void	ft_irc::server::setPassword(std::string password){
	this->_password = password;
}

struct sockaddr_in	ft_irc::server::getServAddr()const{
	return (this->_serv_addr);
}

void	ft_irc::server::setServAddr(long port){

	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htonl(port);
}

int	ft_irc::server::getSockfd(void)const{
	return (this->_sock_fd);
}

void	ft_irc::server::setSockfd(int fd){
	this->_sock_fd = fd;
}

void	ft_irc::server::init(std::string password, long port){
	this->setPort(port);
	this->setPassword(password);
	this->setSockfd(socket(AF_INET, SOCK_STREAM, 0));
	if (this->getSockfd() < 0)
		throw std::runtime_error("Error : socket");
	this->setServAddr(port);
}
