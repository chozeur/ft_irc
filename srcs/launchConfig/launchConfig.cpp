/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:00:42 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:06:44 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launchConfig.hpp"

launchConfig::launchConfig(){}

launchConfig::~launchConfig(){}

void	launchConfig::setConfig(std::string key, std::string value){

	this->config.insert(std::pair<std::string, std::string>(key, value));
}

std::string	launchConfig::getConfig(std::string key){

	return (this->config[key]);
}
