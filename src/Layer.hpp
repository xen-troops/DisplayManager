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
	Layer(ObjectManager& manager, const std::string& name, t_ilm_layer id,
		  t_ilm_uint width, t_ilm_uint height);
	virtual ~Layer();

	void setVisibility(t_ilm_bool visibility) override;
	void setOpacity(t_ilm_float opacity) override;
	void setSource(const IlmRectangle& rect) override;
	void setDestination(const IlmRectangle& rect) override;

private:
	friend class EventHandler;

	void layerNotification(ilmLayerProperties* properties,
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

typedef std::shared_ptr<Layer> LayerPtr;

#endif /* SRC_LAYER_HPP_ */
