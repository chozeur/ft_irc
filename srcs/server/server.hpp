/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/10 18:13:04 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string.h>
#include <sstream>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

# define MAX_CLIENTS 4

namespace ft_irc{

	class Server{

		public:
			Server(void);
			Server(Server const &rhs);
			Server(std::string password, int port, char **env);
			~Server(void);
			Server				&operator=(Server const &rhs);

			void				init(std::string password, int port, char **env);
			void				run(void);
			void				stop(void);
			bool				anyoneHasMessage();
			void				newClient(void);
			void				handleClient(void);

		private:
		
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sockfd;
			char								**_env;
			std::vector<Channel>				_channels;
    		std::vector<Client>					_clients;
	};
}

#endif
