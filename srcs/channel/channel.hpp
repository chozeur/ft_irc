/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcarval <flcarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 13:13:37 by flcarval          #+#    #+#             */
/*   Updated: 2023/02/27 13:24:00 by flcarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>

namespace	ft_irc {

	class	Channel {

	public:
		Channel(void);
		Channel(Channel const &src);
		Channel(std::string name);
		~Channel(void);

		Channel			&operator=(Channel const &src);

		std::string		getName(void) const;
		void			setName(std::string name);

	private:
		std::string		_name;
	};

}

#endif

