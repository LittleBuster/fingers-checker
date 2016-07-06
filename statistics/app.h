/* Fingers Statistics application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __APP_H__
#define __APP_H__

#include "mainwindow.h"
#include <memory>
#include "qapp.h"
#include "configs.h"
#include "log.h"

using namespace std;


class App
{
private:
    const shared_ptr<IQApp> _qapp;
    const shared_ptr<IMainWindow> _mwindow;
    const shared_ptr<IConfigs> _cfg;
    const shared_ptr<ILog> _log;

public:
    App(const shared_ptr<IQApp> &qapp, const shared_ptr<IMainWindow> &mwindow, const shared_ptr<IConfigs> &cfg,
        const shared_ptr<ILog> &log);

    int start();
};


#endif
