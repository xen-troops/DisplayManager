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

#include "Display.hpp"

#include <ilm/ilm_control.h>

#include <algorithm>

#include "Exception.hpp"
#include "Utils.hpp"

using std::inserter;
using std::set_difference;
using std::string;
using std::to_string;
using std::vector;

/*******************************************************************************
 * Display
 ******************************************************************************/

Display::Display(ObjectManager& manager, const string& name, t_ilm_display id)
    : IlmObject(manager, "Display", name, id)
{
    auto ret = ilm_getScreenResolution(mID, &mWidth, &mHeight);

    if (ret != ILM_SUCCESS) {
        throw DmException("Can't get screen " + to_string(mID) + " resolution",
                          ret);
    }
}

Display::~Display()
{
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Display::setSource(const IlmRectangle& rect)
{
    throw DmException("Can't set source for display " + to_string(mID));
}

void Display::setDestination(const IlmRectangle& rect)
{
    throw DmException("Can't set destination for display " + to_string(mID));
}

void Display::setVisibility(t_ilm_bool visibility)
{
    throw DmException("Can't set visibility for display " + to_string(mID));
}

void Display::setOpacity(t_ilm_float opacity)
{
    throw DmException("Can't set opacity for display " + to_string(mID));
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Display::onAddChild(t_ilm_uint id)
{
    // do nothing
}

void Display::onRemoveChild(t_ilm_uint id)
{
    // do nothing
}

void Display::onUpdate(const vector<t_ilm_uint>& ids)
{
    ilmScreenProperties screenProperties;

    auto ret = ilm_getPropertiesOfScreen(mID, &screenProperties);

    if (ret != ILM_SUCCESS) {
        throw DmException("Can't get display property " + to_string(mID), ret);
    }

    vector<t_ilm_uint> allIds(
        screenProperties.layerIds,
        screenProperties.layerIds + screenProperties.layerCount);

    LOG(mLog, DEBUG) << "Current render order: " << Utils::idsToStr(allIds);

    free(screenProperties.layerIds);

    vector<t_ilm_uint> renderIds;

    set_difference(allIds.begin(), allIds.end(), ids.begin(), ids.end(),
                   inserter(renderIds, renderIds.begin()));

    renderIds.insert(renderIds.end(), ids.begin(), ids.end());

    LOG(mLog, DEBUG) << "Set render order: " << Utils::idsToStr(renderIds);

    ret = ilm_displaySetRenderOrder(
        mID, const_cast<t_ilm_layer*>(renderIds.data()), renderIds.size());

    if (ret != ILM_SUCCESS) {
        throw DmException("Can't set screen " + to_string(mID) + "render order",
                          ret);
    }
}
