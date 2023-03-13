/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/13 22:16:17 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/* CONSTRUCTORS */

ft_irc::Client::Client(void){
	return ;
}

ft_irc::Client::Client(Client const & rhs){
	*this = rhs;
	return ;
}

ft_irc::Client::Client(
	int sockfd, 
	const std::string nickname, 
	const std::string username, 
	const std::string realname,
	const std::string password,
	const std::string servername,
	const std::string host ) :
	_sockfd(sockfd),
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_password(password),
	_servername(servername),
	_host(host) {
	return ;
}

/* DESTRUCTOR */

ft_irc::Client::~Client(void){
	return ;
}

/* INIT */

ft_irc::Client&	ft_irc::Client::operator=(Client const &rhs){
	if (this != &rhs){
		this->_sockfd = rhs._sockfd;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_password = rhs._password;
		this->_servername = rhs._servername;
		this->_host = rhs._host;
		this->_channels = rhs._channels;
	}
	return (*this);
}

/* GETTERS */

int	ft_irc::Client::getSockfd(void) const {
	return (this->_sockfd);
}

std::string	ft_irc::Client::getNickname(void) const {
	return (this->_nickname);
}

std::string	ft_irc::Client::getMessage(void) const {
	return (this->_message);
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

std::string	ft_irc::Client::getServerName(void) const {
	return (this->_servername);
}

std::string	ft_irc::Client::getHost(void) const {
	return (this->_host);
}

std::vector<std::string>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

/* SETTERS */


void	ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
	std::cout << "client sockfd : " << _sockfd << std::endl;
	return ;
}

void	ft_irc::Client::setMessage(std::string message){
	this->_message += message;
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

void	ft_irc::Client::setServerName(std::string servername){
	this->_servername = servername;
	return ;
}

void	ft_irc::Client::setChannels(std::vector<std::string> channels){	//! deep copy
	this->_channels = channels;
	return ;
}


/* METHODS */

/* OVERLOADS */

std::ostream& ft_irc::operator<<(std::ostream& os, const ft_irc::Client& client)
{
    os << "Client [sockfd : " << client.getSockfd() << ", nickname : " << client.getNickname() << ", realname :" << client.getRealname() << ", servername : " << client.getServerName() << ", hostname : " << client.getHost() << "]";
    return os;
}

