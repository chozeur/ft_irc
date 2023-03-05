/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/05 22:52:41 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

ft_irc::server::server(void){return ;}

ft_irc::server::server(server const &src){
	*this = src;
	return ;
}

ft_irc::server::server(std::string password, long port, char **env){
	this->init(password, port, env);
	return ;
}

ft_irc::server::~server(void){return ;}

ft_irc::server&	ft_irc::server::operator=(server const &src){
	if (this != &src){
		this->_serv_addr = src._serv_addr;
		this->_sockfd = src._sockfd;
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
	return ;
}

std::string	ft_irc::server::getPassword(void)const{
	return (this->_password);
}

void	ft_irc::server::setPassword(std::string password){
	this->_password = password;
	return ;
}

struct sockaddr_in	ft_irc::server::getServAddr()const{
	return (this->_serv_addr);
}

void	ft_irc::server::setServAddr(void){

	memset(&this->_serv_addr, 0, sizeof(struct sockaddr_in));
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htons(this->_port);
	return ;
}

int	ft_irc::server::getSockfd(void)const{
	return (this->_sockfd);
}

void	ft_irc::server::setSockfd(int fd){
	this->_sockfd = fd;
	return ;
}

char	**ft_irc::server::getEnv(void)const{
	return (this->_env);
}

void	ft_irc::server::setEnv(char **env){
	this->_env = env;	//! deep copy
	return ;
}

void	ft_irc::server::init(std::string password, long port, char **env){
	this->_port = port;
	this->_password = password;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
		throw std::runtime_error("Error : socket");
	this->setServAddr();
	this->_env = env; //! deep copy
	return ;
}

void	ft_irc::server::run(void){
	if (bind(this->_sockfd, (struct sockaddr *) &this->_serv_addr, sizeof(this->_serv_addr)) < 0)
		throw std::runtime_error("Error : binding socket failed");
	listen(this->_sockfd, 5);
	/* TESTS */
	struct sockaddr_in	cli_addr;
	socklen_t			clilen = sizeof(cli_addr);
	int					newsockfd = accept(this->_sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		throw (std::runtime_error("Error : accept"));
	char buffer[256];
	bzero(buffer, 256);
	int n = read(newsockfd, buffer, 255);
	if (n < 0)
		throw (std::runtime_error("Error : reading from socket"));
	std::cout << "Here is the message: " << buffer << std::endl;
	n = write(newsockfd, "I got your message", 18);
	if (n < 0)
		throw (std::runtime_error("Error : writing to socket"));
	close(newsockfd);
	close(this->_sockfd);
	return ;
}

