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
#include "log.h"
#include "configs.h"
#include "database.h"
#include "tcpclient.h"
#include "notify.h"
#include "devicechecker.h"
#include "threadmanager.h"
#include "printclient.h"


int main(void)
{
    auto cfg = make_shared<Configs>();
    auto db = make_shared<Database>();
    auto dbCheck = make_shared<Database>();
    auto client = make_shared<TcpClient>();
    auto tm = make_shared<ThreadManager>();
    auto pClient = make_shared<PrintClient>();
    auto log = make_shared<Log>(cfg, db);
    auto notify = make_shared<Notify>(cfg, client);
    auto devChecker = make_shared<DeviceChecker>(cfg, notify, log);
    auto checker = make_shared<Checker>(log, cfg, dbCheck, pClient);

    auto app = make_shared<App>(checker, devChecker, tm, log, cfg);
    return app->start();
}
