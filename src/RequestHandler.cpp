#include "../include/RequestHandler.hpp"

std::string read_file(const std::string& filepath)
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

std::string generate_response(int status_code, const std::string& content, const std::string& content_type)
{
	std::ostringstream response;
	response << "HTTP/1.1 " << status_code << (status_code == 200 ? " OK" : " ERROR") << "\r\n"; // add error handling
	response << "Content-Type: " << content_type << "\r\n";
	response << "Content-Length: " << content.size() << "\r\n";
	response << "Connection: close\r\n\r\n";
	response << content;
	return response.str();
}

void RequestHandler::handle(int client_socket)
{
	char	buffer[4096];//why a buffer is needed?
	// Because recv() (used for reading from a socket) doesn't guarantee that we receive all data at once.
	int		bytes_received = recv(client_socket, buffer, (sizeof(buffer) - 1), 0);//what recv does?
    // receives data from the client over the socket.

	if (bytes_received > 0)
	{
		buffer[bytes_received] = '\0';//what is bytes_received is somewhere in the middle of the str, is not a problem to NULL it between?
		// To terminate the string properly for safe handling in C++ functions.
		std::istringstream request_stream(buffer);
		std::string method, path, version;
		request_stream >> method >> path >> version;//what happens here? do method, path and version become copies of buffer?
		// GET /index.html HTTP/1.1

		if (method == "GET")
			handle_get(path, client_socket);
		else if (method == "POST")
			handle_post(path, client_socket);
		else if (method == "DELETE")
			handle_delete(path, client_socket);
		else
		{
			std::string response = generate_response(405, "Only Allowed GET/POST/DELETE", "text/plain");
			send(client_socket, response.c_str(), response.size(), 0);
		}
	}
	close(client_socket);//why close? -> Closing the socket frees up system resources.
}

void RequestHandler::handle_get(const std::string& path, int client_socket)
{
	// if (path == "/") path = "/index.html";//why path should equal "/"//4th line******CONFIG
	std::string filepath = "www" + path;
	std::string content = read_file(filepath);
	std::string response;

	if (!content.empty())
	{
		response = generate_response(200, content, "text/html");
	}
	else
	{
		response = generate_response(404, "<h1>404 Not Found</h1>", "text/html");//5th line
	}

	send(client_socket, response.c_str(), response.size(), 0);//what send does?
	// Sends data back to the client through the socket.
}

void RequestHandler::handle_post(const std::string& path, int client_socket)
{
	(void)path;
    std::string filename = "www/uploads/uploaded_image.jpg"; // Use actual filename parsing if needed
    std::ofstream file(filename.c_str(), std::ios::binary);

    if (!file) {
        std::string response = generate_response(500, "Server Error", "text/plain");
        send(client_socket, response.c_str(), response.size(), 0);
        return;
    }

    char buffer[4096];
    int bytes_received;

    // Read data from the socket and write it directly into the file
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
        file.write(buffer, bytes_received);

    file.close();

    std::string response = generate_response(201, "Image uploaded successfully", "text/plain");
    send(client_socket, response.c_str(), response.size(), 0);
}




// void RequestHandler::handle_post(const std::string& path, int client_socket)
// {
//     (void)path;
//     char buffer[8192]; // Initial buffer
//     std::string request;

//     // **Step 1: Read Full Request**
//     int bytes_received;
//     while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
//         buffer[bytes_received] = '\0'; // Null terminate
//         request.append(buffer, bytes_received);
//     }

//     if (request.empty()) {
//         std::string response = generate_response(400, "Bad Request: No data received", "text/plain");
//         send(client_socket, response.c_str(), response.size(), 0);
//         return;
//     }

//     // **Step 2: Extract Content-Type**
//     size_t content_type_pos = request.find("Content-Type: multipart/form-data");
//     if (content_type_pos == std::string::npos) {
//         std::string response = generate_response(400, "Bad Request: Missing multipart/form-data", "text/plain");
//         send(client_socket, response.c_str(), response.size(), 0);
//         return;
//     }

//     // **Step 3: Extract Boundary**
//     size_t boundary_pos = request.find("boundary=");
//     if (boundary_pos == std::string::npos) {
//         std::string response = generate_response(400, "Bad Request: Missing boundary", "text/plain");
//         send(client_socket, response.c_str(), response.size(), 0);
//         return;
//     }

//     size_t boundary_end = request.find("\r\n", boundary_pos);
//     std::string boundary = "--" + request.substr(boundary_pos + 9, boundary_end - (boundary_pos + 9));

//     // **Step 4: Find the File Content**
//     size_t body_start = request.find("\r\n\r\n", boundary_end) + 4;
//     size_t file_start = request.find("\r\n\r\n", body_start) + 4;
//     size_t file_end = request.find(boundary, file_start) - 2; // -2 to remove \r\n

//     if (file_start == std::string::npos || file_end == std::string::npos || file_end <= file_start) {
//         std::string response = generate_response(400, "Bad Request: Invalid file format", "text/plain");
//         send(client_socket, response.c_str(), response.size(), 0);
//         return;
//     }

//     std::string file_content = request.substr(file_start, file_end - file_start);

//     // **Step 5: Save the File**
//     std::string filename = "www/uploads/uploaded_image.jpg"; // Change dynamically if needed
//     std::ofstream file(filename.c_str(), std::ios::binary);

    
//     if (!file) {
//         std::string response = generate_response(500, "Server Error: Cannot save file", "text/plain");
//         send(client_socket, response.c_str(), response.size(), 0);
//         return;
//     }

//     file.write(file_content.c_str(), file_content.size());
//     file.close();

//     // **Step 6: Send Success Response**
//     std::string response = generate_response(201, "Image uploaded successfully", "text/plain");
//     send(client_socket, response.c_str(), response.size(), 0);
// }






void RequestHandler::handle_delete(const std::string& path, int client_socket) {
    std::string filepath = "www" + path;

    if (remove(filepath.c_str()) == 0) {
        std::string response = generate_response(200, "Image deleted successfully", "text/plain");
        send(client_socket, response.c_str(), response.size(), 0);
    } else {
        std::string response = generate_response(404, "File Not Found", "text/plain");
        send(client_socket, response.c_str(), response.size(), 0);
    }
}
