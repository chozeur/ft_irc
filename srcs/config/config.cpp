/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:34:39 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/26 18:37:08 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

ft_irc::config::config(){}

ft_irc::config::~config(){}

void	ft_irc::config::setConfig(std::string key, std::string value){
	this->conf_tab[key] = value;
}

std::string	ft_irc::config::getConfig(std::string key){
	return (this->conf_tab[key]);
}
