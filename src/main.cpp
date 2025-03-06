/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:12:06 by tnakajo           #+#    #+#             */
/*   Updated: 2025/03/06 17:23:43 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/ConfigParser.hpp"
#include "../include/LocationConfig.hpp"
#include "../include/Logger.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdlib.h>

bool	withoutConfig()
{
	try
	{
		Logger::init("server.log");
		Logger::log("Server started with default Config file"); // Write a log message
		const	std::string	defaultFile = "default.config";
		// std::cout << "Not running Server\n";
		Server	server(defaultFile);
		// std::cout << "Server\n";
		server.start();
		
	}
	catch(const std::exception& e)
	{
		std::cerr << "Without! " <<e.what() << std::endl;
		return false;
	}
	return true;
}

bool	startConfig(char *configFileName)
{
	try
	{
		Logger::init("server.log");
		Logger::log("Server started with provided Config file."); // Write a log message
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
	Logger::close();
	return 0;
}
