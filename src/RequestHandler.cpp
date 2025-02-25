#include "../include/RequestHandler.hpp"

std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath.c_str(), std::ios::binary);
    if (!file) return "";
    
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

std::string generate_response(int status_code, const std::string& content, const std::string& content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << content;
    return response.str();
}

void RequestHandler::handle(int client_socket) {
    char buffer[4096];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::istringstream request_stream(buffer);
        std::string method, path, version;
        request_stream >> method >> path >> version;

        if (path == "/") path = "/index.html";
        std::string filepath = "www" + path;

        std::string content = read_file(filepath);
        std::string response;

        if (!content.empty()) {
            response = generate_response(200, content, "text/html");
        } else {
            response = generate_response(404, "<h1>404 Not Found</h1>", "text/html");
        }

        send(client_socket, response.c_str(), response.size(), 0);
    }
    close(client_socket);
}
