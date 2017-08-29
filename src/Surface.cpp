/*
 * Surface.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#include "Surface.hpp"

#include <ilm/ilm_control.h>

#include "Exception.hpp"

using std::string;
using std::to_string;

/*******************************************************************************
 * Surface
 ******************************************************************************/

Surface::Surface(const string& name, t_ilm_surface id) :
	IlmObject("Surface", name, id)
{
}

Surface::~Surface()
{
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Surface::setSourceRectangle(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set source rectangle"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	auto ret = ilm_surfaceSetSourceRectangle(mID, rect.x, rect.y,
											 rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface source rectangle: " +
						  to_string(mID), ret);
	}
}

void Surface::setDestinationRectangle(const IlmRectangle& rect)
{
	LOG(mLog, DEBUG) << "Set destination rectangle"
					 << ", x: " << rect.x << ", y: " << rect.y
					 << ", width: " << rect.width
					 << ", height: " << rect.height;

	auto ret = ilm_surfaceSetDestinationRectangle(mID, rect.x, rect.y,
												  rect.width, rect.height);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface destination rectangle: " +
						  to_string(mID), ret);
	}
}

void Surface::setVisibility(t_ilm_bool visibility)
{
	LOG(mLog, DEBUG) << "Set visibility: " << visibility;

	auto ret = ilm_surfaceSetVisibility(mID, visibility);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface visibility: " +
						  to_string(mID), ret);
	}
}

void Surface::setOpacity(t_ilm_float opacity)
{
	LOG(mLog, DEBUG) << "Set opacity: " << opacity;

	auto ret = ilm_surfaceSetOpacity(mID, opacity);

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't set surface opacity: " + to_string(mID), ret);
	}
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Surface::onAddChild(t_ilm_uint id)
{

}

void Surface::onRemoveChild(t_ilm_uint id)
{

}

void Surface::onUpdate(const std::vector<t_ilm_uint>& ids)
{

}
