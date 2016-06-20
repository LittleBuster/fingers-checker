#ifndef __ICHECKER_H__
#define __ICHECKER_H__


class IChecker
{
public:
    virtual void setInterval(unsigned sec) = 0;
    virtual void start() = 0;
};


#endif
