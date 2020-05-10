/*
 *  DisplayManager
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

#include <execinfo.h>
#include <getopt.h>
#include <unistd.h>

#include <csignal>
#include <fstream>
#include <iostream>
#include <xt/Log.hpp>

#include "DisplayManager.hpp"

using std::cout;
using std::endl;
using std::ofstream;
using std::string;

using xt::Log;

string gCfgFileName;
string gLogFileName;
bool gSystemBus;

/*******************************************************************************
 *
 ******************************************************************************/

void segmentationHandler(int sig)
{
    void* array[20];
    size_t size;

    LOG("Main", ERROR) << "Segmentation fault!";

    size = backtrace(array, 20);

    backtrace_symbols_fd(array, size, STDERR_FILENO);

    raise(sig);
}

void registerSignals()
{
    struct sigaction act {
    };

    act.sa_handler = segmentationHandler;
    act.sa_flags = SA_RESETHAND;

    sigaction(SIGSEGV, &act, nullptr);
}

void waitSignals()
{
    sigset_t set;
    int signal;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    sigprocmask(SIG_BLOCK, &set, nullptr);

    sigwait(&set, &signal);
}

bool commandLineOptions(int argc, char* argv[])
{
    int opt = -1;

    while ((opt = getopt(argc, argv, "sc:v:l:fh?")) != -1) {
        switch (opt) {
            case 's':
                gSystemBus = true;

                break;

            case 'v':
                if (!Log::setLogMask(string(optarg))) {
                    return false;
                }

                break;

            case 'c':
                gCfgFileName = optarg;

                break;

            case 'l':
                gLogFileName = optarg;

                break;

            case 'f':
                Log::setShowFileAndLine(true);

                break;

            default:
                return false;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    try {
        registerSignals();

        if (commandLineOptions(argc, argv)) {
            ofstream logFile;

            if (!gLogFileName.empty()) {
                logFile.open(gLogFileName);
                Log::setStreamBuffer(logFile.rdbuf());
            }

            ConfigPtr config(new Config(gCfgFileName));

            DisplayManager displayManager(config, gSystemBus);

            waitSignals();

            logFile.close();
        }
        else {
            cout << "Usage: " << argv[0]
                 << " [-c <file>] [-l <file>] [-v <level>]" << endl;
            cout << "\t-s -- use system bus" << endl;
            cout << "\t-c -- config file" << endl;
            cout << "\t-l -- log file" << endl;
            cout << "\t-v -- verbose level in format: "
                 << "<module>:<level>;<module:<level>" << endl;
            cout << "\t      use * for mask selection:"
                 << " *:Debug,Mod*:Info" << endl;

            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& e) {
        Log::setStreamBuffer(cout.rdbuf());

        LOG("Main", ERROR) << e.what();

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
