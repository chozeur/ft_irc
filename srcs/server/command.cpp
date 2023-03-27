#include "server.hpp"

void ft_irc::Server::initCommands(void) {
	_commands.insert(std::make_pair("CAP", &Server::cap));
	_commands.insert(std::make_pair("PASS", &Server::pass));
	_commands.insert(std::make_pair("NICK", &Server::nick));
	_commands.insert(std::make_pair("USER", &Server::user));

	_commands.insert(std::make_pair("INVITE", &Server::invite));
	_commands.insert(std::make_pair("JOIN", &Server::join));
	_commands.insert(std::make_pair("KICK", &Server::kick));
	_commands.insert(std::make_pair("LIST", &Server::list));
	_commands.insert(std::make_pair("NAMES", &Server::names));
	_commands.insert(std::make_pair("WHOIS", &Server::whois));
	_commands.insert(std::make_pair("PRIVMSG", &Server::privmsg));
	_commands.insert(std::make_pair("PART", &Server::part));

}

// INITIALISATION CLIENT

void ft_irc::Server::cap(ft_irc::Message* message, const std::string& param) {
	ft_irc::Server *server = message->getServer();
    std::string param2  = param;
    std::string::size_type pos = param2.find('\n');
    std::string first;
    std::string rest;

    if (pos != std::string::npos) {
        first = param2.substr(0, pos);
        rest = param2.substr(pos + 1);
    }

    std::istringstream iss(first);
    std::string command, argument;
    iss >> command >> argument;

    if (argument == "LS")
        std::cout << "\033[1m" << server->getName() << "\033[0m" << " => " << first << std::endl;

    if (!rest.empty() && rest.size() > 1 && argument == "LS") {
        pass(message, rest);
    }

	return ;
}

void ft_irc::Server::pass(ft_irc::Message* message, const std::string& param) {
    ft_irc::Server *server = message->getServer();
    std::string param2  = param;
    std::string::size_type pos = param2.find('\n');
    std::string first;
    std::string rest;

    if (pos != std::string::npos) {
        first = param2.substr(0, pos);
        rest = param2.substr(pos + 1);
    }

    cleanLine(first);
    removeAllOccurrences(first, "\n");
    std::cout << "\033[1m" << server->getName() << "\033[0m" << " => " << first << std::endl;
    std::string::size_type space_pos = first.find(' ');
    server->getClientPointerByFd(message->getSender()->getSockfd())->setPassword(first.substr(space_pos + 1));

    if (!rest.empty() && rest.size() > 1) {
        nick(message, rest);
    }
	return ;
}

void ft_irc::Server::nick(ft_irc::Message* message, const std::string& param) {
    ft_irc::Server *server = message->getServer();
    ft_irc::Client *client = server->getClientPointerByFd(message->getSender()->getSockfd());

    std::string param2 = param;
    std::string::size_type pos = param2.find('\n');
    std::string first;
    std::string rest;

    if (pos != std::string::npos) {
        first = param2.substr(0, pos);
        rest = param2.substr(pos + 1);
    }

    cleanLine(first);
    removeAllOccurrences(first, "\n");

    std::cout << "\033[1m" << server->getName() << "\033[0m" << " => " << first << std::endl;

    std::string::size_type space_pos = first.find(' ');
    std::string nickname = first.substr(space_pos + 1);

    if (nickname.empty()) {
        std::string nick_res = ":" + server->getIp() + " 433 * " + nickname + " NICK :Nickname can't be empty. Usage /nick newNickname\r\n";
        send(client->getSockfd(), nick_res.c_str(), nick_res.length(), 0);
        if (!rest.empty() && rest.size() > 1)
            client->setUserLine(rest);
        return;
    }

    if (!server->parsingNickname(client->getSockfd(), nickname)) {
        std::string nick_res = ":" + server->getIp() + " 433 * " + nickname + " NICK :Nickname is already in use. Usage /nick freeNickname\r\n";
        send(client->getSockfd(), nick_res.c_str(), nick_res.length(), 0);
        if (!rest.empty() && rest.size() > 1)
            client->setUserLine(rest);
        return ;
    }

    if (!client->getNickname().empty()) {
        // L'utilisateur a déjà un nickname, nous devons donc mettre à jour son nickname
        std::string oldNickname = client->getNickname();
        client->setNickname(nickname);

        // Envoie un message d'information à tous les clients du serveur
        std::string msg = ":" + oldNickname + "!" + client->getUsername() + "@" + client->getHost() + " NICK :" + nickname + "\r\n";
        server->sendToAllClients(msg);
    } else {
        // L'utilisateur n'a pas encore de nickname, nous devons simplement enregistrer son nickname
        client->setNickname(nickname);
    }

    if (!rest.empty() && rest.size() > 1)
        user(message, rest); 
    else if (!client->isSet() && !client->getUserLine().empty())
        user(message, client->getUserLine());
}

