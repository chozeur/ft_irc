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

ft_irc::Server&							ft_irc::Server::operator=(Server const &rhs){
	if (this != &rhs){
		this->_name = rhs._name;
		this->_ip = rhs._ip;
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_tstart = rhs._tstart;
		// this->_fds = rhs._fds; //! deep copy
		this->_serv_addr = rhs._serv_addr;
		this->_sockfd = rhs._sockfd;
		this->_env = rhs._env; //! deep copy
		this->_clients = rhs._clients; //? deep copy
		this->_channels = rhs._channels; //? deep copy
		this->_commands = rhs._commands; //? deep copy
	}
	return (*this);
}

/* GETTERS */

std::string								ft_irc::Server::getName(void)const{
	return (this->_name);
}

std::string								ft_irc::Server::getIp(void)const{
	return (this->_ip);
}

long									ft_irc::Server::getPort(void) const {
	return (this->_port);
}

std::string								ft_irc::Server::getPassword(void)const{
	return (this->_password);
}

int										ft_irc::Server::getTstart(void)const{
	return (this->_tstart);
}

struct sockaddr_in						ft_irc::Server::getServAddr()const{
	return (this->_serv_addr);
}

int										ft_irc::Server::getSockfd(void)const{
	return (this->_sockfd);
}

char**									ft_irc::Server::getEnv(void)const{
	return (this->_env);
}

std::vector<ft_irc::Client *>*			ft_irc::Server::getClients() {
	return (&_clients);
}

ft_irc::Client* 						ft_irc::Server::getClientPointerByFd(int fd) {
    std::vector<Client *>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if ((*it)->getSockfd() == fd)
            return (*it);
    }
    return NULL;
}

ft_irc::Client* 						ft_irc::Server::getClientPointerByNick(std::string nick) {
    std::vector<Client *>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if ((*it)->getNickname() == nick)
            return (*it);
    }
    return NULL;
}

std::vector<ft_irc::Client *>::iterator ft_irc::Server::getClientIterator(int fd) {
	std::vector<Client *>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if ((*it)->getSockfd() == fd)
			return it;
	}
	return this->_clients.end();
}

std::vector<ft_irc::Channel*>* 			ft_irc::Server::getChannels() {
    return &_channels;
}

ft_irc::Channel* 						ft_irc::Server::getChannelPointer(std::string name) {
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if ((*it)->getName() == name)
            return (*it);
    }
    return NULL;
}

std::map<std::string, CommandFunction>* ft_irc::Server::getCommands(void) {
    return (&(_commands));
}

struct pollfd*							ft_irc::Server::getFds(void){
	return (_fds);
}

/* SETTERS */

void	ft_irc::Server::setName(std::string name){
	this->_name = name;
	return ;
}

void	ft_irc::Server::setIp(std::string ip){
	this->_ip = ip;
	return ;
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
	initCommands();
	initChannels();
	this->_name = "WERENOIRC";
	this->_ip = "127.0.0.1";
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

	Client* bot = new Client(-1, this);
	bot->setIsBot(true);
	bot->setNickname("MasterBot");
	_clients.push_back(bot);

	for (int i = 1; i <= MAX_CLIENTS; ++i)
	{
		this->_fds[i].fd = -1;
		this->_fds[i].events = POLLIN;
	}
	std::cout << "Server listening on 127.0.0.1:" << _port << std::endl;
	this->_tstart = time(NULL);
	return ;
}

void 	ft_irc::Server::initChannels() {
    ft_irc::Channel* general = new Channel("general");
    ft_irc::Channel* admin = new Channel("admin");
    _channels.push_back(general);
    _channels.push_back(admin);
}

void	ft_irc::Server::run(void) {
	while (server) {
		// std::string pong = std::string("PONG :") + this->_ip + std::string("\r\n");
		// sendToAllClients(pong);

		this->purgeChannels();
		this->purgeClients();
		this->logLoop();

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

					ft_irc::Client* new_client = new Client;
					new_client->setSockfd(clientfd);
					new_client->setIsBot(false);
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
				if (bytes_received <= 0) {
					if (bytes_received == -1)
						std::cerr << "Error: message receiving failed on fd : " << this->_fds[i].fd << std::endl;
					closeClient(i);
				} else {
					std::string message(buffer, bytes_received);

					ft_irc::Message *command = new Message(message, getClientPointerByFd(this->_fds[i].fd), this);
					delete command;
				}
			}
		}
		break ;
	}
	return ;
}

void	ft_irc::Server::stop(void) {
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete ((*it));
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		delete (*it);
	}
	close(this->_sockfd);
	std::cerr << "Turn off server here" << std::endl;
}

