/*
 * Utils.cpp
 *
 *  Created on: Nov 6, 2017
 *      Author: al1
 */

#include "Utils.hpp"

using std::string;
using std::to_string;
using std::vector;

string Utils::idsToStr(const vector<t_ilm_uint>& ids)
{
    string str;

    for (auto id : ids) {
        str += to_string(id) + ", ";
    }

    if (ids.size()) {
        str.erase(str.size() - 2, 2);
    }

    return str;
}
