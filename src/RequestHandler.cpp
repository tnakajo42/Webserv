#include "../include/RequestHandler.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/CGIHandler.hpp"
#include <cstdlib> // For std::strtoul instead of std::stoul

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

// add
// Determine if a request should be handled by CGI
// For example, check if it's in the cgi-bin directory or has specific extension
bool RequestHandler::isCGIRequest(const std::string& path)
{
	if (path.find("/cgi-bin/") == 0 || 
		path.find(".py") != std::string::npos ||
		path.find(".php") != std::string::npos)
		return true;
	return false;
}
// add
std::string RequestHandler::getContentType(const std::string& filepath)
{
	// Get file extension
	size_t dot_pos = filepath.find_last_of('.');
	if (dot_pos == std::string::npos)
		return "application/octet-stream";

	std::string ext = filepath.substr(dot_pos + 1);

	// Map extension to content type
	if (ext == "html" || ext == "htm") return "text/html";
	if (ext == "css") return "text/css";
	if (ext == "js") return "application/javascript";
	if (ext == "json") return "application/json";
	if (ext == "png") return "image/png";
	if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
	if (ext == "gif") return "image/gif";
	if (ext == "svg") return "image/svg+xml";
	if (ext == "txt") return "text/plain";

	return "application/octet-stream";
}
void RequestHandler::parseRequest(Request& request, const std::string& initial_data, 
                               size_t initial_received, int client_socket)
{
	std::string data = initial_data;
	size_t total_received = initial_received;

	// Extract Content-Length
	size_t content_length_pos = data.find("Content-Length: ");
	if (content_length_pos == std::string::npos)
		return; // No Content-Length found

	size_t content_length_end = data.find("\r\n", content_length_pos);
	std::string content_length_str = data.substr(content_length_pos + 16, 
												content_length_end - (content_length_pos + 16));
	size_t content_length = std::strtoul(content_length_str.c_str(), NULL, 10); // Use strtoul instead of stoul

	// Find the end of headers
	size_t header_end = data.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return; // No end of headers found

	// Parse headers
	size_t pos = 0;
	size_t line_end;

	while ((line_end = data.find("\r\n", pos)) != std::string::npos && line_end < header_end)
	{
		std::string line = data.substr(pos, line_end - pos);
		size_t colon_pos = line.find(": ");
		if (colon_pos != std::string::npos)
		{
			std::string header_name = line.substr(0, colon_pos);
			std::string header_value = line.substr(colon_pos + 2);
			request.addHeader(header_name, header_value);
		}    
		pos = line_end + 2;
	}    
	// Calculate how much of the body we've already received
	size_t body_start = header_end + 4;
	size_t body_received = total_received - body_start;
	// If we haven't received the full body, read more data
	if (body_received < content_length)
	{
		size_t remaining = content_length - body_received;
		char* buffer = new char[remaining + 1];
		size_t bytes_read = 0;
		while (bytes_read < remaining)
		{
			ssize_t result = recv(client_socket, buffer + bytes_read, remaining - bytes_read, 0);
			if (result <= 0)
				break; // Error or connection closed
			bytes_read += result;
		}
		
		buffer[bytes_read] = '\0';
		data.append(buffer, bytes_read);
		delete[] buffer;
	}

	// Set request body
	request.setBody(data.substr(body_start));
}

std::string RequestHandler::formatResponse(const Response& response) {
	std::ostringstream http_response;

	// Status line
	http_response << "HTTP/1.1 " << response.getStatusCode() << " ";

	// Status text
	switch (response.getStatusCode())
	{
		case 200: http_response << "OK"; break;
		case 201: http_response << "Created"; break;
		case 204: http_response << "No Content"; break;
		case 400: http_response << "Bad Request"; break;
		case 404: http_response << "Not Found"; break;
		case 500: http_response << "Internal Server Error"; break;
		default: http_response << "Unknown"; break;
	}
	http_response << "\r\n";

	// Headers
	const std::map<std::string, std::string>& headers = response.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); 
			it != headers.end(); ++it)
		http_response << it->first << ": " << it->second << "\r\n";
	// Add Content-Length if not present
	if (headers.find("Content-Length") == headers.end())
		http_response << "Content-Length: " << response.getBody().size() << "\r\n";
	// End of headers
	http_response << "\r\n";
	// Body
	http_response << response.getBody();
	return http_response.str();
}

