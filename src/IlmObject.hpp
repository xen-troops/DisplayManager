/*
 * IlmObject.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_ILMOBJECT_HPP_
#define SRC_ILMOBJECT_HPP_

#include <string>

template <typename T>
class IlmObject
{
public:
	IlmObject(const std::string& name, T id) : mName(name), mID(id) {}

	const std::string& getName() const { return mName; }
	T getID() const { return mID; }

	void setParentName(const std::string& name) { mParentName = name; }
	const std::string& getParentName() const { return mParentName; }

protected:
	std::string mName;
	T mID;
	std::string mParentName;
};



#endif /* SRC_ILMOBJECT_HPP_ */
