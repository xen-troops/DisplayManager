/*
 * ObjectManager.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: al1
 */

#ifndef SRC_EVENTHANDLER_HPP_
#define SRC_EVENTHANDLER_HPP_

#include <ilm/ilm_control.h>
#include <ilm/ilm_common.h>

#include <xen/be/Log.hpp>

#include "Config.hpp"
#include "ObjectManager.hpp"

class EventHandler
{
public:
	EventHandler(ObjectManager& objects, ConfigPtr config);
	~EventHandler();

private:
	ObjectManager& mObjects;
	ConfigPtr mConfig;

	XenBackend::Log mLog;

	static void sObjectNotification(ilmObjectType object, t_ilm_uint id,
									t_ilm_bool created, void* data);

	void objectNotification(ilmObjectType object, t_ilm_uint id,
							t_ilm_bool created);

	void createLayer(t_ilm_layer id);
	void deleteLayer(t_ilm_layer id);

	void createSurface(t_ilm_surface id);
	void deleteSurface(t_ilm_surface id);
};

#endif /* SRC_EVENTHANDLER_HPP_ */
