/*
 * Surface.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_SURFACE_HPP_
#define SRC_SURFACE_HPP_

#include <memory>

#include <ilm/ilm_types.h>

#include <xen/be/Log.hpp>

class Surface
{
public:
	Surface(const std::string& name, t_ilm_surface id);
	~Surface();

private:
	std::string mName;
	t_ilm_surface mID;

	XenBackend::Log mLog;
};

typedef std::shared_ptr<Surface> SurfacePtr;

#endif /* SRC_SURFACE_HPP_ */
