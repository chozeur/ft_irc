/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/14 15:24:01 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/* CONSTRUCTORS */

ft_irc::Client::Client(void){
	std::cout << "New Client created" << std::endl;
	return ;
}

ft_irc::Client::Client(Client const & rhs){
	*this = rhs;
	return ;
}

ft_irc::Client::Client(int sockfd):
	_sockfd(sockfd),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host("") {
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

std::string	ft_irc::Client::getServername(void) const {
	return (this->_servername);
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

void	ft_irc::Client::setServername(std::string servername){
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
    os << "Client [sockfd : " << client.getSockfd() << ", nickname : " << client.getNickname() << ", realname :" << client.getRealname() << ", servername : " << client.getServername() << ", hostname : " << client.getHost() << "]";
    return os;
}