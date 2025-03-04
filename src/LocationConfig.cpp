/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:25:12 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/03 19:16:56 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/LocationConfig.hpp"

LocationConfig::LocationConfig()
:	_port(8080), _host("127.0.0.1"), _root("/var/www/html"), _index("index.html"), _errorPage("/errors/404.html"), _maxClientBodySize(1000000),
_cgiPath("/usr/bin/python3") , _uploadStore("/var/www/uploads"), _allowedMetods()
{
	_allowedMetods.push_back("GET");
	_allowedMetods.push_back("POST");
	_allowedMetods.push_back("DELETE");
}

LocationConfig::LocationConfig(std::map<std::string, std::string>& globalSettings)
{
    std::istringstream changeInt(globalSettings["port"]);
	changeInt >> _port;
    _host = globalSettings["host"];
    _root = globalSettings["root"];
    _index = globalSettings["index"];
    _errorPage = globalSettings["error_page_404"];
	std::istringstream changeInt1(globalSettings["client_max_body"]);
    changeInt1 >> _maxClientBodySize;
    _cgiPath = globalSettings["cgi_python"];
    _uploadStore = globalSettings["upload_dir"];
	_allowedMetods.clear();
	std::istringstream	ss(globalSettings["allowed_methods"]);
	std::string	method;
	while (std::getline(ss, method, ','))
		_allowedMetods.push_back(method);
	if (std::getline(ss, method))
		_allowedMetods.push_back(method);
}

LocationConfig::~LocationConfig()
{}