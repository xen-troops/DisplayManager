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

#include "DBusServer.hpp"

#include <glib-unix.h>

#include "ActionManager.hpp"

/*******************************************************************************
 * Static
 ******************************************************************************/

namespace {
gboolean sTerminationHandler(gpointer data)
{
    LOG("main", INFO) << "Terminating...";

    g_main_loop_quit(reinterpret_cast<GMainLoop*>(data));

    return false;
}
}  // namespace

/*******************************************************************************
 * DBusServer
 ******************************************************************************/

DBusServer::DBusServer(ActionManager& actions, bool systemBus)
    : mActions(actions), mLoop(nullptr), mLog("DBusServer")
{
    LOG(mLog, DEBUG) << "Create " << (systemBus ? "system" : "session")
                     << " bus";

    mLoop = g_main_loop_new(NULL, FALSE);

    g_unix_signal_add(SIGINT, sTerminationHandler, mLoop);
    g_unix_signal_add(SIGTERM, sTerminationHandler, mLoop);

    mBusId = g_bus_own_name(
        (systemBus ? G_BUS_TYPE_SYSTEM : G_BUS_TYPE_SESSION), sBusName,
        static_cast<GBusNameOwnerFlags>(
            G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
            G_BUS_NAME_OWNER_FLAGS_REPLACE),
        DBusServer::sOnBusAcquired, nullptr, nullptr, this, nullptr);
}

DBusServer::~DBusServer()
{
    LOG(mLog, DEBUG) << "Delete";

    g_bus_unown_name(mBusId);
    g_main_loop_unref(mLoop);
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void DBusServer::run()
{
    g_main_loop_run(mLoop);
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void DBusServer::sOnBusAcquired(GDBusConnection* connection, const gchar* name,
                                gpointer userData)
{
    static_cast<DBusServer*>(userData)->onBusAcquired(connection, name);
}

void DBusServer::onBusAcquired(GDBusConnection* connection, const gchar* name)
{
    LOG(mLog, DEBUG) << "Bus acquired, name: " << name;

    auto interface = control_skeleton_new();

    g_signal_connect(interface, "handle-user-event",
                     G_CALLBACK(DBusServer::sHandleUserEvent), this);

    GError* error = nullptr;

    if (!g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(interface),
                                          connection, sObjectPath, &error)) {
        LOG(mLog, ERROR) << "Can't export interface: " << error->message;

        g_main_loop_quit(mLoop);
    }
}

bool DBusServer::sHandleUserEvent(Control* interface,
                                  GDBusMethodInvocation* invocation,
                                  guint userEvent, gpointer userData)
{
    return static_cast<DBusServer*>(userData)->handleUserEvent(
        interface, invocation, userEvent);
}

bool DBusServer::handleUserEvent(Control* interface,
                                 GDBusMethodInvocation* invocation,
                                 guint userEvent)
{
    LOG(mLog, DEBUG) << "User event received: " << userEvent;

    mActions.userEvent(userEvent);

    return false;
}
