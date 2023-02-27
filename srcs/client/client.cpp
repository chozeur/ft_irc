/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/27 13:51:29 by flcarval         ###   ########.fr       */
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

void	ft_irc::Client::setChannels(std::vector<std::string> channels){
	this->_channels = channels;
	return ;
}
