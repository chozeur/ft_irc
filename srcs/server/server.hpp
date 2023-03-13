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
#include <fcntl.h>
#include "../client/client.hpp"
#include "../message/message.hpp"

# define MAX_CLIENTS 3

extern bool	server;

namespace ft_irc{

	class Server{

		public:

			Server(void);
			Server(Server const &rhs);
			Server(std::string password, long port, char **env);

			~Server(void);

			Server									&operator=(Server const &rhs);

		
			long									getPort(void)const;
			std::string								getPassword(void)const;
			struct sockaddr_in						getServAddr(void)const;
			int										getSockfd(void)const;
			char									**getEnv(void)const;
			Client									*getClientPointer(int fd);
			std::vector<Client>::iterator 			getClientIterator(int fd);

			void									setPort(long port);
			void									setPassword(std::string password);
			void									setServAddr(struct sockaddr_in);
			void									setSockfd(int fd);
			void									setEnv(char **env);

			void									init(std::string password, long port, char **env);
			void									run(void);
			int										clientInit(int fd, std::string message);
			void									clientCommand(int fd, std::string message);
			int										createClient(const int sockfd, const std::string nickname, const std::string username, const std::string realname,const std::string password,const std::string servername,const std::string host);
			void									sendIrcResponse(int sockfd, ft_irc::Client *client) const;
		private:
			std::vector<struct pollfd>				_fds;
			std::vector<Client>						_clients;
			long									_port;
			std::string								_password;
			struct sockaddr_in						_serv_addr;
			int										_sockfd;
			char									**_env;
	};
}

#endif