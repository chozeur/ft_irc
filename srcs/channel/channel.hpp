/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:13:37 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/27 13:57:10 by flcarval         ###   ########.fr       */
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
		Channel(Channel const &src);
		Channel(std::string name);
		~Channel(void);

		Channel			&operator=(Channel const &src);

		std::string			getName(void) const;
		void				setName(std::string name);
		std::vector<Client>	getClients(void) const;
		void				setClients(std::vector<Client> clients);

	private:
		std::string			_name;
		std::vector<Client>	_clients;
	};

}

#endif

