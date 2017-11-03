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
		mSource(source),
		mLog("ActionSource") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";
	}

private:
	IlmRectangle mSource;
	XenBackend::Log mLog;
};

class ActionDestination : public ActionObject
{
public:
	ActionDestination(ObjectManager& objects, ObjectType type,
					  const std::string& name,
					  const IlmRectangle& destination) :
		ActionObject(objects, type, name),
		mDestination(destination),
		mLog("ActionDestination") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";

		getObject()->setDestinationRectangle(mDestination);
	}

private:
	IlmRectangle mDestination;
	XenBackend::Log mLog;
};

class ActionParent : public ActionObject
{
public:
	ActionParent(ObjectManager& objects, ObjectType type,
				 const std::string& name, const std::string& parent) :
		ActionObject(objects, type, name),
		mParent(parent),
		mLog("ActionParent") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";
	}

private:
	std::string mParent;
	XenBackend::Log mLog;
};

class ActionOrder: public ActionObject
{
public:
	ActionOrder(ObjectManager& objects, ObjectType type,
				const std::string& name, int order) :
		ActionObject(objects, type, name),
		mOrder(order),
		mLog("ActionOrder") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";
	}

private:
	int mOrder;
	XenBackend::Log mLog;
};

class ActionVisibility: public ActionObject
{
public:
	ActionVisibility(ObjectManager& objects, ObjectType type,
					 const std::string& name, t_ilm_bool value) :
		ActionObject(objects, type, name),
		mValue(value),
		mLog("ActionVisibility") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";
	}

private:
	t_ilm_bool mValue;
	XenBackend::Log mLog;
};

class ActionOpacity: public ActionObject
{
public:
	ActionOpacity(ObjectManager& objects, ObjectType type,
				  const std::string& name, t_ilm_float value) :
		ActionObject(objects, type, name),
		mValue(value),
		mLog("ActionOpacity") {}

	void perform() override
	{
		LOG(mLog, DEBUG) << "Perform";
	}

private:
	t_ilm_float mValue;
	XenBackend::Log mLog;
};

typedef std::shared_ptr<Action> ActionPtr;

/*******************************************************************************
 * Event
 ******************************************************************************/

class Event
{
public:
	Event(EventType event) :
		mEvent(event),
		mLog("Event") {}

	virtual ~Event() {}

	void addAction(ActionPtr action) { mActions.push_back(action); }
	EventType getEventType() const { return mEvent; }
	void doActions()
	{
		LOG(mLog, DEBUG) << "Do actions: " << mActions.size();

		for(auto action : mActions)
		{
			action->perform();
		}
	}

private:
	EventType mEvent;
	std::vector<ActionPtr> mActions;
	XenBackend::Log mLog;
};


class EventObject : public Event
{
public:
	EventObject(EventType event, ObjectType object, const std::string& name) :
		Event(event),
		mObject(object),
		mName(name) {}
	ObjectType getObjectType() const { return mObject; }
	std::string getName() const { return mName; }

private:
	ObjectType mObject;
	std::string mName;
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

	void createLayer(t_ilm_layer id);
	void deleteLayer(t_ilm_layer id);

	void createSurface(t_ilm_surface id);
	void deleteSurface(t_ilm_surface id);

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
	ConfigPtr mConfig;
	XenBackend::Log mLog;

	std::vector<EventPtr> mEvents;

	void init();
	EventPtr createEventCreate(int eventIndex);
	EventPtr createEventDestroy(int eventIndex);
	void createEventActions(int eventIndex, EventPtr event);
	ActionPtr createActionSource(int eventIndex, int actionIndex);
	ActionPtr createActionDestination(int eventIndex, int actionIndex);
	ActionPtr createActionParent(int eventIndex, int actionIndex);
	ActionPtr createActionOrder(int eventIndex, int actionIndex);
	ActionPtr createActionVisibility(int eventIndex, int actionIndex);
	ActionPtr createActionOpacity(int eventIndex, int actionIndex);
	template <typename T, typename S>
	T getType(const std::vector<S>& table, const std::string& name);

	EventPtr getObjectEvent(EventType eventType, ObjectType objectType,
							const std::string& name);

	void onCreateSurface(const std::string& name);
	void onDeleteSurface(const std::string& name);
};

#endif /* SRC_ACTIONMANAGER_HPP_ */
