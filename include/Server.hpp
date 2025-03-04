#pragma once

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "RequestHandler.hpp"
#include "LocationConfig.hpp"

class Server	:	public LocationConfig
{
	private:
		int 				server_socket;
		struct sockaddr_in	server_addr;

	public:
		Server(int port);
		~Server();
		void run();
};
