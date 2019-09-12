/*
 * DBusServer.hpp
 *
 *  Created on: Oct 25, 2017
 *      Author: al1
 */

#ifndef SRC_DBUSSERVER_HPP_
#define SRC_DBUSSERVER_HPP_

#include <xt/Log.hpp>

#include "DBusControlAdapter.hpp"

class ActionManager;

class DBusServer : public DBusControlAdaptee
{
public:
	DBusServer(ActionManager& actions, bool systemBus);
	~DBusServer();

private:
	ActionManager& mActions;
	DBus::Dispatcher::pointer mDispatcher;
	DBus::Connection::pointer mConnection;
	DBusControlAdapter::pointer mAdapter;

	xt::Log mLog;

	void userEvent(uint32_t event) override;
};

#endif /* SRC_DBUSSERVER_HPP_ */
