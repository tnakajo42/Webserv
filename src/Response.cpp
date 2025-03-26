/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:29 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/24 13:07:09 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
:	_statusCode(200)
{}

Response::Response(int status, const std::string& message, ConfigParser& config)
:	_statusCode(status), _statusMessage(message), _config(config)
{}

Response::~Response()
{}

void			Response::setStatusCode(int code)
{
	_statusCode = code;
}

void			Response::setStatusMessage(int code)
{
	_statusMessage = code;
}

void			Response::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void			Response::setBody(const std::string &body)
{
	_body = body;
}

// std::string		Response::buildResponseParameters(int status_code, std::string& content, std::string& content_type)
// {
// 	_statusCode = status_code;
// 	_body = content;
// 	_statusMessage = content_type;
	

// 	std::ostringstream	response;
// 	response << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n"; // add error handling
// 	response << "Content-Type: " << "text/html" << "\r\n";
// 	response << "Content-Length: " << _body.size() << "\r\n";
// 	response << "Connection: close\r\n\r\n";
// 	response << _body;
// 	return response.str();
// }


std::string		Response::buildResponse() const
{
	std::ostringstream	response;
	response << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n"; // add error handling
	response << "Content-Type: " << "text/html" << "\r\n";
	response << "Content-Length: " << _body.size() << "\r\n";
	response << "Connection: close\r\n\r\n";
	response << _body;
	return response.str();
}

std::string		Response::getStatusMessage() const
{
	switch (_statusCode)
	{
		case 200: return "OK";
		case 300: return "Found";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 500: return "Internal Server Error";
	// Add other cases as needed.
		default: return "Unknown";
	}
}
