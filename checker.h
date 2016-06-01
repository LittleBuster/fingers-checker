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
#include "database.h"
#include <tuple>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


class IChecker
{
public:
    virtual void start()=0;
    virtual void setInterval(unsigned interval)=0;
};


class Checker: public IChecker
{
private:
    shared_ptr<ILog> _log;
    shared_ptr<IDatabase> _db;
    io_service _io;
    shared_ptr<deadline_timer> _timer;
    unsigned _interval;

    void check();

    tuple<string,bool> getData(const string &url);

public:
    Checker(const shared_ptr<IDatabase> &db, const shared_ptr<ILog> &log);

    inline void setInterval(unsigned interval) {
        _interval = interval;
    }

    void start();
};


#endif
