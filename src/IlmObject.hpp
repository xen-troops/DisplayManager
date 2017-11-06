/*
 * IlmObject.hpp
 *
 *  Created on: Aug 23, 2017
 *      Author: al1
 */

#ifndef SRC_ILMOBJECT_HPP_
#define SRC_ILMOBJECT_HPP_

#include <list>
#include <memory>
#include <string>

#include <ilm/ilm_types.h>

#include <xen/be/Log.hpp>

struct IlmRectangle
{
	t_ilm_uint x;
	t_ilm_uint y;
	t_ilm_uint width;
	t_ilm_uint height;

	bool operator ==(const IlmRectangle& rect)
	{
		return rect.x == x && rect.y == rect.y &&
			   width == rect.width && height == rect.height;
	}

	bool operator !=(const IlmRectangle& rect)
	{
		return rect.x != x || rect.y != rect.y ||
			   width != rect.width || height != rect.height;
	}
};

class IlmObject;

typedef std::shared_ptr<IlmObject> IlmObjectPtr;

class IlmObject
{
public:

	IlmObject(const std::string& type, const std::string& name, t_ilm_uint id);
	~IlmObject();

	const std::string& getName() const { return mName; }
	t_ilm_uint getID() const { return mID; }

	virtual void setVisibility(t_ilm_bool visibility) = 0;
	virtual void setOpacity(t_ilm_float opacity) = 0;
	virtual void setSourceRectangle(const IlmRectangle& rect) = 0;
	virtual void setDestinationRectangle(const IlmRectangle& rect) = 0;

	void setOrder(int order);
	void setParent(IlmObjectPtr parent);
	void update();
	static void updateAll();

protected:
	std::string mName;
	t_ilm_uint mID;
	IlmObjectPtr mParent;
	int mOrder;
	XenBackend::Log mLog;

private:
	struct Child
	{
		t_ilm_uint id;
		int order;
	};

	std::list<Child> mChildren;
	static std::list<IlmObjectPtr> sUpdateList;

	static void addToUpdateList(IlmObjectPtr object);

	void addChild(t_ilm_uint id, int order);
	void removeChild(t_ilm_uint id);
	void addChildToList(t_ilm_uint id, int order);
	void removeChildFromList(t_ilm_uint id);

	virtual void onAddChild(t_ilm_uint id) = 0;
	virtual void onRemoveChild(t_ilm_uint id) = 0;
	virtual void onUpdate(const std::vector<t_ilm_uint>& ids) = 0;
};



#endif /* SRC_ILMOBJECT_HPP_ */
