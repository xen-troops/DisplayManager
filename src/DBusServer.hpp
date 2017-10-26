/*
 * DBusServer.hpp
 *
 *  Created on: Oct 25, 2017
 *      Author: al1
 */

#ifndef SRC_DBUSSERVER_HPP_
#define SRC_DBUSSERVER_HPP_

#include <thread>

#include <core/dbus/dbus.h>
#include <core/dbus/message.h>

#include <xen/be/Log.hpp>

class DBusServer
{
public:
	DBusServer();
	~DBusServer();

private:
	core::dbus::Bus::Ptr mBus;
	core::dbus::Object::Ptr mObject;
	std::thread mThread;

	XenBackend::Log mLog;

	void introspectHandler(const core::dbus::Message::Ptr& msg);
	void userEventHandler(const core::dbus::Message::Ptr& msg);
};

#endif /* SRC_DBUSSERVER_HPP_ */
