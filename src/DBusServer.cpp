/*
 * DBusServer.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: al1
 */

#include "DBusServer.hpp"

#include <core/dbus/asio/executor.h>

#include "DBusItfControl.hpp"
#include "DBusItfIntrospectable.hpp"

using std::bind;
using std::make_shared;
using std::thread;
using namespace std::placeholders;

using namespace core::dbus;

using namespace com::epam;
using namespace org::freedesktop;

DBusServer::DBusServer() :
	mLog("DBusServer")
{
	mBus = make_shared<Bus>(WellKnownBus::session);
	mBus->install_executor(asio::make_executor(mBus));

	auto service = Service::add_service(mBus, "com.epam.DeviceManager");

	mThread = thread([this]() { mBus->run(); });

	mObject = service->add_object_for_path(
			types::ObjectPath(DisplayManager::Control::default_path()));

	mObject->install_method_handler<DBus::Introspectable::Introspect>(
			bind(&DBusServer::introspectHandler, this, _1));

	mObject->install_method_handler<DisplayManager::Control::userEvent>(
			bind(&DBusServer::userEventHandler, this, _1));

	LOG(mLog, DEBUG) << "Create";
}

DBusServer::~DBusServer()
{
	mBus->stop();

	if (mThread.joinable())
	{
		mThread.join();
	}

	LOG(mLog, DEBUG) << "Delete";
}

void DBusServer::introspectHandler(const core::dbus::Message::Ptr& msg)
{
	LOG(mLog, DEBUG) << "Introspect";

    auto reply = Message::make_method_return(msg);

    auto str = DisplayManager::Control::introspect();

    reply->writer().push_stringn(str.c_str(), str.length());

    mBus->send(reply);
}

void DBusServer::userEventHandler(const Message::Ptr& msg)
{
	LOG(mLog, DEBUG) << "User event: " << msg->reader().pop_uint32();
}
