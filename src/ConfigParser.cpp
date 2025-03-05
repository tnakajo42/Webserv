/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:19:30 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 12:00:31 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigParser.hpp"

#include <fstream>
#include <sstream>

int stringToInt(const std::string& str) {
    std::stringstream ss(str);
    int result;
    ss >> result;
    return result;
}


ConfigParser::ConfigParser(const std::string& configPath)
{
	std::ifstream file(configPath.c_str());
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string key, value;
		if (std::getline(iss, key, '=') && std::getline(iss, value))
		_settings[key] = value;
	}
}
ConfigParser::~ConfigParser()
{}

int ConfigParser::getPort() const
{
	return stringToInt(_settings.at("port"));
}

std::string ConfigParser::getHost() const
{
	return _settings.at(_settings.at("host"));
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
	return stringToInt("client_max_body_size");
}

std::string ConfigParser::getCgiPath() const
{
	return _settings.at("cgi_python");
}

std::string ConfigParser::getUploadDir() const
{
	return _settings.at("upload_dir");
}


// ConfigParser::ConfigParser(std::string& filename)	: _filename(filename)
// {}

// void		ConfigParser::parse()
// {
// 	std::ifstream	file(_filename.c_str());//fstream
// 	if (!file.is_open())
// 		throw std::runtime_error("Error: .config file did not open.");
// 	std::string		line;
// 	while (std::getline(file, line))
// 	{
// 		// std::cout << "------------->" << line << std::endl;
// 		std::istringstream		ss(line);//stream
// 		std::string				key, value;
// 		if (!std::getline(ss, key, '=') || !std::getline(ss, value))
// 			throw std::runtime_error("Error: .config file is not 'key=vale' format.");
// 		_globalSettings[key] = value;
// 	}
// 	file.close();
// }

// void		ConfigParser::displayConfig()
// {
// 	//interesting but map stors it elements in accending order, so the map[key]=value, the key smallest will be the first element like 'a' or '0'
// 	for (std::map<std::string, std::string>::iterator it = _globalSettings.begin(); it != _globalSettings.end(); ++it)
// 		std::cout << it->first << " = " << it->second << std::endl;
// 	std::cout << std::endl;
// }

// std::map<std::string, std::string>&	ConfigParser::getGlobalSettings()
// {
// 	return _globalSettings;
// }