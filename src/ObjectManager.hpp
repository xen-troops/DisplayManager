/*
 *  ObjectManager
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2017 EPAM Systems Inc.
 */

#ifndef SRC_OBJECTMANAGER_HPP_
#define SRC_OBJECTMANAGER_HPP_

#include <unordered_map>
#include <xt/Log.hpp>

#include "Config.hpp"
#include "Display.hpp"
#include "Layer.hpp"
#include "Surface.hpp"

class ObjectManager {
public:
    ObjectManager();
    ~ObjectManager();

    DisplayPtr createDisplay(const DisplayConfig& config);
    DisplayPtr getDisplayByName(const std::string& name) const;
    DisplayPtr getDisplayByID(t_ilm_display id) const;

    LayerPtr createLayer(const LayerConfig& config);
    LayerPtr getLayerByName(const std::string& name) const;
    LayerPtr getLayerByID(t_ilm_layer id) const;
    void deleteLayerByName(const std::string& name);

    SurfacePtr createSurface(const SurfaceConfig& config);
    SurfacePtr getSurfaceByName(const std::string& name) const;
    SurfacePtr getSurfaceByID(t_ilm_surface id) const;
    void deleteSurfaceByName(const std::string& name);

    void addToUpdateList(IlmObjectPtr object);
    void update();

private:
    xt::Log mLog;

    std::unordered_map<std::string, DisplayPtr> mDisplays;
    std::unordered_map<std::string, LayerPtr> mLayers;
    std::unordered_map<std::string, SurfacePtr> mSurfaces;

    std::list<IlmObjectPtr> mUpdateList;

    template <class T>
    T getObjectByName(const std::string& name,
                      const std::unordered_map<std::string, T>& map) const;
    template <class T, typename S>
    T getObjectByID(S id, const std::unordered_map<std::string, T>& map) const;
};

#endif /* SRC_OBJECTMANAGER_HPP_ */
