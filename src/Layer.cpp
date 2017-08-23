/*
 * Layer.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: al1
 */

#include "Layer.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Layer
 ******************************************************************************/

Layer::Layer(const string& name, t_ilm_layer id, int width, int height) :
	IlmObject<t_ilm_layer>(name, id),
	mLog("Layer")
{
	ilmErrorTypes ret = ILM_SUCCESS;

	auto requestedID = mID;

	if ((ret = ilm_layerCreateWithDimension(&mID, width, height)) !=
		ILM_SUCCESS)
	{
		LOG(mLog, WARNING) << "Create layer id: " << mID << " failed. "
						   << "Trying to remove.";

		if ((ret = ilm_layerRemove(mID)) != ILM_SUCCESS)
		{
			throw DmException("Failed to remove layer " + to_string(mID), ret);
		}

		if ((ret = ilm_layerCreateWithDimension(&mID, width, height)) !=
			ILM_SUCCESS)
		{
			throw DmException("Can't create layer " + to_string(mID), ret);
		}
	}

	if (requestedID != mID)
	{
		ilm_layerRemove(mID);

		throw DmException("Can't set requested layer ID " +
						  to_string(requestedID), ret);
	}

	LOG(mLog, DEBUG) << "Create: " << mName << ", id: " << mID;
}

Layer::~Layer()
{
	ilm_layerRemove(mID);

	LOG(mLog, DEBUG) << "Delete: " + mName;
}
