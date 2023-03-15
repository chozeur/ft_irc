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
		this->_sender = rhs._sender;
		this->_receiver = rhs._receiver;
		tis->_channel = rhs._channel;
		this->_payload = rhs._payload;
		this->_callback = rhs._callback;
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


