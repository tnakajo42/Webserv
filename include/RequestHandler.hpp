#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class RequestHandler
{
public:
	static void handle(int client_socket);
private:
    static void handle_get(const std::string& path, int client_socket); // add
    static void handle_post(const std::string& path, int client_socket); // add
    static void handle_delete(const std::string& path, int client_socket); // add
};
