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
#include "../channel/channel.hpp"
#include "../message/message.hpp"
#include "../../includes/utils.hpp"

# define MAX_CLIENTS 4

extern bool	server;

namespace ft_irc{

	class Server{

		public:

			Server(void);
			Server(Server const &rhs);
			Server(std::string password, long port, char **env);

			~Server(void);

			Server									&operator=(Server const &rhs);

			long													getPort(void)const;
			std::string												getPassword(void)const;
			struct sockaddr_in										getServAddr(void)const;
			int														getSockfd(void)const;
			char													**getEnv(void)const;
			Client													*getClientPointer(int fd);
			std::vector<Client>::iterator							getClientIterator(int fd);
			std::map<std::string, void(*)(std::string)>				getCommands(void)const;

			void													setPort(long port);
			void													setPassword(std::string password);
			void													setServAddr(struct sockaddr_in);
			void													setSockfd(int fd);
			void													setEnv(char **env);

			void													init(std::string password, long port, char **env);
			void													run(void);
			void													stop(void);
			int														clientInit(int fd, std::string message);
			int 													parsingNickname(std::string nickname);
			int 													parsingPassword(std::string password)const;
			void													sendIrcResponse(int sockfd, ft_irc::Client *client) const;
			void													closeClient(int i);
		private:
			struct pollfd											_fds[MAX_CLIENTS + 1];
			long													_port;
			std::string												_password;
			struct sockaddr_in										_serv_addr;
			int														_sockfd;
			char													**_env;
			std::vector<Client>										_clients;
			std::vector<Channel>									_channels;
			std::map<std::string, void(*)(std::string)>				_commands;
	};
}

#endif
