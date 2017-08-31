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
using std::toupper;
using std::transform;
using std::vector;

/*******************************************************************************
 * ActionManager
 ******************************************************************************/

ActionManager::ActionManager(ObjectManager& objects, ConfigPtr config) :
	mObjects(objects),
	mLog("ActionManager")
{
	LOG(mLog, DEBUG) << "Create";

	init(config);
}

ActionManager::~ActionManager()
{
	LOG(mLog, DEBUG) << "Delete";
}

/*******************************************************************************
 * Public
 ******************************************************************************/

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

void ActionManager::init(ConfigPtr config)
{
	for(int i = 0; i < config->getEventsCount(); i++)
	{
		string name;

		config->getEventName(i, name);

		EventPtr event;

		auto eventType = getType<EventType, EventsTable>(sEventsTable, name);

		switch(eventType)
		{
			case EventType::CREATE:
			{
				event = createEventCreate(i, config);

				break;
			}
			case EventType::DESTROY:
			{
				event = createEventDestroy(i, config);

				break;
			}

			default:
			{
				throw DmException("Not implemented event: " + name);

				break;
			}
		}

		createEventActions(i, event, config);

		mEvents.push_back(event);
	}
}

EventPtr ActionManager::createEventCreate(int eventIndex, ConfigPtr config)
{
	EventCreateConfig createConfig;

	config->getEventCreateConfig(eventIndex, createConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, createConfig.object);

	return EventPtr(new EventCreate(EventType::CREATE, objectType,
									createConfig.name));
}

EventPtr ActionManager::createEventDestroy(int eventIndex, ConfigPtr config)
{
	EventDestroyConfig destroyConfig;

	config->getEventDestroyConfig(eventIndex, destroyConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, destroyConfig.object);

	return EventPtr(new EventDestroy(EventType::DESTROY, objectType,
									 destroyConfig.name));
}

void ActionManager::createEventActions(int eventIndex, EventPtr event,
									   ConfigPtr config)
{
	auto count = config->getActionsCount(eventIndex);

	for(int i = 0; i < count; i++)
	{
		string name;

		config->getActionName(eventIndex, i, name);

		ActionPtr action;

		auto actionType = getType<ActionType, ActionsTable>(sActionsTable,
															name);

		switch(actionType)
		{
			case ActionType::SOURCE:
			{
				action = createActionSource(eventIndex, i, config);

				break;
			}

			case ActionType::DESTINATION:
			{
				action = createActionDestination(eventIndex, i, config);

				break;
			}

			case ActionType::PARENT:
			{
				action = createActionParent(eventIndex, i, config);

				break;
			}

			case ActionType::ORDER:
			{
				action = createActionOrder(eventIndex, i, config);

				break;
			}

			case ActionType::VISIBILITY:
			{
				action = createActionVisibility(eventIndex, i, config);

				break;
			}

			case ActionType::OPACITY:
			{
				action = createActionOpacity(eventIndex, i, config);

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

ActionPtr ActionManager::createActionSource(int eventIndex, int actionIndex,
											ConfigPtr config)
{
	ActionSourceConfig sourceConfig;

	config->getActionSourceConfig(eventIndex, actionIndex, sourceConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, sourceConfig.object);

	return ActionPtr(new ActionSource(mObjects, objectType, sourceConfig.name,
									  sourceConfig.source));
}

ActionPtr ActionManager::createActionDestination(int eventIndex,
												 int actionIndex,
												 ConfigPtr config)
{
	ActionDestinationConfig destinationConfig;

	config->getActionDestinationConfig(eventIndex, actionIndex,
									   destinationConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, destinationConfig.object);

	return ActionPtr(new ActionSource(mObjects, objectType,
									  destinationConfig.name,
									  destinationConfig.destination));
}

ActionPtr ActionManager::createActionParent(int eventIndex, int actionIndex,
											ConfigPtr config)
{
	ActionParentConfig parentConfig;

	config->getActionParentConfig(eventIndex, actionIndex, parentConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, parentConfig.object);

	return ActionPtr(new ActionParent(mObjects, objectType, parentConfig.name,
									  parentConfig.parent));
}

ActionPtr ActionManager::createActionOrder(int eventIndex, int actionIndex,
										   ConfigPtr config)
{
	ActionOrderConfig orderConfig;

	config->getActionOrderConfig(eventIndex, actionIndex, orderConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, orderConfig.object);

	return ActionPtr(new ActionOrder(mObjects, objectType, orderConfig.name,
									 orderConfig.order));
}

ActionPtr ActionManager::createActionVisibility(int eventIndex, int actionIndex,
												ConfigPtr config)
{
	ActionVisibilityConfig visibilityConfig;

	config->getActionVisibilityConfig(eventIndex, actionIndex,
									  visibilityConfig);

	auto objectType = getType<ObjectType, ObjectsTable>(
			sObjectsTable, visibilityConfig.object);

	return ActionPtr(new ActionVisibility(mObjects, objectType,
										  visibilityConfig.name,
										  visibilityConfig.visibility));
}

ActionPtr ActionManager::createActionOpacity(int eventIndex, int actionIndex,
											 ConfigPtr config)
{
	ActionOpacityConfig opacityConfig;

	config->getActionOpacityConfig(eventIndex, actionIndex, opacityConfig);

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
