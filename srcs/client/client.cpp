/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:30:54 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/10 18:12:33 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/* CONSTRUCTORS */

ft_irc::Client::Client(void){return ;}

ft_irc::Client::Client(Client const & rhs){
	*this = rhs;
	return ;
}

ft_irc::Client::Client(ft_irc::Server &server):
	_server(&server),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host(""){
	memset(&this->_cli_addr, 0, sizeof(struct sockaddr_in));
	this->_cli_addr.sin_family = AF_INET;
	this->_cli_addr.sin_addr.s_addr = INADDR_ANY;
	this->_cli_addr.sin_port = htons(this->_server->getPort());
	this->_cli_len = sizeof(_cli_addr);
	this->_sockfd = accept(this->_server->getSockfd(), (struct sockaddr *) &(this->_cli_addr), &(this->_cli_len));
	if (this->_sockfd < 0)
		throw (std::runtime_error("Error : accept"));
}

ft_irc::Client::Client(Client const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Client&	ft_irc::Client::operator=(Client const &rhs){
	if (this != &rhs){
		this->_cli_addr = rhs._cli_addr;
		this->_cli_len = rhs._cli_len;
		this->_sockfd = rhs._sockfd;
		for (int i = 0; i < 2048; i++)
			this->_buffer[i] = rhs._buffer[i];
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_password = rhs._password;
		this->_host = rhs._host;
		this->_channels = rhs._channels;
	}
	return (*this);
}

/* GETTERS */

struct sockaddr_in	ft_irc::Client::getCliAddr(void) const {
	return (this->_cli_addr);
}

socklen_t	ft_irc::Client::getCliLen(void) const {
	return (this->_cli_len);
}

std::string ft_irc::Client::getIrssiMessage() const {
	return _irssi;
}

char	*ft_irc::Client::getBuffer(void){
	return (this->_buffer);
}

void ft_irc::Client::setIrssiMessage(std::string& append) {
    _irssi += append;
}

void ft_irc::Client::clearIrssiMessage(void) {
	_irssi.clear();
}

// Accesseur pour la liste des channels auxquels le client est connecté
std::vector<std::string> ft_irc::Client::getChannels() const {
	return _channels;
}

// Méthode pour faire rejoindre un client à un channel
void ft_irc::Client::joinChannel(const std::string& channel) {
	_channels.push_back(channel);
}

std::string	ft_irc::Client::getHost(void) const {
	return (this->_host);
}

std::vector<std::string>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

/* SETTERS */

void	ft_irc::Client::setCliAddr(struct sockaddr_in cli_addr){
	this->_cli_addr = cli_addr;
	return ;
}

void	ft_irc::Client::setCliLen(socklen_t cli_len){
	this->_cli_len = cli_len;
	return ;
}

void	ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
	return ;
}

void	ft_irc::Client::setBuffer(char *buffer){
	for (int i = 0; i < 2048 && buffer[i]; i++)
		this->_buffer[i] = buffer[i];
	return ;
}

void	ft_irc::Client::setNickname(std::string nickname){
	this->_nickname = nickname;
	return ;
}

void	ft_irc::Client::setUsername(std::string username){
	this->_username = username;
	return ;
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

void	ft_irc::Client::setChannels(std::vector<std::string> channels){	//! deep copy
	this->_channels = channels;
	return ;
}

/* METHODS */

int	ft_irc::Client::read(void){
	std::string msg("");
	bzero(this->_buffer, 2048);
	int n = 1;
	while (n > 0){
		n = recv(this->_sockfd, this->_buffer, 2047, 0);
		if (n > 0)
			msg += this->_buffer;
	}
	if (n < 0)
		throw (std::runtime_error("Error : recv"));
	for (int i = 0; msg.c_str()[i]; i++)
		this->_buffer[i] = msg.c_str()[i];
	return (n);
}

int	ft_irc::Client::write(std::string msg){
	int n = send(this->_sockfd, msg.c_str(), msg.length(), 0);
	if (n < 0)
		throw (std::runtime_error("Error : send"));
	return (n);
}
