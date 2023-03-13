/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/12 18:45:17 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* CONSTRUCTORS */
ft_irc::Server::Server(void){return ;}

ft_irc::Server::Server(Server const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Server::Server(std::string password, long port, char **env) : _clients(){
	this->init(password, port, env);
	return ;
}

/* DESTRUCTOR */
ft_irc::Server::~Server(void){
	close(this->_sockfd);
	return ;
}

/* INIT */
ft_irc::Server&	ft_irc::Server::operator=(Server const &rhs){
	if (this != &rhs){
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_serv_addr = rhs._serv_addr;
		this->_sockfd = rhs._sockfd;
		this->_env = rhs._env; //! deep copy
	}
	return (*this);
}

/* GETTERS */
long	ft_irc::Server::getPort(void) const {
	return (this->_port);
}

std::string	ft_irc::Server::getPassword(void)const{
	return (this->_password);
}

struct sockaddr_in	ft_irc::Server::getServAddr()const{
	return (this->_serv_addr);
}

int	ft_irc::Server::getSockfd(void)const{
	return (this->_sockfd);
}

char	**ft_irc::Server::getEnv(void)const{
	return (this->_env);
}

/* SETTERS */
void	ft_irc::Server::setPort(long port){
	this->_port = port;
	return ;
}

void	ft_irc::Server::setPassword(std::string password){
	this->_password = password;
	return ;
}

void	ft_irc::Server::setServAddr(struct sockaddr_in serv_addr){
	this->_serv_addr = serv_addr;
	return ;
}

void	ft_irc::Server::setSockfd(int fd){
	this->_sockfd = fd;
	return ;
}

void	ft_irc::Server::setEnv(char **env){
	this->_env = env;	//! deep copy
	return ;
}

/* METHODS */
void	ft_irc::Server::init(std::string password, long port, char **env){
	this->_port = port;
	this->_password = password;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
		throw std::runtime_error("Error : socket");
	memset(&this->_serv_addr, 0, sizeof(struct sockaddr_in));
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_addr.s_addr = INADDR_ANY;
	this->_serv_addr.sin_port = htons(this->_port);
	this->_env = env; //! deep copy

	// this->_fds[0].fd = this->_sockfd;
	// this->_fds[0].events = POLLIN;
	this->_fds = std::vector<struct pollfd>(MAX_CLIENTS + 1);
	this->_fds[0].fd = this->_sockfd;
	this->_fds[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i){
		this->_fds[i].fd = -1; // Initialise les sockets client à -1 pour indiquer qu'elles sont vides
		this->_fds[i].events = POLLIN;
	}
	return ;
}

void	ft_irc::Server::run(void){
	if (bind(this->_sockfd, (struct sockaddr *) &this->_serv_addr, sizeof(this->_serv_addr)) < 0) {
		throw std::runtime_error("Error : binding socket failed");
		return;
	}
	if (listen(this->_sockfd, 5) == -1) {
		std::cerr << "Error: listen failed" << std::endl;
		return ;
	}
	std::cout << "Server listening on 127.0.0.1:" << _port << std::endl;

	while (true) {

		int num_ready_fds = poll(this->_fds.data(), MAX_CLIENTS + 1, -1);
		if (num_ready_fds == -1) {
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}

		if (this->_fds.front().revents & POLLIN) {
			// Accepter une connexion entrante
			struct sockaddr_in client_address;
			socklen_t client_address_length = sizeof(client_address);
			int clientfd = accept(this->_sockfd, (struct sockaddr *)&client_address, &client_address_length);
			if (clientfd == -1)
			{
				std::cerr << "Error: accept failed" << std::endl;
				continue;
			}

			// Ajouter la socket du client à l'ensemble des sockets surveillées
			for (int i = 1; i <= MAX_CLIENTS; ++i)
			{
				if (this->_fds[i].fd == -1)
				{
					// ft_irc::Client new_client(clientfd, "Bruce Lee");
					// this->_clients.push_back(new_client);
					
					this->_fds[i].fd = clientfd;
					
					std::string welcome_msg = "Welcome to the chatroom!\n";
					send(clientfd, welcome_msg.c_str(), welcome_msg.length(), 0);
					break;
				}
				if (i == MAX_CLIENTS) {
					std::cout << "Too many clients connected" << std::endl;
					break;
				}
			}
		}

		for (std::vector<struct pollfd>::iterator it = this->_fds.begin(); it != this->_fds.end(); ++it) {
	
			if ((*it).fd != -1 && (*it).revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv((*it).fd, buffer, sizeof(buffer), 0);
				if (bytes_received == -1)
				{
					std::cerr << "Error: message receiving failed" << std::endl;
					close((*it).fd);
					(*it).fd = -1;
				}
				else if (bytes_received == 0)
				{
					std::cout << "Client closed the connection" << std::endl;
					close((*it).fd);
					(*it).fd = -1;
				}
				else
				{
					std::string message(buffer, bytes_received);
					std::cout << "IRC SERVER : " << message << std::endl;
					std::cout << "IRC SERVER SUBSTR : " << message.substr(0, 7) << std::endl;

					if (message.substr(0, 6) == "CAP LS\n") {
						std::cerr << "WEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
						ft_irc::Client client(((*it).fd), "Bruce Lee");
						this->_clients.push_back(client);
						std::string cap_response = "CAP LS\r\n";
						send((*it).fd, cap_response.c_str(), cap_response.length(), 0);
					}
					else if (message.substr(0, 5) == "NICK ") {
						ft_irc::Client client;
						for (std::vector<ft_irc::Client>::iterator itb = this->_clients.begin(); itb != this->_clients.end(); ++itb) {
							client = *itb;
							std::cerr << "getSockfd() = " << client.getSockfd() << std::endl;
							if ((*it).fd == client.getSockfd()){
								break;
							}
						}
						client.setNickname(message.substr(5));
					}

					if (message.find_first_not_of(' ') == std::string::npos)
						continue;
					else if (message.substr(0, 6) == "/join ") {
						std::string channel_name = message.substr(6);
						if (channel_name.empty()) {
							std::string error_message = "Error: Channel name cannot be empty\n";
							send((*it).fd, error_message.c_str(), error_message.length(), 0);
						}
						else {
							std::string join_message = "Joining channel " + channel_name + "\n";
							send((*it).fd, join_message.c_str(), join_message.length(), 0);
						}
					}
					else {
						std::string error_message = "Error: I can't handle this command: " + message;
						send((*it).fd, error_message.c_str(), error_message.length(), 0);
					}
				}
			}
		}
	}
	return ;
}

