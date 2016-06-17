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

#ifndef __APP_H__
#define __APP_H__

#include "checker.h"
#include "log.h"


class App
{
private:
    shared_ptr<IChecker> _checker;
    shared_ptr<ILog> _log;

public:
    explicit App(const shared_ptr<IChecker> &checker, const shared_ptr<ILog> &log);

    int start();
};


#endif
