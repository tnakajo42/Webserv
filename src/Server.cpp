
/* ************************************************************************** */
/*                                                                            */
/*          :::      ::::::::        w             www          bbbbbbb       */
/*        :+:      :+:    :+:       w    www     ww            b      bb      */
/*      +:+ +:+         +:+        w   ww  w   ww   eeeeeee   b      bb       */
/*    +#+  +:+       +#+          w  ww   w  ww    e    e    bbbbbbbb         */
/*  +#+#+#+#+#+   +#+            w ww    w ww     eeeee     b       bb        */
/*       #+#    #+#             www     www      e         b       bb         */
/*      ###   ########.de      www     www      eeeeeee    bbbbbbbbb          */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


//Server takes config file name and it initialize the Constructor for ConfigParser
Server::Server(const std::string& configPath)
: _config(configPath)
{
	setupServer();
	createEpoll();
}

// Create epoll instance
void	Server::createEpoll()
{
	_epollFd = epoll_create(1024);
	if (_epollFd == -1)
	{
		//Logger::log("Failed to create epoll instance.");
		throw std::runtime_error("Error: epoll_create failed.");
	}
	// Register all sockets with epoll
	for (std::vector<int>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
	{
		int sockfd = *it;
		struct epoll_event event;
		event.events = EPOLLIN;
		event.data.fd = sockfd;
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, sockfd, &event) == -1)
		{
			//Logger::log("epoll_ctl: EPOLL_CTL_ADD failed in createEpoll.");
			std::cerr << "epoll_ctl: EPOLL_CTL_ADD failed in createEpoll." << std::endl;
			close(sockfd);
		}
	}

	std::cout << "Server setup complete." << std::endl;
}

//Setup socket, bind and listen: 
void	Server::setupServer()
{
	std::stack<int>	ports = _config.getPorts();
	while (!ports.empty())
	{
		int	port = ports.top();
		ports.pop();
		
		_serverFd = socket(AF_INET, SOCK_STREAM, 0);
		if (_serverFd < 0)
		{
			//Logger::log("Failed: SOCKET!");
			std::cerr << "Error: Socket FT." <<std::endl;
			continue;
		}
		_address.sin_family = AF_INET;
		_address.sin_port = htons(port);
		_address.sin_addr = _config.getHost();
		_addrlen = sizeof(_address);
		
		if (bind(_serverFd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		{
			//Logger::log("Failed: BIND!");
			throw std::runtime_error("Error: Bind FT.");
		}
		if (listen(_serverFd, 128) < 0)
		{
			//Logger::log("Failed: LISTEN!");
			throw std::runtime_error("Error: Listen FT.");
		}
		
		_sockets.push_back(_serverFd);

		//Logger::logInt("Failed: SOCKET!", port);
		std::cout << "Listening on port " << port << std::endl;
	}
}

Server::~Server()
{
	for (std::vector<int>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
	{
		int sockfd = *it;
		close(sockfd);
	}

	close(_epollFd);

	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
}

void Server::start()
{
	while (true)
	{
		handleEvents();
	}
}

void Server::handleEvents()
{
	struct epoll_event events[MAX_EVENTS];
	int numEvents = epoll_wait(_epollFd, events, MAX_EVENTS, -1);

	for (int i = 0; i < numEvents; i++)
	{
		int eventFd = events[i].data.fd;

		if (std::find(_sockets.begin(), _sockets.end(), eventFd) != _sockets.end())
		{
			acceptClient(eventFd);
		}
		else
		{
			if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))  
			{
				// Client disconnected or error occurred
				removeClient(eventFd);
				continue;
			}
			if (events[i].events & EPOLLIN)  // If the socket is ready for reading
			{
				handleClientRequest(eventFd);
			}
			else if (events[i].events & EPOLLOUT)  // If the socket is ready for writing
			{
				sendClientResponse(eventFd);
			}
		}
	}
}

void Server::sendClientResponse(int clientFd)
{
	Client* client = _clients[clientFd];
	if (!client)
	{ 
		//Logger::logInt("Error: client not found FD ", clientFd);
		// std::cout << "Error: client not found FD " << clientFd << " removed." << std::endl;
		return;
	}

	// Get the response data to send
	std::string response = client->getRequest();

	// Send data in chunks if needed
	if (!client->sendData(response))
	{
		//Logger::logInt("Error: send() failed for FD ", clientFd);
		removeClient(clientFd);  // Remove client if sending failed
		return;
	}

	// If full response was sent, remove client from EPOLLOUT
	if ((size_t)client->gettotalRecevied() == response.size())
	{
		struct epoll_event event;
		event.events = EPOLLIN;  // Remove EPOLLOUT, keep EPOLLIN
		event.data.fd = clientFd;
		if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, clientFd, &event) == -1)
		{
			//Logger::log("epoll_ctl: EPOLL_CTL_MOD inside sendClientResponse failed");
			std::cerr << "epoll_ctl: EPOLL_CTL_MOD inside sendClientResponse failed" << std::endl;
			close(clientFd);
			return;
		}
	}
}


