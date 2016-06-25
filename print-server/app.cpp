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


App::App(const shared_ptr<QApplication> &qapp, const shared_ptr<IPrintServer> &printServer, const shared_ptr<IConfigs> &cfg,
         const shared_ptr<ILog> &log)
{
    _qapp = qapp;
    _printServer = printServer;
    _log = log;
    _cfg = cfg;
}

int App::start()
{
    setlocale(LC_ALL, "Russian");
    cout << "Starting..." << endl;

    _log->setLogFile("C:\\fingers\\fingers.log");
    try {
        _cfg->load("C:\\fingers\\fingers.cfg");
    }
    catch (const string &err) {
        cout << "Configs: " + err << endl;
        return -1;
    }

    try {
        _printServer->start(6000);
    }
    catch(const string &err) {
        _log->local("PrintServer: " + err, LOG_ERROR);
        return -1;
    }
    return _qapp->exec();
}
