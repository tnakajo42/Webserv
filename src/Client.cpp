/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:23:19 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 16:39:27 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd)
:	_fd(fd)
{
	std::ostringstream oss;
	oss << "New client created: " << _fd;
	Logger::log(oss.str());
	std::cout << "New client created: " << _fd << std::endl;
}

Client::~Client()
{
	std::ostringstream oss;
	oss << "Closing client: " << _fd;
	Logger::log(oss.str());
	std::cout << "Closing client: " << _fd << std::endl;
	close(_fd);
}

int		Client::getFd() const
{
	return _fd;
}

bool Client::readData()
{
	char buffer[1024];
	int bytesRead;
	size_t	totalRecevied = 0;
	
	while ((bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0)) > 0) 
	{
		buffer[bytesRead] = '\0'; // Ensure null termination
		_request.append(buffer, bytesRead);
		totalRecevied += bytesRead;
		_totalRecevied = totalRecevied;
		
	// Check if we've reached the end of the HTTP request (double newline)
		if (_request.find("\r\n\r\n") != std::string::npos) 
		{
			std::ostringstream oss;
			oss << "Full request received:\n" << _request;
			Logger::log(oss.str());
			std::cout << "Full request received:\n" << _request << std::endl;
			return true;
		}
	}
	// Handle client disconnect or error:
	if (bytesRead == 0)
	{
		std::ostringstream oss;
		oss << "Client " << _fd << " disconnected.";
		Logger::log(oss.str());
		std::cout << "Client " << _fd << " disconnected." << std::endl;
		return false;
	}
	else if (bytesRead == -1)
	{
		std::ostringstream oss;
		oss << "Error reading from client " << _fd << ": ";
		Logger::log(oss.str());
		std::cerr << "Error reading from client " << _fd << ": " << std::endl;
		return false;
	}
	return false;
}


void	Client::sendData(const std::string &response)
{
	ssize_t bytesSent = send(_fd, response.c_str(), response.length(), 0);
	if (bytesSent == -1)
	{
		std::ostringstream oss;
		oss << "Error sending data to client with SEND FT" << _fd << ": ";
		Logger::log(oss.str());
		std::cerr << "Error sending data to client with SEND FT" << _fd << ": " << std::endl;
	}
	else
	{
		std::ostringstream oss1;
		oss1 << "Sent data to client " << _fd << ": " << response;
		Logger::log(oss1.str());
		std::cout << "Sent data to client " << _fd << ": " << response << std::endl;
	}
}

std::string Client::getRequest() const
{
	
	std::ostringstream oss;
	oss << "Retriving Requests " << _request;
	Logger::log(oss.str());
	std::cout << "Retriving Requests " << _request << std::endl;
	return _request;  // Allow Server to access the raw request
}

size_t		Client::gettotalRecevied()
{
	return _totalRecevied;	
}