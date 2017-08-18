/*
 * Display.hpp
 *
 *  Created on: Jul 20, 2017
 *      Author: al1
 */

#ifndef SRC_DISPLAY_HPP_
#define SRC_DISPLAY_HPP_

#include <memory>
#include <unordered_map>

#include <ilm/ilm_types.h>

#include <xen/be/Log.hpp>

#include "Config.hpp"
#include "Layer.hpp"
#include "ObjectManager.hpp"

class Display
{
public:
	explicit Display(const std::string& name, t_ilm_display id);
	~Display();

	t_ilm_display getID() const { return mID; }

	LayerPtr createLayer(const std::string& name, t_ilm_layer id,
						 int width, int height, int zOrder);

	LayerPtr getLayerByName(const std::string& name) const;

	LayerPtr getLayerByID(t_ilm_layer id) const;

private:
	std::string mName;
	t_ilm_display mID;
	t_ilm_uint mWidth;
	t_ilm_uint mHeight;

	XenBackend::Log mLog;

	std::unordered_map<std::string, LayerPtr> mLayers;
};

typedef std::shared_ptr<Display> DisplayPtr;

class DisplayManager
{
public:
	DisplayManager(ConfigPtr config);
	~DisplayManager();

	DisplayPtr getDisplayByName(const std::string& name) const;
	DisplayPtr getDisplayByID(t_ilm_display id) const;

private:
	ConfigPtr mConfig;
	XenBackend::Log mLog;

	std::unique_ptr<ObjectManager> mObjectManager;
	std::unordered_map<std::string, DisplayPtr> mDisplays;

	void showDisplaysInfo();
	void createDisplays();
	void createLayers();
};

#endif /* SRC_DISPLAY_HPP_ */
