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
    std::cerr << "JOIN FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::kick(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "KICK FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::list(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "LIST FUNCTION CALLED WITH PARAM = " << param << std::endl;
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

