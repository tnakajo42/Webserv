/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:33 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/20 16:29:06 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"
#include "Router.hpp"
#include "Client.hpp"

class CGIHandler
{
	public:
		CGIHandler();
		~CGIHandler();
		Response executeCGI(const Request &request, Client& client, ConfigParser& config);
	private:
		std::map<std::string, std::string> setupCGIEnvironment(const Request &request);
		char** createEnvp(const std::map<std::string, std::string> &env);
		std::string getCGIScriptPath(const Request &request);
		void parseCGIOutput(const std::string &output, Response &response);
};

#endif
