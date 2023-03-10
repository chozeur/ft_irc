/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/10 15:18:56 by rvrignon         ###   ########.fr       */
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
			struct pollfd						_fds[MAX_CLIENTS + 1];
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sockfd;
			char								**_env;
	};
}

#endif
