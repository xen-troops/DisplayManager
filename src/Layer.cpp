/*
 * Layer.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: al1
 */

#include "Layer.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"
#include "ObjectManager.hpp"
#include "Utils.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Layer
 ******************************************************************************/

Layer::Layer(ObjectManager& manager, const string& name, t_ilm_layer id) :
	IlmObject(manager, "Layer", name, id)
{
	getProperties();
}

Layer::~Layer()
{
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Layer::setVisibility(t_ilm_bool visibility)
{
	LOG(mLog, DEBUG) << "Set visibility: " << visibility;

	if (mVisibility == visibility)
	{
		return;
	}

	auto ret = ilm_layerSetVisibility(mID, visibility);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer visibility: " + to_string(mID), ret);
	}

	mVisibility = visibility;
}

void Layer::setOpacity(t_ilm_float opacity)
{
	LOG(mLog, DEBUG) << "Set opacity: " << opacity;

	if (mOpacity == opacity)
	{
		return;
	}

	auto ret = ilm_layerSetOpacity(mID, opacity);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer opacity: " + to_string(mID), ret);
	}

	mOpacity = opacity;
}

void Layer::setSource(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set source"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	if (mSource == rect)
	{
		return;
	}

	auto ret = ilm_layerSetSourceRectangle(mID, rect.x, rect.y,
										   rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer source: " +
						  to_string(mID), ret);
	}

	mSource = rect;
}

void Layer::setDestination(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set destination"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	if (mDestination == rect)
	{
		return;
	}

	auto ret = ilm_layerSetDestinationRectangle(mID, rect.x, rect.y,
												rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer destination: " +
						  to_string(mID), ret);
	}

	mDestination = rect;
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Layer::layerNotification(ilmLayerProperties* properties,
							  t_ilm_notification_mask mask)
{
	if (mask & ILM_NOTIFICATION_VISIBILITY)
	{
		onVisibility(properties->visibility);
	}

	if (mask & ILM_NOTIFICATION_OPACITY)
	{
		onOpacity(properties->opacity);
	}

	if (mask & ILM_NOTIFICATION_ORIENTATION)
	{
		LOG(mLog, DEBUG) << "Layer orientation changed, id: " << mID;
	}

	if (mask & ILM_NOTIFICATION_SOURCE_RECT)
	{
		onSource( {properties->sourceX, properties->sourceY,
				   properties->sourceWidth, properties->sourceHeight} );
	}

	if (mask & ILM_NOTIFICATION_DEST_RECT)
	{
		onDestination( {properties->destX, properties->destY,
						properties->destWidth, properties->destHeight} );
	}

	if (mask & ILM_NOTIFICATION_CONTENT_AVAILABLE)
	{
		LOG(mLog, DEBUG) << "Layer content available, id: " << mID;
	}

	if (mask & ILM_NOTIFICATION_CONTENT_REMOVED)
	{
		LOG(mLog, DEBUG) << "Layer content removed, id: " << mID;
	}

	if (mask & ILM_NOTIFICATION_CONFIGURED)
	{
		LOG(mLog, DEBUG) << "Layer configured, id: " << mID;
	}
}

void Layer::onVisibility(t_ilm_bool visibility)
{
	if (visibility != mVisibility)
	{
		LOG(mLog, WARNING) << "Visibility was changed outside";

		auto ret = ilm_layerSetVisibility(mID, mVisibility);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set layer visibility: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

void Layer::onOpacity(t_ilm_float opacity)
{
	if (opacity != mOpacity)
	{
		LOG(mLog, WARNING) << "Opacity was changed outside";

		auto ret = ilm_layerSetOpacity(mID, opacity);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set layer opacity: " + to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

void Layer::onSource(const IlmRectangle& rect)
{
	if (mSource != rect)
	{
		LOG(mLog, WARNING) << "Source was changed outside";

		auto ret = ilm_layerSetSourceRectangle(mID, mSource.x, mSource.y,
											   mSource.width, mSource.height);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set layer source: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}

}

void Layer::onDestination(const IlmRectangle& rect)
{
	if (mDestination != rect)
	{
		LOG(mLog, WARNING) << "Destination was changed outside";

		auto ret = ilm_layerSetDestinationRectangle(mID, mDestination.x,
													mDestination.y,
													mDestination.width,
													mDestination.height);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set layer destination: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

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
	LOG(mLog, DEBUG) << "Set render order: " << Utils::idsToStr(ids);

	auto ret = ilm_layerSetRenderOrder(mID,
									   const_cast<t_ilm_layer*>(ids.data()),
									   ids.size());

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set layer " + to_string(mID) + "render order",
						  ret);
	}
}

void Layer::getProperties()
{
	ilmLayerProperties property;

	auto ret = ilm_getPropertiesOfLayer(mID, &property);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't get layer properties: " + to_string(mID), ret);
	}

	mVisibility = property.visibility;
	mOpacity = property.opacity;

	mSource = { property.sourceX, property.sourceY,
				property.sourceWidth, property.sourceHeight };

	mDestination = { property.destX, property.destY,
					 property.destWidth, property.destHeight };

	LOG(mLog, DEBUG) << "Origin opacity: " << property.opacity
					 << ", visibility: " << property.visibility;
}
