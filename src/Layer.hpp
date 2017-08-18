/*
 * Layer.hpp
 *
 *  Created on: Aug 22, 2017
 *      Author: al1
 */

#ifndef SRC_LAYER_HPP_
#define SRC_LAYER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include <ilm/ilm_types.h>

#include <xen/be/Log.hpp>

#include "Surface.hpp"

class Layer
{
public:
	Layer(const std::string& name, t_ilm_layer id,
		  int width, int height, int zOrder);
	~Layer();

	t_ilm_layer getID() const { return mID; }

private:
	std::string mName;
	int mZOrder;
	t_ilm_layer mID;

	XenBackend::Log mLog;

	std::unordered_map<std::string, SurfacePtr> mSurfaces;
};

typedef std::shared_ptr<Layer> LayerPtr;

#endif /* SRC_LAYER_HPP_ */
