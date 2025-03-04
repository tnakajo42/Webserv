/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:14:26 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/03 19:09:15 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>


class	LocationConfig
{
	public:
		int							_port;
		std::string					_host;
		std::string					_root;
		std::string					_index;
		std::string					_errorPage;
		int							_maxClientBodySize;
		std::string					_cgiPath;
		std::string					_uploadStore;
		std::vector<std::string>	_allowedMetods;

		LocationConfig();
		LocationConfig(std::map<std::string, std::string>& globalSettings);
		~LocationConfig();

};