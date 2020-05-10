/*
 *  Config
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

#include "Config.hpp"

using std::string;
using std::to_string;

using libconfig::FileIOException;
using libconfig::ParseException;
using libconfig::Setting;
using libconfig::SettingException;
using libconfig::SettingNotFoundException;

/*******************************************************************************
 * Config
 ******************************************************************************/

Config::Config(const string& fileName) : mLog("Config")
{
    const char* cfgName = cDefaultCfgName;

    try {
        if (!fileName.empty()) {
            cfgName = fileName.c_str();
        }

        LOG(mLog, DEBUG) << "Open file: " << cfgName;

        mConfig.readFile(cfgName);

        mDisplaysCount = readSectionCount("displays");
        mLayersCount = readSectionCount("layers");
        mSurfacesCount = readSectionCount("surfaces");
        mEventsCount = readSectionCount("events");
    }
    catch (const FileIOException& e) {
        throw ConfigException("Config: can't open file: " + string(cfgName));
    }
    catch (const ParseException& e) {
        throw ConfigException("Config: " + string(e.getError()) +
                              ", file: " + string(e.getFile()) +
                              ", line: " + to_string(e.getLine()));
    }
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void Config::getDisplayConfig(int index, DisplayConfig& config)
{
    string sectionName = "displays";

    try {
        Setting& setting = mConfig.lookup(sectionName)[index];

        config.name = static_cast<const char*>(setting.lookup("name"));
        config.id = setting.lookup("id");

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] name: " << config.name << ", id: " << config.id;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getLayerConfig(int index, LayerConfig& config)
{
    string sectionName = "layers";

    try {
        config = {};

        Setting& setting = mConfig.lookup(sectionName)[index];

        config.name = static_cast<const char*>(setting.lookup("name"));
        config.id = setting.lookup("id");
        config.display = static_cast<const char*>(setting.lookup("display"));
        config.width = setting.lookup("width");
        config.height = setting.lookup("height");

        config.create = true;
        config.visibility = 1;
        config.opacity = 1.0;

        setting.lookupValue("create", config.create);
        setting.lookupValue("visibility", config.visibility);
        setting.lookupValue("opacity", config.opacity);
        setting.lookupValue("order", config.order);

        config.source = {0, 0, config.width, config.height};
        config.destination = config.source;

        if (setting.exists("source")) {
            readRectangle(setting.lookup("source"), config.source);
        }

        if (setting.exists("destination")) {
            readRectangle(setting.lookup("destination"), config.destination);
        }

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] name: " << config.name << ", id: " << config.id
                         << ", w: " << config.width << ", h: " << config.height;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getSurfaceConfig(int index, SurfaceConfig& config)
{
    string sectionName = "surfaces";

    try {
        config = {};

        Setting& setting = mConfig.lookup(sectionName)[index];

        config.name = static_cast<const char*>(setting.lookup("name"));
        config.id = setting.lookup("id");
        config.layer = static_cast<const char*>(setting.lookup("layer"));
        config.width = setting.lookup("width");
        config.height = setting.lookup("height");

        config.visibility = 1;
        config.opacity = 1.0;

        setting.lookupValue("visibility", config.visibility);
        setting.lookupValue("opacity", config.opacity);
        setting.lookupValue("order", config.order);

        config.source = {0, 0, config.width, config.height};
        config.destination = config.source;

        if (setting.exists("source")) {
            readRectangle(setting.lookup("source"), config.source);
        }

        if (setting.exists("destination")) {
            readRectangle(setting.lookup("destination"), config.destination);
        }

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] name: " << config.name << ", id: " << config.id
                         << ", w: " << config.width << ", h: " << config.height;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getEventName(int index, string& name)
{
    string sectionName = "events";

    try {
        Setting& setting = mConfig.lookup(sectionName)[index];

        name = static_cast<const char*>(setting.lookup("event"));

        LOG(mLog, DEBUG) << sectionName << "[" << index << "] event: " << name;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getEventCreateConfig(int index, EventCreateConfig& config)
{
    string sectionName = "events";

    try {
        Setting& setting = mConfig.lookup(sectionName)[index];

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] object: " << config.object
                         << ", name: " << config.name;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getEventDestroyConfig(int index, EventDestroyConfig& config)
{
    string sectionName = "events";

    try {
        Setting& setting = mConfig.lookup(sectionName)[index];

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] object: " << config.object
                         << ", name: " << config.name;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getEventUserConfig(int index, EventUserConfig& config)
{
    string sectionName = "events";

    try {
        Setting& setting = mConfig.lookup(sectionName)[index];

        config.id = setting.lookup("id");

        LOG(mLog, DEBUG) << sectionName << "[" << index
                         << "] id: " << config.id;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

int Config::getActionsCount(int eventIndex) const
{
    string sectionName = "events";

    try {
        Setting& setting = mConfig.lookup(sectionName)[eventIndex];

        int count = 0;

        if (setting.exists("actions")) {
            count = setting.lookup("actions").getLength();
        }

        LOG(mLog, DEBUG) << "Actions count: " << count;

        return count;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading " + sectionName);
    }
}

void Config::getActionName(int eventIndex, int actionIndex, string& name)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        name = static_cast<const char*>(setting.lookup("action"));

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] action: " << name;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionSourceConfig(int eventIndex, int actionIndex,
                                   ActionSourceConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));

        config.source = {
            static_cast<t_ilm_uint>(-1), static_cast<t_ilm_uint>(-1),
            static_cast<t_ilm_uint>(-1), static_cast<t_ilm_uint>(-1)};

        readRectangle(setting, config.source);

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", x: " << config.source.x
                         << ", y: " << config.source.y
                         << ", w: " << config.source.width
                         << ", h: " << config.source.height;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionDestinationConfig(int eventIndex, int actionIndex,
                                        ActionDestinationConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));

        config.destination = {
            static_cast<t_ilm_uint>(-1), static_cast<t_ilm_uint>(-1),
            static_cast<t_ilm_uint>(-1), static_cast<t_ilm_uint>(-1)};

        readRectangle(setting, config.destination);

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", x: " << config.destination.x
                         << ", y: " << config.destination.y
                         << ", w: " << config.destination.width
                         << ", h: " << config.destination.height;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionParentConfig(int eventIndex, int actionIndex,
                                   ActionParentConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));
        config.parent = static_cast<const char*>(setting.lookup("parent"));

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", parent: " << config.parent;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionOrderConfig(int eventIndex, int actionIndex,
                                  ActionOrderConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));
        config.order = setting.lookup("order");

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", order: " << config.order;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionVisibilityConfig(int eventIndex, int actionIndex,
                                       ActionVisibilityConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));
        config.visibility = setting.lookup("visibility");

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", visibility: " << config.visibility;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

void Config::getActionOpacityConfig(int eventIndex, int actionIndex,
                                    ActionOpacityConfig& config)
{
    try {
        Setting& setting =
            mConfig.lookup("events")[eventIndex].lookup("actions")[actionIndex];

        config = {};

        config.object = static_cast<const char*>(setting.lookup("object"));
        config.name = static_cast<const char*>(setting.lookup("name"));
        config.opacity = setting.lookup("order");

        LOG(mLog, DEBUG) << "events[" << eventIndex << "].actions["
                         << actionIndex << "] object: " << config.object
                         << ", name: " << config.name
                         << ", opacity: " << config.opacity;
    }
    catch (const SettingException& e) {
        throw ConfigException("Config: error reading actions");
    }
}

/*******************************************************************************
 * Private
 ******************************************************************************/

int Config::readSectionCount(const string& name) const
{
    try {
        int count = 0;

        if (mConfig.exists(name)) {
            count = mConfig.lookup(name).getLength();
        }

        LOG(mLog, DEBUG) << name << " count: " << count;

        return count;
    }
    catch (const SettingNotFoundException& e) {
        throw ConfigException("Config: error reading " + name);
    }
}

void Config::readRectangle(Setting& setting, IlmRectangle& rect)
{
    setting.lookupValue("x", rect.x);
    setting.lookupValue("y", rect.y);
    setting.lookupValue("width", rect.width);
    setting.lookupValue("height", rect.height);
}
