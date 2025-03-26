#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "Client.hpp"
#include "Router.hpp"
#include "Logger.hpp"
#include "ConfigParser.hpp"
#include "RequestHandler.hpp"
#include "CGIHandler.hpp"
#include "Logger.hpp"
#include <arpa/inet.h>
#include <iomanip>

#define MAX_EVENTS 1000

class Server
{
	private:
		int						_serverFd;
		int						_epollFd;
		struct sockaddr_in		_address;
		std::map<int, Client*>	_clients; // Store clients by file descriptor
		Router					_router;
		// Logger					_logger;
		socklen_t				_addrlen;
		ConfigParser			_config;
		std::vector<int>		_sockets;

		void					setupServer();
		void					createEpoll();
		void					handleNewConnection();
		void					handleClientRequest(int clientFd);
		void					sendClientResponse(int clientFd);

	public:
		Server(const std::string& configPath);
		~Server();

		void					start();
		void					handleEvents();
		void					acceptClient(int serverFd);
		void					removeClient(int fd);
};

#endif

