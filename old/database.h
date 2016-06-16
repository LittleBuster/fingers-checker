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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>
#include <QtSql>

using namespace std;


class IDatabase
{
public:
    virtual void open(const string &filename)=0;
    virtual bool checkUser(unsigned id, const string &date) const=0;
    virtual void addUser(unsigned id, const string &name, const string &date) const=0;
    virtual tuple<bool, unsigned> incUser(unsigned id, const string &date) const=0;
    virtual void close()=0;
};


class Database: public IDatabase
{
private:
    QSqlDatabase _db;

public:
    void open(const string &filename);

    bool checkUser(unsigned id, const string &date) const;

    void addUser(unsigned id, const string &name, const string &date) const;

    tuple<bool, unsigned> incUser(unsigned id, const string &date) const;

    void close();
};


#endif
