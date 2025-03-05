/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:11 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/05 13:59:18 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

	public:
		Response();
		Response(int status, const std::string& message);
		~Response();
		void setStatusCode(int code);
		void setHeader(const std::string &key, const std::string &value);
		void setBody(const std::string &body);
		std::string getStatusMessage() const;
		
		std::string buildResponse() const;
};

#endif
