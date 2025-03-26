/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:57 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/19 16:21:14 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

std::ofstream Logger::_logFile;

void Logger::init(const std::string &filename)
{
	_logFile.open(filename.c_str(), std::ios::app);
	if (!_logFile.is_open())
	{
		std::cerr << "Failed to open log file: " << filename << std::endl;
	}
}

void Logger::log(const std::string &message)
{
	if (_logFile.is_open())
	{
		std::time_t now = std::time(NULL);
		char timeStr[20];
		std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

		_logFile << "[" << timeStr << "] " << message << std::endl;
	}
	else
	{
		std::cerr << "Logger error: log file is not open!" << std::endl;
	}
}

void Logger::logStr(const std::string& message, std::string& variable)
{
	if (_logFile.is_open())
	{
		std::time_t now = std::time(NULL);
		char timeStr[20];
		std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

		_logFile << "[" << timeStr << "] " << message << variable << std::endl;
	}
	else
	{
		std::cerr << "Logger error: log file is not open!" << std::endl;
	}
}

void Logger::logInt(const std::string &message, int variable)
{
	if (_logFile.is_open())
	{
		std::time_t now = std::time(NULL);
		char timeStr[20];
		std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

		_logFile << "[" << timeStr << "] " << message << variable << std::endl;
	}
	else
	{
		std::cerr << "Logger error: log file is not open!" << std::endl;
	}
}

void Logger::close()
{
	if (_logFile.is_open())
		_logFile.close();
}

