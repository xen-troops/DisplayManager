/*
 * Utils.hpp
 *
 *  Created on: Nov 6, 2017
 *      Author: al1
 */

#ifndef SRC_UTILS_HPP_
#define SRC_UTILS_HPP_

#include <string>
#include <vector>

#include <ilm/ilm_types.h>

class Utils
{
public:
	static std::string idsToStr(const std::vector<t_ilm_uint>& ids);
};



#endif /* SRC_UTILS_HPP_ */
