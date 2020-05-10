/*
 *  EventHandler
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

#ifndef SRC_EVENTHANDLER_HPP_
#define SRC_EVENTHANDLER_HPP_

#include <ilm/ilm_common.h>
#include <ilm/ilm_control.h>

#include <xt/Log.hpp>

#include "ActionManager.hpp"
#include "Config.hpp"

class EventHandler {
public:
    EventHandler(ObjectManager& objects, ActionManager& actions);
    ~EventHandler();

private:
    ObjectManager& mObjects;
    ActionManager& mActions;

    static EventHandler* mInstance;

    xt::Log mLog;

    static void sObjectNotification(ilmObjectType object, t_ilm_uint id,
                                    t_ilm_bool created, void* data);
    static void sLayerNotification(t_ilm_layer id,
                                   ilmLayerProperties* properties,
                                   t_ilm_notification_mask mask);
    static void sSurfaceNotification(t_ilm_surface id,
                                     ilmSurfaceProperties* properties,
                                     t_ilm_notification_mask mask);

    void objectNotification(ilmObjectType object, t_ilm_uint id,
                            t_ilm_bool created);
    void layerNotification(t_ilm_layer id, ilmLayerProperties* properties,
                           t_ilm_notification_mask mask);
    void surfaceNotification(t_ilm_surface id, ilmSurfaceProperties* properties,
                             t_ilm_notification_mask mask);

    void createLayer(t_ilm_layer id);
    void deleteLayer(t_ilm_layer id);

    void createSurface(t_ilm_surface id);
    void deleteSurface(t_ilm_surface id);
};

#endif /* SRC_EVENTHANDLER_HPP_ */
