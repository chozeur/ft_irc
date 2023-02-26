/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/27 00:21:49 by flcarval         ###   ########.fr       */
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

			std::string			getConfMap(std::string key);
			void				setConfMap(std::string key, std::string values);
			long				getPort(void)const;
			void				setPort(long port);
			std::string			getPassword(void)const;
			void				setPassword(std::string password);
			struct sockaddr_in	getServAddr(void)const;
			void				setServAddr(long port);
			int					getSockfd(void)const;
			void				setSockfd(int fd);

			void				init(std::string password, long port);

		private:
			std::map<std::string, std::string>	_conf_map;
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sock_fd;
	};
}

#endif
