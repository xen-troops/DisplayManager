/*
 * DBusServer.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: al1
 */

#include "DBusServer.hpp"

#include "ActionManager.hpp"

using DBus::BUS_SESSION;
using DBus::BUS_SYSTEM;
using DBus::Dispatcher;
using DBus::init;

using std::exception;
using std::runtime_error;
using std::shared_ptr;

DBusServer::DBusServer(ActionManager& actions, bool systemBus)
    : mActions(actions), mLog("DBusServer")
{
    LOG(mLog, DEBUG) << "Create";

    try {
        init();

        mDispatcher = Dispatcher::create();

        if (systemBus) {
            mConnection = mDispatcher->create_connection(BUS_SYSTEM);
        }
        else {
            mConnection = mDispatcher->create_connection(BUS_SESSION);
        }

        if (mConnection->request_name("com.epam.DisplayManager",
                                      DBUS_NAME_FLAG_REPLACE_EXISTING) !=
            DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
            throw(runtime_error("Can't request DBus name"));
        }

        DBusControlAdapter::create(this);

        mAdapter = DBusControlAdapter::create(this);
        mConnection->register_object(mAdapter);
    }
    catch (const std::shared_ptr<DBus::Error> e) {
        throw DmException(e->what());
    }
}

DBusServer::~DBusServer()
{
    LOG(mLog, DEBUG) << "Delete";
}

void DBusServer::userEvent(uint32_t event)
{
    try {
        LOG(mLog, DEBUG) << "User event: " << event;

        mActions.userEvent(event);
    }
    catch (const exception& e) {
        LOG(mLog, ERROR) << e.what();
    }
}
