#ifndef SERVER_HPP
#define SERVER_HPP

#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include "../client/client.hpp"
#include "../channel/channel.hpp"
#include "../message/message.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/colors.hpp"

# define MAX_CLIENTS 4

extern bool	server;

typedef void (*CommandFunction)(ft_irc::Message*, const std::string&);

namespace ft_irc{

	class Client;

	class Server{

		public:

			Server(void);
			Server(Server const &rhs);
			Server(std::string password, long port, char **env);

			~Server(void);

			Server											&operator=(Server const &rhs);

			// GETTERS
			std::string										getName(void)const;
			std::string										getIp(void)const;
			long											getPort(void)const;
			std::string										getPassword(void)const;
			int												getTstart(void)const;
			struct sockaddr_in								getServAddr(void)const;
			int												getSockfd(void)const;
			char											**getEnv(void)const;
			std::vector<Client *>							getClients(void);
			Client											*getClientPointerByFd(int fd);
			Client											*getClientPointerByNick(std::string nick);
			std::vector<Client *>::iterator					getClientIterator(int fd);
			std::vector<Channel*>							*getChannels(void);
			Channel											*getChannelPointer(std::string name);
			std::map<std::string, CommandFunction> 			*getCommands(void);
			struct pollfd									*getFds(void);

			// SETTERS
			void											setName(std::string name);
			void											setIp(std::string ip);
			void											setPort(long port);
			void											setPassword(std::string password);
			void											setServAddr(struct sockaddr_in);
			void											setSockfd(int fd);
			void											setEnv(char **env);

			// METHODS
			void											init(std::string password, long port, char **env);
			void											initCommands(void);
			void											initChannels(void);
			void											run(void);
			void											stop(void);
			void											printClients(void);
			bool 											parsingNickname(int fd, std::string nickname);
			bool 											parsingPassword(std::string password)const;
			void											sendIrcResponse(int sockfd, ft_irc::Client *client) const;
			void											closeClient(int i);
			void											sendToAllClients(std::string &msg);
			int 											uptime(void) const;
			std::string										HRuptime(void) const;
			std::string										HRdate(void) const;
			void											purgeChannels(void);
			void											purgeClients(void);
			void											logLoop(void) const;
			std::vector<std::string>						tenLastLogs(void) const;
			// void											updateChan()

			//SERVER COMMANDS
			static void										cap(ft_irc::Message* Message, const std::string& param);
			static void										pass(ft_irc::Message* Message, const std::string& param);
			static void										nick(ft_irc::Message* Message, const std::string& param);
			static void										user(ft_irc::Message* Message, const std::string& param);
			static void										invite(ft_irc::Message* Message, const std::string& param);
			static void										join(ft_irc::Message* Message, const std::string& param);
			static void										kick(ft_irc::Message* Message, const std::string& param);
			static void										list(ft_irc::Message* Message, const std::string& param);
			static void										names(ft_irc::Message* Message, const std::string& param);
			static void										whois(ft_irc::Message* Message, const std::string& param);
			static void										privmsg(ft_irc::Message* message, const std::string& param);
			static void										part(ft_irc::Message* message, const std::string& param);
			static void										pong(ft_irc::Message* message, const std::string& param);
			static void										quit(ft_irc::Message* message, const std::string& param);
			static void										info(ft_irc::Message* message, const std::string& param);
			static void										topic(ft_irc::Message* message, const std::string& param);
			static void										mode(ft_irc::Message* message, const std::string& param);

		private:
			std::string 									_name;
			std::string										_ip;
			long											_port;
			std::string										_password;
			int												_tstart;

			struct pollfd									_fds[MAX_CLIENTS + 1];
			struct sockaddr_in								_serv_addr;
			int												_sockfd;
			char											**_env;
			std::vector<Client *>							_clients;
			std::vector<Channel *>							_channels;
			std::map<std::string, CommandFunction>			_commands;
	};

	extern std::ofstream log_file;
	class LogStream : public std::ostream {
        public:
            LogStream(std::ostream& os);
    };
	extern LogStream cout;
}

#endif
