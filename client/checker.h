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

#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "log.h"
#include "configs.h"
#include "notify.h"
#include "ichecker.h"
#include <tuple>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


class Checker: public IChecker
{
private:
    shared_ptr<ILog> _log;
    shared_ptr<IConfigs> _cfg;
    shared_ptr<INotify> _notify;
    io_service _io;
    shared_ptr<deadline_timer> _timer;
    unsigned _interval;

    void check();

    void checkDevice(const string &devIp, const string &printer, const string &user, const string &passwd,
                     const string &pageSize);

    void checkDeviceLive(const WebCfg &wc);

    tuple<string,bool> getData(const string &url) const;

    string dateToNum(const boost::posix_time::ptime &time);

public:
    explicit Checker(const shared_ptr<ILog> &log, const shared_ptr<IConfigs> &cfg, const shared_ptr<INotify> &notify);

    inline void setInterval(unsigned interval) { _interval = interval; }

    void start();
};


#endif
