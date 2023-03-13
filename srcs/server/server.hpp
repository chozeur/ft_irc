/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/13 13:21:04 by flcarval         ###   ########.fr       */
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
#include <poll.h>
#include "../client/client.hpp"

# define MAX_CLIENTS 4

namespace ft_irc{

	class Server{

		public:

			Server(void);
			Server(Server const &rhs);
			Server(std::string password, long port, char **env);

			~Server(void);

			Server				&operator=(Server const &rhs);

			long				getPort(void)const;
			std::string			getPassword(void)const;
			struct sockaddr_in	getServAddr(void)const;
			int					getSockfd(void)const;
			char				**getEnv(void)const;

			void				setPort(long port);
			void				setPassword(std::string password);
			void				setServAddr(struct sockaddr_in);
			void				setSockfd(int fd);
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
