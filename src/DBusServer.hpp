/*
 *  DBusServer
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2017 EPAM Systems Inc.
 */

#ifndef SRC_DBUSSERVER_HPP_
#define SRC_DBUSSERVER_HPP_

#include <xt/Log.hpp>

#include "DBusControlAdapter.hpp"

class ActionManager;

class DBusServer : public DBusControlAdaptee {
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
