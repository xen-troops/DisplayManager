/*
 * DisplayManager.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_DISPLAYMANAGER_HPP_
#define SRC_DISPLAYMANAGER_HPP_

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

	XenBackend::Log mLog;

	EventHandler* mEvents;
	ObjectManager mObjects;

	void showDisplaysInfo();
	void createDisplays();
	void createLayers();
};

#endif /* SRC_DISPLAYMANAGER_HPP_ */
