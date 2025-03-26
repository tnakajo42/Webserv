/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:19:37 by tnakajo           #+#    #+#             */
/*   Updated: 2025/03/25 15:11:28 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGIHandler.hpp"
#include <cstdlib> // For std::strtoul instead of std::stoul

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
		path.find(".php") != std::string::npos ||
		path.find(".rb") != std::string::npos)
		{
			return true;
	}
	return false;
}

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

void RequestHandler::parseRequest(Request& request, Client& client) {
    std::string data = client.getRequest();
    size_t total_received = client.gettotalRecevied();
    
    size_t header_end = data.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        std::cerr << "Incomplete request: No header end found" << std::endl;
        return;
    }
    
    std::string headers = data.substr(0, header_end);
    size_t content_length_pos = headers.find("Content-Length: ");
    if (content_length_pos == std::string::npos) {
        return;  // No body expected
    }

    size_t length_start = content_length_pos + 16;
    size_t length_end = headers.find("\r\n", length_start);
    std::string content_length_str = headers.substr(length_start, length_end - length_start);
    size_t content_length = std::strtoul(content_length_str.c_str(), NULL, 10);
    
    size_t body_start = header_end + 4;
    size_t body_received = total_received > body_start ? total_received - body_start : 0;

    // If not all body data is received, read the rest
    if (body_received < content_length) {
        size_t remaining = content_length - body_received;
        char* buffer = new char[remaining + 1];
        size_t bytes_read = 0;
        while (bytes_read < remaining) {
            ssize_t result = recv(client.getFd(), buffer + bytes_read, remaining - bytes_read, 0);
            if (result <= 0) {
                std::cerr << "Failed to read remaining body: " << strerror(errno) << std::endl;
                break;
            }
            bytes_read += result;
        }
        buffer[bytes_read] = '\0';
        data.append(buffer, bytes_read);
        delete[] buffer;
        body_received = content_length;  // Update after reading
    }

    if (body_received >= content_length)
	{
        request.setBody(data.substr(body_start, content_length));
        // std::cout << "Set POST Body: " << request.getBody() << std::endl;  // Debug
    }
	else
        std::cerr << "Incomplete body: " << body_received << " of " << content_length << std::endl;
}

bool RequestHandler::handleCGIRequest(const Client& cl, const Request& req)
{
    Request request = req;
    Client client = cl;
    ConfigParser config = request.getConfig();

    // Parse the request body for POST requests
    if (request.getMethod() == "POST")
        parseRequest(request, client);  // Ensure POST body is set

    // Execute CGI with the parsed request
    CGIHandler cgi;
    Response response = cgi.executeCGI(request, client, config);

    // Build the full response from the Response object
    std::string headers = "HTTP/1.1 " + std::string(response.getStatusCode() == 200 ? "200 OK" : "500 ERROR") + "\r\n";
    const std::map<std::string, std::string>& responseHeaders = response.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = responseHeaders.begin(); it != responseHeaders.end(); ++it) {
        headers += it->first + ": " + it->second + "\r\n";
    }
    std::ostringstream lengthStream;
    lengthStream << response.getBody().length();
    headers += "Content-Length: " + lengthStream.str() + "\r\n";
    headers += "\r\n";

    std::string fullResponse = headers + response.getBody();
    if (!client.sendData(fullResponse)) // If your Client class needs this too
		return false;
	return true;
}

bool		RequestHandler::handle_post(const Request& req, const Client& cl)
{
	Request		request = req;
	Client		client = cl;
	ConfigParser	config = request.getConfig();

	std::string		path = request.getPath();
	std::string		method = request.getMethod();
	int				client_socket = client.getFd();
	std::string		initial_request = client.getRequest();
	size_t			total_received = client.gettotalRecevied();


	char buffer[8192];
	int bytes_received;

	// Step 1: Parse Content-Length from initial request
	size_t content_length = 0;
	size_t content_length_pos = initial_request.find("Content-Length: ");
	if (content_length_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad Request: Missing Content-Length", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}
	std::string length_str = initial_request.substr(content_length_pos + 16, initial_request.find("\r\n", content_length_pos) - (content_length_pos + 16));
	std::istringstream iss(length_str);
	iss >> content_length;
	if (iss.fail())
	{
		std::string response = generateResponse(400, "Bad initial_Request: Invalid Content-Length", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}

	if (content_length > (size_t)config.getMaxBodySize())  // Compare with limit
	{
		std::string response = generateResponse(413, "Payload Too Large", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);  // Send error response
		close(client_socket);  // Optionally close the connection
		return false;
	}
	
	// Step 2: Calculate body already received and remaining
	size_t header_end = initial_request.find("\r\n\r\n") + 4;
	size_t body_received = total_received - header_end;
	size_t body_remaining = content_length - body_received;

	// Step 3: Read the remaining body
	while (body_remaining > 0 && (bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		buffer[bytes_received] = '\0';
		initial_request.append(buffer, bytes_received);
		total_received += bytes_received;
		body_remaining -= bytes_received;
	}

	// Step 4: Find boundary
	size_t boundary_pos = initial_request.find("boundary=");
	if (boundary_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad initial_Request: No boundary", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}

	size_t boundary_end = initial_request.find("\r\n", boundary_pos);
	std::string boundary = "--" + initial_request.substr(boundary_pos + 9, boundary_end - (boundary_pos + 9));
	std::string end_boundary = boundary + "--";

	// Step 5: Find filename
	size_t filename_pos = initial_request.find("filename=\"");
	if (filename_pos == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad initial_Request: No filename", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}

	size_t filename_start = filename_pos + 10;
	size_t filename_end = initial_request.find("\"", filename_start);
	std::string filename = initial_request.substr(filename_start, filename_end - filename_start);

	// Step 6: Find content start and end
	size_t content_start = initial_request.find("\r\n\r\n", filename_end) + 4;
	size_t content_end = initial_request.find(end_boundary, content_start);
	if (content_start == std::string::npos || content_end == std::string::npos)
	{
		std::string response = generateResponse(400, "Bad initial_Request: Invalid content", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}

	// Step 7: Extract file content
	std::string file_content = initial_request.substr(content_start, content_end - content_start - 2);

	// Step 8: Ensure uploads directory exists
	struct stat st;
	if (stat(path.c_str(), &st) != 0)
	{
		if (mkdir(path.c_str(), 0777) != 0)
		{
			std::string response = generateResponse(500, "Server Error: Cannot create uploads dir", "text/plain");
			send(client_socket, response.c_str(), response.size(), 0);
			return false;
		}
	}

	std::string filepath = path + "/" + filename;
	std::ofstream file(filepath.c_str(), std::ios::binary);
	if (!file)
	{
		std::string response = generateResponse(500, "Server Error: Cannot save file", "text/plain");
		send(client_socket, response.c_str(), response.size(), 0);
		return false;
	}

	file.write(file_content.c_str(), file_content.size());
	file.close();

	std::string response = generateResponse(201, "File uploaded successfully", "text/plain");
	send(client_socket, response.c_str(), response.size(), 0);
	return true;
}

bool	RequestHandler::handle_delete(Request& request, Response& response, Client& client)
{
	ConfigParser	config = request.getConfig();
	
	std::string		path = request.getPath();
	
	std::string httpResponse = response.buildResponse();

	// Construct the file path (assuming files are in "www" directory)
	std::string filepath = config.getRoot() + path;

	// Attempt to delete the file
	if (unlink(filepath.c_str()) == 0)
	{
		// File deleted successfully
		if (!client.sendData(httpResponse))
			return false;
		return true;
	}
	else
	{
		// File not found or error occurred
		if (!client.sendData(httpResponse))
			return false;
		return true;
	}
}
