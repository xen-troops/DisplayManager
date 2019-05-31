/*
 * DisplayManager.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "DisplayManager.hpp"

#include "Exception.hpp"

using std::exception;
using std::find;
using std::to_string;
using std::vector;

/*******************************************************************************
 * DisplayManager
 ******************************************************************************/

DisplayManager::DisplayManager(ConfigPtr config) :
	mConfig(config),
	mActions(mObjects, config),
	mServer(mActions),
	mLog("DisplayManager")
{
	LOG(mLog, DEBUG) << "Create";

	auto ret = ilm_init();

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't initialize ilm", ret);
	}

	showDisplaysInfo();
	createDisplays();
	createLayers();

	mEvents.reset(new EventHandler(mObjects, mActions));
}

DisplayManager::~DisplayManager()
{
	LOG(mLog, DEBUG) << "Delete";

	for(auto id : mLayersId)
	{
		ilm_layerRemove(id);
	}
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
		auto ret = ilm_getScreenIDs(&numOfIDs, &screenIDs);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't get screen IDs", ret);
		}

		LOG(mLog, DEBUG) << "Displays count: " << numOfIDs;

		for(t_ilm_uint i = 0; i < numOfIDs; i++)
		{
			ilmScreenProperties props;

			ret = ilm_getPropertiesOfScreen(screenIDs[i], &props);

			if (ret != ILM_SUCCESS)
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

		mObjects.createDisplay(config);
	}
}

void DisplayManager::createLayers()
{
	for(int i = 0; i < mConfig->getLayersCount(); i++)
	{
		LayerConfig config;

		mConfig->getLayerConfig(i, config);

		if (config.create)
		{
			createLayer(config.id, config.width, config.height);
		}
	}
}

void DisplayManager::createLayer(t_ilm_layer id, t_ilm_uint width, t_ilm_uint height)
{
	auto requestedID = id;

	auto ret = ilm_layerCreateWithDimension(&id, width, height);

	if (ret != ILM_SUCCESS)
	{
		LOG(mLog, WARNING) << "Create layer failed. Trying to remove.";

		ret = ilm_layerRemove(requestedID);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Failed to remove layer: " + to_string(requestedID), ret);
		}

		ret = ilm_layerCreateWithDimension(&id, width, height);

		if (ret != ILM_SUCCESS)
		{
			throw DmException("Can't create layer: " + to_string(requestedID), ret);
		}
	}

	if (requestedID != id)
	{
		ilm_layerRemove(id);

		throw DmException("Can't set requested layer ID: " +
						to_string(requestedID), ret);
	}

	mLayersId.push_back(id);
}
