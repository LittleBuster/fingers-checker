/* Fingers checker
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "log.h"
#include <iostream>
#include <fstream>
#include <boost/date_time.hpp>


Log::Log(): _logPath("")
{
}

string Log::makeLogMsg(const string &msg, const LogType type) const
{
    string out;

    const auto dt = boost::posix_time::second_clock::local_time();
    out = "[" + boost::lexical_cast<string>(dt.date()) + "][" + boost::lexical_cast<string>(dt.time_of_day()) + "][";

    switch (type) {
        case LOG_ERROR: {
            out += "ERROR]";
            break;
        }
        case LOG_WARNING: {
            out += "WARNING]";
            break;
        }
        case LOG_INFORMATION: {
            out += "INFO]";
            break;
        }
    }
    out += msg;
    return out;
}

void Log::local(const string &message, const LogType err_type)
{
    const string& msg = makeLogMsg(message, err_type);
    cout << msg << endl;

    if (_logPath == "")
        return;

    try {
        ofstream log;
        log.open(_logPath, ios::out|ios::ate|ios::app);
        log << msg << "\n";
        log.close();
    }
    catch (...) {
        cout << "Fail writing to log file." << endl;
    }
}
