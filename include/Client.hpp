/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:22:05 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 17:12:14 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

class Client
{
	private:
		int			_fd;
		std::string	_request;
		size_t		_totalRecevied;

	public:
		Client(int fd);
		~Client();

		int			getFd() const;
		bool		readData();
		void		sendData(const std::string &response);
		std::string getRequest() const;
		size_t		gettotalRecevied();
};

#endif // CLIENT_HPP
