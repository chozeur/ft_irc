#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>

namespace ft_irc
{
	class Client {

		public:

			Client(void);
			Client(Client const &rhs);
			Client(	const int sockfd, const std::string nickname, const std::string username, const std::string realname,const std::string password, const std::string servername, const std::string host);

			~Client(void);

			Client	&operator=(Client const & rhs);

			int							getSockfd(void) const;
			char						*getBuffer(void);
			std::string					getNickname(void) const;
			std::string					getMessage(void) const;
			std::string					getUsername(void) const;
			std::string					getRealname(void) const;
			std::string					getPassword(void) const;
			std::string					getServerName(void) const;
			std::string					getHost(void) const;
			std::vector<std::string>	getChannels(void) const;

			void						setSockfd(int sockfd);
			void						setBuffer(char *buffer);
			void						setNickname(std::string nickname);
			void						setMessage(std::string nickname);
			void						setUsername(std::string username);
			void						setRealname(std::string realname);
			void						setPassword(std::string password);
			void						setServerName(std::string host);
			void						setHost(std::string host);
			void						setChannels(std::vector<std::string> channels);

		private:
			int							_sockfd;
			std::string					_nickname;
			std::string					_message;
			std::string					_username;
			std::string					_realname;
			std::string					_password;
			std::string					_servername;
			std::string					_host;
			std::vector<std::string>	_channels;
	};

std::ostream& operator<<(std::ostream& os, const ft_irc::Client& client);

}

#endif
