/*
 * ActionManager.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: al1
 */

#include "ActionManager.hpp"

#include <algorithm>

#include "Exception.hpp"

using std::string;
using std::to_string;
using std::toupper;
using std::transform;
using std::vector;

/*******************************************************************************
 * ActionManager
 ******************************************************************************/

ActionManager::ActionManager(ObjectManager& objects, ConfigPtr config) :
	mObjects(objects),
	mConfig(config),
	mLog("ActionManager")
{
	LOG(mLog, DEBUG) << "Create";

	init();
}

ActionManager::~ActionManager()
{
	LOG(mLog, DEBUG) << "Delete";
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void ActionManager::createLayer(t_ilm_layer id)
{
	if (mObjects.getLayerByID(id))
	{
		LOG(mLog, DEBUG) << "Create layer, id: " << id;

		return;
	}

	LOG(mLog, WARNING) << "Unhandled layer " << id << " created";
}

void ActionManager::deleteLayer(t_ilm_layer id)
{
	if (mObjects.getLayerByID(id))
	{
		LOG(mLog, DEBUG) << "Deletes layer, id: " << id;

		return;
	}

	LOG(mLog, WARNING) << "Unhandled layer " << id << " deleted";
}

void ActionManager::createSurface(t_ilm_surface id)
{
	for(int i = 0; i < mConfig->getSurfacesCount(); i++)
	{
		SurfaceConfig config;

		mConfig->getSurfaceConfig(i, config);

		if (id == config.id)
		{
			LOG(mLog, DEBUG) << "Create surface, id: " << id;

			auto surface = mObjects.createSurface(config);

			onCreateSurface(surface->getName());

			mObjects.update();

			return;
		}
	}

	LOG(mLog, WARNING) << "Unhandled surface " << id << " created";
}

void ActionManager::deleteSurface(t_ilm_surface id)
{
	if (mObjects.getSurfaceByID(id))
	{
		LOG(mLog, DEBUG) << "Delete surface, id: " << id;

		auto surface = mObjects.getSurfaceByID(id);

		if (!surface)
		{
			throw DmException("Can't find surface " + to_string(id));
		}
		onDeleteSurface(surface->getName());

		mObjects.deleteSurfaceByName(surface->getName());

		mObjects.update();

		return;
	}

	LOG(mLog, WARNING) << "Unhandled surface " << id << " deleted";
}

/*******************************************************************************
 * Private
 ******************************************************************************/

const vector<ActionManager::EventsTable> ActionManager::sEventsTable =
{
	{ EventType::CREATE,  "CREATE" },
	{ EventType::DESTROY, "DESTROY"}
};

const vector<ActionManager::ObjectsTable> ActionManager::sObjectsTable =
{
	{ ObjectType::LAYER,   "LAYER"   },
	{ ObjectType::SURFACE, "SURFACE" }
};

const vector<ActionManager::ActionsTable> ActionManager::sActionsTable =
{
	{ ActionType::SOURCE,      "SOURCE"      },
	{ ActionType::DESTINATION, "DESTINATION" },
	{ ActionType::PARENT,      "PARENT"      },
	{ ActionType::ORDER,       "ORDER"       },
	{ ActionType::VISIBILITY,  "VISIBILITY"  },
	{ ActionType::OPACITY,     "OPACITY"     }
};

void ActionManager::init()
{
	for(int i = 0; i < mConfig->getEventsCount(); i++)
	{
		string name;

		mConfig->getEventName(i, name);

		EventPtr event;

		auto eventType = getType<EventType, EventsTable>(sEventsTable, name);

		switch(eventType)
		{
			case EventType::CREATE:
			{
				event = createEventCreate(i);

				break;
			}
			case EventType::DESTROY:
			{
				event = createEventDestroy(i);

				break;
			}

			default:
			{
				throw DmException("Not implemented event: " + name);

				break;
			}
		}

		createEventActions(i, event);

		mEvents.push_back(event);
	}
}

EventPtr ActionManager::createEventCreate(int eventIndex)
{
	EventCreateConfig createConfig;

	mConfig->getEventCreateConfig(eventIndex, createConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, createConfig.object);

	return EventPtr(new EventObject(EventType::CREATE, objectType,
									createConfig.name));
}

EventPtr ActionManager::createEventDestroy(int eventIndex)
{
	EventDestroyConfig destroyConfig;

	mConfig->getEventDestroyConfig(eventIndex, destroyConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, destroyConfig.object);

	return EventPtr(new EventObject(EventType::DESTROY, objectType,
									destroyConfig.name));
}

void ActionManager::createEventActions(int eventIndex, EventPtr event)
{
	auto count = mConfig->getActionsCount(eventIndex);

	for(int i = 0; i < count; i++)
	{
		string name;

		mConfig->getActionName(eventIndex, i, name);

		ActionPtr action;

		auto actionType = getType<ActionType, ActionsTable>(sActionsTable,
															name);

		switch(actionType)
		{
			case ActionType::SOURCE:
			{
				action = createActionSource(eventIndex, i);

				break;
			}

			case ActionType::DESTINATION:
			{
				action = createActionDestination(eventIndex, i);

				break;
			}

			case ActionType::PARENT:
			{
				action = createActionParent(eventIndex, i);

				break;
			}

			case ActionType::ORDER:
			{
				action = createActionOrder(eventIndex, i);

				break;
			}

			case ActionType::VISIBILITY:
			{
				action = createActionVisibility(eventIndex, i);

				break;
			}

			case ActionType::OPACITY:
			{
				action = createActionOpacity(eventIndex, i);

				break;
			}

			default:
			{
				throw DmException("Not implemented action: " + name);

				break;
			}
		}

		event->addAction(action);
	}
}

ActionPtr ActionManager::createActionSource(int eventIndex, int actionIndex)
{
	ActionSourceConfig sourceConfig;

	mConfig->getActionSourceConfig(eventIndex, actionIndex, sourceConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, sourceConfig.object);

	return ActionPtr(new ActionSource(mObjects, objectType, sourceConfig.name,
									  sourceConfig.source));
}

ActionPtr ActionManager::createActionDestination(int eventIndex,
												 int actionIndex)
{
	ActionDestinationConfig destinationConfig;

	mConfig->getActionDestinationConfig(eventIndex, actionIndex,
										destinationConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, destinationConfig.object);

	return ActionPtr(new ActionSource(mObjects, objectType,
									  destinationConfig.name,
									  destinationConfig.destination));
}

ActionPtr ActionManager::createActionParent(int eventIndex, int actionIndex)
{
	ActionParentConfig parentConfig;

	mConfig->getActionParentConfig(eventIndex, actionIndex, parentConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, parentConfig.object);

	return ActionPtr(new ActionParent(mObjects, objectType, parentConfig.name,
									  parentConfig.parent));
}

ActionPtr ActionManager::createActionOrder(int eventIndex, int actionIndex)
{
	ActionOrderConfig orderConfig;

	mConfig->getActionOrderConfig(eventIndex, actionIndex, orderConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, orderConfig.object);

	return ActionPtr(new ActionOrder(mObjects, objectType, orderConfig.name,
									 orderConfig.order));
}

ActionPtr ActionManager::createActionVisibility(int eventIndex, int actionIndex)
{
	ActionVisibilityConfig visibilityConfig;

	mConfig->getActionVisibilityConfig(eventIndex, actionIndex,
									   visibilityConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, visibilityConfig.object);

	return ActionPtr(new ActionVisibility(mObjects, objectType,
										  visibilityConfig.name,
										  visibilityConfig.visibility));
}

ActionPtr ActionManager::createActionOpacity(int eventIndex, int actionIndex)
{
	ActionOpacityConfig opacityConfig;

	mConfig->getActionOpacityConfig(eventIndex, actionIndex, opacityConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, opacityConfig.object);

	return ActionPtr(new ActionOpacity(mObjects, objectType, opacityConfig.name,
									   opacityConfig.opacity));
}

template <typename T, typename S>
T ActionManager::getType(const vector<S>& table, const string& name)
{
	string upperName = name;

	transform(upperName.begin(), upperName.end(), upperName.begin(),
			  [] (unsigned char c) { return toupper(c); } );

	for(auto elem : table)
	{
		if (upperName == elem.name)
		{
			return elem.type;
		}
	}

	throw DmException("Unknown type " + name);
}

EventPtr ActionManager::getObjectEvent(EventType eventType,
									   ObjectType objectType,
									   const string& name)
{
	for(auto event : mEvents)
	{
		if (event->getEventType() == eventType)
		{
			auto eventObject = dynamic_cast<EventObject*>(event.get());

			if (eventObject->getObjectType() == objectType &&
				eventObject->getName() == name)
			{
				return event;
			}
		}
	}

	return EventPtr();
}

void ActionManager::onCreateSurface(const std::string& name)
{
	auto event = getObjectEvent(EventType::CREATE, ObjectType::SURFACE, name);

	if (event)
	{
		event->doActions();
	}
}

void ActionManager::onDeleteSurface(const std::string& name)
{
	auto event = getObjectEvent(EventType::DESTROY, ObjectType::SURFACE, name);

	if (event)
	{
		event->doActions();
	}
}

