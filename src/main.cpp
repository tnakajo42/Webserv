/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:12:06 by tnakajo           #+#    #+#             */
/*   Updated: 2025/03/24 13:06:30 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <stdexcept>
#include <iostream>

bool	withoutConfig()
{
	try
	{
		//For Log
		Logger::init("server.log");
		//Logger::log("Server started with default Config file");
		
		//Start Server with default config
		const	std::string	defaultFile = "default.config";
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

bool	startConfig(char *configFileName)
{
	try
	{
		std::string	fileName = configFileName;

		//For Log
		Logger::init("server.log");
		//Logger::logStr("Server started with Config file: ", fileName);

		//Start Server with given config
		Server	server(fileName);
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
