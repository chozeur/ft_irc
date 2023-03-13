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

std::vector<struct pollfd>	ft_irc::Server::getFds(void) const {
	return (this->_fds);
}

std::map<int,ft_irc::Client>	ft_irc::Server::getClients(void) const {
	return (this->_clients);
}

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

void	ft_irc::Server::setFds(std::vector<struct pollfd> fds){
	this->_fds = fds;
	return ;
}

std::map<int,ft_irc::Client>	ft_irc::Server::setClients(std::map<int,ft_irc::Client> clients){
	this->_clients = clients;
	return (this->_clients);
}

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
					ft_irc::Client new_client(clientfd, "Bruce Lee");
					this->_clients[this->_fds[i].fd] = new_client;

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

			if ((*it).fd != -1 && (*it).revents & POLLIN){
				ft_irc::Message	msg;
				while (this->_clients[(*it).fd].getNickname() == "" &&
						this->_clients[(*it).fd].getUsername() == "" &&
						this->_clients[(*it).fd].getRealname() == "" &&
						this->_clients[(*it).fd].getHost() == "") {
						std::cerr << "Waiting for client to send NICK, USER" << std::endl;
						char buffer[1024];
						int bytes_received = recv((*it).fd, buffer, 1024, 0);
						if (bytes_received > 0){
							msg.appendPayload(buffer);
							if (std::string(buffer).find("NICK")){}
						}
				}
				std::cerr << msg.getPayload() << std::endl;
			}
		}
	return ;
	}
}

