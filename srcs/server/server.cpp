/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvrignon <rvrignon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/03/09 22:06:10 by rvrignon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/* CONSTRUCTORS */
ft_irc::Server::Server(void) { return; }

ft_irc::Server::Server(Server const &rhs)
{
	*this = rhs;
	return;
}

ft_irc::Server::Server(std::string password, int port, char **env)
{
	this->init(password, port, env);
	return;
}

/* DESTRUCTOR */
ft_irc::Server::~Server(void)
{
	close(this->_sockfd);
	return;
}

/* INIT */
ft_irc::Server &ft_irc::Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_serv_addr = rhs._serv_addr;
		this->_sockfd = rhs._sockfd;
		this->_env = rhs._env; //! deep copy
	}
	return (*this);
}

/* METHODS */
void ft_irc::Server::init(std::string password, int port, char **env)
{
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
	return;
}

bool ft_irc::Server::anyoneHasMessage(void)
{
	for (std::vector<ft_irc::Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		ft_irc::Client &client = *it;
		if (client.hasMessage())
		{
			return (true);
		}
	}
	return (false);
}

void ft_irc::Server::newClient(void)
{
	// Accepter une connexion entrante
	sockaddr_in client_address;
	socklen_t client_address_length = sizeof(client_address);
	int client_socket = accept(this->_sockfd, (sockaddr *)&client_address, &client_address_length);
	if (client_socket == -1) {
		std::cerr << "Erreur lors de l'acceptation d'une connexion entrante" << std::endl;
		return;
	}
	std::cout << "Nouvelle connexion entrante" << std::endl;

	// Créer un objet Client pour gérer la nouvelle connexion
	ft_irc::Client new_client(client_socket);

	// Ajouter le client à la liste des clients connectés
	this->_clients.push_back(new_client);
}

void ft_irc::Server::handleClient(void)
{
	clock_t start_time = clock();
    for (std::vector<ft_irc::Client>::iterator it = this->_clients.begin(); true; ++it) {
        clock_t current_time = clock();
        int elapsed_time = (current_time - start_time) / CLOCKS_PER_SEC * 1000;
        if (elapsed_time >= 500) {
            break;
        }
		ft_irc::Client &client = *it;
		std::string message;
		while (client.hasMessage()) {
			message += client.recvMessage();
		}
		std::cout << message;
		//handleMessage here;
		if (it == this->_clients.end() - 1)
			it = this->_clients.begin() - 1;
	}
	std::cout << std::endl;
}

void ft_irc::Server::run(void)
{
	if (bind(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) < 0)
		throw std::runtime_error("Error : binding socket failed");

	if (listen(this->_sockfd, 10) == -1) {
		std::cerr << "Error: listen failed" << std::endl;
		return;
	}

	std::cout << "Server listening on 127.0.0.1:" << this->_port << std::endl;

	// Boucle principale pour accepter les connexions entrantes
	while (true) {
		newClient();
		handleClient();
	}

	return;
}

void ft_irc::Server::stop(void)
{
	close(this->_sockfd);
	std::cout << "Server 127.0.0.1:" << this->_port << " has been stopped" << std::endl;
}
