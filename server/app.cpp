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


App::App(const shared_ptr<IChecker> &checker, const shared_ptr<ILog> &log)
{
    _checker = checker;
    _log = log;
}

int App::start()
{
    _log->setKeyFile("/var/log/keys.log");
    _log->setLogFile("/var/log/fingers.log");
    _checker->setInterval(5);
    _checker->start();
    return 0;
}
