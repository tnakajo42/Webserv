/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:57 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/04 20:51:55 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Logger.hpp"
#include <iostream>
#include <ctime>

Logger::Logger()
{}

Logger::Logger(const std::string &filename)
{
	_logFile.open(filename.c_str(), std::ios::app);
}

Logger::~Logger()
{
	if (_logFile.is_open())
		_logFile.close();
}

void Logger::log(const std::string &message)
{
	if (_logFile.is_open())
	{
		std::time_t now = std::time(NULL);  // Replace nullptr with NULL
		_logFile << "[" << std::ctime(&now) << "] " << message << std::endl;
	}
}
