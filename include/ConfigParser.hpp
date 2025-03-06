/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:10:54 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 19:18:51 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <arpa/inet.h>  // inet_pton
#include <cstring>      // strerror
#include <cerrno>
#include <cstdlib>

class ConfigParser
{
	private:
		std::map<std::string, std::string> _settings;
	
	public:
		ConfigParser(const std::string& configPath);
		~ConfigParser();
		int getPort() const;
		struct in_addr getHost() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::string getErrorPage() const;
		int getbodySize() const;
		std::string getCgiPath() const;
		std::string getUploadDir() const;
};