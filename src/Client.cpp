/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnakajo <nj.takayuki@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:23:19 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/21 18:13:03 by tnakajo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd)
:	_fd(fd), _totalRecevied(0) {}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_request = other._request;
		_totalRecevied = other._totalRecevied;
	}
	return *this;
}

Client::~Client()
{
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
			return true;
	}
	// Handle client disconnect or error:
	if (bytesRead == 0)
	{
		//Logger::logInt("Client disconnected: ", _fd);
		// std::cout << "Client " << _fd << " disconnected." << std::endl;
		return false;
	}
	else if (bytesRead == -1)
	{
		//Logger::logInt("Error reading from client: ", _fd);
		// std::cerr << "Error reading from client " << _fd << ": " << std::endl;
		return false;
	}
	return false;
}


bool	Client::sendData(const std::string &response)
{
	ssize_t bytesSent = send(_fd, response.c_str(), response.length(), 0);
	if (bytesSent < 0)
	{
		//Logger::logInt("Error sending data to client with SEND FT: ", _fd);
		// std::cerr << "Error sending data to client with SEND FT" << _fd << ": " << std::endl;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false; // Try again later
		return true; // Error, should close connection
	}
	else
	{
		//Logger::logInt("Sent data to client: ", _fd);
		// std::cout << "Sent data to client " << _fd << ": " << response << std::endl;
		_totalRecevied += bytesSent; // Update progress
        return (_totalRecevied >= response.size());
	}
}

std::string Client::getRequest() const
{
	std::string	req = _request;
	//Logger::logStr("Retriving Requests ", req);
	// std::cout << "Retriving Requests " << _request << std::endl;
	return _request;  // Allow Server to access the raw request
}

size_t		Client::gettotalRecevied()
{
	return _totalRecevied;	
}