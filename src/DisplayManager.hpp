/*
 *  DisplayManager
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

#ifndef SRC_DISPLAYMANAGER_HPP_
#define SRC_DISPLAYMANAGER_HPP_

#include "ActionManager.hpp"
#include "Config.hpp"
#include "DBusServer.hpp"
#include "EventHandler.hpp"
#include "ObjectManager.hpp"
#include <ilm/ilm_types.h>

class DisplayManager {
public:
    DisplayManager(ConfigPtr config, bool systemBus);
    ~DisplayManager();

    void run();

private:
    ConfigPtr mConfig;
    ObjectManager mObjects;
    ActionManager mActions;
    DBusServer mServer;
    std::unique_ptr<EventHandler> mEvents;

    std::vector<t_ilm_layer> mLayersId;

    xt::Log mLog;

    std::map<t_ilm_uint, ilmScreenProperties*>&& getIlmScreenInfoInfo();
    void createDisplays(const std::map<t_ilm_uint, ilmScreenProperties*>& availableScreens);
    void createLayers();
    void createLayer(t_ilm_layer id, t_ilm_uint width, t_ilm_uint height);
};

#endif /* SRC_DISPLAYMANAGER_HPP_ */
