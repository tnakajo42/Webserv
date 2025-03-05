#include "../include/Server.hpp"
#include "../include/RequestHandler.hpp"


Server::Server(const std::string& configPath) : _config(configPath)
{
	setupServer();
}

void	Server::setupServer()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_config.getPort());
	_address.sin_addr.s_addr = INADDR_ANY;
	_addrlen = sizeof(_address);
	
	bind(_serverFd, (struct sockaddr*)&_address, sizeof(_address));
	listen(_serverFd, 10);

	_epollFd = epoll_create(1024);

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = _serverFd;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &event);

	std::cout << "Server started on port " << _config.getPort() << std::endl;
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
			// handleClientRequest(eventFd);
		}
		else
		{
			Client* client = _clients[eventFd];
			if (!client->readData())
				removeClient(eventFd);
			else
			{
				handleClientRequest(client->getFd());
				std::cout << "Handling request for client " << client->getFd() << std::endl;
			}
		}
	}
}

void Server::acceptClient()
{
	// std::cout << "here1133333\n" ;
	int newFd = accept(_serverFd, (struct sockaddr*)&_address, &_addrlen);
	// int newFd = accept(_serverFd, NULL, NULL);
	if (newFd < 0)
    {
        std::cerr << "accept." << std::endl;
        return;
    }
    std::cout << "Accepted client with fd: " << newFd << std::endl;

	Client* newClient = new Client(newFd);
	_clients[newFd] = newClient;

	struct epoll_event clientEvent;
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = newFd;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, newFd, &clientEvent);
	handleClientRequest(newFd);
}

void Server::removeClient(int fd)
{
	if (_clients.find(fd) != _clients.end())
	{
		epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, NULL);
		delete _clients[fd];
		_clients.erase(fd);
		std::cout << "Client " << fd << " removed." << std::endl;
	}
}

void Server::handleClientRequest(int clientFd)
{
	// std::cout << "here\n" ;
	Client* client = _clients[clientFd];
	client->readData();
	std::string requestData = client->getRequest(); // Get request from client
	
	std::cout << "Raw HTTP Request from client " << clientFd << ":\n" << requestData << std::endl;

	Request request;
	request.parseRequest(requestData); // Parse request

	Response response = _router.routeRequest(request);  // Route the request
	std::string httpResponse = response.buildResponse();
	std::cout << "Sending response:\n" << httpResponse << std::endl;
	std::string path = request.getPath();
	if (request.getMethod() == "GET")
		RequestHandler::handle_get(path, clientFd, _config);
	if (request.getMethod() == "POST")
		RequestHandler::handle_post(path, clientFd, client->getRequest(), client->gettotalRecevied(), _config);
	if (request.getMethod() == "DELETE")
		RequestHandler::handle_delete(path, clientFd, _config);
	std::cout << "Response sent. Server should keep running..." << httpResponse << std::endl;  // Debugging
}


// Server::Server(int port)
// {
// 	_addrLen = sizeof(_serverAddr);
// 	//Create a socket
// 	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (_serverFd == -1)
// 	{
// 		std::cerr << "Socket creation failed" << std::endl;
// 		exit(EXIT_FAILURE);
// 	}

// 	_serverAddr.sin_family = AF_INET;
// 	_serverAddr.sin_addr.s_addr = INADDR_ANY;
// 	_serverAddr.sin_port = htons(port);

// 	//Bind
// 	if (bind(_serverFd, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0)
// 	{
// 		std::cerr << "Binding failed" << std::endl;
// 		exit(EXIT_FAILURE);
// 	}

// 	//Listen
// 	if (listen(_serverFd, SOMAXCONN) < 0)
// 	{
// 		std::cerr << "Listening failed" << std::endl;
// 		exit(EXIT_FAILURE);
// 	}

//     // Create an epoll instance
// 	_epollFd = epoll_create1(0);
// 	if (_epollFd == -1)
// 	{
// 		std::cerr << "epoll_create1" << std::endl;
//         exit(EXIT_FAILURE);
// 	}

//     // Add the server socket to epoll
// 	_event.events = EPOLLIN;
// 	_event.data.fd = _serverFd;
// 	epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &_event);	

// 	std::cout << "Server is running on port " << port << std::endl;
// }

// Server::~Server()
// {
// 	close(_serverFd);//wher it open?
// }

// void Server::run()
// {
// 	while (true)//when it becomes false?
// 	{
// 		int	numEvents = epoll_wait(_epollFd, _events, MAX_EVENTS, -1);
// 		for (int i = 0; i < numEvents; ++i)
// 		{
// 			int	eventFd = _events[i].data.fd;

// 			if (eventFd == _serverFd)
// 			{
// 				//new client
// 				_newClient = accept(_serverFd, (struct sockaddr*)&_serverAddr, )
// 			}
// 		}
// 		int client_socket = accept(_serverFd, NULL, NULL);//why use accept and what it returns?
// 		if (client_socket >= 0)
// 			RequestHandler::handle(client_socket);
// 	}
// }
