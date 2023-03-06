/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/06 15:58:44 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

ft_irc::Client::Client(void){return ;}

ft_irc::Client::Client(Client const & src){
	*this = src;
	return ;
}

ft_irc::Client::Client(ft_irc::Server &server){
	this->_server = &server;
	memset(&this->_cli_addr, 0, sizeof(struct sockaddr_in));
	this->_cli_addr.sin_family = AF_INET;
	this->_cli_addr.sin_addr.s_addr = INADDR_ANY;
	this->_cli_addr.sin_port = htons(this->_server->getPort());
	this->_cli_len = sizeof(_cli_addr);
	this->_sockfd = accept(this->_server->getSockfd(), (struct sockaddr *) &(this->_cli_addr), &(this->_cli_len));
	if (this->_sockfd < 0)
		throw (std::runtime_error("Error : accept"));
}

ft_irc::Client::~Client(void){
	close(this->_sockfd);
	return ;
}

ft_irc::Client&	ft_irc::Client::operator=(Client const &src){
	if (this != &src){
		this->_cli_addr = src._cli_addr;
		this->_cli_len = src._cli_len;
		this->_sockfd = src._sockfd;
		for (int i = 0; i < 2048; i++)
			this->_buffer[i] = src._buffer[i];
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_realname = src._realname;
		this->_password = src._password;
		this->_host = src._host;
		this->_channels = src._channels;
	}
	return (*this);
}

struct sockaddr_in	ft_irc::Client::getCliAddr(void)const{
	return (this->_cli_addr);
}

void	ft_irc::Client::setCliAddr(struct sockaddr_in cli_addr){
	this->_cli_addr = cli_addr;
	return ;
}

socklen_t	ft_irc::Client::getCliLen(void) const {
	return (this->_cli_len);
}

void	ft_irc::Client::setCliLen(socklen_t cli_len){
	this->_cli_len = cli_len;
	return ;
}

int	ft_irc::Client::getSockfd(void) const {
	return (this->_sockfd);
}

void	ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
	return ;
}

char	*ft_irc::Client::getBuffer(void){
	return (this->_buffer);
}

void	ft_irc::Client::setBuffer(char *buffer){
	for (int i = 0; i < 2048 && buffer[i]; i++)
		this->_buffer[i] = buffer[i];
	return ;
}

std::string	ft_irc::Client::getNickname(void) const {
	return (this->_nickname);
}

void	ft_irc::Client::setNickname(std::string nickname){
	this->_nickname = nickname;
	return ;
}

std::string	ft_irc::Client::getUsername(void) const {
	return (this->_username);
}

void	ft_irc::Client::setUsername(std::string username){
	this->_username = username;
	return ;
}

std::string	ft_irc::Client::getRealname(void) const {
	return (this->_realname);
}

void	ft_irc::Client::setRealname(std::string realname){
	this->_realname = realname;
	return ;
}

std::string	ft_irc::Client::getPassword(void) const {
	return (this->_password);
}

void	ft_irc::Client::setPassword(std::string password){
	this->_password = password;
	return ;
}

std::string	ft_irc::Client::getHost(void) const {
	return (this->_host);
}

void	ft_irc::Client::setHost(std::string host){
	this->_host = host;
	return ;
}

std::vector<std::string>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

void	ft_irc::Client::setChannels(std::vector<std::string> channels){	//! deep copy
	this->_channels = channels;
	return ;
}

int	ft_irc::Client::read(void){
	bzero(this->_buffer, 2048);
	int	n = recv(this->_sockfd, this->_buffer, 2047, 0);
	if (n < 0)
		throw (std::runtime_error("Error : recv"));
	return (n);
}

int	ft_irc::Client::write(std::string msg){
	int n = send(this->_sockfd, msg.c_str(), msg.length(), 0);
	if (n < 0)
		throw (std::runtime_error("Error : send"));
	return (n);
}