void ft_irc::Server::user(ft_irc::Message* message, const std::string& param) {
    ft_irc::Server *server = message->getServer();
    std::string line  = param;

    cleanLine(line);
    removeAllOccurrences(line, "\n");
    std::cout << "\033[1m" << server->getName() << "\033[0m" << " => " << line << std::endl;
    std::string::size_type space_pos1 = line.find(' ');
    std::string::size_type space_pos2 = line.find(' ', space_pos1 + 1);
    std::string::size_type space_pos3 = line.find(' ', space_pos2 + 1);
    std::string::size_type colon_pos = line.find(':');
    std::string::size_type end_pos = line.find('\n');

    std::string username = line.substr(space_pos1 + 1, space_pos2 - space_pos1 - 1);
    std::string hostname = line.substr(space_pos2 + 1, space_pos3 - space_pos2 - 1);
    std::string servername = line.substr(space_pos3 + 1, colon_pos - space_pos3 - 2);
    std::string realname = line.substr(colon_pos + 1, end_pos - 1);

    ft_irc::Client *client = server->getClientPointerByFd(message->getSender()->getSockfd());
    client->setUsername(username);
    client->setHost(hostname);
    client->setServername(servername);
    client->setRealname(realname);

    if (!server->parsingPassword(client->getPassword())) {
        std::string pass_res = "NOTICE " + client->getNickname() + " :Invalid password. Please try again.\r\n";
        send(client->getSockfd(), pass_res.c_str(), pass_res.length(), 0);

        int fd = client->getSockfd();

        // Set a -1 le fd ouvert dans la struct pollfd
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (server->getFds()[i].fd == fd)
                server->getFds()[i].fd = -1;
        }

        // Retirer le client de la liste des clients et supprimer son objet de la mémoire
        std::vector<ft_irc::Client *> *clients = server->getClients();
        for (std::vector<ft_irc::Client *>::iterator it = clients->begin(); it != clients->end(); ++it) {
            if (*it == client) {
                clients->erase(it);
                delete client;
                break;
            }
        }

        close(fd);
        return ;
    }

    if (client->getNickname().empty()) {
        client->setUserLine(param);
        return ;
    }

    server->sendIrcResponse(client->getSockfd(), client);

    client->setIsSet(true);

    return ;
}

