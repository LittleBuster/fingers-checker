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
    void addThread(const shared_ptr<IChecker> &checker);

    void startAll();
};


#endif
