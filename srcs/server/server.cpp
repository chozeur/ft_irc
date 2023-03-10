/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/10 18:12:31 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* CONSTRUCTORS */
ft_irc::Server::Server(void) { return; }

ft_irc::Server::Server(Server const &rhs)
{
	*this = rhs;
	return;
}

ft_irc::Server::Server(std::string password, int port, char **env)
{
	this->init(password, port, env);
	return;
}

/* DESTRUCTOR */
ft_irc::Server::~Server(void)
{
	close(this->_sockfd);
	return;
}

/* INIT */
ft_irc::Server &ft_irc::Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_serv_addr = rhs._serv_addr;
		this->_sockfd = rhs._sockfd;
		this->_env = rhs._env; //! deep copy
	}
	return (*this);
}

/* METHODS */
void ft_irc::Server::init(std::string password, int port, char **env)
{
	this->_port = port;
	this->_password = password;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
		throw std::runtime_error("Error : socket");
	memset(&this->_serv_addr, 0, sizeof(struct sockaddr_in));
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htons(this->_port);
	this->_env = env; //! deep copy
	return ;
}

void	ft_irc::Server::run(void){
	if (bind(this->_sockfd, (struct sockaddr *) &this->_serv_addr, sizeof(this->_serv_addr)) < 0)
		throw std::runtime_error("Error : binding socket failed");
	listen(this->_sockfd, 5);
	return ;
}

void ft_irc::Server::stop(void)
{
	close(this->_sockfd);
	std::cout << "Server 127.0.0.1:" << this->_port << " has been stopped" << std::endl;
}
