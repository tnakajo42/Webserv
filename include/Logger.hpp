/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:50 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 18:45:05 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>

class Logger
{
	private:
		static std::ofstream _logFile;

	public:
		static void init(const std::string &filename);
		static void log(const std::string &message);
		static void close();
};

#endif

