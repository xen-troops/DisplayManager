/*
 * Auto-generated header file.
 * Build timestamp: Thu Oct 26 15:14:57 2017

*/
#ifndef AUTO_AA9177C4_559C_444A_BDF2_867CC1A9588B_H_
#define AUTO_AA9177C4_559C_444A_BDF2_867CC1A9588B_H_

#include <core/dbus/types/variant.h>

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

namespace com
{
namespace epam
{
namespace DisplayManager
{
struct Control
{
static const std::string& default_path() { static const std::string s{"/com/epam/DisplayManager"}; return s; }
static const std::string& name() { static const std::string s{"com.epam.DisplayManager.Control"}; return s; }
static const std::string& introspect() { static const std::string s{"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\"><node name=\"/com/epam/DisplayManager\"><interface name=\"com.epam.DisplayManager.Control\"><method name=\"userEvent\"><arg name=\"event\" type=\"u\" direction=\"in\" /></method></interface></node>"}; return s; }
struct userEvent
{
typedef Control Interface;
static const std::string& name() { static const std::string s{"userEvent"}; return s; }
static const std::chrono::milliseconds& default_timeout() { static const std::chrono::milliseconds s{1000}; return s; }
typedef std::uint32_t ArgumentType;
typedef void ResultType;
};
};
}
}
}
#endif
