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

Layer::Layer(const string& name, t_ilm_layer id,
			 t_ilm_uint width, t_ilm_uint height) :
	IlmObject("Layer", name, id)
{
	ilmErrorTypes ret = ILM_SUCCESS;

	auto requestedID = mID;

	if ((ret = ilm_layerCreateWithDimension(&mID, width, height)) !=
		ILM_SUCCESS)
	{
		LOG(mLog, WARNING) << "Create layer failed. Trying to remove.";

		if ((ret = ilm_layerRemove(mID)) != ILM_SUCCESS)
		{
			throw DmException("Failed to remove layer: " + to_string(mID), ret);
		}

		if ((ret = ilm_layerCreateWithDimension(&mID, width, height)) !=
			ILM_SUCCESS)
		{
			throw DmException("Can't create layer: " + to_string(mID), ret);
		}
	}

	if (requestedID != mID)
	{
		ilm_layerRemove(mID);

		throw DmException("Can't set requested layer ID: " +
						  to_string(requestedID), ret);
	}
}

Layer::~Layer()
{
	ilm_layerRemove(mID);
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Layer::setVisibility(t_ilm_bool visibility)
{
	LOG(mLog, DEBUG) << "Set visibility: " << visibility;

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_layerSetVisibility(mID, visibility)) != ILM_SUCCESS)
	{
		throw DmException("Can't set layer visibility: " + to_string(mID), ret);
	}
}

void Layer::setOpacity(t_ilm_float opacity)
{
	LOG(mLog, DEBUG) << "Set opacity: " << opacity;

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_layerSetOpacity(mID, opacity)) != ILM_SUCCESS)
	{
		throw DmException("Can't set layer opacity: " + to_string(mID), ret);
	}
}

void Layer::setSourceRectangle(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set source rectangle"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_layerSetSourceRectangle(mID, rect.x, rect.y,
										   rect.width, rect.height)) !=
		ILM_SUCCESS)
	{
		throw DmException("Can't set source rectangle: " +
						  to_string(mID), ret);
	}
}

void Layer::setDestinationRectangle(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set destination rectangle"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_layerSetDestinationRectangle(mID, rect.x, rect.y,
												rect.width, rect.height)) !=
		ILM_SUCCESS)
	{
		throw DmException("Can't set source rectangle: " +
						  to_string(mID), ret);
	}
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Layer::onAddChild(t_ilm_uint id)
{

}

void Layer::onRemoveChild(t_ilm_uint id)
{

}

void Layer::onUpdate(const std::vector<t_ilm_uint>& ids)
{

}
