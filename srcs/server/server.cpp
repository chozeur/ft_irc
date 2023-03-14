#include "server.hpp"

/* CONSTRUCTORS */

ft_irc::Server::Server(void) { return; }

ft_irc::Server::Server(Server const &rhs)
{
	*this = rhs;
	return;
}

ft_irc::Server::Server(std::string password, long port, char **env) : _clients()
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

/* GETTERS */

long ft_irc::Server::getPort(void) const
{
	return (this->_port);
}

std::string ft_irc::Server::getPassword(void) const
{
	return (this->_password);
}

struct sockaddr_in ft_irc::Server::getServAddr() const
{
	return (this->_serv_addr);
}

int ft_irc::Server::getSockfd(void) const
{
	return (this->_sockfd);
}

char **ft_irc::Server::getEnv(void) const
{
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

void ft_irc::Server::setPort(long port)
{
	this->_port = port;
	return;
}

void ft_irc::Server::setPassword(std::string password)
{
	this->_password = password;
	return;
}

void ft_irc::Server::setServAddr(struct sockaddr_in serv_addr)
{
	this->_serv_addr = serv_addr;
	return;
}

void ft_irc::Server::setSockfd(int fd)
{
	this->_sockfd = fd;
	return;
}

void ft_irc::Server::setEnv(char **env)
{
	this->_env = env; //! deep copy
	return;
}

/* METHODS */

void ft_irc::Server::init(std::string password, long port, char **env)
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
	if (bind(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) == -1)
		throw std::runtime_error("Error : binding socket failed");
	if (listen(this->_sockfd, MAX_CLIENTS) == -1)
		std::cerr << "Error: listen failed" << std::endl;
	this->_fds = std::vector<struct pollfd>(MAX_CLIENTS + 1);
	this->_fds[0].fd = this->_sockfd;
	this->_fds[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i) {
		this->_fds[i].fd = -1;
		this->_fds[i].events = POLLIN;
	}
	return;
}

void ft_irc::Server::run(void)
{
	std::cout << "Server listening on 127.0.0.1:" << _port << std::endl;
	while (server)
	{
		int num_ready_fds = poll(this->_fds.data(), MAX_CLIENTS + 1, -1);
		if (num_ready_fds == -1) {
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}
		if (this->_fds[0].revents & POLLIN)
		{			
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
					break;
				}
				else if (i == MAX_CLIENTS)
				{
					std::cout << "Too many clients connected" << std::endl;
					break;
				}
			}
		}

		for (int i = 1; i <= MAX_CLIENTS; ++i)
		{
			if (this->_fds[i].fd != -1 && this->_fds[i].revents & POLLIN)
			{
				char buffer[1024];
				int bytes_received = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
				if (bytes_received == -1)
				{
					std::cerr << "Error: message receiving failed on fd : " << this->_fds[i].fd << std::endl;
					std::vector<Client>::iterator client_it = getClientIterator(this->_fds[i].fd);
					if (client_it != this->_clients.end()) {
						this->_clients.erase(client_it);
					}
					close(this->_fds[i].fd);
					this->_fds[i].fd = -1;
				}
				else if (bytes_received == 0)
				{
					std::cerr << "Client " << this->_fds[i].fd << " closed the connection" << std::endl;
					std::vector<Client>::iterator client_it = getClientIterator(this->_fds[i].fd);
					if (client_it != this->_clients.end()) {
						this->_clients.erase(client_it);
					}
					close(this->_fds[i].fd);
					this->_fds[i].fd = -1;
				}
				else
				{
					std::string message(buffer, bytes_received);	
					if (message.substr(0, 8) == "CAP LS\r\n" || message.substr(0, 4) == "NICK" || message.substr(0, 4) == "USER") {
						if (clientInit(this->_fds[i].fd, message)) {
							sendIrcResponse(this->_fds[i].fd, getClientPointer(this->_fds[i].fd));
						}
					} else
						std::cerr << "clientfd " << this->_fds[i].fd << " : " << message;
				}
			}
		}
	}
	return;
}

