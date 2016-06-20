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