void RequestHandler::handleCGIRequest(const std::string& path, int client_socket, 
                                   const std::string& initial_request, size_t initial_received,
                                   ConfigParser& config, const std::string& method) {
    (void)config; // Suppress unused warning if config isn't used
    
    // Create a Request object to pass to CGI handler
    Request request;
    request.setPath(path);
    request.setMethod(method);
    
    // For POST requests, we need to parse the body
    if (method == "POST")
		// Parse the initial request to extract headers and body
        parseRequest(request, initial_request, initial_received, client_socket);
    else
	{
		// For GET requests, extract query string if present
        size_t query_pos = path.find('?');
        if (query_pos != std::string::npos)
		{
			request.setQueryString(path.substr(query_pos + 1));
            request.setPath(path.substr(0, query_pos));
        }
    }
    
    // Execute the CGI script
    CGIHandler cgi;
	std::cout << "CGI Request - Method: " << method << ", Path: " << path << ", Body: " << request.getBody() << std::endl;
    Response response = cgi.executeCGI(request); // this is not working 
    std::cout << "OK: " << response.getStatusCode() << std::endl;

    // Convert Response object to HTTP response string
    std::string http_response = formatResponse(response);
    
    // Send response to client
    send(client_socket, http_response.c_str(), http_response.size(), 0);
}

void	RequestHandler::handle_get(std::string& path, int client_socket, ConfigParser& config)
{
	// Check if this should be handled by CGI
	if (isCGIRequest(path))
	{
		handleCGIRequest(path, client_socket, "", 0, config, "GET");
		return;
	}
	std::string filepath = "www" + (path == "/" ? config.getIndex() : path); //add
    std::cout << "Serving static file: " << filepath << std::endl; //add
	// // ConfigParser settings("default.config");
	// if (path == "/")
	// 	path = config.getIndex();
	// path = "/index.html";//why path should equal "/"//4th line******CONFIG

	// std::string	filepath = config.getIndex();
	std::string content = readFile(filepath);
	std::string	errorPath = config.getErrorPage();
	std::string contentError = readFile(errorPath);
	std::string response;
	if (!content.empty())
		response = generateResponse(200, content, "text/html");
	else
		response = generateResponse(404, contentError, "text/html");//5th line
	send(client_socket, response.c_str(), response.size(), 0);//what send does?
	// Sends data back to the client through the socket.
}

void RequestHandler::handle_post(const std::string& path, int client_socket, const std::string& initial_request, size_t initial_received, ConfigParser& config) {
	// (void)config;
	if (isCGIRequest(path))
	{
		handleCGIRequest(path, client_socket, initial_request, initial_received, config, "POST");
		return;
	}
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
	(void)config; // Suppress unused parameter warning if config isn’t used

	// Check if this should be handled by CGI
	if (isCGIRequest(path))
	{
		handleCGIRequest(path, client_socket, "", 0, config, "DELETE");
		return;
	}

	// Construct the file path (assuming files are in "www" directory)
	std::string filepath = "www" + path;

	// Attempt to delete the file
	if (unlink(filepath.c_str()) == 0) {
		// File deleted successfully
		std::string response = generateResponse(204, "", "text/plain"); // 204 No Content
		send(client_socket, response.c_str(), response.size(), 0);
	} else {
		// File not found or error occurred
		std::string response = generateResponse(404, "<h1>404 Not Found</h1>", "text/html");
		send(client_socket, response.c_str(), response.size(), 0);
	}
}
