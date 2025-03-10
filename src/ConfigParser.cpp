/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:19:30 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 19:46:42 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigParser.hpp"

int stringToIntConfigParser(const std::string& str)
{
	std::stringstream ss(str);
	int result;
	if (ss >> result)
		return result;
	return -1;
}

ConfigParser::ConfigParser(const std::string& configPath)
{
	std::ifstream file(configPath.c_str());
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '='))
			_settings[key];
		if (std::getline(iss, value) && !value.empty())
		_settings[key] = value;
		else
			_settings[key] = "empty";
	}
}

ConfigParser::~ConfigParser()
{}

int ConfigParser::getPort() const
{
	try
	{
		std::string portStr = _settings.at("port");
		if (_settings.at("port") == "empty")
			return -1;
		for (int i = 0; portStr[i]; ++i)
		{
			if (!isdigit(portStr[i]))
				return -1;
		}
	
		int	portInt = stringToIntConfigParser(_settings.at("port"));
		
		if (portInt <= 0 || portInt > 65535)
			return -1;
		return portInt;
	}
	catch (const std::out_of_range& ex)
	{
		return -1;
	}
}

bool validateAndConvertHost(const std::string &host, struct in_addr &addr)
{
	if (inet_pton(AF_INET, host.c_str(), &addr) == 1)
	{
		return true;  // Valid IPv4
	}
	else if (inet_pton(AF_INET6, host.c_str(), &addr) == 1)
	{
		std::cout << "Error here!!!!!!!!!!!!!!!!!!!!\n";
		return true;  // Valid IPv6
	}
	else
	{
		std::cerr << "Invalid host IP: " << host << " Error: " << strerror(errno) << std::endl;
		return false; // Invalid IP
	}
}

struct in_addr ConfigParser::getHost() const
{
	try
	{
		struct in_addr ipAddr;
		
		if (!validateAndConvertHost(_settings.at("host"), ipAddr))
		{
			std::cerr << "Invalid IP in config. Exiting...\n";
			exit(EXIT_FAILURE);
		}
		return ipAddr;
	}
	catch(const std::exception& e)
	{
		std::cerr << "No host key in config file. Exiting...\n";
		exit(EXIT_FAILURE);
	}
	
}

std::string ConfigParser::getRoot() const
{
	return _settings.at("root");
}

std::string ConfigParser::getIndex() const
{
	return _settings.at("index");
}

std::string ConfigParser::getErrorPage() const
{
	return _settings.at("error_page_404");
}

int ConfigParser::getbodySize() const
{
	return stringToIntConfigParser("client_max_body_size");
}

std::string ConfigParser::getCgiPath() const
{
	return _settings.at("cgi_python");
}

std::string ConfigParser::getUploadDir() const
{
	return _settings.at("upload_dir");
}