int ft_irc::Server::clientInit(int fd, std::string message)
{
	if (message.substr(0, 8) == "CAP LS\r\n") {
		
		std::string::size_type nick_pos = message.find("NICK");
		std::string::size_type user_pos = message.find("USER");
		std::string::size_type pos1 = message.find('\n');
		std::string first_line = message.substr(0, pos1 - 1);
		
		if (nick_pos != std::string::npos)
		{
			std::string::size_type pos2 = message.find('\n', pos1 + 1);
			std::string second_line = message.substr(pos1 + 1, pos2 - pos1 - 2);
			
			if (user_pos != std::string::npos) {
				// CAP LS
				// NICK rvrignon
				// USER rvrignon rvrignon localhost :Roland VRIGNON

				std::string::size_type pos3 = message.find('\n', pos2 + 1);
				std::string third_line = message.substr(pos2 + 1, pos3 - pos2 - 2);

				std::cout << "IRC SERVER => " << first_line << std::endl;
				std::cout << "IRC SERVER => " << second_line << std::endl;
				std::cout << "IRC SERVER => " << third_line << std::endl;

				// nickname
				std::string::size_type space_pos = second_line.find(' ');
				std::string nickname = second_line.substr(space_pos + 1);

				std::string::size_type space_pos1 = third_line.find(' ');
				std::string::size_type space_pos2 = third_line.find(' ', space_pos1 + 1);
				std::string::size_type space_pos3 = third_line.find(' ', space_pos2 + 1);
				std::string::size_type colon_pos = third_line.find(':');
				std::string username = third_line.substr(space_pos1 + 1, space_pos2 - space_pos1 - 1);
				std::string hostname = third_line.substr(space_pos2 + 1, space_pos3 - space_pos2 - 1);
				std::string servername = third_line.substr(space_pos3 + 1, colon_pos - space_pos3 - 2);
				std::string realname = third_line.substr(colon_pos + 1);
				
				// Print variables
				// std::cout << "nickname: " << nickname << std::endl;
				// std::cout << "username: " << username << std::endl;
				// std::cout << "hostname: " << hostname << std::endl;
				// std::cout << "servername: " << servername << std::endl;
				// std::cout << "realname: " << realname << std::endl;

				if (createClient(fd, nickname, username, realname, "No Password", servername, hostname)){
					return (1);
				}
			}
			else {
				std::cout << "IRC SERVER => " << first_line << std::endl;
				std::cout << "IRC SERVER => " << second_line << std::endl;
			}
		} else {
			std::cout << "IRC SERVER => " << first_line << std::endl;
		}
	}
	
	else if (message.substr(0, 4) == "NICK") {
		std::string::size_type user_pos = message.find("USER");
		
		std::string::size_type pos1 = message.find('\n');
		std::string second_line = message.substr(0, pos1 - 1);
		
		if (user_pos != std::string::npos) {
			// NICK rvrignon
			// USER rvrignon rvrignon localhost :Roland VRIGNON
			std::string::size_type pos2 = message.find('\n', pos1 + 1);
			std::string third_line = message.substr(pos1 + 1, pos2 - pos1  - 2);

			std::cout << "IRC SERVER => " << second_line << std::endl;
			std::cout << "IRC SERVER => " << third_line << std::endl;

			// nickname
			std::string::size_type space_pos = second_line.find(' ');
			std::string nickname = second_line.substr(space_pos + 1);

			std::string::size_type space_pos1 = third_line.find(' ');
			std::string::size_type space_pos2 = third_line.find(' ', space_pos1 + 1);
			std::string::size_type space_pos3 = third_line.find(' ', space_pos2 + 1);
			std::string::size_type colon_pos = third_line.find(':');
			std::string::size_type end_pos = third_line.find('\n');
			std::string username = third_line.substr(space_pos1 + 1, space_pos2 - space_pos1 - 1);
			std::string hostname = third_line.substr(space_pos2 + 1, space_pos3 - space_pos2 - 1);
			std::string servername = third_line.substr(space_pos3 + 1, colon_pos - space_pos3 - 2);
			std::string realname = third_line.substr(colon_pos + 1, end_pos - 1);

			// Print variables
			// std::cout << "nickname: " << nickname << std::endl;
			// std::cout << "username: " << username << std::endl;
			// std::cout << "hostname: " << hostname << std::endl;
			// std::cout << "servername: " << servername << std::endl;
			// std::cout << "realname: " << realname << std::endl;

			// We have all infos, we can create Client we can send all.
			if (createClient(fd, nickname, username, realname, "No Password", servername, hostname)){
				return (1);
			}
		}
		else {
			// NICK rvrignon
			std::cout << "IRC SERVER => " << second_line << std::endl;
		}
	}
	
	else if (message.substr(0, 4) == "USER") {
		std::string::size_type pos1 = message.find('\n');
		std::string third_line = message.substr(0, pos1 - 1);

		std::cout << "IRC SERVER => " << third_line << std::endl;

		// Extract the substrings corresponding to the username, hostname, servername, and realname fields
		std::string::size_type space_pos1 = third_line.find(' ');
		std::string::size_type space_pos2 = third_line.find(' ', space_pos1 + 1);
		std::string::size_type space_pos3 = third_line.find(' ', space_pos2 + 1);
		std::string::size_type colon_pos = third_line.find(':');
		std::string username = third_line.substr(space_pos1 + 1, space_pos2 - space_pos1 - 1);
		std::string hostname = third_line.substr(space_pos2 + 1, space_pos3 - space_pos2 - 1);
		std::string servername = third_line.substr(space_pos3 + 1, colon_pos - space_pos3 - 2);
		std::string realname = third_line.substr(colon_pos + 1);
		
		// Print variables
		// std::cout << "nickname: " << username << std::endl;
		// std::cout << "username: " << username << std::endl;
		// std::cout << "hostname: " << hostname << std::endl;
		// std::cout << "servername: " << servername << std::endl;
		// std::cout << "realname: " << realname << std::endl;

		// We have all infos, we can create Client we can send all.
		if (createClient(fd, username, username, realname, "No Password", servername, hostname)){
			return (1);
		}
	}

	return (0);
}

