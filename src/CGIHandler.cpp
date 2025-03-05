/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:08:48 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/04 20:47:25 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGIHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

CGIHandler::CGIHandler()
{}
CGIHandler::~CGIHandler()
{}

Response CGIHandler::executeCGI(const Request &request)
{
    (void)request;  // Suppress warning if request isn't used

    const char *args[] = {"/usr/bin/python3", "-u", NULL};  // Fix: use const char*
    execve(args[0], (char *const *)args, NULL);

    std::cerr << "execve failed" << std::endl;
    exit(1);  // Fix: use exit() from <cstdlib>
}

// Response CGIHandler::executeCGI(const Request &request)
// {
//     Response response;
//     int pipe_fd[2];
//     pipe(pipe_fd);

// 	(void)request;
//     pid_t pid = fork();
//     if (pid == 0) {  // Child process
//         dup2(pipe_fd[1], STDOUT_FILENO);
//         close(pipe_fd[0]);
        
//         const char *args[] = {"/usr/bin/python3", "-u", NULL};
//         execve(args[0], args, NULL);
//         exit(1);
//     } else {  // Parent process
//         close(pipe_fd[1]);
        
//         char buffer[1024];
//         int bytesRead = read(pipe_fd[0], buffer, sizeof(buffer));
//         buffer[bytesRead] = '\0';

//         response.setBody(std::string(buffer));
//         response.setHeader("Content-Type", "text/html");

//         waitpid(pid, NULL, 0);
//     }

//     return response;
// }
