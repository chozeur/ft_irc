/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:13:49 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/07 22:40:47 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

/* CONSTRUCTORS */

ft_irc::Channel::Channel(void){return ;}

ft_irc::Channel::Channel(Channel const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Channel::Channel(std::string name): _name(name){return ;}

/* DESTRUCTOR */

ft_irc::Channel::~Channel(void){return ;}

/* INIT */

ft_irc::Channel&	ft_irc::Channel::operator=(Channel const &rhs){
	if (this != &rhs){
		this->_name = rhs._name;
		this->_clients = rhs._clients;
		this->_operators = rhs._operators;
		this->_banned_clients = rhs._banned_clients;
	}
	return (*this);
}

/* GETTERS */

std::string	ft_irc::Channel::getName(void) const {
	return (this->_name);
}

std::vector<ft_irc::Client>	ft_irc::Channel::getClients(void) const {
	return (this->_clients);
}

std::vector<ft_irc::Client>	ft_irc::Channel::getOperators(void) const {
	return (this->_operators);
}

std::vector<ft_irc::Client>	ft_irc::Channel::getBannedClients(void) const {
	return (this->_banned_clients);
}

/* SETTERS */

void	ft_irc::Channel::setName(std::string name){
	this->_name = name;
	return ;
}

void	ft_irc::Channel::setClients(std::vector<ft_irc::Client> clients){
	this->_clients = clients;
	return ;
}

void	ft_irc::Channel::setOperators(std::vector<ft_irc::Client> operators){
	this->_operators = operators;
	return ;
}

void	ft_irc::Channel::setBannedClients(std::vector<ft_irc::Client> banned_clients){
	this->_banned_clients = banned_clients;
	return ;
}