// METHODS

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
    size_t pos = param2.find(" ");
    param2 = param2.substr(pos + 1);
    cleanLine(param2);
    removeAllOccurrences(param2, "#");

    std::cerr << "JOIN param = " << param2 << std::endl;

    if (pos == std::string::npos) {
        std::string chan_res = ":" + server->getIp() + " 461 * " + message->getSender()->getNickname() + " JOIN :Channel name missing. Usage: /join #channel\r\n";
        send(message->getSender()->getSockfd(), chan_res.c_str(), chan_res.length(), 0);
        return ;
    }

    // Si le canal n'existe pas, on le crée et on l'ajoute à la liste des canaux du serveur    
    channel = server->getChannelPointer(param2);
    if (!channel) {
        channel = new Channel(param2);
        channels->push_back(channel);
    }

    // On ajoute l'utilisateur qui a envoyé le message au canal
    channel->addClient(message->getSender());

    message->getSender()->addChannel(channel);

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

    std::string test = param;
    cleanLine(test);
    size_t postwo = test.find(' ');
    std::string param2 = test.substr(postwo + 1);

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


    // Si le paramètre commence par un '#', c'est un message à envoyer à un canal
    if (param2[0] == '#') {
        removeAllOccurrences(param2, "#");

        // On récupère le canal correspondant au nom
        // channel = server->getChannelPointer(channelName);
        // On check si le channel est dans les channels du Sender
        channel = message->getSender()->getChanPointer(channelName);

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
    } 
    
    else {
        // Si le paramètre ne commence pas par un '#', c'est un message à envoyer à un client
        std::string nickname;
        std::string contenu;
        std::size_t colonPos = param2.find(" :");
        if (colonPos != std::string::npos) {
            nickname = param2.substr(0, colonPos);
            contenu = param2.substr(colonPos + 2);
        }

        Client *client = server->getClientPointerByNick(nickname);

        if (!client) {
            // Si le client n'existe pas, on envoie un message d'erreur à l'utilisateur
            std::string msg = ":" + server->getIp() + " 401 " + message->getSender()->getNickname() + " " + nickname + " :No such nick/channel\r\n";
            if (send(message->getSender()->getSockfd(), msg.c_str(), msg.length(), 0) == -1) {
                std::cerr << "Error SEND" << std::endl;
            }
        } else if (client->isBot()) {
            // Si le client est un bot, on traite le message avec la méthode handleMessage() de la classe BotClient
            // std::string messageToSend = ":" + message->getSender()->getNickname() + " PRIVMSG " + client->getNickname() + " :" + contenu + "\r\n";
            // send(message->getSender()->getSockfd(), messageToSend.c_str(), messageToSend.length(), 0);
            client->handleMessage(server->getSockfd(), contenu, client, message->getSender());
        } else {
            // Si le client n'est pas un bot, on envoie le message à ce client
            std::string msg = ":" + message->getSender()->getNickname() + "!" + message->getSender()->getUsername() + "@" + message->getSender()->getHost() + " PRIVMSG " + client->getNickname() + " :" + contenu + "\r\n";
            if (send(client->getSockfd(), msg.c_str(), msg.length(), 0) == -1)
                std::cerr << "Error SEND" << std::endl;
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

void ft_irc::Server::part(ft_irc::Message* message, const std::string& param) {

    ft_irc::Server *server = message->getServer();
    ft_irc::Channel *channel;
    // std::vector<Channel*> *channels = server->getChannels();

    // On supprime le caractère '#' au début du paramètre pour récupérer le nom du canal
    std::string param2 = param;
    size_t pos = param2.find(" ");
    param2 = param2.substr(pos + 1);
    cleanLine(param2);
    removeAllOccurrences(param2, "#");
    size_t pos2 = param2.find(":");
    param2 = param2.substr(pos2 + 1);

    if (pos2 == std::string::npos) {
        std::string chan_res = ":" + server->getIp() + " 461 * " + message->getSender()->getNickname() + " PART :Channel name missing. Usage: /part #channel\r\n";
        send(message->getSender()->getSockfd(), chan_res.c_str(), chan_res.length(), 0);
        return ;
    }

    channel = server->getChannelPointer(param2);

    channel->removeClient(*(message->getSender()));

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::cerr << "SIZE1--> " << message->getSender()->getChannels().size() << std::endl;

    message->getSender()->removeChannel(*channel);

    std::cerr << "SIZE2--> " << message->getSender()->getChannels().size() << std::endl;

    // std::string part_msg = ":" + message->getSender()->getNickname() + "!"  + message->getSender()->getNickname() + "@localhost PART #" + channel->getName() + ": " + "\r\n";
    std::string part_msg = message->getSender()->getNickname() + ":" + " PART #" + channel->getName() + "\r\n";
    for (std::vector<Client *>::const_iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it) {
        if (*it != message->getSender()) {
            if (send((*it)->getSockfd(), part_msg.c_str(), part_msg.length(), 0) == -1) {
                std::cerr << "Error SEND" << std::endl;
            }
        }
    }

    // Send a message to confirm the client's departure
    std::string confirm_msg = ":" + message->getServer()->getIp() + " 301 " + message->getSender()->getNickname() + " #" + channel->getName() + " :Goodbye!\r\n";
    if (send(message->getSender()->getSockfd(), confirm_msg.c_str(), confirm_msg.length(), 0) == -1) {
        std::cerr << "Error SEND" << std::endl;
    }


}

    // ft_irc::Client  *sender = message->getSender();
    
    // std::vector<ft_irc::Channel *> Chann = sender->getChannels();
    
    // std::string msg = "PART <" + param + ">\n"/* sender->getNickname() + " has left the channel." */;

    // for (std::vector<ft_irc::Channel *>::iterator it = Chann.begin(); it != Chann.end(); ++it) {
    //     if ((*it)->getName() == param) {
    //         for (std::vector<ft_irc::Client *>::const_iterator it2 = (*it)->getClients().begin(); it2 != (*it)->getClients().end(); ++it2){
    //             send((*it2)->getSockfd(), msg.c_str(), msg.length(), 0);
    //             // std::cerr << "WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEeee" << std::endl;
    //         }
    //         break ;
    //     }
    // }
    
    // return ;