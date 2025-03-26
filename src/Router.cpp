/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnakajo <nj.takayuki@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:39 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/22 16:19:58 by tnakajo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

Router::Router()
{}

Router::~Router()
{}

void		Router::addRoute(const std::string &path, const std::string &handler)
{
	_routes[path] = handler;
}

bool		Router::fileExists(const std::string& path)
{
	struct stat	buffer;//what is struct stat???
	return (stat(path.c_str(), &buffer) == 0);
}

std::string Router::readFile(const std::string& path)
{
	// std::stringstream file(path.c_str());
	std::ifstream file(path.c_str());
	if (!file)
		return "";

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

Response	Router::routeRequest(const Request& request, ConfigParser& config)
{
	std::string filePath = (request.getPath() == "/" ? config.getIndex() : config.getRoot() + request.getPath());
	std::map<std::string, std::string> redirectMap;
	redirectMap["www/home"] = "/";
	redirectMap["www/about"] = "/about.html";
	redirectMap["www/about-us"] = "/about.html";

	if (redirectMap.find(filePath) != redirectMap.end())
	{
		std::string newLocation = redirectMap[filePath];
		std::cout << "Redirecting " << filePath << " to " << newLocation << std::endl;
		
		Response response(302, "Found", config);
		response.setHeader("Location", newLocation);
		response.setBody("<html><head><meta http-equiv='refresh' content='4.2;url=" + newLocation + "'></head><body><h1>You will see the magic in 4.2 seconds...</h1></body></html>");
		return response;
	}
	struct stat pathStat;
	if (stat(filePath.c_str(), &pathStat) == 0)
	{
		if (S_ISDIR(pathStat.st_mode)) // If it's a directory
		{
			std::string directoryHtml = generateDirectoryListing(filePath);
			Response response(200, "OK", config);
			response.setBody(directoryHtml);
			return response;
		}
		else // It's a file, serve it
		{
			std::string content = readFile(filePath);
			Response response(200, "OK", config);
			response.setBody(content);
			return response;
		}
	}
	Response response(404, "Not Found", config);
	response.setBody(request.getBody());
	return response;
}

std::string Router::generateDirectoryListing(const std::string& dirPath) {
	DIR* dir = opendir(dirPath.c_str());
	if (!dir)
		return "<html><body><h1>403 Forbidden</h1></body></html>";

	std::ostringstream html;
	html << "<html><head><title>Index of " << dirPath << "</title></head><body>";
	html << "<h1>Index of " << dirPath << "</h1><ul>";

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue; // Skip current and parent directory
		std::string str = dirPath;
		str.erase(0, 4); 
		html << "<li><a href=\"" << "./" + str + "/" << name << "\">" << name << "</a></li>";
	}
	html << "</ul></body></html>";
	closedir(dir);
	return html.str();
}
