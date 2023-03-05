/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/05 22:52:31 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string.h>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


namespace ft_irc{

	class server{

		public:
			server(void);
			server(server const &src);
			server(std::string password, long port, char **env);
			~server(void);

			server				&operator=(server const &src);

			long				getPort(void)const;
			void				setPort(long port);
			std::string			getPassword(void)const;
			void				setPassword(std::string password);
			struct sockaddr_in	getServAddr(void)const;
			void				setServAddr(void);
			int					getSockfd(void)const;
			void				setSockfd(int fd);
			char				**getEnv(void)const;
			void				setEnv(char **env);

			void				init(std::string password, long port, char **env);
			void				run(void);

		private:
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sockfd;
			char								**_env;
	};
}

#endif
