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

#include "threadmanager.h"


void ThreadManager::newThread(const shared_ptr<IChecker> &checker)
{
    checker->start();
}

void ThreadManager::addThread(const shared_ptr<IChecker> &checker)
{
    _checkerPoll.push_back(checker);
}

void ThreadManager::startAll()
{
    for (const auto &c : _checkerPoll) {
        thread cTh(bind(&ThreadManager::newThread, this, c));
        cTh.detach();
    }
}
