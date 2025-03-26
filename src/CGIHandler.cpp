/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:48 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/24 11:09:00 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include "Response.hpp"

CGIHandler::CGIHandler() {}
CGIHandler::~CGIHandler() {}

Response CGIHandler::executeCGI(const Request &req, Client& client, ConfigParser& config)
{
	std::string	requestData = client.getRequest();
	Request		request = req;
	Response	response;

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		Logger::log("Failed to create pipe");
		response.setStatusCode(500);
		response.setBody("Internal Server Error: CGI execution failed");
		return response;
	}
	pid_t pid = fork();
	if (pid == -1)
	{
		Logger::log("Failed to fork process");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		response.setStatusCode(500);
		response.setBody("Internal Server Error: CGI execution failed");
		return response;
	}
	if (pid == 0) {  // Child process
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		
		if (request.getMethod() == "GET" && !request.getBody().empty())
		{
			std::cout << "this is GET" << std::endl;
		}
		if (request.getMethod() == "POST" && !request.getBody().empty())
		{
			int stdin_pipe[2];
			if (pipe(stdin_pipe) == -1) {
				std::cerr << "Failed to create stdin pipe: " << strerror(errno) << std::endl;
				exit(1);
			}
			if (dup2(stdin_pipe[0], STDIN_FILENO) == -1) {
				std::cerr << "Failed to dup2 stdin: " << strerror(errno) << std::endl;
				exit(1);
			}
			close(stdin_pipe[0]);
			// std::cerr << "----> Writing POST Body to stdin: " << request.getBody() << std::endl;
			ssize_t bytes_written = write(stdin_pipe[1], request.getBody().c_str(), request.getBody().size());
			if (bytes_written == -1) {
				std::cerr << "Failed to write to stdin: " << strerror(errno) << std::endl;
				exit(1);
			}
			close(stdin_pipe[1]);
		} else if (request.getMethod() == "POST") {
			std::cerr << "Warning: POST request but body is empty!" << std::endl;
		}
	
		std::map<std::string, std::string> env = setupCGIEnvironment(request);
		char **envp = createEnvp(env);
		std::string scriptPath = getCGIScriptPath(request);
		const char *interpreter;
		if (scriptPath.find(".php") != std::string::npos)
			interpreter = "/usr/bin/php-cgi";
		else if (scriptPath.find(".py") != std::string::npos)
			interpreter = "/usr/bin/python3";
		else if (scriptPath.find(".rb") != std::string::npos)
			interpreter = "/usr/bin/ruby";
		else {
			std::cerr << "Unknown script type: " << scriptPath << std::endl;
			exit(1);
		}
		std::cout << "Child - Executing: " << interpreter << " " << scriptPath << std::endl;
		const char *args[] = {interpreter, scriptPath.c_str(), NULL};
		execve(args[0], (char *const *)args, envp);
		std::cerr << "execve failed: " << strerror(errno) << std::endl;
		for (int i = 0; envp[i] != NULL; i++)
			delete[] envp[i];
		delete[] envp;
		exit(1);
	}
	else
	{
		char buffer[4096];
		std::string output;
		ssize_t bytesRead;
		int status;

		// Parent process
		close(pipe_fd[1]);
		while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			output += buffer;
		}
		waitpid(pid, &status, 0);
		close(pipe_fd[0]);
        // Log the raw output for debugging
        Logger::logStr("Raw CGI Output:\n", output); // Taka add here
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			parseCGIOutput(output, response); // Taka add here
		else
		{
				request.parseRequest(requestData);
				Router	router;
				response = router.routeRequest(request, config);
				std::string httpResponse = response.buildResponse();
				response.setStatusCode(500);
		}
	}
	return response;
}

std::map<std::string, std::string>	CGIHandler::setupCGIEnvironment(const Request &request)
{
	std::map<std::string, std::string> env;

	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "Webserv/1.0";
	env["REQUEST_METHOD"] = request.getMethod();
	env["SCRIPT_NAME"] = request.getPath();
	env["PATH_INFO"] = "";
	env["QUERY_STRING"] = request.getQueryString();
	env["CONTENT_TYPE"] = request.getHeader("Content-Type");
	env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
	env["REDIRECT_STATUS"] = "200";
	env["SCRIPT_FILENAME"] = getCGIScriptPath(request);

	// Convert headers to HTTP_ prefixed variables
	std::map<std::string, std::string> headers = request.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		std::string name = "HTTP_" + it->first;
		for (size_t i = 0; i < name.length(); i++)
		{
			if (name[i] == '-')
				name[i] = '_';
			else
				name[i] = std::toupper(name[i]);
		}
		env[name] = it->second;
	}
	return env;
}

char** CGIHandler::createEnvp(const std::map<std::string, std::string> &env)
{
	char **envp = new char*[env.size() + 1];
	int i = 0;

	for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it)
	{
		std::string entry = it->first + "=" + it->second;
		envp[i] = new char[entry.length() + 1];
		std::strcpy(envp[i], entry.c_str());
		i++;
	}
	envp[i] = NULL;
	return envp;
}

std::string CGIHandler::getCGIScriptPath(const Request &request)
{
	// Extract script path from request
	// This is a simplified version - you might need more complex path handling
	std::string path = request.getPath();
	if (path.find("/cgi-bin/") == 0)
	{
		std::string fullPath = "www" + path;
		std::cout << "Resolved script path: " << fullPath << std::endl;
		return fullPath;
	}
	// Default script if no specific script is requested
	return "www/index.html";
}

void CGIHandler::parseCGIOutput(const std::string &output, Response &response)
{
	//add
	if (response.getHeader("Connection").empty())
		response.setHeader("Connection", "keep-alive");
	// Find the boundary between headers and body (empty line)

	size_t headerEnd = output.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
	{
		headerEnd = output.find("\n\n");
		if (headerEnd == std::string::npos)
		{
			// No headers found, assume entire output is body
			response.setStatusCode(200);
			response.setHeader("Content-Type", "text/html");
			response.setBody(output);
			return;
		}
	}

	// Extract headers and body
	std::string headerSection = output.substr(0, headerEnd);
	std::string body = output.substr(headerEnd + (output[headerEnd+1] == '\n' ? 2 : 4));

	// Parse headers
	size_t start = 0;
	size_t end = headerSection.find("\n");
	
	while (start < headerSection.length() && end != std::string::npos)
	{
		std::string line = headerSection.substr(start, end - start);
		if (line[line.length() - 1] == '\r')
			line = line.substr(0, line.length() - 1);
		size_t colon = line.find(":");
		if (colon != std::string::npos)
		{
			std::string name = line.substr(0, colon);
			std::string value = line.substr(colon + 1);
			// Trim leading whitespace from value
			while (!value.empty() && std::isspace(value[0]))
				value = value.substr(1);
			// Special handling for Status header
			if (name == "Status")
				response.setStatusCode(std::atoi(value.c_str()));
			else
				response.setHeader(name, value);
		}
		
		start = end + 1;
		end = headerSection.find("\n", start);
	}
	if (response.getStatusCode() == 0)
		response.setStatusCode(200);
	if (response.getHeader("Content-Type").empty())
		response.setHeader("Content-Type", "text/html");
	response.setBody(body);
}
