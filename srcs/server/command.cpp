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
	ft_irc::Server *server = message->getServer();
	ft_irc::Channel *channel;
    std::vector<Channel> *channels = server->getChannels();

	std::string param2 = param;
	
	if (param2.find("&") == 0) {
		// CANAL PRIVE
		removeAllOccurrences(param2, "&");
		channel = server->getChannelPointer(param2);
		if (!channel) {
			channel = new Channel(param2);
			channels->push_back(*(channel));
		}
		channel->addClient(*(message->getSender()));
		std::string msg = message->getSender()->getNickname() + ":" + + " JOIN &" + channel->getName() + '\n'; 
		send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0);
		
		// send names of clients in the channel to the user who just joined
		std::string names_msg = ":" + server->getIp() + " 353 " + message->getSender()->getNickname() + " = &" + channel->getName() + " :";
		std::vector<Client> clients = channel->getClients();
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
			names_msg += it->getNickname() + " ";
		}
		names_msg += '\n';
		send(message->getSender()->getSockfd(), names_msg.c_str(), names_msg.length(), 0);
	} 
	
	else if (param2.find("#") == 0) {
		// CANAL PUBLIC
		removeAllOccurrences(param2, "#");
		channel = server->getChannelPointer(param2);
		if (!channel) {
			channel = new Channel(param2);
			channels->push_back(*(channel));
		}
		std::cerr << *channel << std::endl;
		channel->addClient(*(message->getSender()));
		std::string msg = message->getSender()->getNickname() + ":" + + " JOIN #" + channel->getName() + '\n'; 
		send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0);
		
		// send names of clients in the channel to the user who just joined
		std::string names_msg = ":" + server->getIp() + " 353 " + message->getSender()->getNickname() + " = #" + channel->getName() + " :";
		std::vector<Client> clients = channel->getClients();
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
			names_msg += it->getNickname() + " ";
		}
		names_msg += '\n';
		std::cerr << *channel << std::endl;
		send(message->getSender()->getSockfd(), names_msg.c_str(), names_msg.length(), 0);
	} 
	
	else {
		// ERROR
	}
}

void ft_irc::Server::kick(ft_irc::Message* message, const std::string& param) {
	(void)message;
    std::cerr << "KICK FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ; 
}

void ft_irc::Server::list(ft_irc::Message* message, const std::string& param) {
	(void)param;
	std::string list = message->getServer()->getName() + " => List of channels :\n";
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