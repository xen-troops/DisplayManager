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
using std::to_string;


EventHandler* EventHandler::mInstance = nullptr;

/*******************************************************************************
 * EventHandler
 ******************************************************************************/

EventHandler::EventHandler(ObjectManager& objects, ActionManager& actions) :
	mObjects(objects),
	mActions(actions),
	mLog("EventHandler")
{
	LOG(mLog, DEBUG) << "Create";

	if (mInstance)
	{
		throw DmException("Only one instance of event handler is allowed");
	}

	mInstance = this;

	auto ret = ilm_registerNotification(sObjectNotification, nullptr);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't register notification", ret);
	}
}

EventHandler::~EventHandler()
{
	ilm_unregisterNotification();

	mInstance = nullptr;

	LOG(mLog, DEBUG) << "Delete";
}

void EventHandler::sObjectNotification(ilmObjectType object, t_ilm_uint id,
										t_ilm_bool created, void* data)
{
	mInstance->objectNotification(object, id, created);
}

void EventHandler::sLayerNotification(t_ilm_layer id,
									  ilmLayerProperties* properties,
									  t_ilm_notification_mask mask)
{
	mInstance->layerNotification(id, properties, mask);
}

void EventHandler::sSurfaceNotification(t_ilm_surface id,
										ilmSurfaceProperties* properties,
										t_ilm_notification_mask mask)
{
	mInstance->surfaceNotification(id, properties, mask);
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
				auto ret = ilm_surfaceAddNotification(id, sSurfaceNotification);

				if (ret != ILM_SUCCESS)
				{
					throw DmException("Can't set surface notification: " +
									  to_string(id), ret);
				}

				mActions.createSurface(id);
			}
			else
			{
				mActions.deleteSurface(id);
			}
		}

		if (object == ILM_LAYER)
		{
			if (created)
			{
				auto ret = ilm_layerAddNotification(id, sLayerNotification);

				if (ret != ILM_SUCCESS)
				{
					throw DmException("Can't set surface notification: " +
									  to_string(id), ret);
				}

				mActions.createLayer(id);
			}
			else
			{
				mActions.deleteLayer(id);
			}
		}
	}
	catch(const exception& e)
	{
		LOG(mLog, ERROR) << e.what();
	}
}

void EventHandler::layerNotification(t_ilm_layer id,
									 ilmLayerProperties* properties,
									 t_ilm_notification_mask mask)
{
	try
	{
		auto layer = mObjects.getLayerByID(id);

		if (layer)
		{
			layer->layerNotification(properties, mask);
		}
	}
	catch(const exception& e)
	{
		LOG(mLog, ERROR) << e.what();
	}
}

void EventHandler::surfaceNotification(t_ilm_surface id,
									   ilmSurfaceProperties* properties,
									   t_ilm_notification_mask mask)
{
	try
	{
		auto surface = mObjects.getSurfaceByID(id);

		if (surface)
		{
			surface->surfaceNotification(properties, mask);
		}
	}
	catch(const exception& e)
	{
		LOG(mLog, ERROR) << e.what();
	}
}
