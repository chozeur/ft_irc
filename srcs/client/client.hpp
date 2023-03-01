/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:32 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/01 15:06:38 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>

namespace ft_irc
{
	class Client {
		public:
			Client(void);
			Client(Client const & src);
			
			~Client(void);

			Client	&operator=(Client const & rhs);

			struct sockaddr_in			getCliAddr()const;
			std::string					getNickname(void) const;
			void						setNickname(std::string nickname);
			std::string					getUsername(void) const;
			void						setUsername(std::string username);
			std::string					getRealname(void) const;
			void						setRealname(std::string realname);
			std::string					getPassword(void) const;
			void						setPassword(std::string password);
			std::string					getHost(void) const;
			void						setHost(std::string host);
			std::vector<std::string>	getChannels(void) const;
			void						setChannels(std::vector<std::string> channels);

		private:
			struct sockaddr_in			_cli_addr;
			std::string					_nickname;
			std::string					_username;
			std::string					_realname;
			std::string					_password;
			std::string					_host;
			std::vector<std::string>	_channels;
	};
}

#endif
