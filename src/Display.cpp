/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: al1
 */

#include "Display.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Display
 ******************************************************************************/

Display::Display(const string& name, t_ilm_display id) :
	IlmObject<t_ilm_display>(name, id),
	mLog("Display" + to_string(id))
{
	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_getScreenResolution(mID, &mWidth, &mHeight)) != ILM_SUCCESS)
	{
		throw DmException("Can't get screen " + to_string(id) + ", resolution",
						  ret);
	}

	LOG(mLog, DEBUG) << "Create display id: " << mID
					 << ", width: " << mWidth
					 << ", height: " << mHeight;
}

Display::~Display()
{
	LOG(mLog, DEBUG) << "Delete display id: " << mID;
}

/*******************************************************************************
 * Public
 ******************************************************************************/


