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
	_userLine(""){
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
	_userLine(""){
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
	_userLine("") {
	return ;
}

/* DESTRUCTOR */

ft_irc::Client::~Client(void){
	std::cerr << "Client destructor called" << std::endl;
	return ;
}

/* INIT */

ft_irc::Client&	ft_irc::Client::operator=(Client const &rhs){
	if (this != &rhs){
		this->_isSet = rhs._isSet;
		this->_isBot = rhs._isBot;
		this->_sockfd = rhs._sockfd;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_password = rhs._password;
		this->_host = rhs._host;
		this->_userLine = rhs._userLine;
		this->_channels = rhs._channels;
	}
	return (*this);
}

bool ft_irc::Client::operator==(const Client &other) const {
	return this->_nickname == other._nickname;
}

/* GETTERS */

int	ft_irc::Client::getSockfd(void) const {
	return (this->_sockfd);
}

std::string	ft_irc::Client::getNickname(void) const {
	return (this->_nickname);
}

std::string	ft_irc::Client::getUsername(void) const {
	return (this->_username);
}

std::string	ft_irc::Client::getRealname(void) const {
	return (this->_realname);
}

std::string	ft_irc::Client::getPassword(void) const {
	return (this->_password);
}

std::string	ft_irc::Client::getHost(void) const {
	return (this->_host);
}

std::string	ft_irc::Client::getServername(void) const {
	return (this->_servername);
}

std::string	ft_irc::Client::getUserLine(void) const {
	return (this->_userLine);
}

std::vector<std::string>	ft_irc::Client::getChannels(void) const {
	return (this->_channels);
}

/* SETTERS */

void	ft_irc::Client::setSockfd(int sockfd){
	this->_sockfd = sockfd;
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

void	ft_irc::Client::setRealname(std::string realname){
	this->_realname = realname;
	return ;
}

void	ft_irc::Client::setPassword(std::string password){
	this->_password = password;
	return ;
}

void	ft_irc::Client::setHost(std::string host){
	this->_host = host;
	return ;
}

void	ft_irc::Client::setServername(std::string servername){
	this->_servername = servername;
	return ;
}

void	ft_irc::Client::setUserLine(std::string userLine){
	this->_userLine = userLine;
	return ;
}

void	ft_irc::Client::setChannels(std::vector<std::string> channels){	//! deep copy
	this->_channels = channels;
	return ;
}

void ft_irc::Client::setIsBot(bool isBot) {
    _isBot = isBot;
}

void ft_irc::Client::setIsSet(bool isSet) {
    _isSet = isSet;
}

/* METHODS */

bool ft_irc::Client::isBot() const {
    return _isBot;
}

bool ft_irc::Client::isSet() const {
    return _isSet;
}

void ft_irc::Client::handleMessage(int serverSockFd, std::string text, Client *bot, Client *receiver) {
    (void)text;
	(void)serverSockFd;
	std::string response;

	if (text == "ping") {
        response = "Pong!";
    } else if (text == "hello") {
        response = "Bonjour!";
    } else if (text == "time") {
        time_t now = time(0);
        response = "Il est " + std::string(ctime(&now));
    } else if (text == "help") {
		response = "Les commandes disponibles sont : ping, hello, time.";
	} else {
        response = "Commande inconnue";
    }

    std::string messageToSend = ":" + bot->getNickname() + " PRIVMSG " + receiver->getNickname() + " :" + response + "\r\n";
    send(receiver->getSockfd(), messageToSend.c_str(), messageToSend.length(), 0);
}


/* OVERLOADS */

std::ostream& ft_irc::operator<<(std::ostream& os, const ft_irc::Client& client)
{
    os << "Client [sockfd : " << client.getSockfd() << ", nickname : " << client.getNickname() << ", realname :" << client.getRealname() << ", servername : " << client.getServername() << ", hostname : " << client.getHost() << "]";
    return os;
}