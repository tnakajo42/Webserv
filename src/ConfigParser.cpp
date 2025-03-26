/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:19:30 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/24 16:30:30 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

int stringToIntConfigParser(const std::string& str)
{
	std::stringstream ss(str);
	int result;
	if (ss >> result)
	return result;
	return -1;
}

ConfigParser::ConfigParser()
{}

ConfigParser::ConfigParser(const std::string& configPath)
{
	if (configPath.find(".config\0") == std::string::npos)
		throw std::runtime_error("Error: File is not .config format.");
	std::ifstream file(configPath.c_str());
	if (!file)
		throw std::runtime_error("Error: Config File not found.");
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
			throw std::runtime_error("Error: A Config File Value is missing.");
	}
	_get = false;
	_post = false;
	_delete = false;
	if (!checkKeys() ||	!checkValues())
		throw std::runtime_error("Error: Config File incorrect.");
}

ConfigParser::~ConfigParser()
{}

bool	ConfigParser::checkKeys()
{
	std::string	levels[] = 
	{
		"port", "host", "root", "index", "error_page_404", "client_max_body_size",
			"cgi_python", "upload_dir", "allowed_methods"
	};
	
	int	index = -1;;
	for (std::map<std::string, std::string>::iterator it = _settings.begin();
		it != _settings.end(); ++it)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (it->first == levels[i])
			{
				index = i;
				break;
			}
		}
		if (index != -1 && it->first == levels[index])
			continue;
		else
		{
			if (!it->first.empty())
				std::cerr << "Unkown Key: " << it->first << std::endl;
			else
			std::cerr << "Unkown Key." << std::endl;
			return false;
		}
	}
	if (_settings.size() != 9)
	{
		std::cerr << "Config variables incomplete." << std::endl;
		return false;
	}
	return true;
	
}

bool	ConfigParser::checkValues()
{
	if (!parsePort())
		return	false;
	if (!parseHost())
		return	false;
	if (!parseRootIndexErrorCGIUpload())
		return	false;
	if (!parseClientMaxBodySize())
		return	false;
	if (!parseMethods())
		return false;
	return true;
}

int		ConfigParser::getType()
{
	return _typePort;
}

bool	ConfigParser::morePorts(std::string& ports)
{
	std::istringstream	iss(ports);
	std::string			portStr;
	int					portInt;

	while (std::getline(iss, portStr, ',') || std::getline(iss, portStr))
	{
		for (int i = 0; portStr[i]; ++i)
		{
			if (!isdigit(portStr[i]))
			{
				std::cerr << "In " << portStr << " this is invalid: " << portStr[i] << std::endl;
				return false;
			}
		}
		portInt = stringToIntConfigParser(portStr);
			
		if (portInt <= 0 || portInt > 65535)
		{
			std::cerr << "Port is not within range 0 to 65535: " << portInt << std::endl;
			return false;
		}
		_ports.push(portInt);
	}
	return true;
}

bool		ConfigParser::parsePort()
{
	std::string	portStr = _settings.at("port");

	if (portStr.find(','))
	{
		if (morePorts(portStr))
			_typePort = 2;
		else
			return false;
	}
	else
	{
		for (int i = 0; portStr[i]; ++i)
		{
			if (!isdigit(portStr[i]))
			{
				std::cerr << "In " << portStr << " this is invalid: " << portStr[i] << std::endl;
				return false;
			}
		}
	
		int	portInt = stringToIntConfigParser(_settings.at("port"));
		
		if (portInt <= 0 || portInt > 65535)
		{
			std::cerr << "Port is not within range 0 to 65535 ->" << portInt << std::endl;
			return false;
		}
		_port = portInt;
		_ports.push(portInt);
		_typePort = 1;
	}
	return true;
}


int ConfigParser::getPort()
{
	if (_typePort == 1)
		return _port;
	if (_typePort == 2)
	{
		if (!_ports.empty())
		{
			int	popInt = _ports.top();
			_ports.pop();
			return popInt;
		}
		return -1;
	}
	return -1;
}

std::stack<int> ConfigParser::getPorts()
{
	return _ports;
}

bool			ConfigParser::parseHost()
{
	std::string		hostStr = _settings.at("host");
	struct in_addr	ipAddr;
	
	if (inet_pton(AF_INET, hostStr.c_str(), &ipAddr) == 1)
	{
		_ipAddr = ipAddr;
		return true;  // Valid IPv4
	}
	else if (inet_pton(AF_INET6, hostStr.c_str(), &ipAddr) == 1)
	{
		_ipAddr = ipAddr;
		return true;  // Valid IPv6
	}
	else
	{
		std::cerr << "Invalid host IP: " << hostStr << std::endl;
		return false; // Invalid IP
	}
}