void 	ft_irc::Server::sendIrcResponse(int sockfd, ft_irc::Client *client) const {
	if (!client) {
		std::cerr << "Error: Null client pointer passed to sendIrcResponse" << std::endl;
		return;
	}

	std::stringstream ss;
	ss << _port;
	std::string port = ss.str();

	std::ostringstream stream;

	stream << "CAP * LS :\r\n";
	std::string cap_response = stream.str();
	stream.str("");

	stream << ":" << _ip << " 001 " << client->getNickname() << " :";
	colors::green(stream);
	stream << "Welcome to the " << _name << " " << client->getNickname() << "!" << client->getUsername() << "@" << client->getHost();
	colors::reset(stream);
	stream << "\r\n";
	std::string welcome_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 002 " << client->getNickname() << " :";
	colors::green(stream);
	stream << "Your host is " << client->getServername() << ":" << port << ", running version 1.0.2";
	colors::reset(stream);
	stream << "\r\n";
	std::string version_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 003 " << client->getNickname() << " :";
	colors::green(stream);
	stream << "This server was created in 42 School";
	colors::reset(stream);
	stream << "\r\n";
	std::string created_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 004 " << client->getNickname() << " :";
	colors::yellow(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream 	<< "________________________________________________________________________\n";
	colors::yellow(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream 	<< "________________________________________________________________________\n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "                                    \n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "                                   (_)\n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< " __      _____ _ __ ___ _ __   ___  _ \n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< " \\ \\ /\\ / / _ \\ '__/ _ \\ '_ \\ / _ \\| |\n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "  \\ V  V /  __/ | |  __/ | | | (_) | |\n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "   \\_/\\_/ \\___|_|  \\___|_| |_|\\___/|_| . RC\n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "                                      \n";
	colors::blue(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream << "                ";
	stream 	<< "                                      \n";
	colors::yellow(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream 	<< "________________________________________________________________________\n";
	colors::yellow(stream);
	colors::bold(stream);
	colors::blink(stream);
	stream 	<< "________________________________________________________________________\n";
	colors::reset(stream);
	stream << "\r\n";
	std::string ascii_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 005 " << client->getNickname() << " :";
	colors::red(stream);colors::on_grey(stream);colors::bold(stream);
	stream << "CHANNELS(" << _channels.size() << "):";
	colors::reset(stream);
	colors::yellow(stream);colors::italic(stream);colors::bold(stream);
	for (std::vector<Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); it++) {
		stream << " #" << (*it)->getName();
	}
	colors::reset(stream);
	stream << "\r\n";
	std::string channels_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 006 " << client->getNickname() << " :";
	colors::green(stream);colors::on_grey(stream);colors::bold(stream);
	stream << "   USERS(" << _clients.size() << "):";
	colors::reset(stream);
	colors::white(stream);colors::italic(stream);colors::bold(stream);
	for (std::vector<Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++) {
		stream << " @" << (*it)->getNickname();
	}
	colors::reset(stream);
	stream << "\r\n";
	std::string users_msg = stream.str();
	stream.str("");

	stream << ":" << _ip << " 007 " << client->getNickname() << " :";
	colors::blue(stream);colors::on_grey(stream);colors::italic(stream);
	stream << "     uptime:";
	colors::reset(stream);
	stream << ' ' << this->HRuptime();
	colors::reset(stream);
	stream << "\r\n";
	std::string uptime_msg = stream.str();
	stream.str("");

	send(sockfd, cap_response.c_str(), cap_response.length(), 0);
	send(sockfd, welcome_msg.c_str(), welcome_msg.length(), 0);
	send(sockfd, version_msg.c_str(), version_msg.length(), 0);
	send(sockfd, created_msg.c_str(), created_msg.length(), 0);
	send(sockfd, ascii_msg.c_str(), ascii_msg.length(), 0);
	send(sockfd, channels_msg.c_str(), channels_msg.length(), 0);
	send(sockfd, users_msg.c_str(), users_msg.length(), 0);
	send(sockfd, uptime_msg.c_str(), uptime_msg.length(), 0);
}

bool 	ft_irc::Server::parsingNickname(int fd, std::string nickname) {
	if (nickname.empty())
		return false;
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        if ((*it)->getNickname() == nickname && (*it)->getSockfd() != fd)
            return false;
    return true;
}

bool	ft_irc::Server::parsingPassword(std::string password)const{
	if (this->_password != password)
		return (false);
	return (true);
}

void 	ft_irc::Server::closeClient(int i) {
	std::cerr << "\033[1m" << _name << "\033[0m => [" << getClientPointerByFd(_fds[i].fd)->getNickname() << "] CONNECTION CLOSED" << std::endl;
	std::vector<Client *>::iterator client_it = getClientIterator(this->_fds[i].fd);
	if (client_it != this->_clients.end()) {
		this->_clients.erase(client_it);
		// delete (*(client_it));
	}
	close(this->_fds[i].fd);
	this->_fds[i].fd = -1;
}

void 	ft_irc::Server::printClients(void) {
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		std::cout << **it << std::endl;
	}
}

void 	ft_irc::Server::sendToAllClients(std::string &msg) {
    for (std::vector<ft_irc::Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
        if (!(*it)->isBot()) {
			if (send((*it)->getSockfd(), msg.c_str(), msg.size(), 0) == -1) {
				std::cerr << "Error sending message to client" << std::endl;
			}
		}
    }
}

int ft_irc::Server::uptime() const {return (time(NULL) - this->_tstart);}

std::string	ft_irc::Server::HRuptime() const {
	std::stringstream stream;
	int uptime = time(NULL) - this->_tstart;
	int days = uptime / 86400;
	int hours = (uptime % 86400) / 3600;
	int minutes = (uptime % 3600) / 60;
	int seconds = uptime % 60;
	(void)days;
	(void)hours;
	stream << minutes << " minutes, " << seconds << " seconds";
	return (stream.str());
}

// std::string	ft_irc::Server::info(void) const {
// 	std::stringstream stream;
// 	stream << "> name: " << this->_name << std::endl;
// 	stream << "> IP: " << this->_ip << std::endl;
// 	stream << "> port: " << this->_port << std::endl;
// 	stream << "> password: " << "**********" << std::endl;
// 	stream << "> max clients: " << MAX_CLIENTS << std::endl;
// 	stream << "> uptime: " << this->HRuptime() << std::endl;
// 	stream << "> channels: " << this->_channels.size() << std::endl;
// 	stream << "> clients: " << this->_clients.size() << std::endl;
// 	return (stream.str());
// }


void	ft_irc::Server::purgeChannels(void) {
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it) {
		if ((*it)->getClients().size() == 0 && (*it)->getName() != "general" && (*it)->getName() != "admin") {
			std::cout << "Deleting channel " << (*it)->getName() << std::endl;
			std::cout << (*it) << std::endl;
			delete (*it);
			this->_channels.erase(it);
			if (it == this->_channels.end())
				break;
		}
	}
}

void	ft_irc::Server::purgeClients(void) {
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it) {
		if ((*it)->getSockfd() == -1 && (*it)->getNickname() != "MasterBot") {
			std::cout << "Deleting client " << (*it)->getNickname() << std::endl;
			std::cout << (*it) << std::endl;
			delete (*it);
			this->_clients.erase(it);
			if (it == this->_clients.end())
				break;
		}
	}
}

