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

#ifndef __THREADMANAGER_H__
#define __THREADMANAGER_H__

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include "ichecker.h"

using namespace std;


class IThreadManager
{
public:
    virtual void addThread(const shared_ptr<IChecker> &checker) = 0;
    virtual void startAll() = 0;
};


class ThreadManager: public IThreadManager
{
private:
    vector< shared_ptr<IChecker> > _checkerPoll;

    void newThread(const shared_ptr<IChecker> &checker);

public:
    /**
     * Add new checker class in poll
     * @checker: checker class
     */
    void addThread(const shared_ptr<IChecker> &checker);

    /*
     * Multi-thread start
     */
    void startAll();
};


#endif
