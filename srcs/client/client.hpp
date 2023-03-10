/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:32 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/10 14:15:23 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <poll.h>
#include <sstream>

#include <unistd.h>

namespace ft_irc{
	class Client {
		public:
			Client();
			Client(int sockfd);
			Client(Client const &rhs);
			Client& operator=(Client const &rhs);
			~Client();

			// Getters
			int 							getSocket() const;
			std::string 					getPseudo() const;
			std::vector<std::string> 		getChannels() const;
			std::string						getIrssiMessage() const;
			int								getIrssiMessageSize() const;
			
			// Setters
			void							setIrssiMessage(std::string& append);

			// Methods
			std::string 					recvMessage() const;
			void 							joinChannel(const std::string& channel);
			void 							quitChannel(const std::string& channel);
			void 							addMessage(const std::string& message);
			bool							hasMessage(void);
			void							clearIrssiMessage(void);

		private:
			struct pollfd					_fds[1];
			int								_sockfd;
			std::string						_irssi;
			std::string						_pseudo
	};
}

#endif