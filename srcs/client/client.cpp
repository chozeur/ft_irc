/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/03/07 22:43:01 by flcarval         ###   ########.fr       */
=======
/*   Updated: 2023/03/10 18:19:48 by rvrignon         ###   ########.fr       */
>>>>>>> rolexx
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/* CONSTRUCTORS */

ft_irc::Client::Client(void){return ;}

ft_irc::Client::Client(Client const & rhs){
	*this = rhs;
	return ;
}

ft_irc::Client::Client(ft_irc::Server &server):
	_server(&server),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host(""){
	memset(&this->_cli_addr, 0, sizeof(struct sockaddr_in));
	this->_cli_addr.sin_family = AF_INET;
	this->_cli_addr.sin_addr.s_addr = INADDR_ANY;
	this->_cli_addr.sin_port = htons(this->_server->getPort());
	this->_cli_len = sizeof(_cli_addr);
	this->_sockfd = accept(this->_server->getSockfd(), (struct sockaddr *) &(this->_cli_addr), &(this->_cli_len));
	if (this->_sockfd < 0)
		throw (std::runtime_error("Error : accept"));
}

/* DESTRUCTOR */

ft_irc::Client::~Client(void){
	close(this->_sockfd);
	return ;
}

/* INIT */

ft_irc::Client&	ft_irc::Client::operator=(Client const &rhs){
	if (this != &rhs){
		this->_cli_addr = rhs._cli_addr;
		this->_cli_len = rhs._cli_len;
		this->_sockfd = rhs._sockfd;
		for (int i = 0; i < 2048; i++)
			this->_buffer[i] = rhs._buffer[i];
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_password = rhs._password;
		this->_host = rhs._host;
		this->_channels = rhs._channels;
	}
	return (*this);
}

/* GETTERS */

struct sockaddr_in	ft_irc::Client::getCliAddr(void) const {
	return (this->_cli_addr);
}

socklen_t	ft_irc::Client::getCliLen(void) const {
	return (this->_cli_len);
}

int	ft_irc::Client::getSockfd(void) const {
	return (this->_sockfd);
}

char	*ft_irc::Client::getBuffer(void){
	return (this->_buffer);
}

std::string	ft_irc::Client::getNickname(void) const {
	return (this->_nickname);
}

std::string	ft_irc::Client::getUsername(void) const {
	return (this->_username);
}

std::string	ft_irc::Client::getRealname(void) const {
	return (this->_realname);
}

std::string	ft_irc::Client::getPassword(void) const {
	return (this->_password);
}

std::string	ft_irc::Client::getHost(void) const {
	return (this->_host);
}

std::vector<std::string>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

/* SETTERS */

void	ft_irc::Client::setCliAddr(struct sockaddr_in cli_addr){
	this->_cli_addr = cli_addr;
	return ;
}

void	ft_irc::Client::setCliLen(socklen_t cli_len){
	this->_cli_len = cli_len;
	return ;
}

void	ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
	return ;
}

void	ft_irc::Client::setBuffer(char *buffer){
	for (int i = 0; i < 2048 && buffer[i]; i++)
		this->_buffer[i] = buffer[i];
	return ;
}

void	ft_irc::Client::setNickname(std::string nickname){
	this->_nickname = nickname;
	return ;
}

void	ft_irc::Client::setUsername(std::string username){
	this->_username = username;
	return ;
}

void	ft_irc::Client::setRealname(std::string realname){
	this->_realname = realname;
	return ;
}

void	ft_irc::Client::setPassword(std::string password){
	this->_password = password;
	return ;
}

void	ft_irc::Client::setHost(std::string host){
	this->_host = host;
	return ;
}

void	ft_irc::Client::setChannels(std::vector<std::string> channels){	//! deep copy
	this->_channels = channels;
	return ;
}


/* METHODS */
