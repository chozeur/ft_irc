/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:36 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/02 16:46:10 by tbrebion         ###   ########.fr       */
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


namespace ft_irc{

	class server{

		public:
			server(void);
			server(server const &src);
			~server(void);

			server				&operator=(server const &src);

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
			long								_port;
			std::string							_password;
			struct sockaddr_in					_serv_addr;
			int									_sock_fd;
	};
}

#endif
