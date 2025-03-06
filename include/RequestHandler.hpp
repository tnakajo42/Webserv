#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>  // Add this for mkdir()
#include "ConfigParser.hpp"

class RequestHandler
{
	public:
		// static void handle(int client_socket);
		static void	handle_get(std::string& path, int client_socket, ConfigParser& config); // add
		static void	handle_post(const std::string& path, int client_socket, const std::string& initial_request, size_t initial_received, ConfigParser& config);
		static void	handle_delete(const std::string& path, int client_socket, ConfigParser& config); // add
};

