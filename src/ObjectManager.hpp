/*
 * ObjectManager.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_OBJECTMANAGER_HPP_
#define SRC_OBJECTMANAGER_HPP_

#include <unordered_map>

#include <xen/be/Log.hpp>

#include "Display.hpp"
#include "Layer.hpp"
#include "Surface.hpp"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	DisplayPtr createDisplay(const std::string& name, t_ilm_display id);
	DisplayPtr getDisplayByName(const std::string& name) const;
	DisplayPtr getDisplayByID(t_ilm_display id) const;

	LayerPtr createLayer(const std::string& name, t_ilm_layer id,
						 int width, int height);
	LayerPtr getLayerByName(const std::string& name) const;
	LayerPtr getLayerByID(t_ilm_layer id) const;

	SurfacePtr createSurface(const std::string& name, t_ilm_surface id);
	SurfacePtr getSurfaceByName(const std::string& name) const;
	SurfacePtr getSurfaceByID(t_ilm_surface id) const;
	void deleteSurfaceByID(t_ilm_surface id);

private:
	XenBackend::Log mLog;

	std::unordered_map<std::string, DisplayPtr> mDisplays;
	std::unordered_map<std::string, LayerPtr> mLayers;
	std::unordered_map<std::string, SurfacePtr> mSurfaces;

	template<class T>
	T getObjectByName(const std::string& name,
					  const std::unordered_map<std::string, T>& map) const;
	template<class T, typename S>
	T getObjectByID(S id,
					const std::unordered_map<std::string, T>& map) const;
};



#endif /* SRC_OBJECTMANAGER_HPP_ */
