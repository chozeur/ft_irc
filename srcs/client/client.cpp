/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/05 23:10:45 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

ft_irc::Client::Client(void){return ;}

ft_irc::Client::Client(Client const & src){
	*this = src;
	return ;
}

ft_irc::Client::~Client(void){return ;}

ft_irc::Client&	ft_irc::Client::operator=(Client const &src){
	if (this != &src){
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
	for (int i = 0; i < 1024 && buffer[i]; i++)
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
