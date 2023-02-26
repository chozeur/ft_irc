/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 19:00:47 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../config/config.hpp"


namespace ft_irc{

	class server{	//TODO respect coplien form

		public:
			server();
			~server();

			typedef ft_irc::config config;

			config 				&getConf();
			struct sockaddr_in	*getServAddr()const;
			int					getSockfd()const;
			void				setSockfd(int fd);

		private:
			config				conf;
			struct sockaddr_in	*serv_addr;
			int					sock_fd;
	};
}

#endif
