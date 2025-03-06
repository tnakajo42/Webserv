#include "../include/RequestHandler.hpp"

std::string		readFile(const std::string& filepath)
{
	std::ifstream file(filepath.c_str(), std::ios::binary);//what does std::ios::binary do?
	// 	-> The std::ios::binary flag opens the file in binary mode. 
	//	Normally, files are opened in text mode,
	if (!file)
		return "";
    
	std::ostringstream content;
	content << file.rdbuf();//what rdbuf() do?
	// file.rdbuf() gets the stream buffer of file and directly loads the entire file into content (a string stream).
	return content.str();
}

std::string		generateResponse(int status_code, const std::string& content, const std::string& content_type)
{
	std::ostringstream response;
	response << "HTTP/1.1 " << status_code << (status_code == 200 ? " OK" : " ERROR") << "\r\n"; // add error handling
	response << "Content-Type: " << content_type << "\r\n";
	response << "Content-Length: " << content.size() << "\r\n";
	response << "Connection: close\r\n\r\n";
	response << content;
	return response.str();
}

void			RequestHandler::handle_get(std::string& path, int client_socket, ConfigParser& config)
{
	// ConfigParser settings("default.config");
	if (path == "/")
		path = config.getIndex();
	// path = "/index.html";//why path should equal "/"//4th line******CONFIG

	std::string filepath = "www" + path;
	std::string content = readFile(filepath);
	std::string response;
	if (!content.empty())
		response = generateResponse(200, content, "text/html");
	else
		response = generateResponse(404, "<h1>404 Not Found</h1>", "text/html");//5th line
	send(client_socket, response.c_str(), response.size(), 0);//what send does?
	// Sends data back to the client through the socket.
}

void RequestHandler::handle_post(const std::string& path, int client_socket, const std::string& initial_request, size_t initial_received, ConfigParser& config) {
	(void)config;
	if (path != "/uploads")
	{
		std::string response = generateResponse(404, "Not Found", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	char buffer[8192];
	std::string request = initial_request; // Start with what handle already read
	int bytes_received;
	size_t total_received = initial_received;

	// Step 1: Parse Content-Length from initial request
	size_t content_length = 0;
	size_t content_length_pos = request.find("Content-Length: ");
	if (content_length_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad Request: Missing Content-Length", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}
	std::string length_str = request.substr(content_length_pos + 16, request.find("\r\n", content_length_pos) - (content_length_pos + 16));
	std::istringstream iss(length_str);
	iss >> content_length;
	if (iss.fail())
	{
		std::string response = generateResponse(400, "Bad Request: Invalid Content-Length", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	// Step 2: Calculate body already received and remaining
	size_t header_end = request.find("\r\n\r\n") + 4;
	size_t body_received = total_received - header_end;
	size_t body_remaining = content_length - body_received;

	// Step 3: Read the remaining body
	while (body_remaining > 0 && (bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		buffer[bytes_received] = '\0';
		request.append(buffer, bytes_received);
		total_received += bytes_received;
		body_remaining -= bytes_received;
	}

	std::cout << "Received full request (total length: " << total_received << ", body length: " << content_length << "):\n" << request << std::endl;

	// Step 4: Find boundary
	size_t boundary_pos = request.find("boundary=");
	if (boundary_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad Request: No boundary", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	size_t boundary_end = request.find("\r\n", boundary_pos);
	std::string boundary = "--" + request.substr(boundary_pos + 9, boundary_end - (boundary_pos + 9));
	std::string end_boundary = boundary + "--";

	// Step 5: Find filename
	size_t filename_pos = request.find("filename=\"");
	if (filename_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad Request: No filename", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	size_t filename_start = filename_pos + 10;
	size_t filename_end = request.find("\"", filename_start);
	std::string filename = request.substr(filename_start, filename_end - filename_start);

	// Step 6: Find content start and end
	size_t content_start = request.find("\r\n\r\n", filename_end) + 4;
	size_t content_end = request.find(end_boundary, content_start);
	if (content_start == std::string::npos || content_end == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad Request: Invalid content", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	// Step 7: Extract file content
	std::string file_content = request.substr(content_start, content_end - content_start - 2);

	// Step 8: Ensure uploads directory exists
	struct stat st;
	if (stat("www/uploads", &st) != 0)
	{
		if (mkdir("www/uploads", 0777) != 0)
		{
			std::string response = generateResponse(500, "Server Error: Cannot create uploads dir", "text/plain");
			send(client_socket, response.c_str(), response.size(), 0);
			return;
		}
	}

	std::string filepath = "www/uploads/" + filename;
	std::ofstream file(filepath.c_str(), std::ios::binary);
	if (!file)
	{
		std::string response = generateResponse(500, "Server Error: Cannot save file", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return;
	}

	file.write(file_content.c_str(), file_content.size());
	file.close();

	std::string response = generateResponse(201, "File uploaded successfully", "text/plain");
	send(client_socket, response.c_str(), response.size(), 0);
}

void RequestHandler::handle_delete(const std::string& path, int client_socket, ConfigParser& config)
{
	(void)config;
	std::string filepath = "www" + path;

	if (remove(filepath.c_str()) == 0)
	{
		std::string response = generateResponse(200, "Image deleted successfully", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
	}
	else
	{
		std::string response = generateResponse(404, "File Not Found", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
	}
}
