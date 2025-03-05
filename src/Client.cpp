/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:23:19 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 16:13:48 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd) : _fd(fd)
{
	std::cout << "New client created: " << _fd << std::endl;
	// readData();
}

Client::~Client()
{
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
        std::cout <<"------------------->" << buffer << std::endl;
		_request.append(buffer, bytesRead);
		// _request += buffer;   // Append new data
		totalRecevied += bytesRead;
		_totalRecevied = totalRecevied;
		
        // std::cout << "Received data from client " << _fd << ":\n" << buffer << std::endl;
		
        // Check if we've reached the end of the HTTP request (double newline)
        if (_request.find("\r\n\r\n") != std::string::npos) 
        {
			std::cout << "Full request received:\n" << _request << "\nCHECKING...\n" << std::endl;
            return true;
        }
    }
    // Handle client disconnect or error:
    if (bytesRead == 0)
    {
		std::cout << "Client " << _fd << " disconnected." << std::endl;
        return false;
    }
    else if (bytesRead == -1)
    {
		std::cerr << "Error reading from client " << _fd << ": " << std::endl;
        return false;
    }
	return false;

}


void	Client::sendData(const std::string &response)
{
	std::cout << "RESPONSE-------------------------> " << response << ": " << std::endl;
	ssize_t bytesSent = send(_fd, response.c_str(), response.length(), 0);
    if (bytesSent == -1)
    {
		std::cerr << "Error sending data to client " << _fd << ": " << std::endl;
    }
    else
    {
		std::cout << "Sent data to client " << _fd << ": " << response << std::endl;
    }
}

std::string Client::getRequest() const
{
	
	std::cout << "Retriving Requests " << _request << std::endl;
	return _request;  // Allow Server to access the raw request
}

size_t		Client::gettotalRecevied()
{
	return _totalRecevied;	
}
// bool Client::readData()
// {
//     char buffer[1024];
//     int bytesRead;

//     while ((bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0)) > 0) 
//     {
//         buffer[bytesRead] = '\0'; // Ensure null termination
//         _request += buffer;   // Append new data

//         std::cout << "Received data from client " << _fd << ":\n" << buffer << std::endl;

//         // Check if we've reached the end of the HTTP request (double newline)
//         if (_request.find("\r\n\r\n") != std::string::npos) 
//         {
//             std::cout << "Full request received:\n" << _request << "\nCHECKING...\n" << std::endl;

//             // ✅ SEND A BASIC RESPONSE
//             std::string response = 
//                 "HTTP/1.1 200 OK\r\n"
//                 "Content-Type: text/html\r\n"
//                 "Content-Length: 13\r\n"
//                 "\r\n"
//                 "Hello, World!";
//             send(_fd, response.c_str(), response.length(), 0);

//             std::cout << "Response sent to client " << _fd << "\n" << std::endl;
//             return true;
//         }
//     }

//     return bytesRead != 0; // If bytesRead is 0, client disconnected
// }