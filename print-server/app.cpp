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
#include <locale>

using namespace std;


App::App(const shared_ptr<QApplication> &qapp, const shared_ptr<IPrintServer> &printServer,
         const shared_ptr<IConfigs> &cfg, const shared_ptr<ILog> &log):
         _qapp(move(qapp)), _printServer(move(printServer)), _log(move(log)), _cfg(move(cfg))

{
}

int App::start()
{
    setlocale(LC_ALL, "Russian");
    cout << "Starting PrintServer...";

    _log->setLogFile("fingers.log");
    try {
        _cfg->load("fingers.cfg");
    }
    catch (const string &err) {
        cout << "[FAIL]" << endl;
        _log->local("Configs: " + err, LOG_ERROR);
        return -1;
    }

    const auto &sc = _cfg->getServerCfg();

    try {
        _printServer->start(sc.port, sc.clients);
    }
    catch(const string &err) {
        cout << "[FAIL]" << endl;
        _log->local("PrintServer: " + err, LOG_ERROR);
        return -1;
    }
    return _qapp->exec();
}
