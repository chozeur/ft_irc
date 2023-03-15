#include "message.hpp"

/* CONSTRUCTORS */

ft_irc::Message::Message(void){return ;}

ft_irc::Message::Message(Message const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Message::Message(std::string payload,
		ft_irc::Client* sender,
		ft_irc::Server* server
		):
		_payload(payload),
		_sender(sender),
		_server(server),
		_receiver(NULL),
		_channel(NULL),
		_callback(NULL) {
	this->parsePayload();
	return ;
}

/* DESTRUCTOR */

ft_irc::Message::~Message(void){
	return ;
}

/* INIT */

ft_irc::Message&	ft_irc::Message::operator=(Message const &rhs){
	if (this != &rhs){
		this->_server = rhs._server;
		this->_sender = rhs._sender;
		this->_receiver = rhs._receiver;
		this->_channel = rhs._channel;
		this->_payload = rhs._payload;
		this->_callback = rhs._callback;
	}
	return (*this);
}

/* GETTERS */

ft_irc::Server*	ft_irc::Message::getServer(void) const {
	return (this->_server);
}

ft_irc::Client*	ft_irc::Message::getSender(void) const {
	return (this->_sender);
}

ft_irc::Client*	ft_irc::Message::getReceiver(void) const {
	return (this->_receiver);
}

ft_irc::Channel*	ft_irc::Message::getChannel(void) const {
	return (this->_channel);
}

std::string	ft_irc::Message::getPayload(void) const {
	return (this->_payload);
}

void	(*ft_irc::Message::getCallback(void))(ft_irc::Client&, ft_irc::Client&, std::string){
	return (this->_callback);
}

/* SETTERS */

void	ft_irc::Message::setServer(ft_irc::Server* server){
	this->_server = server;
	return ;
}

void	ft_irc::Message::setSender(ft_irc::Client* sender){
	this->_sender = sender;
	return ;
}

void	ft_irc::Message::setReceiver(ft_irc::Client* receiver){
	this->_receiver = receiver;
	return ;
}

void	ft_irc::Message::setChannel(ft_irc::Channel* channel){
	this->_channel = channel;
	return ;
}

void	ft_irc::Message::setPayload(std::string payload){
	this->_payload = payload;
	return ;
}

void	ft_irc::Message::setCallback(void (*callback)(ft_irc::Client&, ft_irc::Client&, std::string)){
	this->_callback = callback;
	return ;
}

void	ft_irc::Message::parsePayload(void) {
	cleanLine(_payload);
	std::cerr << _server->getName() << " [" << _sender->getNickname() << "] => " << _payload << std::endl;

	size_t pos = _payload.find(' ');
	std::string cmd = _payload.substr(0, pos);
	std::string param = _payload.substr(pos + 1);

	std::map<std::string, CommandFunction>* commands = _server->getCommands();
	if (commands->find(cmd) != commands->end()) {
		CommandFunction func = (*commands)[cmd];
		func(this, param);
	}
}


std::ostream& ft_irc::operator<<(std::ostream& os, const ft_irc::Message& message)
{
    os << "New Message instance created for " << message.getSender()->getNickname();
    return os;
}