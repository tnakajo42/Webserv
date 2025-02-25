#include "../include/Server.hpp"

Server::Server(int port) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << port << std::endl;
}

Server::~Server() {
    close(server_socket);
}

void Server::run() {
    while (true) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket >= 0) {
            RequestHandler::handle(client_socket);
        }
    }
}
