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

	mObjects.update();

	mEvents = new EventHandler(mObjects, mConfig);
}

DisplayManager::~DisplayManager()
{
	delete mEvents;

	LOG(mLog, DEBUG) << "Delete";
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

		mObjects.createDisplay(config);
	}
}

void DisplayManager::createLayers()
{
	vector<IlmObjectPtr> updateObjects;

	for(int i = 0; i < mConfig->getLayersCount(); i++)
	{
		LayerConfig config;

		mConfig->getLayerConfig(i, config);

		mObjects.createLayer(config);
	}
}
