/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launchConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:52:34 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:06:36 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAUNCHCONFIG_HPP
#define LAUNCHCONFIG_HPP

#include <iostream>
#include <map>

class launchConfig {

	public:
		launchConfig();
		~launchConfig();

		void	setConfig(std::string key, std::string values);
		
		std::string	getConfig(std::string key);
		
	
	private:
		std::map<std::string, std::string> config;
};

#endif
