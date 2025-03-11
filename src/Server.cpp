#include "../include/Server.hpp"

Server::Server(const std::string& configPath) : _config(configPath)
{
	setupServer();
}

void	Server::setupServer()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
	{
		Logger::log("Failed: SOCKET!");
		throw std::runtime_error("Error: Socket FT.");
	}
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_config.getPort());
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

	_epollFd = epoll_create(1024);

	struct epoll_event event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = _serverFd;
	// epoll_ctl(_epollFd, EPOLL_CTL_MOD, _serverFd, &event);
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &event) == -1)
	{
		perror("epoll_ctl: EPOLL_CTL_ADD failed");
		close(_serverFd);
		return;
	}
	

	std::ostringstream	oss;
	oss << "Server started on port " << _config.getPort() << std::endl;
	Logger::log(oss.str());
	std::cout << oss.str() << std::endl;
}

Server::~Server()
{
	close(_serverFd);
	close(_epollFd);
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete it->second;
	}
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

		if (eventFd == _serverFd)
		{
			acceptClient();
		}
		else
		{
			std::map<int, Client*>::iterator it;
			it = _clients.find(eventFd);
			if (it != _clients.end())
			{
				Client* client = _clients[eventFd];
				handleClientRequest(client->getFd());
			}
			else
			{
				std::ostringstream oss;
				oss << "Error: No client foud for FD " << eventFd;
				Logger::log(oss.str());
				std::cout << oss.str() << std::endl;
			}
		}
	}
}

void Server::acceptClient()
{
	int newFd = accept(_serverFd, (struct sockaddr*)&_address, &_addrlen);

	if (newFd < 0)
	{
		Logger::log("Failed: ACCEPT!");
		std::cerr << "accept." << std::endl;
		throw std::runtime_error("Error: Accept FT");
	}
	std::ostringstream oss;
	oss << "Accepted client with fd: " << newFd;;
	Logger::log(oss.str());
	std::cout << "Accepted client with fd: " << newFd << std::endl;

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
	// epoll_ctl(_epollFd, EPOLL_CTL_ADD, newFd, &clientEvent);
	// handleClientRequest(newFd);
}

void Server::removeClient(int fd)
{
	std::ostringstream oss;
	oss << "Client " << fd << " removed.";
	Logger::log(oss.str());
	std::cout << "Client " << fd << " removed." << std::endl;
	if (_clients.find(fd) != _clients.end())
	{
		// epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);//???maybe take off
		delete _clients[fd];
		_clients.erase(fd);
	}
	close(fd);
}

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
	std::string requestData = client->getRequest(); // Get request from client
	
	std::ostringstream oss;
	oss << "Raw HTTP Request from client " << clientFd << ":\n" << requestData;
	Logger::log(oss.str());
	std::cout << "Raw HTTP Request from client " << clientFd << ":\n" << requestData << std::endl;

	Request request;
	request.parseRequest(requestData); // Parse request

	Response response = _router.routeRequest(request);  // Route the request
	std::string httpResponse = response.buildResponse();
	std::ostringstream oss1;
	oss1 << "Sending response:\n" << httpResponse;
	Logger::log(oss1.str());
	std::cout << "Sending response:\n" << httpResponse << std::endl;

	std::string path = request.getPath();
	if (request.getMethod() == "GET")
		RequestHandler::handle_get(path, clientFd, _config);
	if (request.getMethod() == "POST")
		RequestHandler::handle_post(path, clientFd, client->getRequest(), client->gettotalRecevied(), _config);
	if (request.getMethod() == "DELETE")
		RequestHandler::handle_delete(path, clientFd, _config);
	std::ostringstream oss2;
	oss2 << "Response sent. Server should keep running..." << httpResponse;
	Logger::log(oss2.str());
	std::cout << "Response sent. Server should keep running..." << httpResponse << std::endl;  // Debugging
}
