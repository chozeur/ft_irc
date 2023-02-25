/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/25 19:52:25 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "../config/config.hpp"


namespace irc{

	class server{

		public:
			server();
			~server();

			irc::config &getConf();
			struct sockaddr_in	*getServAddr()const;
			int	getSockfd()const;
			void	setSockfd(int fd);
			
		private:
			irc::config conf;
			struct sockaddr_in *serv_addr;
			int sock_fd;
	};
}

#endif
