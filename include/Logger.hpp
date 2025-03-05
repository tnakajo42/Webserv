/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:50 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/04 20:50:28 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include "Server.hpp"

class Logger
{
	private:
		std::ofstream	_logFile;
		
	public:
		Logger();
		Logger(const std::string &filename);
		~Logger();

		void log(const std::string &message);
};

#endif
