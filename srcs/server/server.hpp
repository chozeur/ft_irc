/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/06 13:36:12 by flcarval         ###   ########.fr       */
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

	class Server{

		public:
			Server(void);
			Server(Server const &src);
			Server(std::string password, long port, char **env);
			~Server(void);

			Server				&operator=(Server const &src);

			long				getPort(void)const;
			void				setPort(long port);
			std::string			getPassword(void)const;
			void				setPassword(std::string password);
			struct sockaddr_in	getServAddr(void)const;
			void				setServAddr(struct sockaddr_in);
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
