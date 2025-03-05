/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:24 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/04 21:00:41 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <string>
#include <map>
#include "Response.hpp"
#include "Request.hpp"
#include "CGIHandler.hpp"
#include <sstream>
#include <sys/stat.h>


class Router
{
	private:
		std::map<std::string, std::string>	_routes; // Maps URL paths to files or handlers
		bool fileExists(const std::string& path);
		std::string readFile(const std::string& path);

	public:
		Router();
		~Router();
		void addRoute(const std::string &path, const std::string &handler);
		Response routeRequest(const Request &request);
		Response handleRequest(const Request& req);
};

#endif
