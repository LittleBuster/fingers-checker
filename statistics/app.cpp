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
#include <QMessageBox>


App::App(const shared_ptr<IQApp> &qapp, const shared_ptr<IMainWindow> &mwindow, const shared_ptr<IConfigs> &cfg,
         const shared_ptr<ILog> &log):
    _qapp(move(qapp)), _mwindow(move(mwindow)), _cfg(move(cfg)), _log(move(log))
{
}

int App::start()
{
    _log->setLogFile("stat.log");
    try {
        _cfg->load("C:\\Qt\\projects\\build-Statisctics-Desktop_Qt_5_7_0_MinGW_32bit-Release\\release\\stat.cfg");
    }
    catch (const string &err) {
        _log->local("Configs: " + err, LOG_ERROR);
        QMessageBox::critical(0, "Ошибка", ("Конфигурации: " + err).c_str(), QMessageBox::Ok);
        return -1;
    }
    _mwindow->show();
    return _qapp->exec();
}
