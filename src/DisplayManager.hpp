/*
 * DisplayManager.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_DISPLAYMANAGER_HPP_
#define SRC_DISPLAYMANAGER_HPP_

#include "ActionManager.hpp"
#include "DBusServer.hpp"
#include "Config.hpp"
#include "EventHandler.hpp"
#include "ObjectManager.hpp"

class DisplayManager
{
public:
	DisplayManager(ConfigPtr config);
	~DisplayManager();

private:
	ConfigPtr mConfig;
	ObjectManager mObjects;
	ActionManager mActions;
	DBusServer mServer;
	std::unique_ptr<EventHandler> mEvents;

	XenBackend::Log mLog;

	void showDisplaysInfo();
	void createDisplays();
	void createLayers();
};

#endif /* SRC_DISPLAYMANAGER_HPP_ */
