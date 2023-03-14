/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/14 17:23:25 by tbrebion         ###   ########.fr       */
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

ft_irc::Client* ft_irc::Server::getClientPointer(int fd) {
    std::vector<Client>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (it->getSockfd() == fd)
            return &(*it);
    }
    return NULL;
}

std::vector<ft_irc::Client>::iterator ft_irc::Server::getClientIterator(int fd) {
    std::vector<Client>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if (it->getSockfd() == fd)
            return it;
    }
    return this->_clients.end();
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
	if (bind(this->_sockfd, (struct sockaddr *) &this->_serv_addr, sizeof(this->_serv_addr)) < 0) {
		throw std::runtime_error("Error : binding socket failed");
		return;
	}
	if (listen(this->_sockfd, 5) == -1) {
		std::cerr << "Error: listen failed" << std::endl;
		return ;
	}
	this->_fds[0].fd = this->_sockfd;
	this->_fds[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i)
	{
		this->_fds[i].fd = -1;
		this->_fds[i].events = POLLIN;
	}
	std::cout << "Server listening on 127.0.0.1:" << _port << std::endl;
	return ;
}

void	ft_irc::Server::run(void){
	while (server) {

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
			if (clientfd == -1) {
				std::cerr << "Error: accept failed" << std::endl;
				continue;
			}

			// Ajouter la socket du client à l'ensemble des sockets surveillées
			for (int i = 1; i <= MAX_CLIENTS; ++i)
			{
				if (this->_fds[i].fd == -1)
				{
					this->_fds[i].fd = clientfd;
					
					ft_irc::Client new_client;
					new_client.setSockfd(clientfd);
					_clients.push_back(new_client);
						
					break;
				}
				if (i == MAX_CLIENTS) {
					std::cout << "Too many clients connected" << std::endl;
					break;
				}
			}
		}

		for (int i = 1; i <= MAX_CLIENTS; ++i) {
			if (this->_fds[i].fd != -1 && this->_fds[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytes_received == -1 || bytes_received == 0) {
					if (bytes_received == -1)
						std::cerr << "Error: message receiving failed on fd : " << this->_fds[i].fd << std::endl;
					else 
						std::cerr << "Client " << this->_fds[i].fd << " closed the connection" << std::endl;
					closeClient(i);
				} else {													
					std::string message(buffer, bytes_received);

					if (message.substr(0, 8) == "CAP LS\r\n" || message.substr(0, 4) == "PASS" || message.substr(0, 4) == "NICK" || message.substr(0, 4) == "USER"){
						int value = clientInit(this->_fds[i].fd, message);
						if (value > 0) {
							std::cout << *(getClientPointer(this->_fds[i].fd)) << std::endl;
							sendIrcResponse(this->_fds[i].fd, getClientPointer(this->_fds[i].fd));
						} else if (value == -1 || value == -2) {
							closeClient(i);
							// Send to irssi why it is closed (-1 bad password || -2 bad nickname)
						}
					} else {
						std::cout << message;
					}

					
				}
			}
		}
		break ;
	}
	return ;
}

int ft_irc::Server::clientInit(int fd, std::string message){

    std::string line;
    std::string::size_type start_pos = 0;

    while (start_pos != std::string::npos) {
        std::string::size_type end_pos = message.find("\r\n", start_pos);
        if (end_pos != std::string::npos) {
            line = message.substr(start_pos, end_pos - start_pos);
            start_pos = end_pos + 2;
        } else {
            line = message.substr(start_pos);
            start_pos = end_pos;
        }
		if (line.find("CAP LS") != std::string::npos){
			std::cout << "IRC SERVER => " << line << std::endl;		
		}
        if (line.find("PASS") != std::string::npos){
			std::cout << "IRC SERVER => " << line << std::endl;
			std::string::size_type space_pos = line.find(' ');
			if (!parsingPassword(line.substr(space_pos + 1))){
				return (-1);
			}
			(getClientPointer(fd))->setPassword(line.substr(space_pos + 1));
		}
		
		if (line.find("NICK") != std::string::npos){
			std::cout << "IRC SERVER => " << line << std::endl;
			std::string::size_type space_pos = line.find(' ');
			if (!parsingNickname(line.substr(space_pos + 1))){
				return (-2);
			}
			(getClientPointer(fd))->setNickname(line.substr(space_pos + 1));		
		}
		if (line.find("USER") != std::string::npos){
			std::cout << "IRC SERVER => " << line << std::endl;
			std::string::size_type space_pos1 = line.find(' ');
			std::string::size_type space_pos2 = line.find(' ', space_pos1 + 1);
			std::string::size_type space_pos3 = line.find(' ', space_pos2 + 1);
			std::string::size_type colon_pos = line.find(':');
			std::string::size_type end_pos = line.find('\n');

			std::string username = line.substr(space_pos1 + 1, space_pos2 - space_pos1 - 1);
			std::string hostname = line.substr(space_pos2 + 1, space_pos3 - space_pos2 - 1);
			std::string servername = line.substr(space_pos3 + 1, colon_pos - space_pos3 - 2);
			std::string realname = line.substr(colon_pos + 1, end_pos - 1);

			getClientPointer(fd)->setUsername(username);
			getClientPointer(fd)->setHost(hostname);
			getClientPointer(fd)->setServername(servername);
			getClientPointer(fd)->setRealname(realname);
			return (1);		
		} 
	}
    return 0;
}

void ft_irc::Server::sendIrcResponse(int sockfd, ft_irc::Client *client) const {
    if (!client) {
        std::cerr << "Error: Null client pointer passed to sendIrcResponse" << std::endl;
        return;
    }
    std::string cap_response = "CAP * LS :\r\n";
    std::string welcome_msg = ":" + client->getServername() + " 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + "\r\n";
    std::string version_msg = ":" + client->getServername() + " 002 " + client->getNickname() + " :Your host is " + client->getServername() + ", running version X.Y.Z\r\n";
    std::string created_msg = ":" + client->getServername() + " 003 " + client->getNickname() + " :This server was created on [date]\r\n";
    send(sockfd, cap_response.c_str(), cap_response.length(), 0);
    send(sockfd, welcome_msg.c_str(), welcome_msg.length(), 0);
    send(sockfd, version_msg.c_str(), version_msg.length(), 0);
    send(sockfd, created_msg.c_str(), created_msg.length(), 0);
}

int ft_irc::Server::parsingNickname(std::string nickname){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if ((*it).getNickname() == nickname)
			return (0);
	return (1);
}

int ft_irc::Server::parsingPassword(std::string password)const{
	if (this->_password != password)
		return (0);
	return (1);
}

void ft_irc::Server::closeClient(int i) {
	std::vector<Client>::iterator client_it = getClientIterator(this->_fds[i].fd);
	if (client_it != this->_clients.end()) {
		this->_clients.erase(client_it);
	}
	close(this->_fds[i].fd);
	this->_fds[i].fd = -1;
}