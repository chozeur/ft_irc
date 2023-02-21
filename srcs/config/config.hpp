/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:52:34 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 15:18:07 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <map>

namespace irc{
	
	class config {

		public:
			config();
			~config();

			void	setConfig(std::string key, std::string values);
			
			std::string	getConfig(std::string key);
			
		
		private:
			std::map<std::string, std::string> conf_tab;
	};
}

#endif