struct in_addr ConfigParser::getHost() const
{
	return	_ipAddr;
}

bool directoryExists(const std::string& path)
{
	DIR* dir = opendir(path.c_str());
	if (dir)
	{
		closedir(dir);
		return true; // Directory exists
	}
	std::cerr << " Wrong dir path: " << path << std::endl;
	return false;
}

bool fileExists(const std::string& path)
{
	if (access(path.c_str(), F_OK) == 0)
		return true;
	else
	{
		std::cerr << " Wrong path: " << path << std::endl;
		return false;
	}
}

bool		ConfigParser::parseRootIndexErrorCGIUpload()
{
	std::string		rootPath	= _settings.at("root");
	std::string		indexPath	= rootPath + "/" + _settings.at("index");
	std::string		errorPath	= rootPath + "/" + _settings.at("error_page_404");
	std::string		cgiPath		= _settings.at("cgi_python");
	std::string		uploadPath	= _settings.at("upload_dir");

	if (!directoryExists(rootPath))
		return false;
	if (!fileExists(indexPath))
		return false;
	if (!fileExists(errorPath))
		return false;
	if (!directoryExists(cgiPath))
		return false;
	if (!directoryExists(uploadPath))
		return false;
	return true;	
}

std::string ConfigParser::getRoot() const
{
	std::string		rootStr = _settings.at("root");
	return			rootStr;
}

std::string ConfigParser::getIndex() const
{
	std::string		rootPath	= _settings.at("root");
	std::string		indexPath	= rootPath + "/" + _settings.at("index");
	return			indexPath;
}

std::string ConfigParser::getErrorPage() const
{
	std::string		rootPath	= _settings.at("root");
	std::string		errorPath	= rootPath + "/" + _settings.at("error_page_404");
	return			errorPath;
}

bool	ConfigParser::parseClientMaxBodySize()
{
	std::string client_max_body_sizeStr = _settings.at("client_max_body_size");
	for (int i = 0; client_max_body_sizeStr[i]; ++i)
	{
		if (!isdigit(client_max_body_sizeStr[i]))
		{
			std::cerr << "Not a postive Int: " << client_max_body_sizeStr << std::endl;
			return false;
		}
	}
	int	client_max_body_sizeInt = stringToIntConfigParser(_settings.at("client_max_body_size"));
	_maxBodySize = client_max_body_sizeInt;
	if (client_max_body_sizeInt < 1 || client_max_body_sizeInt > 2147483647)
	{
		std::cerr << "Not postive within and INT's margin: " << client_max_body_sizeInt << std::endl;
		return false;
	}
	return true;
}

int ConfigParser::getMaxBodySize() const
{
	return _maxBodySize;
}

std::string ConfigParser::getCgiPath() const
{
	std::string		rootPath	= _settings.at("root");
	return rootPath + _settings.at("cgi_python");
}

std::string ConfigParser::getUploadDir() const
{
	// std::string		rootPath	= _settings.at("root");
	return _settings.at("upload_dir");
}

bool	ConfigParser::parseMethods()
{
	std::istringstream	iss(_settings.at("allowed_methods"));
	std::string			methodsStr;
	int					get = 0;
	int					post = 0;
	int					del = 0;

	while (std::getline(iss, methodsStr, ','))
	{
		if (methodsStr == "GET")
		{
			get++;
			_get = true;
			_methods.push(methodsStr);
		}
		else if (methodsStr == "POST")
		{
			post++;
			_post = true;
			_methods.push(methodsStr);
		}
		else if (methodsStr == "DELETE")
		{
			del++;
			_delete = true;
			_methods.push(methodsStr);
		}
		else
		{
			std::cerr << "Incorrect methods: " << methodsStr << std::endl;
			return false;
		}
	}
	// if (get != 1 || post != 1 || del != 1)
	// {
	// 	std::cerr << "Methods do not include GET,POST and DELETE." << std::endl;		
	// 	return false;
	// }
	return true;
}


bool	ConfigParser::getMethod(const std::string& type)
{
	if (type == "GET")
		return _get;
	if (type == "POST")
		return _post;
	if (type == "DELETE")
		return _delete;
	return false;
}
