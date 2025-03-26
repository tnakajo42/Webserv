#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>  // Add this for mkdir()
#include <sys/types.h>
#include "ConfigParser.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Router.hpp"
#include "Client.hpp"

class RequestHandler
{
	public:
		static bool			handle_post(const Request& req, const Client& cl);
		static bool			handle_delete(Request& req, Response& res, Client& client);
		static bool			handleCGIRequest(const Client& client, const Request& req);
		static bool			isCGIRequest(const std::string& path);

	private:
		// Helper methods for CGI
		static void			parseRequest(Request& request, Client& client);
		static std::string	getContentType(const std::string& filepath);
};
