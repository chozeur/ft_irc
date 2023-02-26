/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:00:42 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:18:28 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

irc::config::config(){}

irc::config::~config(){}

void	irc::config::setConfig(std::string key, std::string value){

	this->conf_tab.insert(std::pair<std::string, std::string>(key, value));
	// could be done with
	this->conf_tab[key] = value;
}

std::string	irc::config::getConfig(std::string key){

	return (this->conf_tab[key]);
}
