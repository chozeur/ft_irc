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
    ft_irc::Server *server = message->getServer();
    ft_irc::Channel *channel;
    std::vector<Channel*> *channels = server->getChannels();

    std::string param2 = param;

    if (param2.find("#") == 0) {
        // CANAL PUBLIC
        removeAllOccurrences(param2, "#");
        channel = server->getChannelPointer(param2);
        if (!channel) {
            channel = new Channel(param2);
            channels->push_back(channel);
        }
        channel->addClient(message->getSender());
        std::string msg = message->getSender()->getNickname() + ":" + + " JOIN #" + channel->getName() + "\r\n";

        // send welcome message to the joining client
        send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0);

		// send names of clients in the channel to all clients except the joining client
		std::string names_msg = ":" + message->getServer()->getIp() + " 353 " + message->getSender()->getNickname() + " = #" + channel->getName() + " :";
		const std::vector<Client *>& clients = channel->getClients();
		for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
			if ((*it)->getSockfd() != message->getSender()->getSockfd()) {
				names_msg += (*it)->getNickname() + " ";
				send((*it)->getSockfd(), names_msg.c_str(), names_msg.length(), 0);
			}
		}
		names_msg += "\r\n";
        
        // send JOIN message to all clients in the channel
        std::string join_msg = message->getSender()->getNickname() + " JOIN #" + channel->getName() + "\r\n";
        for (std::vector<Channel*>::iterator it = channels->begin(); it != channels->end(); ++it) {
            std::vector<Client *> clients = (*it)->getClients();
            for (std::vector<Client *>::iterator it2 = clients.begin(); it2 != clients.end(); ++it2) {
                if ((*it2)->getSockfd() != message->getSender()->getSockfd()) {
                    send((*it2)->getSockfd(), join_msg.c_str(), join_msg.length(), 0);
                }
            }
        }
    } else {
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
    std::vector<Channel*> *channels = message->getServer()->getChannels();
    for (std::vector<Channel*>::iterator it = channels->begin(); it != channels->end(); ++it) {
        std::string test = "-- " + (*it)->getName() + "\n";
        list += test;
    }
    send(fd, list.c_str(), list.length(), 0);
    return;
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

void	ft_irc::Server::nick(ft_irc::Message* message, const std::string& param) {
	(void)message;
	std::cerr << "NICK FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ;
}
