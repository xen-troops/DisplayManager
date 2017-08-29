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

#include "IlmObject.hpp"

class Surface : public IlmObject
{
public:
	Surface(const std::string& name, t_ilm_surface id);
	~Surface();

	void setVisibility(t_ilm_bool visibility) override;
	void setOpacity(t_ilm_float opacity) override;
	void setSourceRectangle(const IlmRectangle& rect) override;
	void setDestinationRectangle(const IlmRectangle& rect) override;

private:
	void onAddChild(t_ilm_uint id) override;
	void onRemoveChild(t_ilm_uint id) override;
	void onUpdate(const std::vector<t_ilm_uint>& ids) override;
};

typedef std::shared_ptr<Surface> SurfacePtr;

#endif /* SRC_SURFACE_HPP_ */
