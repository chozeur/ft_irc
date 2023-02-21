/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbrebion <tbrebion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 15:15:40 by tbrebion          #+#    #+#             */
/*   Updated: 2023/02/21 17:01:32 by tbrebion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

irc::server::server() {}

irc::server::~server() {}

irc::config	&irc::server::getConf(){

	return (conf);
}
