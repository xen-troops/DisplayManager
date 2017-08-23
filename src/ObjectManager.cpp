/*
 * ObjectManager.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "ObjectManager.hpp"

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
}

/*******************************************************************************
 * Public
 ******************************************************************************/

DisplayPtr ObjectManager::createDisplay(const string& name, t_ilm_display id)
{
	LOG(mLog, DEBUG) << "Create display name: " << name << ", id: " << id;

	if (getDisplayByName(name) || getDisplayByID(id))
	{
		throw DmException("Display already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	DisplayPtr display(new Display(name, id));

	mDisplays[name] = display;

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

LayerPtr ObjectManager::createLayer(const string& name, t_ilm_layer id,
									int width, int height)
{
	LOG(mLog, DEBUG) << "Create layer name: " << name << ", id: " << id;

	if (getLayerByName(name) || getLayerByID(id))
	{
		throw DmException("Layer already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	LayerPtr layer(new Layer(name, id, width, height));

	mLayers[name] = layer;

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

SurfacePtr ObjectManager::createSurface(const string& name, t_ilm_surface id)
{
	LOG(mLog, DEBUG) << "Create surface name: " << name << ", id: " << id;

	if (getSurfaceByName(name) || getSurfaceByID(id))
	{
		throw DmException("Surface already exists",
						  ILM_ERROR_RESOURCE_ALREADY_INUSE);
	}

	SurfacePtr surface(new Surface(name, id));

	mSurfaces[name] = surface;

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

void ObjectManager::deleteSurfaceByID(t_ilm_surface id)
{
	auto surface = getSurfaceByID(id);

	mSurfaces.erase(surface->getName());
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

