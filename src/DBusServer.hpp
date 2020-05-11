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

#include <gio/gio.h>
#include <glib.h>

#include <xt/Log.hpp>

#include "DBusControl.h"

class ActionManager;

class DBusServer {
public:
    DBusServer(ActionManager& actions, bool systemBus);
    ~DBusServer();

    void run();

private:
    static constexpr const char* sBusName = "com.epam.DisplayManager";
    static constexpr const char* sObjectPath = "/com/epam/DisplayManager";

    ActionManager& mActions;
    GMainLoop* mLoop;
    guint mBusId;
    xt::Log mLog;

    static void sOnBusAcquired(GDBusConnection* connection, const gchar* name,
                               gpointer userData);
    void onBusAcquired(GDBusConnection* connection, const gchar* name);

    static bool sHandleUserEvent(Control* interface,
                                 GDBusMethodInvocation* invocation,
                                 guint userEvent, gpointer userData);
    bool handleUserEvent(Control* interface, GDBusMethodInvocation* invocation,
                         guint userEvent);
};

#endif /* SRC_DBUSSERVER_HPP_ */
