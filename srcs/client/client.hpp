#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
// # include <string.h>
# include <unistd.h>
# include <algorithm>
# include "../channel/channel.hpp"
# include "../../includes/utils.hpp"


// #include <iostream>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <arpa/inet.h>


namespace ft_irc
{
	class	Channel;

	class Client {

		public:

			Client(void);
			Client(Client const &rhs);
			Client(int sockfd);

			virtual ~Client(void);

			Client	&operator=(Client const & rhs);
			bool 	operator==(const Client &other) const;

			int							getSockfd(void) const;
			std::string					getNickname(void) const;
			std::string					getUsername(void) const;
			std::string					getRealname(void) const;
			std::string					getPassword(void) const;
			std::string					getHost(void) const;
			std::string					getServername(void) const;
			std::string					getUserLine(void) const;
			std::vector<ft_irc::Channel*>	getChannels(void) const;

			void						setSockfd(int sockfd);
			char						*getBuffer(void);
			void						setBuffer(char *buffer);
			void						setNickname(std::string nickname);
			void						setUsername(std::string username);
			void						setRealname(std::string realname);
			void						setPassword(std::string password);
			void						setHost(std::string host);
			void						setServername(std::string servername);
			void						setUserLine(std::string userLine);
			void						setChannels(std::vector<ft_irc::Channel*> channels);
		    void						setIsBot(bool isBot);
		    void						setIsSet(bool isSet);
			bool						isBot() const;
			bool						isSet() const;
			void						handleMessage(int serverSockFd, std::string text, Client *bot, Client *receiver);

			std::string					unicorn(void) const;
			std::string					gpt(std::string prompt) const;

			void 						removeChannel(Channel const &channel);
			void 						addChannel(Channel *channel);
			ft_irc::Channel*			getChanPointer(std::string name);

		private:
			bool						_isSet;
			bool						_isBot;
			int							_sockfd;
			std::string					_nickname;
			std::string					_username;
			std::string					_realname;
			std::string					_password;
			std::string					_host;
			std::string					_servername;
			std::string					_userLine;
			std::vector<ft_irc::Channel*>	_channels;
	};

std::ostream& operator<<(std::ostream& os, const ft_irc::Client& client);

}

#endif
