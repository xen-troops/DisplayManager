/*
 * Auto-generated header file.
 * Build timestamp: Thu Oct 26 15:12:54 2017

*/
#ifndef AUTO_D3406A40_5B8F_4699_98A0_70A8840C1625_H_
#define AUTO_D3406A40_5B8F_4699_98A0_70A8840C1625_H_

#include <core/dbus/types/variant.h>

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

namespace org
{
namespace freedesktop
{
namespace DBus
{
struct Introspectable
{
static const std::string& default_path() { static const std::string s{"/com/epam/DisplayManager"}; return s; }
static const std::string& name() { static const std::string s{"org.freedesktop.DBus.Introspectable"}; return s; }
static const std::string& introspect() { static const std::string s{"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\"><node name=\"/com/epam/DisplayManager\"><interface name=\"org.freedesktop.DBus.Introspectable\"><method name=\"Introspect\"><arg name=\"data\" direction=\"out\" type=\"s\"/></method></interface></node>"}; return s; }
struct Introspect
{
typedef Introspectable Interface;
static const std::string& name() { static const std::string s{"Introspect"}; return s; }
static const std::chrono::milliseconds& default_timeout() { static const std::chrono::seconds s{10}; return s; }
typedef void ArgumentType;
typedef std::string ResultType;
};
};
}
}
}
#endif
