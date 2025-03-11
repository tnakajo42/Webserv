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

class RequestHandler
{
	public:
		// static void handle(int client_socket);
		static void	handle_get(std::string& path, int client_socket, ConfigParser& config); // add
		static void	handle_post(const std::string& path, int client_socket, const std::string& initial_request, size_t initial_received, ConfigParser& config);
		static void	handle_delete(const std::string& path, int client_socket, ConfigParser& config); // add
	private:
		// Helper methods for CGI
		static bool isCGIRequest(const std::string& path);
		static void handleCGIRequest(const std::string& path, int client_socket, 
			const std::string& initial_request, size_t initial_received,
			ConfigParser& config, const std::string& method);
		static void parseRequest(Request& request, const std::string& initial_data, 
			size_t initial_received, int client_socket);
		static std::string formatResponse(const Response& response);
		static std::string getContentType(const std::string& filepath);
};
