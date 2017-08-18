/*
 * ObjectManager.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: al1
 */

#include <string>

#include "ObjectManager.hpp"
#include "Exception.hpp"

/*******************************************************************************
 * ObjectManager
 ******************************************************************************/

ObjectManager::ObjectManager() :
	mLog("ObjectManager")
{
	LOG(mLog, DEBUG) << "Create";

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_registerNotification(sObjectNotification, this)) !=
		ILM_SUCCESS)
	{
		throw DmException("Can't register notification, error", ret);
	}
}

ObjectManager::~ObjectManager()
{
	ilm_unregisterNotification();

	LOG(mLog, DEBUG) << "Delete";
}

void ObjectManager::sObjectNotification(ilmObjectType object, t_ilm_uint id,
										t_ilm_bool created, void* data)
{
	static_cast<ObjectManager*>(data)->objectNotification(object, id, created);
}

void ObjectManager::objectNotification(ilmObjectType object, t_ilm_uint id,
									   t_ilm_bool created)
{
	if (object == ILM_SURFACE)
	{
		LOG(mLog, DEBUG) << "Surface, id: " << id << ", created: " << created;
	}

	if (object == ILM_LAYER)
	{
		LOG(mLog, DEBUG) << "Layer, id: " << id << ", created: " << created;
	}
}

