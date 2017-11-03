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
 * EventHandler
 ******************************************************************************/

EventHandler::EventHandler(ActionManager& actions) :
	mActions(actions),
	mLog("EventHandler")
{
	LOG(mLog, DEBUG) << "Create";

	auto ret = ilm_registerNotification(sObjectNotification, this);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't register notification", ret);
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

