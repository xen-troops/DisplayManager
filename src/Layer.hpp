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

#include "IlmObject.hpp"

class Layer : public IlmObject
{
public:
	Layer(const std::string& name, t_ilm_layer id,
		  t_ilm_uint width, t_ilm_uint height);
	~Layer();

	void setVisibility(t_ilm_bool visibility) override;
	void setOpacity(t_ilm_float opacity) override;
	void setSourceRectangle(const IlmRectangle& rect);
	void setDestinationRectangle(const IlmRectangle& rect);

private:
	void onAddChild(t_ilm_uint id) override;
	void onRemoveChild(t_ilm_uint id) override;
	void onUpdate(const std::vector<t_ilm_uint>& ids) override;
};

typedef std::shared_ptr<Layer> LayerPtr;

#endif /* SRC_LAYER_HPP_ */
