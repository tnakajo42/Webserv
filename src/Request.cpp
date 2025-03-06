/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:17 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 16:43:46 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

Request::Request()
{}

Request::~Request()
{}

int stringToIntRequest(const std::string& str)
{
	std::stringstream ss(str);
	int result;
	ss >> result;
	return result;
}

bool Request::parseRequest(const std::string &rawRequest)
{
	std::istringstream requestStream(rawRequest);
	std::string line;

	// Parse request line (e.g., "GET /index.html HTTP/1.1")
	if (!std::getline(requestStream, line) || line.empty())
		return false;
	std::istringstream lineStream(line);
	lineStream >> _method >> _path >> _httpVersion;

	// Parse headers
	while (std::getline(requestStream, line) && !line.empty())
	{
		std::size_t delimiter = line.find(": ");
		if (delimiter != std::string::npos)
		{
			std::string key = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 2);
			_headers[key] = value;
		}
	}

	// Read body if Content-Length is present
	if (_headers.find("Content-Length") != _headers.end())
	{
		// int contentLength = std::stoi(_headers["Content-Length"]);
		int contentLength = stringToIntRequest(_headers["Content-Length"]);
		_body.resize(contentLength);
		requestStream.read(&_body[0], contentLength);
	}

	return true;
}

std::string Request::getMethod() const
{
	return _method;
}
std::string Request::getPath() const
{
	return _path;
}
std::string Request::getHeader(const std::string &key) const
{ 
	return _headers.count(key) ? _headers.at(key) : "";
}
std::string Request::getBody() const
{
	return _body;
}
