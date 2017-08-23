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

#include "IlmObject.hpp"

class Surface : public IlmObject<t_ilm_surface>
{
public:
	Surface(const std::string& name, t_ilm_surface id);
	~Surface();

private:
	XenBackend::Log mLog;
};

typedef std::shared_ptr<Surface> SurfacePtr;

#endif /* SRC_SURFACE_HPP_ */
