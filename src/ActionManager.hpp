/*
 * ActionManager.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: al1
 */

#ifndef SRC_ACTIONMANAGER_HPP_
#define SRC_ACTIONMANAGER_HPP_

#include <memory>
#include <vector>

#include <xen/be/Log.hpp>

#include "Config.hpp"
#include "Exception.hpp"
#include "ObjectManager.hpp"

/*******************************************************************************
 * Enums
 ******************************************************************************/

enum class EventType
{
	CREATE,
	DESTROY
};

enum class ObjectType
{
	LAYER,
	SURFACE
};

enum class ActionType
{
	SOURCE,
	DESTINATION,
	PARENT,
	ORDER,
	VISIBILITY,
	OPACITY
};

/*******************************************************************************
 * Action
 ******************************************************************************/

class Action
{
public:
	virtual ~Action() {}
	virtual void perform() = 0;
};

class ActionObject : public Action
{
public:
	ActionObject(ObjectManager& objects, ObjectType type,
				 const std::string& name) :
		mObjects(objects),
		mType(type),
		mName(name) {}

protected:
	IlmObjectPtr getObject()
	{
		IlmObjectPtr object;

		if (mType == ObjectType::LAYER)
		{
			object = mObjects.getLayerByName(mName);
		}
		else if (mType == ObjectType::SURFACE)
		{
			object = mObjects.getSurfaceByName(mName);
		}

		if (!object)
		{
			throw DmException("Object with " + mName + " doesn't exist");
		}

		return object;
	}

private:
	ObjectManager& mObjects;
	ObjectType mType;
	std::string mName;
};

class ActionSource : public ActionObject
{
public:
	ActionSource(ObjectManager& objects, ObjectType type,
				 const std::string& name, const IlmRectangle& source) :
		ActionObject(objects, type, name),
		mSource(source) {}

	void perform() override
	{

	}

private:
	IlmRectangle mSource;
};

class ActionDestination : public ActionObject
{
public:
	ActionDestination(ObjectManager& objects, ObjectType type,
					  const std::string& name,
					  const IlmRectangle& destination) :
		ActionObject(objects, type, name),
		mDestination(destination) {}

	void perform() override {}

private:
	IlmRectangle mDestination;
};

class ActionParent : public ActionObject
{
public:
	ActionParent(ObjectManager& objects, ObjectType type,
				 const std::string& name, const std::string& parent) :
		ActionObject(objects, type, name),
		mParent(parent) {}

	void perform() override {}

private:
	std::string mParent;
};

class ActionOrder: public ActionObject
{
public:
	ActionOrder(ObjectManager& objects, ObjectType type,
				const std::string& name, int order) :
		ActionObject(objects, type, name),
		mOrder(order) {}

	void perform() override {}

private:
	int mOrder;
};

class ActionVisibility: public ActionObject
{
public:
	ActionVisibility(ObjectManager& objects, ObjectType type,
					 const std::string& name, t_ilm_bool value) :
		ActionObject(objects, type, name),
		mValue(value) {}

	void perform() override {}

private:
	t_ilm_bool mValue;
};

class ActionOpacity: public ActionObject
{
public:
	ActionOpacity(ObjectManager& objects, ObjectType type,
				  const std::string& name, t_ilm_float value) :
		ActionObject(objects, type, name),
		mValue(value) {}

	void perform() override {}

private:
	t_ilm_float mValue;
};

typedef std::shared_ptr<Action> ActionPtr;

/*******************************************************************************
 * Event
 ******************************************************************************/

class Event
{
public:
	Event(EventType event) : mEvent(event) {}
	void addAction(ActionPtr action) { mActions.push_back(action); }

private:
	EventType mEvent;
	std::vector<ActionPtr> mActions;
};

class EventCreate : public Event
{
public:
	EventCreate(EventType event, ObjectType object, const std::string& name) :
		Event(event) {}
};

class EventDestroy : public Event
{
public:
	EventDestroy(EventType event, ObjectType object, const std::string& name) :
		Event(event) {}
};

typedef std::shared_ptr<Event> EventPtr;

/*******************************************************************************
 * ActionManager
 ******************************************************************************/

class ActionManager
{
public:
	ActionManager(ObjectManager& objects, ConfigPtr config);
	~ActionManager();

private:

	struct EventsTable
	{
		EventType type;
		const char* name;
	};

	static const std::vector<EventsTable> sEventsTable;

	struct ObjectsTable
	{
		ObjectType type;
		const char* name;
	};

	static const std::vector<ObjectsTable> sObjectsTable;

	struct ActionsTable
	{
		ActionType type;
		const char* name;
	};

	static const std::vector<ActionsTable> sActionsTable;

	ObjectManager& mObjects;
	XenBackend::Log mLog;

	std::vector<EventPtr> mEvents;

	void init(ConfigPtr config);
	EventPtr createEventCreate(int eventIndex, ConfigPtr config);
	EventPtr createEventDestroy(int eventIndex, ConfigPtr config);
	void createEventActions(int eventIndex, EventPtr event, ConfigPtr config);
	ActionPtr createActionSource(int eventIndex, int actionIndex,
								 ConfigPtr config);
	ActionPtr createActionDestination(int eventIndex, int actionIndex,
								 ConfigPtr config);
	ActionPtr createActionParent(int eventIndex, int actionIndex,
								 ConfigPtr config);
	ActionPtr createActionOrder(int eventIndex, int actionIndex,
								ConfigPtr config);
	ActionPtr createActionVisibility(int eventIndex, int actionIndex,
									 ConfigPtr config);
	ActionPtr createActionOpacity(int eventIndex, int actionIndex,
								  ConfigPtr config);
	template <typename T, typename S>
	T getType(const std::vector<S>& table, const std::string& name);
};

#endif /* SRC_ACTIONMANAGER_HPP_ */
