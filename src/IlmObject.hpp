/*
 *  IlmObject
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2017 EPAM Systems Inc.
 */

#ifndef SRC_ILMOBJECT_HPP_
#define SRC_ILMOBJECT_HPP_

#include <ilm/ilm_types.h>

#include <list>
#include <memory>
#include <string>
#include <xt/Log.hpp>

struct IlmRectangle {
    t_ilm_uint x;
    t_ilm_uint y;
    t_ilm_uint width;
    t_ilm_uint height;

    bool operator==(const IlmRectangle& rect)
    {
        return rect.x == x && rect.y == y && rect.width == width &&
               rect.height == height;
    }

    bool operator!=(const IlmRectangle& rect)
    {
        return rect.x != x || rect.y != y || rect.width != width ||
               rect.height != height;
    }
};

class IlmObject;
class ObjectManager;

typedef std::shared_ptr<IlmObject> IlmObjectPtr;

class IlmObject {
public:
    IlmObject(ObjectManager& manager, const std::string& type,
              const std::string& name, t_ilm_uint id);
    ~IlmObject();

    const std::string& getName() const { return mName; }
    t_ilm_uint getID() const { return mID; }

    virtual void setVisibility(t_ilm_bool visibility) = 0;
    virtual void setOpacity(t_ilm_float opacity) = 0;
    virtual void setSource(const IlmRectangle& rect) = 0;
    virtual void setDestination(const IlmRectangle& rect) = 0;

    void setOrder(int order);
    void setParent(IlmObjectPtr parent);
    void update();

protected:
    ObjectManager& mManager;
    std::string mName;
    t_ilm_uint mID;
    IlmObjectPtr mParent;
    int mOrder;
    xt::Log mLog;

private:
    struct Child {
        t_ilm_uint id;
        int order;
    };

    std::list<Child> mChildren;

    void addChild(t_ilm_uint id, int order);
    void removeChild(t_ilm_uint id);
    void addChildToList(t_ilm_uint id, int order);
    void removeChildFromList(t_ilm_uint id);

    virtual void onAddChild(t_ilm_uint id) = 0;
    virtual void onRemoveChild(t_ilm_uint id) = 0;
    virtual void onUpdate(const std::vector<t_ilm_uint>& ids) = 0;
};

#endif /* SRC_ILMOBJECT_HPP_ */
