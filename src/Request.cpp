/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:17 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/25 14:56:06 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int stringToIntRequest(const std::string& str)
{
	std::stringstream ss(str);
	int result;
	ss >> result;
	return result;
}

Request::Request()
{}

Request::Request(ConfigParser& config)
:	_config(config) // , _contentLength(0)
{}

Request::Request(const Request& other)
:	_method(other._method), _path(other._path), _httpVersion(other._httpVersion), _headers(other._headers),
_body(other._body), _contentLength(other._contentLength), _queryString(other._queryString), _config(other._config)
{}

Request& Request::operator=(const Request& other)
{
	{
		if (this != &other)
		{
			_method = other._method;
			_path = other._path;
			_httpVersion = other._httpVersion;
			_headers = other._headers;
			_body = other._body;
			_contentLength = other._contentLength;
			_queryString = other._queryString;
			_config = other._config;
		}
		return *this;
	}
}

Request::~Request()
{}

void	Request::readBody()
{
	std::string filepath;
	if (_path == "/uploads")
	{
		_path = _config.getUploadDir();
		filepath = _path;
	}
	else
	{
		std::string filepath = (_path == "/" ? _config.getIndex() : _config.getRoot() + _path);
	}
	std::ifstream file(filepath.c_str(), std::ios::binary);//what does std::ios::binary do?
	// 	-> The std::ios::binary flag opens the file in binary mode. 
	//	Normally, files are opened in text mode,
	if (!file)
	{
		std::string filepath = _config.getErrorPage();
		std::ifstream file(filepath.c_str(), std::ios::binary);
		std::ostringstream content;
		content << file.rdbuf();//what rdbuf() do?
		// file.rdbuf() gets the stream buffer of file and directly loads the entire file into content (a string stream).
		_body = content.str();
	}
	else
	{
		std::ostringstream content;
		content << file.rdbuf();//what rdbuf() do?
		// file.rdbuf() gets the stream buffer of file and directly loads the entire file into content (a string stream).
		_body = content.str();
	}	
}

bool	Request::parseRequest(const std::string &rawRequest)
{
	std::istringstream requestStream(rawRequest);
	std::string line;

	// Parse request line (e.g., "GET /index.html HTTP/1.1")
	if (!std::getline(requestStream, line) || line.empty())
		return false;
	std::istringstream lineStream(line);
	lineStream >> _method >> _path >> _httpVersion;

	if (!_config.getMethod(_method))
	{
		_method = "NOT_valid";
	}
	// Split path and query string
	size_t questionMarkPos = _path.find('?');
	if (questionMarkPos != std::string::npos)
	{
		_queryString = _path.substr(questionMarkPos + 1);
		_path = _path.substr(0, questionMarkPos);
	}
	else
		_queryString = "";

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
		int contentLength = stringToIntRequest(_headers["Content-Length"]);
		_body.resize(contentLength);
		_contentLength = contentLength;
		readBody();
	}
	else
		readBody();
	return true;
}

std::string Request::getQueryString() const {
    return _queryString;
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

ConfigParser	Request::getConfig()
{
	return _config;
}