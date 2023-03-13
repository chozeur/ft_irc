/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/13 20:48:09 by rvrignon         ###   ########.fr       */
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
#include "../message/message.hpp"

# define MAX_CLIENTS 4

extern bool	server;

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
			Client				findClient(int fd);
			int					clientInit(int fd, std::string message);
			void				clientCommand(int fd, std::string message);
			int					createClient(const int sockfd, const std::string nickname, const std::string username, const std::string realname,const std::string password,const std::string servername,const std::string host);
			void				sendIrcResponse(const int sockfd, ft_irc::Client client);
		private:
			std::vector<struct pollfd>			_fds;
			std::vector<Client>					_clients;
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sockfd;
			char								**_env;
	};
}

#endif