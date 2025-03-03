/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cadenegr <neo_dgri@hotmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:14:26 by cadenegr          #+#    #+#             */
/*   Updated: 2025/02/28 12:19:03 by cadenegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>


class	LocationConfig
{
	public:
		std::string					_root;
		std::string					_index;
		std::vector<std::string>	_allowedMetods;
		bool						_directoryListing;
		std::string					_uploadStore;
		std::string					_cgiPath;
};