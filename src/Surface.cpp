/*
 * Surface.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "Surface.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"
#include "ObjectManager.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Surface
 ******************************************************************************/

Surface::Surface(ObjectManager& manager, const string& name, t_ilm_surface id) :
	IlmObject(manager, "Surface", name, id)
{
	getProperties();

	auto ret = ilm_surfaceSetType(id, ILM_SURFACETYPE_DESKTOP);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface type: " + to_string(mID), ret);
	}
}

Surface::~Surface()
{
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Surface::setVisibility(t_ilm_bool visibility)
{
	LOG(mLog, DEBUG) << "Set visibility: " << visibility;

	if (mVisibility == visibility)
	{
		return;
	}

	auto ret = ilm_surfaceSetVisibility(mID, visibility);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface visibility: " +
						  to_string(mID), ret);
	}

	mVisibility = visibility;
}

void Surface::setOpacity(t_ilm_float opacity)
{
	LOG(mLog, DEBUG) << "Set opacity: " << opacity;

	if (mOpacity == opacity)
	{
		return;
	}

	auto ret = ilm_surfaceSetOpacity(mID, opacity);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface opacity: " + to_string(mID), ret);
	}

	mOpacity = opacity;
}

void Surface::setSource(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set source"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	if (mSource == rect)
	{
		return;
	}

	auto ret = ilm_surfaceSetSourceRectangle(mID, rect.x, rect.y,
											 rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface source: " +
						  to_string(mID), ret);
	}

	mSource = rect;
}

void Surface::setDestination(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set destination"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	if (mDestination == rect)
	{
		return;
	}

	auto ret = ilm_surfaceSetDestinationRectangle(mID, rect.x, rect.y,
												  rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface destination: " +
						  to_string(mID), ret);
	}

	mDestination = rect;
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Surface::surfaceNotification(ilmSurfaceProperties* properties,
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
		LOG(mLog, DEBUG) << "Surface orientation changed, id: " << mID;
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
		LOG(mLog, DEBUG) << "Surface content available, id: " << mID;
	}

	if (mask & ILM_NOTIFICATION_CONTENT_REMOVED)
	{
		LOG(mLog, DEBUG) << "Surface content removed, id: " << mID;
	}

	if (mask & ILM_NOTIFICATION_CONFIGURED)
	{
		LOG(mLog, DEBUG) << "Surface configured, id: " << mID
						 << ", w: " << properties->origSourceWidth
						 << ", h: " << properties->origSourceHeight;
	}
}

void Surface::onVisibility(t_ilm_bool visibility)
{
	if (visibility != mVisibility)
	{
		LOG(mLog, WARNING) << "Visibility was changed outside";

		auto ret = ilm_surfaceSetVisibility(mID, mVisibility);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set surface visibility: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

void Surface::onOpacity(t_ilm_float opacity)
{
	if (opacity != mOpacity)
	{
		LOG(mLog, WARNING) << "Opacity was changed outside";

		auto ret = ilm_surfaceSetOpacity(mID, opacity);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set surface opacity: " + to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

void Surface::onSource(const IlmRectangle& rect)
{
	if (mSource != rect)
	{
		LOG(mLog, WARNING) << "Source was changed outside";

		auto ret = ilm_surfaceSetSourceRectangle(mID, mSource.x, mSource.y,
												 mSource.width, mSource.height);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set surface source: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}

}

void Surface::onDestination(const IlmRectangle& rect)
{
	if (mDestination != rect)
	{
		LOG(mLog, WARNING) << "Destination was changed outside";

		auto ret = ilm_surfaceSetDestinationRectangle(mID, mDestination.x,
													  mDestination.y,
													  mDestination.width,
													  mDestination.height);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't set surface destination: " +
							  to_string(mID), ret);
		}

		ilm_commitChanges();
	}
}

void Surface::onAddChild(t_ilm_uint id)
{
	throw DmException("Can't add child to surface " + to_string(mID));
}

void Surface::onRemoveChild(t_ilm_uint id)
{
	throw DmException("Can't remove child from surface " + to_string(mID));
}

void Surface::onUpdate(const std::vector<t_ilm_uint>& ids)
{

}

void Surface::getProperties()
{
	ilmSurfaceProperties property;

	auto ret = ilm_getPropertiesOfSurface(mID, &property);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't get surface properties: " +
						  to_string(mID), ret);
	}

	mVisibility = property.visibility;
	mOpacity = property.opacity;

	mSource = { property.sourceX, property.sourceY,
				property.sourceWidth, property.sourceHeight };

	mDestination = { property.destX, property.destY,
					 property.destWidth, property.destHeight };

	LOG(mLog, DEBUG) << "Origin width: " << property.origSourceWidth
					 << ", height: " << property.origSourceHeight
					 << ", opacity: " << property.opacity
					 << ", visibility: " << property.visibility;
}
