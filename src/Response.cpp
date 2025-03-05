/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:29 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 14:01:54 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"
#include <sstream>

Response::Response() : _statusCode(200)
{}

Response::Response(int status, const std::string& message)
: _statusCode(status), _statusMessage(message)
{}

Response::~Response()
{}

void Response::setStatusCode(int code)
{
	_statusCode = code;
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

void Response::setBody(const std::string &body)
{
	_body = body;
}

std::string Response::buildResponse() const
{
    std::stringstream ss;
    ss << _statusCode;
    std::string statusMessage = getStatusMessage();
    std::string response = "HTTP/1.1 " + ss.str() + " " + statusMessage + "\r\n";

    // Add all headers
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
	std::stringstream contentLength;
    contentLength << _body.length();
    response += "Content-Length: " + contentLength.str() + "\r\n\r\n";
    response += _body; // Don't forget to add the body itself

    return response;
}

// std::string Response::buildResponse() const
// {
// 	std::stringstream ss;
// 	ss << _statusCode;
// 	std::string response = "HTTP/1.1 " + ss.str() + " OK\r\n";
	
// 	std::stringstream contentLength;
// 	contentLength << _body.length();
// 	response += "Content-Length: " + contentLength.str() + "\r\n\r\n";

//     return response;
// }

std::string Response::getStatusMessage() const {
    switch (_statusCode) {
        case 200: return "OK";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        // Add other cases as needed.
        default: return "Unknown";
    }
}
