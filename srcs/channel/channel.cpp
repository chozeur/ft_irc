/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:13:49 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/27 13:45:57 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

ft_irc::Channel::Channel(void){return ;}

ft_irc::Channel::Channel(Channel const &src){
	*this = src;
	return ;
}

ft_irc::Channel::Channel(std::string name){
	this->_name = name;
	return ;
}

ft_irc::Channel::~Channel(void){return ;}

ft_irc::Channel&	ft_irc::Channel::operator=(Channel const &src){
	if (this != &src){
		this->_name = src._name;
	}
	return (*this);
}

std::string	ft_irc::Channel::getName(void) const {
	return (this->_name);
}

void	ft_irc::Channel::setName(std::string name){
	this->_name = name;
	return ;
}
