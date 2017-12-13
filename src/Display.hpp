/*
 * Display.hpp
 *
 *  Created on: Jul 20, 2017
 *      Author: al1
 */

#ifndef SRC_DISPLAY_HPP_
#define SRC_DISPLAY_HPP_

#include <memory>

#include <ilm/ilm_types.h>

#include "IlmObject.hpp"

class Display : public IlmObject
{
public:
	explicit Display(ObjectManager& manager, const std::string& name,
					 t_ilm_display id);
	~Display();

	void setVisibility(t_ilm_bool visibility) override;
	void setOpacity(t_ilm_float opacity) override;
	void setSource(const IlmRectangle& rect) override;
	void setDestination(const IlmRectangle& rect) override;

private:
	t_ilm_uint mWidth;
	t_ilm_uint mHeight;

	void onAddChild(t_ilm_uint id) override;
	void onRemoveChild(t_ilm_uint id) override;
	void onUpdate(const std::vector<t_ilm_uint>& ids) override;
};

typedef std::shared_ptr<Display> DisplayPtr;

#endif /* SRC_DISPLAY_HPP_ */
