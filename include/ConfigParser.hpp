/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:10:54 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/12 18:49:03 by cadenegr         ###   ########.fr       */
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
#include <stack>
#include <dirent.h>
#include <unistd.h>

class ConfigParser
{
	private:
		std::string							_fileName;
		std::map<std::string, std::string>	_settings;
		int									_port;
		std::stack<int>						_ports;
		int									_typePort;
		struct in_addr						_ipAddr;
		int									_maxBodySize;
		std::stack<std::string>				_methods;
	
	public:
		ConfigParser(const std::string& configPath);
		~ConfigParser();

		bool			checkKeys();
		bool			checkValues();
		bool			morePorts(std::string& ports);
		bool			parsePort();
		int				getPort();
		std::stack<int>	getPorts();
		int				getType();
		bool			parseHost();
		struct in_addr	getHost() const;
		bool			parseRootIndexErrorCGIUpload();
		std::string		getRoot() const;
		std::string		getIndex() const;
		std::string		getErrorPage() const;
		std::string		getCgiPath() const;
		std::string		getUploadDir() const;
		bool			parseClientMaxBodySize();
		int				getMaxBodySize() const;
		bool			parseMethods();
};

// #include <map>
// #include <iostream>
// #include <string>
// #include <fstream>
// #include <sstream>
// #include <stdexcept>
// #include <sstream>
// #include <vector>
// #include <arpa/inet.h>  // inet_pton
// #include <cstring>      // strerror
// #include <cerrno>
// #include <cstdlib>

// class ConfigParser
// {
// 	private:
// 		std::map<std::string, std::string> _settings;
	
// 	public:
// 		ConfigParser(const std::string& configPath);
// 		~ConfigParser();

// 		int				getPort() const;
// 		struct in_addr	getHost() const;
// 		std::string		getRoot() const;
// 		std::string		getIndex() const;
// 		std::string		getErrorPage() const;
// 		int				getbodySize() const;
// 		std::string		getCgiPath() const;
// 		std::string		getUploadDir() const;
// 		bool			configChecker();
// };