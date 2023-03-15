#include "server.hpp"

void ft_irc::Server::initCommands(void) {
	_commands.insert(std::make_pair("INVITE", &Server::invite));
	_commands.insert(std::make_pair("JOIN", &Server::join));
	_commands.insert(std::make_pair("KICK", &Server::kick));
	_commands.insert(std::make_pair("LIST", &Server::list));
	_commands.insert(std::make_pair("NAMES", &Server::names));
	_commands.insert(std::make_pair("WHOIS", &Server::whois));
}

void ft_irc::Server::invite(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "INVITE FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::join(ft_irc::Message* message, const std::string& param) {
	(void)message;
	std::string param2 = param;
    std::vector<Channel> channels = *(message->getServer()->getChannels());
	removeAllOccurrences(param2, "#");

	ft_irc::Channel *channel = getChannelPointer(param2);
	return ; 	
}

void ft_irc::Server::kick(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "KICK FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::list(ft_irc::Message* message, const std::string& param) {
	(void)param;
	std::string list = "IRC SERVER => List of channels :\n";
	int fd = message->getSender()->getSockfd();
	std::vector<Channel> channels = *(message->getServer()->getChannels());
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		std::string test = "-- " + (*it).getName() + "\n";
		list += test;
	}
	send(fd, list.c_str(), list.length(), 0);
	return ; 
}

void ft_irc::Server::names(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "NAMES FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::whois(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "WHOIS FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