void ft_irc::Server::clientCommand(int fd, std::string message) {
	(void)fd;
	(void)message;
	std::cerr << "This is an other command" << std::endl;
}

int ft_irc::Server::createClient(const int sockfd, const std::string nickname, const std::string username, const std::string realname,const std::string password,const std::string servername,const std::string host) { 
	// Return 0 if Nickname is already taken
	ft_irc::Client client(sockfd, nickname, username, realname, password, servername, host);
	this->_clients.push_back(client);
	return 1;
}

void ft_irc::Server::sendIrcResponse(int sockfd, ft_irc::Client *client) const {
    if (!client) {
        std::cerr << "Error: Null client pointer passed to sendIrcResponse" << std::endl;
        return;
    }
    std::string cap_response = "CAP * LS :\r\n";
    std::string welcome_msg = ":" + client->getServerName() + " 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHost() + "\r\n";
    std::string version_msg = ":" + client->getServerName() + " 002 " + client->getNickname() + " :Your host is " + client->getServerName() + ", running version X.Y.Z\r\n";
    std::string created_msg = ":" + client->getServerName() + " 003 " + client->getNickname() + " :This server was created on [date]\r\n";
    send(sockfd, cap_response.c_str(), cap_response.length(), 0);
    send(sockfd, welcome_msg.c_str(), welcome_msg.length(), 0);
    send(sockfd, version_msg.c_str(), version_msg.length(), 0);
    send(sockfd, created_msg.c_str(), created_msg.length(), 0);
}
