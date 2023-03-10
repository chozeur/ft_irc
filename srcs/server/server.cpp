/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/10 18:05:51 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* CONSTRUCTORS */
ft_irc::Server::Server(void){return ;}

ft_irc::Server::Server(Server const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Server::Server(std::string password, long port, char **env){
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
	this->_fds[0].fd = this->_sockfd;
	this->_fds[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i)
	{
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

		int num_ready_fds = poll(this->_fds, MAX_CLIENTS + 1, -1);
		if (num_ready_fds == -1) {
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}

		if (this->_fds[0].revents & POLLIN) {
			// Accepter une connexion entrante
			struct sockaddr_in client_address;
			socklen_t client_address_length = sizeof(client_address);
			int clientfd = accept(this->_sockfd, (struct sockaddr *)&client_address, &client_address_length);
			if (clientfd == -1)
			{
				std::cerr << "Error: accept failed" << std::endl;
				continue;
			}

			ft_irc::Client new_client;
			new_client.setSockfd(clientfd);
			_clients.push_back(new_client);

			// Ajouter la socket du client à l'ensemble des sockets surveillées
			for (int i = 1; i <= MAX_CLIENTS; ++i)
			{
				if (this->_fds[i].fd == -1)
				{
					this->_fds[i].fd = clientfd;
					std::cout << "New client connected || i = " << i << std::endl;
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

		for (std::vector<ft_irc::Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
			ft_irc::Client &client = *it;
			std::cout << "Client : " << client.getSockfd() << std::endl;
		}

		for (int i = 1; i <= MAX_CLIENTS; ++i) {
			std::cout << "------------------" << std::endl;
			std::cout << "i = " << i << std::endl;
			std::cout << "this->_fds[i].fd = " << this->_fds[i].fd << std::endl;
			std::cout << "this->_fds[i].revents = " << this->_fds[i].revents << std::endl;
				
			if (this->_fds[i].fd != -1 && this->_fds[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytes_received == -1)
				{
					std::cerr << "Error: message receiving failed" << std::endl;
					close(this->_fds[i].fd);
					this->_fds[i].fd = -1;
				}
				else if (bytes_received == 0)
				{
					std::cout << "Client closed the connection" << std::endl;
					close(this->_fds[i].fd);
					this->_fds[i].fd = -1;
				}
				else
				{
					std::cout << "Hello world 2" << std::endl;
					ft_irc::Client client_test;
					
					for (std::vector<ft_irc::Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
						ft_irc::Client &client = *it;
						std::cout << "i = " << i << std::endl;
						std::cout << "getSockfd() = " << client.getSockfd() << std::endl;
						if (i == client.getSockfd() - 3){
							client_test = client;
							break;
						}
					}
										
					std::string message(buffer, bytes_received);
					if (message.find_first_not_of(' ') == std::string::npos) 
						continue;
					std::cout << message;

					if (message.substr(0, 7) == "CAP LS\n") {
						std::string cap_response = "CAP LS\r\n";
						send(this->_fds[i].fd, cap_response.c_str(), cap_response.length(), 0);
					}
					else if (message.substr(0, 5) == "NICK ") {
						client_test.setNickname(message.substr(5));
					}
					// else if (message.substr(0, 5) == "USER ") {
					// 	std::string user_info = message.substr(5);
					// 	client_test.setUs
					// }


					// else if (message.substr(0, 5) == "NICK ") {
					// 	std::string nick = message.substr(5);
					// 	std::string nick_response = "NICK " + nick + "\r\n";
					// 	send(this->_fds[i].fd, nick_response.c_str(), nick_response.length(), 0);
					// }
					// else if (message.substr(0, 5) == "USER ") {
					// 	std::string user_info = message.substr(5);
					// 	std::string user_response = "USER " + user_info + "\r\n";
					// 	send(this->_fds[i].fd, user_response.c_str(), user_response.length(), 0);
					// }					
					if (message.substr(0, 6) == "/join ") {
						std::string channel_name = message.substr(6);
						if (channel_name.empty()) {
							std::string error_message = "Error: Channel name cannot be empty\n";
							send(this->_fds[i].fd, error_message.c_str(), error_message.length(), 0);
						}
						else {
							std::string join_message = "Joining channel " + channel_name + "\n";
							send(this->_fds[i].fd, join_message.c_str(), join_message.length(), 0);
						}
					}
					else {
						std::string error_message = "Error: I can't handle this command: " + message;
						send(this->_fds[i].fd, error_message.c_str(), error_message.length(), 0);
					}
				}
			}
		
			if (i == MAX_CLIENTS)
				break ;
		}
		break ;
	}
	return ;
}

