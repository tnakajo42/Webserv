/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:33 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/06 16:38:28 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Logger.hpp"

class CGIHandler
{
	public:
		CGIHandler();
		~CGIHandler();
		Response executeCGI(const Request &request);
};

#endif
