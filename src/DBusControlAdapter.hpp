/*
 *  DBusControlAdapter
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

#ifndef DBUSCONTROLADAPTER_HPP
#define DBUSCONTROLADAPTER_HPP

#include <dbus-cxx.h>
#include <stdint.h>

#include <memory>
#include <string>

#include "DBusControlAdaptee.hpp"

class DBusControlAdapter : public DBus::Object {
protected:
    DBusControlAdapter(DBusControlAdaptee* adaptee,
                       std::string path = "/com/epam/DisplayManager");

public:
    typedef std::shared_ptr<DBusControlAdapter> pointer;

    static pointer create(DBusControlAdaptee* adaptee,
                          std::string path = "/com/epam/DisplayManager");
};
#endif /* DBUSCONTROLADAPTER_HPP */
