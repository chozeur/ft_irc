/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:00:42 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:18:28 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launchConfig.hpp"

irc::launchConfig::launchConfig(){}

irc::launchConfig::~launchConfig(){}

void	irc::launchConfig::setConfig(std::string key, std::string value){

	this->config.insert(std::pair<std::string, std::string>(key, value));
}

std::string	irc::launchConfig::getConfig(std::string key){

	return (this->config[key]);
}
