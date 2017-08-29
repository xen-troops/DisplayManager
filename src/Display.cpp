/*
 * Display.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: al1
 */

#include "Display.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Display
 ******************************************************************************/

Display::Display(const string& name, t_ilm_display id) :
	IlmObject("Display", name, id)
{
	auto ret = ilm_getScreenResolution(mID, &mWidth, &mHeight);

	if (ret != ILM_SUCCESS)
	{
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

void Display::setSourceRectangle(const IlmRectangle& rect)
{
	throw DmException("Can't set source rectangle for display " +
					  to_string(mID));
}

void Display::setDestinationRectangle(const IlmRectangle& rect)
{
	throw DmException("Can't set destination rectangle for display " +
					  to_string(mID));
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

void Display::onUpdate(const std::vector<t_ilm_uint>& ids)
{
	LOG(mLog, DEBUG) << "Set render order: " << ids.size();

	auto ret = ilm_displaySetRenderOrder(mID,
										 const_cast<t_ilm_layer*>(ids.data()),
										 ids.size());

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set screen " + to_string(mID) + "render order",
						  ret);
	}
}
