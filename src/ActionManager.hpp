/*
 * ActionManager.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: al1
 */

#ifndef SRC_ACTIONMANAGER_HPP_
#define SRC_ACTIONMANAGER_HPP_

#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
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
	DESTROY,
	USER
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
	Action(const std::string& name) : mName(name) {}
	virtual ~Action() {}

	std::string getName() const { return mName; }
	virtual void perform() = 0;

private:
	std::string mName;
};

class ActionObject : public Action
{
public:
	ActionObject(const std::string& actionName, ObjectManager& objects,
				 ObjectType type, const std::string& objectName) :
		Action(actionName),
		mObjects(objects),
		mType(type),
		mObjectName(objectName) {}

protected:
	IlmObjectPtr getObject()
	{
		IlmObjectPtr object;

		if (mType == ObjectType::LAYER)
		{
			object = mObjects.getLayerByName(mObjectName);
		}
		else if (mType == ObjectType::SURFACE)
		{
			object = mObjects.getSurfaceByName(mObjectName);
		}

		if (!object)
		{
			throw DmException("Object " + mObjectName + " doesn't exist");
		}

		return object;
	}

protected:
	ObjectManager& mObjects;
	ObjectType mType;
	std::string mObjectName;
};

class ActionSource : public ActionObject
{
public:
	ActionSource(ObjectManager& objects, ObjectType type,
				 const std::string& name, const IlmRectangle& source) :
		ActionObject("SOURCE", objects, type, name),
		mSource(source) {}

	void perform() override
	{
		getObject()->setSource(mSource);
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
		ActionObject("DESTINATION", objects, type, name),
		mDestination(destination) {}

	void perform() override
	{
		getObject()->setDestination(mDestination);
	}

private:
	IlmRectangle mDestination;
};

class ActionParent : public ActionObject
{
public:
	ActionParent(ObjectManager& objects, ObjectType type,
				 const std::string& name, const std::string& parent) :
		ActionObject("PARENT", objects, type, name),
		mParentName(parent) {}

	void perform() override
	{
		IlmObjectPtr object;

		if (mType == ObjectType::LAYER)
		{
			object = mObjects.getDisplayByName(mParentName);
		}
		else if (mType == ObjectType::SURFACE)
		{
			object = mObjects.getLayerByName(mParentName);
		}

		if (!object)
		{
			throw DmException("Parent " + mParentName + " doesn't exist");
		}

		getObject()->setParent(object);
	}

private:
	std::string mParentName;
};

class ActionOrder: public ActionObject
{
public:
	ActionOrder(ObjectManager& objects, ObjectType type,
				const std::string& name, int order) :
		ActionObject("ORDER", objects, type, name),
		mOrder(order) {}

	void perform() override
	{
		getObject()->setOrder(mOrder);
	}

private:
	int mOrder;
};

class ActionVisibility: public ActionObject
{
public:
	ActionVisibility(ObjectManager& objects, ObjectType type,
					 const std::string& name, t_ilm_bool value) :
		ActionObject("VISIBILITY", objects, type, name),
		mValue(value) {}

	void perform() override
	{
		getObject()->setVisibility(mValue);
	}

private:
	t_ilm_bool mValue;
};

class ActionOpacity: public ActionObject
{
public:
	ActionOpacity(ObjectManager& objects, ObjectType type,
				  const std::string& name, t_ilm_float value) :
		ActionObject("OPACITY", objects, type, name),
		mValue(value) {}

	void perform() override
	{
		getObject()->setOpacity(mValue);
	}

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
	Event(EventType event) :
		mEvent(event),
		mLog("Event") {}

	virtual ~Event() {}

	void addAction(ActionPtr action) { mActions.push_back(action); }
	EventType getEventType() const { return mEvent; }
	void doActions()
	{
		for(auto action : mActions)
		{
			LOG(mLog, DEBUG) << "Do action: " << action->getName();

			try
			{
				action->perform();
			}
			catch(const std::exception& e)
			{
				LOG(mLog, ERROR) << e.what();
			}
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

class EventUser : public Event
{
public:
	EventUser(uint32_t id) :
		Event(EventType::USER),
		mID(id) {}
	uint32_t getID() const { return mID; }

private:
	uint32_t mID;
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

	void userEvent(uint32_t id);

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

	typedef std::function<void()> AsyncCall;

	ObjectManager& mObjects;
	ConfigPtr mConfig;
	bool mTerminate;
	XenBackend::Log mLog;

	std::vector<EventPtr> mEvents;

	std::mutex mMutex;
	std::condition_variable mCondVar;
	std::thread mThread;

	std::list<AsyncCall> mAsyncCalls;

	void init();

	void asyncCall(AsyncCall f);
	void run();

	void asyncCreateLayer(t_ilm_layer id);
	void asyncDeleteLayer(t_ilm_layer id);
	void asyncCreateSurface(t_ilm_surface id);
	void asyncDeleteSurface(t_ilm_surface id);
	void asyncUserEvent(uint32_t id);

	EventPtr createEventCreate(int eventIndex);
	EventPtr createEventDestroy(int eventIndex);
	EventPtr createEventUser(int eventIndex);
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
	EventPtr getUserEvent(uint32_t id);

	void onCreateLayer(const std::string &name);
	void onDeleteLayer(const std::string &name);
	void onCreateSurface(const std::string &name);
	void onDeleteSurface(const std::string &name);
	void onUserEvent(uint32_t id);
};

#endif /* SRC_ACTIONMANAGER_HPP_ */
