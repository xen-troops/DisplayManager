/*
 * ObjectManager.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: al1
 */

#include "EventHandler.hpp"

#include <string>

#include "Exception.hpp"

using std::exception;

/*******************************************************************************
 * ObjectManager
 ******************************************************************************/

EventHandler::EventHandler(ObjectManager& objects, ConfigPtr config) :
	mObjects(objects),
	mConfig(config),
	mLog("EventHandler")
{
	LOG(mLog, DEBUG) << "Create";

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_registerNotification(sObjectNotification, this)) !=
		ILM_SUCCESS)
	{
		throw DmException("Can't register notification, error", ret);
	}
}

EventHandler::~EventHandler()
{
	ilm_unregisterNotification();

	LOG(mLog, DEBUG) << "Delete";
}

void EventHandler::sObjectNotification(ilmObjectType object, t_ilm_uint id,
										t_ilm_bool created, void* data)
{
	static_cast<EventHandler*>(data)->objectNotification(object, id, created);
}

void EventHandler::objectNotification(ilmObjectType object, t_ilm_uint id,
									   t_ilm_bool created)
{
	try
	{
		if (object == ILM_SURFACE)
		{
			if (created)
			{
				createSurface(id);
			}
			else
			{
				mObjects.deleteSurfaceByID(id);
			}
		}

		if (object == ILM_LAYER)
		{
			if (created)
			{
				createLayer(id);
			}
			else
			{
				deleteLayer(id);
			}
		}
	}
	catch(const exception& e)
	{
		LOG(mLog, ERROR) << e.what();
	}
}

void EventHandler::createLayer(t_ilm_layer id)
{
	if (mObjects.getLayerByID(id))
	{
		LOG(mLog, DEBUG) << "Create layer, id: " << id;

		return;
	}

	LOG(mLog, WARNING) << "Unhandled layer " << id << " created";
}

void EventHandler::deleteLayer(t_ilm_layer id)
{
	if (mObjects.getLayerByID(id))
	{
		LOG(mLog, DEBUG) << "Deletes layer, id: " << id;

		return;
	}

	LOG(mLog, WARNING) << "Unhandled layer " << id << " deleted";
}

void EventHandler::createSurface(t_ilm_surface id)
{
	for(int i = 0; i < mConfig->getSurfacesCount(); i++)
	{
		SurfaceConfig config;

		mConfig->getSurfaceConfig(i, config);

		if (id == config.id)
		{
			LOG(mLog, DEBUG) << "Create surface, id: " << id;

			mObjects.createSurface(config.name, config.id);

			return;
		}
	}

	LOG(mLog, WARNING) << "Unhandled surface " << id << " created";
}

void EventHandler::deleteSurface(t_ilm_surface id)
{
	if (mObjects.getSurfaceByID(id))
	{
		LOG(mLog, DEBUG) << "Delete surface, id: " << id;

		mObjects.deleteSurfaceByID(id);

		return;
	}

	LOG(mLog, WARNING) << "Unhandled surface " << id << " deleted";
}
