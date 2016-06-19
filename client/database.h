#ifndef __DATABASE_H__
#define __DATABASE_H__

class IDatabase
{
public:
    virtual void close() = 0;
};


class Database: public IDatabase
{
public:
    Database();

    void close();
};


#endif
