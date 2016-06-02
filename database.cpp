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

#include "database.h"
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


void Database::open(const string &filename)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(filename.c_str());
    if (!_db.open())
        throw string(_db.lastError().text().toStdString());
}

void Database::addUser(unsigned id, const string &name, const string &date) const
{
    QSqlQuery q;
    vector<string> datetime;
    const string &uid = boost::lexical_cast<string>(id);

    boost::split(datetime, date, boost::is_any_of(" "));
    string str = "INSERT INTO users (uid, name, status, date, timeIn) VALUES ('" + uid + "', '" + name + "', '" + datetime[0] +
                   "', '" + datetime[1] + "')";

    bool f = q.exec(str.c_str());
    if (!f)
        throw string("Fail add new user in Database");
}

bool Database::incUser(unsigned id, const string &date) const
{
    bool retVal = false;
    vector<string> datetime;
    const string &uid = boost::lexical_cast<string>(id);
    boost::split(datetime, date, boost::is_any_of(" "));

    QSqlQuery query(string("SELECT timeIn,input FROM users WHERE uid='" + uid + "',date='" + datetime[0] + "'").c_str());

    while (query.next()) {
        unsigned hour;
        vector<string> time;

        string timeIn = query.value(0).toString().toStdString();
        unsigned input = query.value(1).toString().toUInt();

        boost::split(time, datetime[1], boost::is_any_of(":"));
        hour = boost::lexical_cast<unsigned>(time[0]);

        if (input == 0 && hour > 10 && hour < 15 && timeIn != datetime[1]) {
            QSqlQuery q;
            string str = "UPDATE users SET input='1',timeDin='" + datetime[1] + "' WHERE uid='" + uid + "',date='" + datetime[0]  + "'";

            bool f = q.exec(str.c_str());
            if (!f) {
                throw string("Can not update dinner time in Database.");
                break;
            }
            retVal = true;
        }
        /*
         * Add last visit
         */
        if (timeIn != datetime[1]) {
            QSqlQuery q;
            string str = "UPDATE users SET timeOut='" + datetime[1] + "' WHERE uid='" + uid + "',date='" + datetime[0]  + "'";

            bool f = q.exec(str.c_str());
            if (!f) {
                throw string("Can not update last time in Database.");
                break;
            }
        }
    }
    return retVal;
}

bool Database::checkUser(unsigned id, const string &date) const
{
    vector<string> datetime;
    const string &uid = boost::lexical_cast<string>(id);
    boost::split(datetime, date, boost::is_any_of(" "));

    QSqlQuery query(string("SELECT uid FROM users WHERE uid='" + uid + "',date='" + datetime[0] + "'").c_str());
    while (query.next()) {
        unsigned _uid = query.value(0).toUInt();
        if (id == _uid)
            return true;
    }
    return false;
}

void Database::close()
{
    _db.close();
}
