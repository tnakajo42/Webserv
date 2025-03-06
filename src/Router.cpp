/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:39 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 16:52:11 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Router.hpp"

Router::Router()
{}

Router::~Router()
{}

void		Router::addRoute(const std::string &path, const std::string &handler)
{
	_routes[path] = handler;
}

//to test or???
Response	Router::handleRequest(const Request& req)
{
	Response res;
	if (req.getMethod() == "GET")
	{
		res.setStatusCode(200);
		res.setBody("Hello, World!");
	}
	else
	{
		res.setStatusCode(405);
		res.setBody("Method Not Allowed");
	}
	return res;
}

bool		Router::fileExists(const std::string& path)
{
	struct stat	buffer;//what is struct stat???
	return (stat(path.c_str(), &buffer) == 0);
}

std::string Router::readFile(const std::string& path)
{
	std::stringstream file(path.c_str());
	if (!file)
		return "";

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

Response	Router::routeRequest(const Request& request)
{
	std::string filePath = "www" + (request.getPath() == "/" ? "/index.html" : request.getPath());
	std::cout << "Checking if file exists: " << filePath << std::endl;

	if (fileExists(filePath))
	{
		Response response(200, "OK");
		response.setBody(readFile(filePath));
		return response;
	}

	// File not found -> return 404 response
	Response response(404, "Not Found");
	response.setBody("<h1>404 Not Found</h1>");
	return response;
}
