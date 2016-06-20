#ifndef __DEVICECHECKER_H__
#define __DEVICECHECKER_H__

#include "ichecker.h"


class DeviceChecker: public IChecker
{
private:
    unsigned _interval;

public:
    inline void setInterval(unsigned sec) { _interval = sec; }

    void start();
};


#endif
