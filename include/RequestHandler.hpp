#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class RequestHandler {
public:
    static void handle(int client_socket);
};
