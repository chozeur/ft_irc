/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/07 17:57:55 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

ft_irc::Server::Server(void){return ;}

ft_irc::Server::Server(Server const &src){
	*this = src;
	return ;
}

ft_irc::Server::Server(std::string password, long port, char **env){
	this->init(password, port, env);
	return ;
}

ft_irc::Server::~Server(void){
	close(this->_sockfd);
	return ;
}

ft_irc::Server&	ft_irc::Server::operator=(Server const &src){
	if (this != &src){
		this->_port = src._port;
		this->_password = src._password;
		this->_serv_addr = src._serv_addr;
		this->_sockfd = src._sockfd;
		this->_env = src._env; //! deep copy
	}
	return (*this);
}

long	ft_irc::Server::getPort(void) const {
	return (this->_port);
}

void	ft_irc::Server::setPort(long port){
	this->_port = port;
	return ;
}

std::string	ft_irc::Server::getPassword(void)const{
	return (this->_password);
}

void	ft_irc::Server::setPassword(std::string password){
	this->_password = password;
	return ;
}

struct sockaddr_in	ft_irc::Server::getServAddr()const{
	return (this->_serv_addr);
}

void	ft_irc::Server::setServAddr(struct sockaddr_in serv_addr){
	this->_serv_addr = serv_addr;
	return ;
}

int	ft_irc::Server::getSockfd(void)const{
	return (this->_sockfd);
}

void	ft_irc::Server::setSockfd(int fd){
	this->_sockfd = fd;
	return ;
}

char	**ft_irc::Server::getEnv(void)const{
	return (this->_env);
}

void	ft_irc::Server::setEnv(char **env){
	this->_env = env;	//! deep copy
	return ;
}

void	ft_irc::Server::init(std::string password, long port, char **env){
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

