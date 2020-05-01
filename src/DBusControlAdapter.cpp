#include "DBusControlAdapter.hpp"

using DBus::MethodBase;
using DBus::Object;

using sigc::mem_fun;

using std::shared_ptr;
using std::string;

DBusControlAdapter::DBusControlAdapter(DBusControlAdaptee* adaptee, string path)
    : Object(path)
{
    MethodBase::pointer tmpMethod;

    tmpMethod = this->create_method<void, uint32_t>(
        "com.epam.DisplayManager.Control", "userEvent",
        mem_fun(adaptee, &DBusControlAdaptee::userEvent));

    tmpMethod->set_arg_name(0, "event");
}

shared_ptr<DBusControlAdapter> DBusControlAdapter::create(
    DBusControlAdaptee* adaptee, string path)
{
    return shared_ptr<DBusControlAdapter>(
        new DBusControlAdapter(adaptee, path));
}
