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

#include "mainwindow.h"
#include "app.h"
#include "qapp.h"
#include "configs.h"
#include "log.h"
#include "database.h"
#include "tcpclient.h"


int main(int argc, char *argv[])
{
    auto log = make_shared<Log>();
    auto client = make_shared<TcpClient>();
    auto cfg = make_shared<Configs>();
    auto db = make_shared<Database>();
    auto qapp = make_shared<QApp>(argc, argv);
    auto mwindow = make_shared<MainWindow>(db, cfg, log, client);

    auto app = make_shared<App>(qapp, mwindow, cfg, log);
    return app->start();
}
