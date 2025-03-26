/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:46:11 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/24 10:53:31 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <sstream>
#include "ConfigParser.hpp"

class Response
{
	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		ConfigParser						_config;

	public:
		Response();
		Response(int status, const std::string& message, ConfigParser& config);
		~Response();

		void		setStatusCode(int		code);
		void		setStatusMessage(int code);
		void 		setHeader(const std::string &key, const std::string &value);
		void 		setBody(const std::string &body);

		int											getStatusCode() const { return _statusCode; } // add
		const std::map<std::string, std::string>&	getHeaders() const { return _headers; } // add
		std::string									getBody() const { return _body; } // add

		// add this function
		std::string									getHeader(const std::string &key) const
		{
			return _headers.count(key) ? _headers.at(key) : "";
		}

		std::string									getStatusMessage() const;
		std::string									buildResponse() const;
		std::string									buildResponseParameters(int status_code, std::string& content, std::string& content_type);

};

#endif
