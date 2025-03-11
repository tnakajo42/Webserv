/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _Request.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnakajo <tnakajo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:45:47 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/11 17:11:30 by tnakajo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>
#include <iostream>

class Request
{
	private:
		std::string							 _method;
		std::string							 _path;
		std::string							 _httpVersion;
		std::map<std::string, std::string>	 _headers;
		std::string							 _body;
		std::string							 _queryString;

	public:
		Request();
		~Request();
		bool parseRequest(const std::string &rawRequest);
		
		std::string getMethod() const;
		std::string getPath() const;
		std::string getHeader(const std::string &key) const;
		std::string getBody() const;
		std::string getQueryString() const { return _queryString; }
		const std::map<std::string, std::string>& getHeaders() const { return _headers; }


		void setMethod(const std::string& m) { _method = m; }
		void setPath(const std::string& p) { _path = p; }
		void setQueryString(const std::string& q) { _queryString = q; }
		void setBody(const std::string& b) { _body = b; }
		void addHeader(const std::string& name, const std::string& value) { _headers[name] = value; }
};

#endif
