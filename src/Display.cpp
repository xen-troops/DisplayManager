/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: al1
 */

#include "Display.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::exception;
using std::find_if;
using std::pair;
using std::string;
using std::to_string;

/*******************************************************************************
 * DisplayManager
 ******************************************************************************/

DisplayManager::DisplayManager(ConfigPtr config) :
	mConfig(config),
	mLog("DisplayManager")
{
	LOG(mLog, DEBUG) << "Create";

	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_init()) != ILM_SUCCESS)
	{
		throw DmException("Can't initialize ilm", ret);
	}

	showDisplaysInfo();
	createDisplays();
	createLayers();

	mObjectManager.reset(new ObjectManager());
}

DisplayManager::~DisplayManager()
{
	mDisplays.clear();

	mObjectManager.reset();

	ilm_destroy();

	LOG(mLog, DEBUG) << "Delete";
}

DisplayPtr DisplayManager::getDisplayByName(const string& name) const
{
	auto display = mDisplays.find(name);

	if (display == mDisplays.end())
	{
		return DisplayPtr();
	}

	return display->second;
}

DisplayPtr DisplayManager::getDisplayByID(t_ilm_display id) const
{
	auto display = find_if(mDisplays.begin(), mDisplays.end(),
						 [id] (const pair<string, DisplayPtr> &display)
						 { return display.second->getID() == id; });

	if (display == mDisplays.end())
	{
		return DisplayPtr();
	}

	return display->second;
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void DisplayManager::showDisplaysInfo()
{
	t_ilm_uint numOfIDs = 0;
	t_ilm_uint* screenIDs = nullptr;

	try
	{
		ilmErrorTypes ret = ILM_SUCCESS;

		if ((ret = ilm_getScreenIDs(&numOfIDs, &screenIDs)) != ILM_SUCCESS)
		{
			throw DmException("Can't get screen IDs", ret);
		}

		LOG(mLog, DEBUG) << "Displays count: " << numOfIDs;

		for(t_ilm_uint i = 0; i < numOfIDs; i++)
		{
			ilmScreenProperties props;

			if ((ret = ilm_getPropertiesOfScreen(screenIDs[i], &props)) !=
				ILM_SUCCESS)
			{
				throw DmException("Can't get screen " +
								  to_string(screenIDs[i]) + " properties",
								  ret);
			}

			LOG(mLog, DEBUG) << "Display id: " << screenIDs[i]
							 << ", connector: " << props.connectorName
							 << ", width: " << props.screenWidth
							 << ", height: " << props.screenHeight;
		}

		free(screenIDs);
	}
	catch(const exception &e)
	{
		free(screenIDs);

		throw;
	}
}

void DisplayManager::createDisplays()
{
	for(int i = 0; i < mConfig->getDisplaysCount(); i++)
	{
		DisplayConfig config;

		mConfig->getDisplayConfig(i, config);

		LOG(mLog, DEBUG) << "Create display name: " << config.name
						 << ", id: " << config.displayID;

		if (getDisplayByName(config.name) || getDisplayByID(config.displayID))
		{
			throw DmException("Display " + to_string(config.displayID) +
							  " already exists",
							  ILM_ERROR_RESOURCE_ALREADY_INUSE);
		}

		DisplayPtr display(new Display(config.name, config.displayID));

		mDisplays[config.name] = display;
	}
}

void DisplayManager::createLayers()
{
	for(int i = 0; i < mConfig->getLayersCount(); i++)
	{
		LayerConfig config;

		mConfig->getLayerConfig(i, config);

		auto display = getDisplayByName(config.displayName);

		if (!display)
		{
			throw DmException("Can't find display name: " + config.displayName,
							  ILM_ERROR_RESOURCE_NOT_FOUND);
		}

		display->createLayer(config.name, config.layerID, config.width,
							 config.height, config.zOrder);
	}
}

/*******************************************************************************
 * Display
 ******************************************************************************/

Display::Display(const string& name, t_ilm_display id) :
	mName(name),
	mID(id),
	mLog("Display" + to_string(id))
{
	ilmErrorTypes ret = ILM_SUCCESS;

	if ((ret = ilm_getScreenResolution(mID, &mWidth, &mHeight)) != ILM_SUCCESS)
	{
		throw DmException("Can't get screen " + to_string(id) + ", resolution",
						  ret);
	}

	LOG(mLog, DEBUG) << "Create display id: " << mID
					 << ", width: " << mWidth
					 << ", height: " << mHeight;
}

Display::~Display()
{
	LOG(mLog, DEBUG) << "Delete display id: " << mID;
}

/*******************************************************************************
 * Public
 ******************************************************************************/

LayerPtr Display::createLayer(const string& name, t_ilm_layer id,
							  int width, int height, int zOrder)
{
	LOG(mLog, DEBUG) << "Create layer: " << name;

	if (getLayerByName(name) || getLayerByID(id))
	{
		throw DmException("Layer " + to_string(id) + " already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	LayerPtr layer(new Layer(name, id, width, height, zOrder));

	mLayers[name] = layer;

	return layer;
}

LayerPtr Display::getLayerByName(const string& name) const
{
	auto layer = mLayers.find(name);

	if (layer == mLayers.end())
	{
		return LayerPtr();
	}

	return layer->second;
}

LayerPtr Display::getLayerByID(t_ilm_layer id) const
{
	auto layer = find_if(mLayers.begin(), mLayers.end(),
						 [id] (const pair<string, LayerPtr> &layer)
						 { return layer.second->getID() == id; });

	if (layer == mLayers.end())
	{
		return LayerPtr();
	}

	return layer->second;
}
