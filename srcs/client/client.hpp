#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
#include <algorithm>


namespace ft_irc
{
	class Client {

		public:
		
			Client(void);
			Client(Client const &rhs);
			Client(int sockfd);

			~Client(void);

			Client	&operator=(Client const & rhs);
			bool 	operator==(const Client &other) const;

			int							getSockfd(void) const;
			std::string					getNickname(void) const;
			std::string					getUsername(void) const;
			std::string					getRealname(void) const;
			std::string					getPassword(void) const;
			std::string					getHost(void) const;
			std::string					getServername(void) const;
			std::vector<std::string>	getChannels(void) const;

			void						setSockfd(int sockfd);
			char						*getBuffer(void);
			void						setBuffer(char *buffer);
			void						setNickname(std::string nickname);
			void						setUsername(std::string username);
			void						setRealname(std::string realname);
			void						setPassword(std::string password);
			void						setHost(std::string host);
			void						setServername(std::string servername);
			void						setChannels(std::vector<std::string> channels);

		private:
			int							_sockfd;
			std::string					_nickname;
			std::string					_username;
			std::string					_realname;
			std::string					_password;
			std::string					_host;
			std::string					_servername;
			std::vector<std::string>	_channels;
	};

std::ostream& operator<<(std::ostream& os, const ft_irc::Client& client);

}

#endif
