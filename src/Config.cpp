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
 * Copyright (C) 2016 EPAM Systems Inc.
 */

#include "Config.hpp"

using std::string;
using std::to_string;

using libconfig::Setting;
using libconfig::FileIOException;
using libconfig::ParseException;
using libconfig::SettingException;
using libconfig::SettingNotFoundException;

/*******************************************************************************
 * Config
 ******************************************************************************/

Config::Config(const string& fileName) :
	mLog("Config")
{
	const char* cfgName = cDefaultCfgName;

	try
	{
		if (!fileName.empty())
		{
			cfgName = fileName.c_str();
		}

		LOG(mLog, DEBUG) << "Open file: " << cfgName;

		mConfig.readFile(cfgName);

		mDisplaysCount = readSectionCount("displays");
		mLayersCount = readSectionCount("layers");
		mSurfacesCount = readSectionCount("surfaces");
	}
	catch(const FileIOException& e)
	{
		throw ConfigException("Config: can't open file: " + string(cfgName));
	}
	catch(const ParseException& e)
	{
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

	try
	{
		Setting& setting = mConfig.lookup(sectionName)[index];

		config.name = static_cast<const char*>(setting.lookup("name"));
		config.id = setting.lookup("id");

		LOG(mLog, DEBUG) << sectionName << "[" << index << "] name: "
						 << config.name
						 << ", id: " << config.id;
	}
	catch(const SettingException& e)
	{
		throw ConfigException("Config: error reading " + sectionName);
	}
}

void Config::getLayerConfig(int index, LayerConfig& config)
{
	string sectionName = "layers";

	try
	{
		config = {};

		Setting& setting = mConfig.lookup(sectionName)[index];

		config.name = static_cast<const char*>(setting.lookup("name"));
		config.id = setting.lookup("id");
		config.display = static_cast<const char*>(setting.lookup("display"));
		config.width = setting.lookup("width");
		config.height = setting.lookup("height");

		config.visibility = 1;
		config.opacity = 1.0;

		setting.lookupValue("visible", config.visibility);
		setting.lookupValue("opacity", config.opacity);

		config.source = { 0, 0, config.width, config.height };
		config.destination = config.source;

		if (setting.exists("source"))
		{
			readRectangle(setting.lookup("source"), config.source);
		}

		if (setting.exists("destination"))
		{
			readRectangle(setting.lookup("destination"), config.destination);
		}

		setting.lookupValue("order", config.order);

		LOG(mLog, DEBUG) << sectionName << "[" << index << "] name: "
						 << config.name
						 << ", id: " << config.id
						 << ", w: " << config.width << ", h: " << config.height;
	}
	catch(const SettingException& e)
	{
		throw ConfigException("Config: error reading " + sectionName);
	}
}

void Config::getSurfaceConfig(int index, SurfaceConfig& config)
{
	string sectionName = "surfaces";

	try
	{
		Setting& setting = mConfig.lookup(sectionName)[index];

		config.name = static_cast<const char*>(setting.lookup("name"));
		config.id = setting.lookup("id");
		config.layer = static_cast<const char*>(setting.lookup("layer"));
		config.width = setting.lookup("width");
		config.height = setting.lookup("height");

		if (setting.exists("source"))
		{
			readRectangle(setting.lookup("source"), config.source);
		}

		if (setting.exists("destination"))
		{
			readRectangle(setting.lookup("destination"), config.destination);
		}

		LOG(mLog, DEBUG) << sectionName << "[" << index << "] name: "
						 << config.name
						 << ", id: " << config.id
						 << ", w: " << config.width << ", h: " << config.height;
	}
	catch(const SettingException& e)
	{
		throw ConfigException("Config: error reading " + sectionName);
	}
}

/*******************************************************************************
 * Private
 ******************************************************************************/

int Config::readSectionCount(const string& name)
{
	try
	{
		auto count = mConfig.lookup(name).getLength();

		LOG(mLog, DEBUG) << name << " count: " << count;

		return count;
	}
	catch(const SettingNotFoundException& e)
	{
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
