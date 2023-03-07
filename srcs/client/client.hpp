/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:32 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/07 22:31:26 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include "../server/server.hpp"


namespace ft_irc
{
	class Client {

		public:
		
			Client(void);
			Client(Client const &rhs);
			Client(ft_irc::Server &server);

			~Client(void);

			Client	&operator=(Client const & rhs);

			struct sockaddr_in			getCliAddr(void) const;
			socklen_t					getCliLen(void) const;
			int							getSockfd(void) const;
			std::string					getNickname(void) const;
			std::string					getUsername(void) const;
			std::string					getRealname(void) const;
			std::string					getPassword(void) const;
			std::string					getHost(void) const;
			std::vector<std::string>	getChannels(void) const;

			void						setCliAddr(struct sockaddr_in cli_addr);
			void						setCliLen(socklen_t cli_len);
			void						setSockfd(int sockfd);
			char						*getBuffer(void);
			void						setBuffer(char *buffer);
			void						setNickname(std::string nickname);
			void						setUsername(std::string username);
			void						setRealname(std::string realname);
			void						setPassword(std::string password);
			void						setHost(std::string host);
			void						setChannels(std::vector<std::string> channels);

			int							read(void);
			int							write(std::string message);

		private:

			ft_irc::Server				*_server;
			struct sockaddr_in			_cli_addr;
			socklen_t					_cli_len;
			int							_sockfd;
			char						_buffer[2048];
			std::string					_nickname;
			std::string					_username;
			std::string					_realname;
			std::string					_password;
			std::string					_host;
			std::vector<std::string>	_channels;
	};
}

#endif
