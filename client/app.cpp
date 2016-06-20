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

#include "app.h"
#include <iostream>
#include <boost/thread.hpp>


App::App(const shared_ptr<IChecker> &checker, const shared_ptr<IChecker> &devChecker, const shared_ptr<IThreadManager> &tm, const shared_ptr<ILog> &log, const shared_ptr<IConfigs> &cfg)
{
    _checker = checker;
    _log = log;
    _cfg = cfg;
    _devChecker = devChecker;
    _tm = tm;
}

int App::start()
{
    _log->setLogFile("/var/log/fingers.log");

    try {
        _cfg->load("/etc/fingers.cfg");
    }
    catch (const string &err) {
        cout << err << endl;
        _log->local(err, LOG_ERROR);
        return -1;
    }

    const auto &cc = _cfg->getCheckerCfg();
    _checker->setInterval(cc.interval);
    _devChecker->setInterval(cc.devInterval);

    _tm->addThread(_checker);
    _tm->addThread(_devChecker);
    _tm->startAll();

    for (;;) {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
    return 0;
}
