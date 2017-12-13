/*
 * ObjectManager.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "ObjectManager.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::pair;
using std::string;
using std::to_string;
using std::unordered_map;

/*******************************************************************************
 * ObjectManager
 ******************************************************************************/

ObjectManager::ObjectManager() :
	mLog("ObjectManager")
{
	LOG(mLog, DEBUG) << "Create";
}

ObjectManager::~ObjectManager()
{
	LOG(mLog, DEBUG) << "Delete";

	mSurfaces.clear();
	mLayers.clear();
	mDisplays.clear();

	update();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

DisplayPtr ObjectManager::createDisplay(const DisplayConfig& config)
{
	LOG(mLog, DEBUG) << "Create display name: " << config.name
					 << ", id: " << config.id;

	if (getDisplayByName(config.name) || getDisplayByID(config.id))
	{
		throw DmException("Display already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	DisplayPtr display(new Display(*this, config.name, config.id));

	mDisplays[config.name] = display;

	return display;
}

DisplayPtr ObjectManager::getDisplayByName(const string& name) const
{
	return getObjectByName<DisplayPtr>(name, mDisplays);
}

DisplayPtr ObjectManager::getDisplayByID(t_ilm_display id) const
{
	return getObjectByID<DisplayPtr, t_ilm_display>(id, mDisplays);
}

LayerPtr ObjectManager::createLayer(const LayerConfig& config)
{
	LOG(mLog, DEBUG) << "Create layer name: " << config.name
					 << ", id: " << config.id;

	if (getLayerByName(config.name) || getLayerByID(config.id))
	{
		throw DmException("Layer already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	LayerPtr layer(new Layer(*this, config.name, config.id,
							 config.width, config.height));

	layer->setVisibility(config.visibility);
	layer->setOpacity(config.opacity);
	layer->setOrder(config.order);
	layer->setSource(config.source);
	layer->setDestination(config.destination);

	auto display = getDisplayByName(config.display);

	if (!display)
	{
		throw DmException("Can't get display name: " + config.display,
						  ILM_ERROR_RESOURCE_NOT_FOUND);
	}

	layer->setParent(display);

	mLayers[config.name] = layer;

	return layer;
}

LayerPtr ObjectManager::getLayerByName(const string& name) const
{
	return getObjectByName<LayerPtr>(name, mLayers);
}

LayerPtr ObjectManager::getLayerByID(t_ilm_layer id) const
{
	return getObjectByID<LayerPtr, t_ilm_layer>(id, mLayers);
}

SurfacePtr ObjectManager::createSurface(const SurfaceConfig& config)
{
	LOG(mLog, DEBUG) << "Create surface name: " << config.name
					 << ", id: " << config.id;

	if (getSurfaceByName(config.name) || getSurfaceByID(config.id))
	{
		throw DmException("Surface already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	SurfacePtr surface(new Surface(*this, config.name, config.id));

	auto layer = getLayerByName(config.layer);

	if (!layer)
	{
		throw DmException("Can't get layer name: " + config.layer,
						  ILM_ERROR_RESOURCE_NOT_FOUND);
	}

	surface->setParent(layer);

	surface->setVisibility(config.visibility);
	surface->setOpacity(config.opacity);
	surface->setOrder(config.order);
	surface->setSource(config.source);
	surface->setDestination(config.destination);

	mSurfaces[config.name] = surface;

	return surface;
}

SurfacePtr ObjectManager::getSurfaceByName(const string& name) const
{
	return getObjectByName<SurfacePtr>(name, mSurfaces);
}

SurfacePtr ObjectManager::getSurfaceByID(t_ilm_surface id) const
{
	return getObjectByID<SurfacePtr, t_ilm_surface>(id, mSurfaces);
}

void ObjectManager::deleteSurfaceByName(const string& name)
{
	mSurfaces.erase(name);
}

void ObjectManager::addToUpdateList(IlmObjectPtr object)
{
	auto it = find(mUpdateList.begin(), mUpdateList.end(), object);

	if (it == mUpdateList.end())
	{
		mUpdateList.push_back(object);
	}
}

void ObjectManager::update()
{
	while(mUpdateList.size())
	{
		mUpdateList.front()->update();
		mUpdateList.pop_front();
	}

	LOG(mLog, DEBUG) << "Commit changes";

	ilm_commitChanges();
}

/*******************************************************************************
 * Private
 ******************************************************************************/

template<class T>
T ObjectManager::getObjectByName(const string& name,
								 const unordered_map<string, T>& map) const
{
	auto object = map.find(name);

	if (object == map.end())
	{
		return T();
	}

	return object->second;
}

template<class T, typename S>
T ObjectManager::getObjectByID(S id,
							   const unordered_map<std::string, T>& map) const
{
	auto object = find_if(map.begin(), map.end(),
						 [id] (const pair<string, T> &object)
						 { return object.second->getID() == id; });

	if (object == map.end())
	{
		return T();
	}

	return object->second;
}

