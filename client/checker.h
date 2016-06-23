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
#include "ichecker.h"
#include "printclient.h"
#include "database.h"
#include <tuple>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


class Checker: public IChecker
{
private:
    shared_ptr<ILog> _log;
    shared_ptr<IConfigs> _cfg;
    shared_ptr<IPrintClient> _pClient;
    shared_ptr<IDatabase> _db;
    io_service _io;
    shared_ptr<deadline_timer> _timer;
    unsigned _interval;

    void check();

    void checkDevice(size_t index);

    tuple<string,bool> getData(const string &url) const;

public:
    explicit Checker(const shared_ptr<ILog> &log, const shared_ptr<IConfigs> &cfg, const shared_ptr<IDatabase> &db,
                     const shared_ptr<IPrintClient> &pClient);

    inline void setInterval(unsigned interval) { _interval = interval; }

    void start();
};


#endif
