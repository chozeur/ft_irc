/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:13:37 by flcarval          #+#    #+#             */
/*   Updated: 2023/03/07 22:35:56 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include "../client/client.hpp"

namespace	ft_irc {

	class	Channel {

		public:

			Channel(void);
			Channel(Channel const &rhs);
			Channel(std::string name);

			~Channel(void);

			Channel				&operator=(Channel const &rhs);

			std::string			getName(void) const;
			std::vector<Client>	getClients(void) const;
			std::vector<Client>	getOperators(void) const;
			std::vector<Client>	getBannedClients(void) const;

			void				setName(std::string name);
			void				setClients(std::vector<Client> clients);
			void				setOperators(std::vector<Client> operators);
			void				setBannedClients(std::vector<Client> banned_clients);

		private:

			std::string			_name;
			std::vector<Client>	_clients;
			std::vector<Client>	_operators;
			std::vector<Client>	_banned_clients;
	};

}

#endif

