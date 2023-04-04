#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <algorithm>
# include <sstream>
# include "../client/client.hpp"

namespace ft_irc {

	class	Client;

	class Channel {

		public:

			Channel(void);
			Channel(Channel const &rhs);
			Channel(std::string name);

			~Channel(void);

			Channel						&operator=(Channel const &rhs);

			std::string					getName(void) const;
			std::string					getTopic(void) const;
			std::vector<Client*> const	&getClients(void) const;
			std::vector<Client>	const 	&getOperators(void) const;
			std::vector<Client>	const 	&getBannedClients(void) const;

			void						setName(std::string const &name);
			void						setTopic(std::string const &name);
			void						setClients(std::vector<Client *> const &clients);
			void						setOperators(std::vector<Client> const &operators);
			void						setBannedClients(std::vector<Client> const &banned_clients);

			void						addClient(Client *client);
			void						removeClient(Client const &client);
			void						addOperator(Client &client);
			void						removeOperator(Client const &client);
			void						addBannedClient(Client &client);
			void						removeBannedClient(Client const &client);
			int							isClientBanned(Client const &client) const;
			int							isClientOp(Client const &client) const;

		private:
			std::string					_name;
			std::string					_topic;
			std::vector<Client *>		_clients;
			std::vector<Client>			_operators;
			std::vector<Client>			_banned_clients;
	};

std::ostream& operator<<(std::ostream& os, ft_irc::Channel* channel);

}

#endif


