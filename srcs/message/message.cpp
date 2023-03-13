#include "message.hpp"

/* CONSTRUCTORS */

ft_irc::Message::Message(void){return ;}

ft_irc::Message::Message(Message const &rhs){
	*this = rhs;
	return ;
}

ft_irc::Message::Message(std::string payload): _payload(payload){return ;}

/* DESTRUCTOR */

ft_irc::Message::~Message(void){return ;}

/* INIT */

ft_irc::Message&	ft_irc::Message::operator=(Message const &rhs){
	if (this != &rhs){
		this->_payload = rhs._payload;
	}
	return (*this);
}

/* GETTERS */

ft_irc::Client&	ft_irc::Message::getSender(void) const {
	return (*this->_sender);
}

ft_irc::Client&	ft_irc::Message::getReceiver(void) const {
	return (*this->_receiver);
}

std::string	ft_irc::Message::getPayload(void) const {
	return (this->_payload);
}

void	(*ft_irc::Message::getCallback(void))(ft_irc::Client&, ft_irc::Client&, std::string){
	return (this->_callback);
}

/* SETTERS */

void	ft_irc::Message::setSender(ft_irc::Client& sender){
	this->_sender = &sender;
	return ;
}

void	ft_irc::Message::setReceiver(ft_irc::Client& receiver){
	this->_receiver = &receiver;
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

/* METHODS */

void	ft_irc::Message::execute(void){
	if (this->_callback)
		this->_callback(*this->_sender, *this->_receiver, this->_payload);
	return ;
}

void	ft_irc::Message::appendPayload(std::string payload){
	this->_payload += payload;
	return ;
}

std::string	ft_irc::Message::parsePayload(void) const {
	std::size_t pos = this->_payload.find(" ");
	std::string arg = this->_payload.substr(pos+1, this->_payload.size() - pos);
	return (arg);
}
