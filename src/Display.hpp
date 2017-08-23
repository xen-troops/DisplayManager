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

#include <xen/be/Log.hpp>

#include "IlmObject.hpp"

class Display : public IlmObject<t_ilm_display>
{
public:
	explicit Display(const std::string& name, t_ilm_display id);
	~Display();

private:
	t_ilm_uint mWidth;
	t_ilm_uint mHeight;

	XenBackend::Log mLog;
};

typedef std::shared_ptr<Display> DisplayPtr;

#endif /* SRC_DISPLAY_HPP_ */
