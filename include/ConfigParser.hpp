/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:10:54 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 12:00:44 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>

class ConfigParser
{
	private:
		std::map<std::string, std::string> _settings;
	
	public:
		ConfigParser(const std::string& configPath);
		~ConfigParser();
		int getPort() const;
		std::string getHost() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::string getErrorPage() const;
		int getbodySize() const;
		std::string getCgiPath() const;
		std::string getUploadDir() const;
};