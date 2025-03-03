/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:12:06 by tnakajo           #+#    #+#             */
/*   Updated: 2025/03/03 15:40:27 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/ConfigParser.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdlib.h>


int	main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			std::string	filename = av[1];
			ConfigParser	configFile(filename);
			configFile.parse();
			configFile.displayConfig();
			std::map<std::string, std::string>	configResults;
			configResults = configFile.getGlobalSettings();
			std::istringstream		ss(configResults["port"]);
			int port;
			ss >> port;
			Server	server(port);
			server.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	else
	{
		Server	server(8082);//configfile 1st line
		server.run();
	}
	return 0;
}
