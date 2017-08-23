/*
 * Layer.hpp
 *
 *  Created on: Aug 22, 2017
 *      Author: al1
 */

#ifndef SRC_LAYER_HPP_
#define SRC_LAYER_HPP_

#include <memory>

#include <ilm/ilm_types.h>

#include <xen/be/Log.hpp>

#include "IlmObject.hpp"

class Layer : public IlmObject<t_ilm_layer>
{
public:
	Layer(const std::string& name, t_ilm_layer id, int width, int height);
	~Layer();

private:
	XenBackend::Log mLog;
};

typedef std::shared_ptr<Layer> LayerPtr;

#endif /* SRC_LAYER_HPP_ */
