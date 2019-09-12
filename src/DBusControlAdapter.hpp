#ifndef DBUSCONTROLADAPTER_HPP
#define DBUSCONTROLADAPTER_HPP

#include <dbus-cxx.h>
#include <memory>
#include <stdint.h>
#include <string>

#include "DBusControlAdaptee.hpp"

class DBusControlAdapter : public DBus::Object
{
protected:
	DBusControlAdapter(DBusControlAdaptee* adaptee,
					   std::string path = "/com/epam/DisplayManager");

public:
	typedef std::shared_ptr<DBusControlAdapter> pointer;

	static pointer create(
		DBusControlAdaptee* adaptee,
		std::string path = "/com/epam/DisplayManager");
};
#endif /* DBUSCONTROLADAPTER_HPP */
