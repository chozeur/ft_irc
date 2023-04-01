#include "client.hpp"

/* CONSTRUCTORS */

ft_irc::Client::Client(void):
	_isSet(false),
	_isBot(false),
	_sockfd(-1),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host(""),
	_userLine(""),
	_idle(time(0)),
	_signon(time(0)) {
	return ;
}

ft_irc::Client::Client(Client const & rhs):
	_isSet(false),
	_isBot(false),
	_sockfd(-1),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host(""),
	_userLine(""),
	_idle(time(0)),
	_signon(time(0)) {
	*this = rhs;
	return ;
}

ft_irc::Client::Client(int sockfd):
	_isSet(false),
	_isBot(false),
	_sockfd(sockfd),
	_nickname(""),
	_username(""),
	_realname(""),
	_password(""),
	_host(""),
	_userLine(""),
	_idle(time(0)),
	_signon(time(0)) {
	return ;
}

/* DESTRUCTOR */

ft_irc::Client::~Client(void){
	std::cerr << "Client destructor called" << std::endl;
	return ;
}

/* INIT */

ft_irc::Client&				ft_irc::Client::operator=(Client const &rhs){
	if (this != &rhs){
		this->_isSet = rhs._isSet;
		this->_isBot = rhs._isBot;
		this->_sockfd = rhs._sockfd;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_password = rhs._password;
		this->_host = rhs._host;
		this->_servername = rhs._servername;
		this->_userLine = rhs._userLine;
		this->_channels = rhs._channels;
		this->_idle = rhs._idle;
	}
	return (*this);
}

bool 						ft_irc::Client::operator==(const Client &other) const {
	return this->_nickname == other._nickname;
}

/* GETTERS */

int							ft_irc::Client::getSockfd(void) const {
	return (this->_sockfd);
}

std::string					ft_irc::Client::getNickname(void) const {
	return (this->_nickname);
}

std::string					ft_irc::Client::getUsername(void) const {
	return (this->_username);
}

std::string					ft_irc::Client::getRealname(void) const {
	return (this->_realname);
}

std::string					ft_irc::Client::getPassword(void) const {
	return (this->_password);
}

std::string					ft_irc::Client::getHost(void) const {
	return (this->_host);
}

std::string					ft_irc::Client::getServername(void) const {
	return (this->_servername);
}

std::string					ft_irc::Client::getUserLine(void) const {
	return (this->_userLine);
}

time_t						ft_irc::Client::getIdle(void) const {
	return (this->_idle);
}

time_t						ft_irc::Client::getSignon(void) const {
	return (this->_signon);
}

std::vector<ft_irc::Channel*>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

/* SETTERS */

void						ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
	return ;
}

void						ft_irc::Client::setNickname(std::string nickname){
	this->_nickname = nickname;
	return ;
}

void						ft_irc::Client::setUsername(std::string username){
	this->_username = username;
	return ;
}

void						ft_irc::Client::setRealname(std::string realname){
	this->_realname = realname;
	return ;
}

void						ft_irc::Client::setPassword(std::string password){
	this->_password = password;
	return ;
}

void						ft_irc::Client::setHost(std::string host){
	this->_host = host;
	return ;
}

void						ft_irc::Client::setServername(std::string servername){
	this->_servername = servername;
	return ;
}

void						ft_irc::Client::setUserLine(std::string userLine){
	this->_userLine = userLine;
	return ;
}

void						ft_irc::Client::setIdle(void){
	this->_idle = time(0);
	return ;
}


void						ft_irc::Client::setChannels(std::vector<ft_irc::Channel*> channels){	//! deep copy
	this->_channels = channels;
	return ;
}

void 						ft_irc::Client::setIsBot(bool isBot) {
    _isBot = isBot;
}

void 						ft_irc::Client::setIsSet(bool isSet) {
    _isSet = isSet;
}

/* METHODS */

bool 						ft_irc::Client::isBot() const {
    return _isBot;
}

bool 						ft_irc::Client::isSet() const {
    return _isSet;
}

void 						ft_irc::Client::handleMessage(int serverSockFd, std::string text, Client *bot, Client *receiver) {
    (void)text;
	(void)serverSockFd;
	std::string	response = this->gpt(text);

	if (response.find("\n") != std::string::npos){
		std::vector<std::string> lines = split(response, "\n");
		for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
			if (*it != ""){
				std::string messageToSend = ":" + bot->getNickname() + " PRIVMSG " + receiver->getNickname() + " :" + *it + "\r\n";
				send(receiver->getSockfd(), messageToSend.c_str(), messageToSend.length(), 0);
			}
		}
	} else {
		std::string messageToSend = ":" + bot->getNickname() + " PRIVMSG " + receiver->getNickname() + " :" + response + "\r\n";
		send(receiver->getSockfd(), messageToSend.c_str(), messageToSend.length(), 0);
	}
}

void 		ft_irc::Client::addChannel(Channel *channel) {
	this->_channels.push_back(channel);
}

