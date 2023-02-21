/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 17:01:32 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../config/config.hpp"


namespace irc{

	class server{

		public:
			server();
			~server();

			irc::config &getConf();
			
		private:
			irc::config conf;
	};
}

#endif
