/*
 * Surface.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "Surface.hpp"

using std::string;

/*******************************************************************************
 * Surface
 ******************************************************************************/

Surface::Surface(const string& name, t_ilm_surface id) :
	mName(name),
	mID(id),
	mLog("Surface")
{
	LOG(mLog, DEBUG) << "Create: " << mName << ", id: " << mID;
}

Surface::~Surface()
{
	LOG(mLog, DEBUG) << "Delete: " + mName;
}
