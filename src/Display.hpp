/*
 *  Display
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

#ifndef SRC_DISPLAY_HPP_
#define SRC_DISPLAY_HPP_

#include <ilm/ilm_types.h>

#include <memory>

#include "IlmObject.hpp"

class Display : public IlmObject {
public:
    explicit Display(ObjectManager& manager, const std::string& name,
                     t_ilm_display id);
    ~Display();

    void setVisibility(t_ilm_bool visibility) override;
    void setOpacity(t_ilm_float opacity) override;
    void setSource(const IlmRectangle& rect) override;
    void setDestination(const IlmRectangle& rect) override;

private:
    t_ilm_uint mWidth;
    t_ilm_uint mHeight;

    void onAddChild(t_ilm_uint id) override;
    void onRemoveChild(t_ilm_uint id) override;
    void onUpdate(const std::vector<t_ilm_uint>& ids) override;
};

typedef std::shared_ptr<Display> DisplayPtr;

#endif /* SRC_DISPLAY_HPP_ */