ft_irc::Channel*			ft_irc::Client::getChanPointer(std::string name) {
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if ((*it)->getName() == name)
            return (*it);
    }
    return NULL;
}

void						ft_irc::Client::removeChannel(Channel const &channel){
	std::vector<Channel *>::iterator it = std::find(this->_channels.begin(), this->_channels.end(), &channel);
    if (it != this->_channels.end())
        this->_channels.erase(it);
}

/* OVERLOADS */

std::ostream& ft_irc::operator<<(std::ostream& os, const ft_irc::Client& client)
{
    os << "Client [sockfd : " << client.getSockfd() << ", nickname : " << client.getNickname() << ", realname :" << client.getRealname() << ", servername : " << client.getServername() << ", hostname : " << client.getHost() << "]";
    return os;
}

/* OTHER */

std::string	ft_irc::Client::unicorn(void) const {
	const char *host = "artscene.textfiles.com";
	const char *path = "/asciiart/unicorn";
	const char *method = "GET";

	// Get the server's address
	struct hostent *server = gethostbyname(host);
	if (server == NULL) {
		std::cerr << "Error: Could not resolve host" << std::endl;
		return ("ERROR IN UNICORN");
	}

	// Create a socket
	int portno = 80;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << "Error: Could not create socket" << std::endl;
		return ("ERROR IN UNICORN");
	}

	// Set up the server address structure
	struct sockaddr_in serv_addr;
	std::memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
	serv_addr.sin_port = htons(portno);

	// Connect to the server
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		std::cerr << "Error: Could not connect to server" << std::endl;
		return ("ERROR IN UNICORN");
	}

	// Construct the HTTP request
	std::string request = std::string(method) + " " + std::string(path) + " HTTP/1.1\r\n";
	request += "Host: " + std::string(host) + "\r\n\r\n";

	// Send the request
	if (send(sockfd, request.c_str(), request.length(), 0) < 0) {
		std::cerr << "Error: Could not send request" << std::endl;
		return ("ERROR IN UNICORN");
	}

	// Receive the response
	const int buffer_len = 1024;
	char buffer[buffer_len];
	std::string response = "";
	int bytes_received;
	do {
		bytes_received = recv(sockfd, buffer, buffer_len-1, 0);
		if (bytes_received < 0) {
			std::cerr << "Error: There was an error receiving the response" << std::endl;
			return ("ERROR IN UNICORN");
		}
		buffer[bytes_received] = '\0';
		response += std::string(buffer);
		response = remove_html_header(response);
	} while (bytes_received > 0);

	// Close the socket
	close(sockfd);

	return (response);
}

std::string	ft_irc::Client::gpt(std::string prompt) const {
	// Initialize the post data
	std::string post_data = std::string("prompt=") + prompt;

	// Create a TCP socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1) {
		std::cerr << "socket() failed" << std::endl;
		throw (std::exception());
	}

	// Resolve the IP address of the Flask web server
	struct addrinfo hints, *result;
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int res = getaddrinfo("92.151.62.116", "4242", &hints, &result);

	if (res != 0) {
		std::cerr << "getaddrinfo() failed: " << gai_strerror(res) << std::endl;
		close(sock);
		throw (std::exception());
	}

	// Connect to the Flask web server
	res = connect(sock, result->ai_addr, result->ai_addrlen);

	if (res == -1) {
		std::cerr << "connect() failed" << std::endl;
		freeaddrinfo(result);
		close(sock);
		throw (std::exception());
	}

	std::cout << "Connected to Flask web server" << std::endl;

	// Send the POST request
	std::string post_request =
			"POST / HTTP/1.1\r\n"
			"Host: 92.151.62.116:4242\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: " + std::to_string(post_data.size()) + "\r\n"
			"\r\n" + post_data;

	res = send(sock, post_request.c_str(), post_request.size(), 0);

	if (res == -1) {
		std::cerr << "send() failed" << std::endl;
		freeaddrinfo(result);
		close(sock);
		throw (std::exception());
	}

	std::cout << "POST request sent" << std::endl;

	// Receive the HTTP response
	char buf[1024];
	std::string http_response;

	do {
		res = recv(sock, buf, sizeof(buf), 0);

		if (res > 0) {
			http_response.append(buf, res);
		} else if (res == 0) {
			std::cerr << "Connection closed by server" << std::endl;
		} else {
			std::cerr << "recv() failed" << std::endl;
			freeaddrinfo(result);
			close(sock);
			throw (std::exception());
		}
	} while (res > 0);

	// Extract the response body from the HTTP response
	std::string response_body;

	std::string::size_type pos = http_response.find("\r\n\r\n");

	if (pos != std::string::npos) {
		response_body = http_response.substr(pos + 4);
	} else {
		std::cerr << "Invalid HTTP response" << std::endl;
		freeaddrinfo(result);
		close(sock);
		throw (std::exception());
	}

	// Print the response body
	std::cout << "Response: " << response_body << std::endl;

	// Cleanup
	freeaddrinfo(result);
	close(sock);
	return response_body;
}