void Server::acceptClient(int serverFd)
{
	int newFd = accept(serverFd, (struct sockaddr*)&_address, &_addrlen);
	if (newFd < 0)
	{
		//Logger::log("Failed: ACCEPT!");
		std::cerr << "Accept failed on socket " << serverFd << std::endl;
		return;
	}
	//Logger::logInt("Accepted client with fd: ", newFd);
	// std::cout << "Accepted client with fd: " << newFd << std::endl;
	_clients[newFd] = new Client(newFd);

	struct epoll_event clientEvent;
	clientEvent.events = EPOLLIN | EPOLLOUT;
	clientEvent.data.fd = newFd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, newFd, &clientEvent) == -1)
	{
		//Logger::log("epoll_ctl: EPOLL_CTL_ADD failed in acceptClient.");
		std::cerr << "epoll_ctl: EPOLL_CTL_ADD failed in acceptClient." << std::endl;
		close(newFd);
		return;
	}
}


void Server::removeClient(int fd)
{
	//Logger::logInt("Client removed FD: ", fd);
	// std::cout << "Client " << fd << " removed." << std::endl;
	if (_clients.find(fd) != _clients.end())
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);  // Remove from epoll
	close(fd);
}

void Server::handleClientRequest(int clientFd)
{
	Client* client = _clients[clientFd];
	if (!client)
	{ 
		//Logger::logInt("Error: client not found FD ", clientFd);
		// std::cout << "Error: client not found FD " << clientFd << " removed." << std::endl;
		return;
	}
	if (!client->readData())
	{
		removeClient(clientFd);
		return;
	}
	std::string requestData = client->getRequest();

	//Logger::logInt("Raw HTTP Request from client ", clientFd );
	//Logger::log(requestData);
	
	// Parse request line (e.g., "GET /index.html HTTP/1.1")
	Request request(_config);
	request.parseRequest(requestData);

	Response response = _router.routeRequest(request, _config);

	std::string httpResponse = response.buildResponse();

	//Logger::logStr("Sending response:\n", httpResponse);
	// std::cout << "Sending response:\n" << httpResponse << std::endl;

	std::string path = request.getPath();
	//Logger::logStr("Request PATH: ", path);

	if (RequestHandler::isCGIRequest(path))
	{
		if (!RequestHandler::handleCGIRequest(*client, request))
		{
			removeClient(clientFd);
			return;
		}
	}
	else if (request.getMethod() == "POST")
	{
		if (!RequestHandler::handle_post(request, *client))
		{
			removeClient(clientFd);
			return;
		}

	}
	else if (request.getMethod() == "DELETE")
	{
		if (!RequestHandler::handle_delete(request, response, *client))
		{
			removeClient(clientFd);
			return;
		}
	}
	//not sure if must if or else if
	if (request.getMethod() == "GET")
	{
		if (!client->sendData(httpResponse))//added this part*****************---->
		{
			//Logger::logInt("Error: send() failed for FD ", clientFd);
			removeClient(clientFd);  // Remove client if sending failed
			return;
		}
	}
}
