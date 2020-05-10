/*
 *  Surface
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

#ifndef SRC_SURFACE_HPP_
#define SRC_SURFACE_HPP_

#include <ilm/ilm_types.h>

#include <memory>

#include "IlmObject.hpp"

class Surface : public IlmObject {
public:
    Surface(ObjectManager& manager, const std::string& name, t_ilm_surface id);
    virtual ~Surface();

    void setVisibility(t_ilm_bool visibility) override;
    void setOpacity(t_ilm_float opacity) override;
    void setSource(const IlmRectangle& rect) override;
    void setDestination(const IlmRectangle& rect) override;

private:
    friend class EventHandler;

    void surfaceNotification(ilmSurfaceProperties* properties,
                             t_ilm_notification_mask mask);

    void onVisibility(t_ilm_bool visibility);
    void onOpacity(t_ilm_float opacity);
    void onSource(const IlmRectangle& rect);
    void onDestination(const IlmRectangle& rect);

    void onAddChild(t_ilm_uint id) override;
    void onRemoveChild(t_ilm_uint id) override;
    void onUpdate(const std::vector<t_ilm_uint>& ids) override;

    void getProperties();

    t_ilm_bool mVisibility;
    t_ilm_float mOpacity;
    IlmRectangle mSource;
    IlmRectangle mDestination;
};

typedef std::shared_ptr<Surface> SurfacePtr;

#endif /* SRC_SURFACE_HPP_ */
