/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/09 20:30:46 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

ft_irc::Client::Client(void) {
	return ;
}

// Constructeur
ft_irc::Client::Client(int sockfd) : _sockfd(sockfd) {
	std::ostringstream oss;
	oss << sockfd;
	std::string pseudo = oss.str();
	this->_pseudo = pseudo;
	std::cout << "New Client Hell yeah !" << std::endl;
	std::cout << "Sockfd : " << this->_sockfd << std::endl;
	std::cout << "Pseudo : " << this->_pseudo << std::endl;
	return ;
}

ft_irc::Client::Client(Client const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Client&	ft_irc::Client::operator=(Client const &rhs){
	this->_channels = rhs._channels;
	this->_messages = rhs._messages;
	this->_pseudo = rhs._pseudo;
	this->_sockfd = rhs._sockfd;
	return (*this);
}

// Destructeur
ft_irc::Client::~Client() {}

// Accesseur pour le socket du client
int ft_irc::Client::getSocket() const {
	return _sockfd;
}

// Accesseur pour le pseudo du client
std::string ft_irc::Client::getPseudo() const {
	return _pseudo;
}

// Accesseur pour la liste des channels auxquels le client est connecté
std::vector<std::string> ft_irc::Client::getChannels() const {
	return _channels;
}

// Méthode pour faire rejoindre un client à un channel
void ft_irc::Client::joinChannel(const std::string& channel) {
	_channels.push_back(channel);
}

// Méthode pour faire quitter un client d'un channel
void ft_irc::Client::quitChannel(const std::string& channel) {
	std::vector<std::string>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		if (*it == channel) {
			_channels.erase(it);
			break;
		}
	}
}

std::string ft_irc::Client::recvMessage() const {
	std::string message;
	char buffer[1024];
	int sockfd = this->_sockfd;
	
	int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
	if (bytes_received == -1) {
		std::cerr << "Error: message receiving failed" << std::endl;
		close(sockfd);
		sockfd = -1;
	}
	else if (bytes_received == 0) {
		std::cout << "Client closed the connection" << std::endl;
		close(sockfd);
		sockfd = -1;
	}
	else  {
		std::string message(buffer, bytes_received);
		return message;
	}
	return ("");
}

// Méthode pour ajouter un message à la liste des messages en attente d'envoi pour un client
void ft_irc::Client::addMessage(const std::string& message) {
	_messages.push_back(message);
}

bool ft_irc::Client::hasMessage(void)
{
    // Surveillance de la socket pour détecter les événements
    this->_fds[0].fd = this->_sockfd;
    this->_fds[0].events = POLLIN;
    int num_ready_fds = poll(this->_fds, 1, 0);
    if (num_ready_fds == -1) {
        std::cerr << "Error: poll failed" << std::endl;
        return false;
    }

    // Vérification si la socket a des données en attente
    if (this->_fds[0].revents & POLLIN) {
        return true;
    }

    return false;
}
