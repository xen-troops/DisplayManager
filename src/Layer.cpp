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
	auto requestedID = mID;

	auto ret = ilm_layerCreateWithDimension(&mID, width, height);

	if (ret != ILM_SUCCESS)
	{
		LOG(mLog, WARNING) << "Create layer failed. Trying to remove.";

		ret = ilm_layerRemove(mID);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Failed to remove layer: " + to_string(mID), ret);
		}

		ret = ilm_layerCreateWithDimension(&mID, width, height);

		if (ret != ILM_SUCCESS)
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

	auto ret = ilm_layerSetVisibility(mID, visibility);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer visibility: " + to_string(mID), ret);
	}
}

void Layer::setOpacity(t_ilm_float opacity)
{
	LOG(mLog, DEBUG) << "Set opacity: " << opacity;

	auto ret = ilm_layerSetOpacity(mID, opacity);

	if (ret != ILM_SUCCESS)
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

	auto ret = ilm_layerSetSourceRectangle(mID, rect.x, rect.y,
										   rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer source rectangle: " +
						  to_string(mID), ret);
	}
}

void Layer::setDestinationRectangle(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set destination rectangle"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	auto ret = ilm_layerSetDestinationRectangle(mID, rect.x, rect.y,
												rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer destination rectangle: " +
						  to_string(mID), ret);
	}
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Layer::onAddChild(t_ilm_uint id)
{
	LOG(mLog, DEBUG) << "Add surface " << id;

	auto ret = ilm_layerAddSurface(mID, id);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't add surface to layer: " + to_string(mID), ret);
	}
}

void Layer::onRemoveChild(t_ilm_uint id)
{
	LOG(mLog, DEBUG) << "Remove surface " << id;

	// No need to remove if it is removed in system?
#if 0
	auto ret = ilm_layerRemoveSurface(mID, id);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't remove surface from layer: " + to_string(mID),
						  ret);
	}
#endif
}

void Layer::onUpdate(const std::vector<t_ilm_uint>& ids)
{
	LOG(mLog, DEBUG) << "Set render order: " << ids.size();

	auto ret = ilm_layerSetRenderOrder(mID,
									   const_cast<t_ilm_layer*>(ids.data()),
									   ids.size());

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer " + to_string(mID) + "render order",
						  ret);
	}
}
