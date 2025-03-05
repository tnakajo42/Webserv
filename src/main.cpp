/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:12:06 by tnakajo           #+#    #+#             */
/*   Updated: 2025/03/05 12:09:27 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/ConfigParser.hpp"
#include "../include/LocationConfig.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdlib.h>

bool	withoutConfig()
{
	try
	{
		const	std::string	defaultFile = "default.config";
		std::cerr << "Not running Server\n";
		Server	server(defaultFile);
		server.start();
		
	}
	catch(const std::exception& e)
	{
		std::cerr << "Without! " <<e.what() << std::endl;
		return false;
	}
	return true;
}

// std::cout << "check" << "\n";
bool	startConfig(char *configFileName)
{
	try
	{
		const	std::string	configFile = configFileName;
		Server	server(configFile);
		server.start();
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}


int	main(int ac, char **av)
{
	if (ac > 2)
		return 1;
	if (ac == 2)
	{
		if (!startConfig(av[1]))
			return 1;
	}
	else
	{
		if (!withoutConfig())
			return 1;
	}
	return 0;
}


// int	main(int ac, char **av)
// {
// 	if (ac == 2)
// 	{
// 		try
// 		{
// 			std::string	filename = av[1];
// 			ConfigParser	configFile(filename);
// 			configFile.parse();
// 			configFile.displayConfig();
// 			std::map<std::string, std::string>	configResults;
// 			configResults = configFile.getGlobalSettings();
// 			LocationConfig	local(configResults);
// 			// std::istringstream		ss(configResults["port"]);
// 			// int port;
// 			// ss >> port;
// 			// std::istringstream		ss1(configResults["client_max_body_size"]);
// 			// int	maxSize;
// 			// ss1 >> maxSize;
			
// 			Server	server(local._port);
// 			server.run();
// 		}
// 		catch(const std::exception& e)
// 		{
// 			std::cerr << e.what() << std::endl;
// 			return 1;
// 		}
// 	}
// 	else
// 	{
// 		LocationConfig	local;
// 		Server	server(local._port);//configfile 1st line
// 		server.run();
// 	}
// 	return 0;
// }
