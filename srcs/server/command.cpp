#include "server.hpp"

void ft_irc::Server::initCommands(void) {
	_commands.insert(std::make_pair("INVITE", &Server::invite));
	_commands.insert(std::make_pair("JOIN", &Server::join));
	_commands.insert(std::make_pair("KICK", &Server::kick));
	_commands.insert(std::make_pair("LIST", &Server::list));
	_commands.insert(std::make_pair("NAMES", &Server::names));
	_commands.insert(std::make_pair("WHOIS", &Server::whois));
	_commands.insert(std::make_pair("NICK", &Server::nick));
	_commands.insert(std::make_pair("PRIVMSG", &Server::privmsg));

}

void ft_irc::Server::invite(ft_irc::Message* message, const std::string& param) {
	(void)message;
	std::cerr << "INVITE FUNCTION CALLED WITH PARAM = " << param << std::endl;
	return ;
}

void ft_irc::Server::join(ft_irc::Message* message, const std::string& param) {
    // On récupère le serveur, le canal et la liste des canaux du serveur
    ft_irc::Server *server = message->getServer();
    ft_irc::Channel *channel;
    std::vector<Channel*> *channels = server->getChannels();

    // On supprime le caractère '#' au début du paramètre pour récupérer le nom du canal
    std::string param2 = param;
    removeAllOccurrences(param2, "#");

    // Si le canal n'existe pas, on le crée et on l'ajoute à la liste des canaux du serveur
    channel = server->getChannelPointer(param2);
    if (!channel) {
        channel = new Channel(param2);
        channels->push_back(channel);
    }

    // On ajoute l'utilisateur qui a envoyé le message au canal
    channel->addClient(message->getSender());

    // On envoie un message de bienvenue à l'utilisateur qui a rejoint le canal
    std::string msg = message->getSender()->getNickname() + ":" + " JOIN #" + channel->getName() + "\r\n";
    if (send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
        std::cerr << "Error SEND" << std::endl;
    }

    // AU client qui rejoint
    // ------------------------------

    // On envoie la liste des noms des clients présents dans le canal à l'utilisateur qui vient de rejoindre le canal
    std::string names_msg = ":" + message->getServer()->getIp() + " 353 " + message->getSender()->getNickname() + " = #" + channel->getName() + " :";
    const std::vector<Client *>& clients = channel->getClients();

    // On parcourt la liste des clients dans le canal et on ajoute leur nom au message
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        names_msg += " " + (*it)->getNickname();
    }

    // On termine le message avec un espace et un retour à la ligne
    names_msg += " \r\n";

    // std::cerr << names_msg << std::endl;

    // A tous les clients presents ds le canal
    // ---------------------------------

    // On envoie le message de la liste des noms des clients présents dans le canal à tous les clients du canal
    for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (send((*it)->getSockfd(), names_msg.c_str(), names_msg.length(), 0) == -1) {
            std::cerr << "2 ERROR SEND" << std::endl;
        }
    }


    // send JOIN message to all clients in the channel
    std::string join_msg = message->getSender()->getNickname() + ": JOIN #" + channel->getName() + "\r\n";
    std::vector<Client *> channel_clients = channel->getClients();
    for (std::vector<Client *>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it) {
        if ((*it)->getSockfd() != message->getSender()->getSockfd()) {
            if (send((*it)->getSockfd(), join_msg.c_str(), join_msg.length(), 0) == -1) {
                std::cerr << "3 ERROR SEND" << std::endl;
            }
        }
    }
}

void ft_irc::Server::privmsg(ft_irc::Message* message, const std::string& param) {
    // On récupère le serveur, le canal et la liste des canaux du serveur
    ft_irc::Server *server = message->getServer();
    ft_irc::Channel *channel;
    std::string channelName;
    std::string messageText;

    std::string param2 = param;

    std::size_t pos = param2.find('#');
    if (pos != std::string::npos) {
        pos++;
        std::size_t endpos = param2.find(" :", pos);
        if (endpos != std::string::npos) {
            channelName = param2.substr(pos, endpos - pos);
        }
    }

    std::size_t messagePos = param2.find(" :");
    if (messagePos != std::string::npos) {
        messageText = param2.substr(messagePos + 2);
    }


    removeAllOccurrences(param2, "#");

    // Si le paramètre commence par un '#', c'est un message à envoyer à un canal
    if (param[0] == '#') {
        // On récupère le canal correspondant au nom
        channel = server->getChannelPointer(channelName);
        if (!channel) {
            // Si le canal n'existe pas, on envoie un message d'erreur à l'utilisateur
            std::string msg = ":" + server->getIp() + " 401 " + message->getSender()->getNickname() + " " + param2 + " :No such channel\r\n";
            if (send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
                std::cerr << "Error SEND" << std::endl;
            }
        } else {
            // On envoie le message à tous les clients du canal, sauf celui qui a envoyé le message
            std::string msg = ":" + message->getSender()->getNickname() + "!" + message->getSender()->getUsername() + "@" + message->getSender()->getHost() + " PRIVMSG #" + channel->getName() + " :" + messageText + "\r\n";
            std::vector<Client *> channel_clients = channel->getClients();
            for (std::vector<Client *>::iterator it = channel_clients.begin(); it != channel_clients.end(); ++it) {
                if ((*it)->getNickname() != message->getSender()->getNickname()) {
                    if (send((*it)->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
                        std::cerr << "Error SEND" << std::endl;
                    }
                }
            }
        }
    } else {
        // Si le paramètre ne commence pas par un '#', c'est un message à envoyer à un client
        Client *client = server->getClientPointer(message->getSender()->getSockfd());
        if (!client) {
            // Si le client n'existe pas, on envoie un message d'erreur à l'utilisateur
            std::string msg = ":" + server->getIp() + " 401 " + message->getSender()->getNickname() + " " + param2 + " :No such nick/channel\r\n";
            if (send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
                std::cerr << "Error SEND" << std::endl;
            }
        } else {
            // Si le client existe, on envoie le message à ce client
            std::string msg = ":" + message->getSender()->getNickname() + "!" + message->getSender()->getUsername() + "@" + message->getSender()->getHost() + " PRIVMSG " + client->getNickname() + " :" + param2 + "\r\n";
            if (send(client->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
                std::cerr << "Error SEND" << std::endl;
            }
        }
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
	std::cerr << "Sender = " << *(message->getSender()) << std::endl;
	if (message->getServer()->parsingNickname(param)){
		std::string nick_res = ":" + message->getSender()->getNickname() + "!" + message->getSender()->getUsername() + "@" + message->getSender()->getHost() +" NICK :" + param;
		message->getSender()->setNickname(param);
		send(message->getSender()->getSockfd(), nick_res.c_str(), nick_res.length(), 0);
	}
	else {
		std::string nick_res = ":" + message->getServer()->getIp() + " 433 * " + param + ":Nickname is already in use.";
		send(message->getSender()->getSockfd(), nick_res.c_str(), nick_res.length(), 0);
	}
	return ;
}
