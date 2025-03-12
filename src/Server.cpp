
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
#include "../include/Server.hpp"


Server::Server(const std::string& configPath) : _config(configPath)
{
	setupServer();
}

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
			Logger::log("Failed: SOCKET!");
			std::cerr << "Error: Socket FT." <<std::endl;
			continue;
		}
		_address.sin_family = AF_INET;
		_address.sin_port = htons(port);
		_address.sin_addr = _config.getHost();
		_addrlen = sizeof(_address);
		
		if (bind(_serverFd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		{
			Logger::log("Failed: BIND!");
			throw std::runtime_error("Error: Bind FT.");
		}
		if (listen(_serverFd, 10) < 0)
		{
			Logger::log("Failed: LISTEN!");
			throw std::runtime_error("Error: Listen FT.");
		}
		
		_sockets.push_back(_serverFd);

		Logger::logInt("Failed: SOCKET!", port);
		std::cout << "Listening on port " << port << std::endl;
	}

	// Create epoll instance
	_epollFd = epoll_create(1024);
	if (_epollFd == -1)
	{
		Logger::log("Failed to create epoll instance.");
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
			Logger::log("epoll_ctl: EPOLL_CTL_ADD failed");
			close(sockfd);
		}
	}
	
	std::cout << "Server setup complete." << std::endl;
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
			std::map<int, Client*>::iterator it = _clients.find(eventFd);
			it = _clients.find(eventFd);
			if (it != _clients.end())
			{
				handleClientRequest(eventFd);
			}
			else
			{
				Logger::logInt("Error: No client foud for FD ", eventFd);
				std::cout << "Error: No client foud for FD " << eventFd << std::endl;
			}
		}
	}
}

void Server::acceptClient(int serverFd)
{
    int newFd = accept(serverFd, (struct sockaddr*)&_address, &_addrlen);
    if (newFd < 0)
    {
        Logger::log("Failed: ACCEPT!");
        std::cerr << "Accept failed on socket " << serverFd << std::endl;
        return;
    }

    std::ostringstream oss;
    oss << "Accepted client with fd: " << newFd;
    Logger::log(oss.str());
    std::cout << oss.str() << std::endl;

    _clients[newFd] = new Client(newFd);

    struct epoll_event clientEvent;
    clientEvent.events = EPOLLIN;
    clientEvent.data.fd = newFd;
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, newFd, &clientEvent) == -1)
    {
        perror("epoll_ctl: EPOLL_CTL_ADD failed");
        close(newFd);
        return;
    }
}


void Server::removeClient(int fd)
{
	std::ostringstream oss;
	oss << "Client " << fd << " removed.";
	Logger::log(oss.str());
	std::cout << "Client " << fd << " removed." << std::endl;
	if (_clients.find(fd) != _clients.end())
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
	close(fd);
}

// changed
void Server::handleClientRequest(int clientFd)
{
	Client* client = _clients[clientFd];
	if (!client)
	{ 
		std::ostringstream oss;
		oss << "Error: client not found FD " << clientFd << " removed.";
		Logger::log(oss.str());
		std::cout << "Error: client not found FD " << clientFd << " removed." << std::endl;
		return;
	}
	if (!client->readData())
	{
		removeClient(clientFd);
		return;
	}
	std::string requestData = client->getRequest();
    std::ostringstream oss;
    oss << "Raw HTTP Request from client " << clientFd << ":\n" << requestData;
    Logger::log(oss.str());
    std::cout << oss.str() << std::endl;

    Request request;
    request.parseRequest(requestData);
    Response response = _router.routeRequest(request);
    std::string httpResponse = response.buildResponse();
    std::ostringstream oss1;
    oss1 << "Sending response:\n" << httpResponse;
    Logger::log(oss1.str());
    std::cout << oss1.str() << std::endl;

    std::string path = request.getPath();
    if (request.getMethod() == "GET")
        RequestHandler::handle_get(path, clientFd, _config);
    else if (request.getMethod() == "POST")
        RequestHandler::handle_post(path, clientFd, client->getRequest(), client->gettotalRecevied(), _config);
    else if (request.getMethod() == "DELETE")
        RequestHandler::handle_delete(path, clientFd, _config);
    std::cout << "Response sent. Server should keep running..." << httpResponse << std::endl;

	send(clientFd, httpResponse.c_str(), httpResponse.size(), 0);
    std::ostringstream oss2;
    oss2 << "Response sent. Server should keep running...";
    Logger::log(oss2.str());
    std::cout << oss2.str() << std::endl;
}
