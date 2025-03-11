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

	public:
		Request();
		~Request();
		bool parseRequest(const std::string &rawRequest);
		
		std::string getMethod() const;
		std::string getPath() const;
		std::string getHeader(const std::string &key) const;
		std::string getBody() const;
};

#endif