void	ft_irc::Server::logLoop(void) const {
	std::stringstream stream;
	std::time_t now = std::time(NULL);
    std::tm* local_time = std::localtime(&now);
    int hour = local_time->tm_hour;
	int min = local_time->tm_min;

	stream << std::endl << std::endl;

	colors::green(stream);colors::bold(stream);
	stream << "____________________________________________________________" << std::endl;
	stream << "----------------------SERVER LOGS---------------------------" << std::endl;
	colors::reset(stream);

	stream << std::endl;

	colors::yellow(stream);colors::bold(stream);
	stream << "\t  Server " << this->_name << " started at " << hour << ':' << min << std::endl;
	colors::reset(stream);

	stream << std::endl;

	colors::blue(stream);colors::bold(stream);
	stream << "\t  IP: " << this->_ip;
	colors::reset(stream);
	colors::bright_grey(stream);colors::bold(stream);
	stream << "\t\tPort: " << this->_port << std::endl;
	colors::reset(stream);

	stream << std::endl;

	colors::red(stream);colors::bold(stream);
	stream << "\t  Password: " << this->_password;
	colors::reset(stream);
	colors::bright_grey(stream);colors::bold(stream);
	stream << "\tMax clients: " << MAX_CLIENTS << std::endl;
	colors::reset(stream);

	stream << std::endl;

	colors::cyan(stream);colors::bold(stream);colors::underline(stream);
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	stream << "\t\t\tChannels" << std::endl;
	colors::reset(stream);
	colors::bright_grey(stream);colors::bold(stream);
	std::vector<Channel *>::const_iterator it = this->_channels.begin();
	colors::reset(stream);
	while (it != this->_channels.end()) {
		colors::on_bright_grey(stream);colors::bold(stream);
		if ((*it)->getClients().size() > 0){
			colors::bright_white(stream);
		}
		else {
			colors::white(stream);
		}
		stream << '#' << (*it)->getName() << ':' << (*it)->getClients().size();
		colors::reset(stream);
		stream << std::endl;
		colors::bright_grey(stream);colors::bold(stream);
		if ((*it)->getClients().size() > 0){
			std::vector<Client *>::const_iterator it2 = (*it)->getClients().begin();
			while (it2 != (*it)->getClients().end()) {
				stream << '\t' << '@' << (*it2)->getNickname() << std::endl;
				++it2;
			}
		} else {
			stream << std::endl;
		}
		++it;
	}
	colors::reset(stream);

	stream << std::endl;

	colors::yellow(stream);colors::bold(stream);colors::underline(stream);
	stream << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	stream << "\t\t\tClients" << std::endl;
	colors::reset(stream);
	colors::bright_grey(stream);colors::bold(stream);
	std::vector<Client *>::const_iterator it2 = this->_clients.begin();
	while (it2 != this->_clients.end()) {
		stream << '@' << (*it2)->getNickname() << std::endl;
		++it2;
	}
	colors::reset(stream);

	stream << std::endl;

	colors::green(stream);colors::bold(stream);
	stream << "------------------------------------------------------------" << std::endl;
	stream << "____________________________________________________________" << std::endl;
	colors::reset(stream);
	std::cout << stream.str();
	return ;
}
