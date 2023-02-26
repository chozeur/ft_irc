/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/26 19:38:54 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


namespace ft_irc{

	class server{	//TODO respect coplien form

		public:
			server(void);
			server(server const &src);
			~server(void);

			server	&operator=(server const &src);

			struct sockaddr_in	*getServAddr()const;
			void				setConfMap(std::string key, std::string values);
			std::string			getConfMap(std::string key);
			int					getSockfd()const;
			void				setSockfd(int fd);

		private:
			std::map<std::string, std::string> 	_conf_map;
			struct sockaddr_in					*_serv_addr;
			int									_sock_fd;
	};
}

#endif
