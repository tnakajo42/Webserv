/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:19:30 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/03 14:17:27 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConfigParser.hpp"

ConfigParser::ConfigParser(std::string& filename)	: _filename(filename)
{}

void		ConfigParser::parse()
{
	std::ifstream	file(_filename.c_str());//fstream
	if (!file.is_open())
	throw "Error: .config file did not open.";;
	std::string		line;
	while (std::getline(file, line))
	{
		// std::cout << "------------->" << line << std::endl;
		std::istringstream		ss(line);//stream
		std::string				key, value;
		if (!std::getline(ss, key, '=') || !std::getline(ss, value))
			throw "Error: .config file is not 'key=vale' format.";
		_globalSettings[key] = value;
	}
	file.close();
}

void		ConfigParser::displayConfig()
{
	//interesting but map stors it elements in accending order, so the map[key]=value, the key smallest will be the first element like 'a' or '0'
	for (std::map<std::string, std::string>::iterator it = _globalSettings.begin(); it != _globalSettings.end(); ++it)
		std::cout << it->first << " = " << it->second << std::endl;
	std::cout << std::endl;
}

std::map<std::string, std::string>&	ConfigParser::getGlobalSettings()
{
	return _globalSettings;
}