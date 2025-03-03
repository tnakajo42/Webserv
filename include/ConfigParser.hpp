/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:10:54 by cadenegr          #+#    #+#             */
/*   Updated: 2025/03/03 14:17:34 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>

class	ConfigParser
{
	private:
		std::map<std::string, std::string>		_globalSettings;
		std::string								_filename;
	public:
		ConfigParser(std::string& filename);
		void				parse();
		void				displayConfig();

		std::map<std::string, std::string>&	getGlobalSettings();
};