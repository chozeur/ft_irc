#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include "../client/client.hpp"
# include "../channel/channel.hpp"
# include "../../includes/utils.hpp"

namespace	ft_irc {

	class	Server;

	class	Message {

		public:

			Message(void);
			Message(Message const &rhs);
			Message(std::string payload, ft_irc::Client* sender, ft_irc::Server* server);

			~Message(void);

			Message				&operator=(Message const &rhs);

			ft_irc::Server*		getServer(void) const;
			ft_irc::Client*		getSender(void) const;
			std::string			getPayload(void) const;

			// ft_irc::Client*		getReceiver(void) const;
			// ft_irc::Channel*	getChannel(void) const;
			// void				(*getCallback(void))(ft_irc::Client&, ft_irc::Client&, std::string);

			void				setServer(ft_irc::Server* server);
			void				setSender(ft_irc::Client* sender);
			void				setPayload(std::string payload);

			// void				setReceiver(ft_irc::Client* receiver);
			// void				setChannel(ft_irc::Channel* channel);
			// void				setCallback(void (*callback)(ft_irc::Client&, ft_irc::Client&, std::string));

			void				parsePayload(void);

		private:

			std::string			_payload;
			ft_irc::Client		*_sender;
			ft_irc::Server		*_server;
			
			// ft_irc::Client		*_receiver;
			// ft_irc::Channel		*_channel;
			// void				(*_callback)(ft_irc::Client&, ft_irc::Client&, std::string);
	};

std::ostream& operator<<(std::ostream& os, const ft_irc::Message& message);

}

# include "../server/server.hpp"

#endif
