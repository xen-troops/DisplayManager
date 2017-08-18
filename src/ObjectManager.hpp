/*
 * ObjectManager.hpp
 *
 *  Created on: Jul 19, 2017
 *      Author: al1
 */

#ifndef SRC_OBJECTMANAGER_HPP_
#define SRC_OBJECTMANAGER_HPP_

#include <ilm/ilm_control.h>
#include <ilm/ilm_common.h>

#include <xen/be/Log.hpp>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

private:

	static void sObjectNotification(ilmObjectType object, t_ilm_uint id,
									t_ilm_bool created, void* data);

	void objectNotification(ilmObjectType object, t_ilm_uint id,
							t_ilm_bool created);

	XenBackend::Log mLog;
};

#endif /* SRC_OBJECTMANAGER_HPP_ */
