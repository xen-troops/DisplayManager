/*
 * IlmObject.cpp
 *
 *  Created on: Aug 28, 2017
 *      Author: al1
 */

#include <algorithm>
#include <vector>

#include <ilm/ilm_control.h>

#include "Exception.hpp"
#include "IlmObject.hpp"

using std::find;
using std::find_if;
using std::list;
using std::string;
using std::to_string;
using std::vector;

/*******************************************************************************
 * IlmObject
 ******************************************************************************/

IlmObject::IlmObject(const string& type, const string& name, t_ilm_uint id) :
	mName(name),
	mID(id),
	mOrder(0),
	mLog(type + "[" + to_string(id) + "]")
{
	LOG(mLog, DEBUG) << "Create";
}

IlmObject::~IlmObject()
{
	LOG(mLog, DEBUG) << "Delete";

	if (mParent)
	{
		mParent->removeChild(mID);
		addToUpdateList(mParent);
	}

	mParent.reset();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void IlmObject::setOrder(int order)
{
	LOG(mLog, DEBUG) << "Set order " << order;

	if (mParent)
	{
		mParent->removeChildFromList(mID);
		mParent->addChildToList(mID, order);

		addToUpdateList(mParent);
	}

	mOrder = order;
}

void IlmObject::setParent(IlmObjectPtr parent)
{
	if (parent)
	{
		LOG(mLog, DEBUG) << "Set parent " << parent->getName();
	}
	else
	{
		LOG(mLog, DEBUG) << "Remove parent";
	}

	if (mParent)
	{
		mParent->removeChild(mID);

		addToUpdateList(mParent);
	}

	mParent = parent;

	if (mParent)
	{
		mParent->addChild(mID, mOrder);

		addToUpdateList(mParent);
	}
}

void IlmObject::update()
{
	LOG(mLog, DEBUG) << "Update";

	vector<t_ilm_uint> ids;

	for(auto child : mChildren)
	{
		ids.push_back(child.id);
	}

	onUpdate(ids);
}

void IlmObject::updateAll()
{
	while(sUpdateList.size())
	{
		sUpdateList.front()->update();
		sUpdateList.pop_front();
	}

	auto ret = ilm_commitChanges();

	if (ret != ILM_SUCCESS)
	{
		throw DmException("Can't commit changes");
	}
}

/*******************************************************************************
 * Private
 ******************************************************************************/

list<IlmObjectPtr> IlmObject::sUpdateList;

void IlmObject::addToUpdateList(IlmObjectPtr object)
{
	auto it = find(sUpdateList.begin(), sUpdateList.end(), object);

	if (it == sUpdateList.end())
	{
		sUpdateList.push_back(object);
	}
}

void IlmObject::addChild(t_ilm_uint id, int order)
{
	LOG(mLog, DEBUG) << "Add child " << id;

	onAddChild(id);

	addChildToList(id, order);
}

void IlmObject::addChildToList(t_ilm_uint id, int order)
{
	for(auto it = mChildren.begin(); it != mChildren.end(); it++)
	{
		if (order < it->order)
		{
			mChildren.insert(it, {id, order});

			return;
		}
	}

	mChildren.push_back({id, order});
}

void IlmObject::removeChild(t_ilm_uint id)
{
	LOG(mLog, DEBUG) << "Remove child " << id;

	onRemoveChild(id);

	removeChildFromList(id);
}

void IlmObject::removeChildFromList(t_ilm_uint id)
{
	auto it = find_if(mChildren.begin(), mChildren.end(),
					  [id](const Child& child)
					  { return child.id == id; });

	if (it != mChildren.end())
	{
		mChildren.erase(it);
	}
}
