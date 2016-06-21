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

#ifndef __DEVICECHECKER_H__
#define __DEVICECHECKER_H__

#include "ichecker.h"
#include "configs.h"
#include "notify.h"
#include "log.h"
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


class DeviceChecker: public IChecker
{
private:
    shared_ptr<IConfigs> _cfg;
    shared_ptr<INotify> _notify;
    shared_ptr<ILog> _log;
    unsigned _interval;
    io_service _io;
    shared_ptr<deadline_timer> _timer;
    bool _failPrint[DEV_COUNT] = {false, false, false};
    bool _failDevs[DEV_COUNT] = {false, false, false};

    void checkDeviceLife();

public:
    explicit DeviceChecker(const shared_ptr<IConfigs> &cfg, const shared_ptr<INotify> &notify, const shared_ptr<ILog> &log);

    /**
     * Setting checking interval
     * @sec: seconds
     */
    inline void setInterval(unsigned sec) { _interval = sec; }

    /*
     * Start timer
     */
    void start();
};


#endif
