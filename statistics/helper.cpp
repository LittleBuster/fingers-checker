/* Fingers Statistics application
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence 3
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#include "helper.h"
#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

string dateTimeToNum(const boost::posix_time::ptime &time)
{
    vector<string> dt;
    vector<string> d;

    const string &datetime = boost::lexical_cast<string>(time);
    boost::split(dt, datetime, boost::is_any_of(" "));
    boost::split(d, dt[0], boost::is_any_of("-"));

    string out = d[0] + "-";

    if (d[1] == "Jan")
        out += "1";
    if (d[1] == "Feb")
        out += "2";
    if (d[1] == "Mar")
        out += "3";
    if (d[1] == "Apr")
        out += "4";
    if (d[1] == "May")
        out += "5";
    if (d[1] == "Jun")
        out += "6";
    if (d[1] == "Jul")
        out += "7";
    if (d[1] == "Aug")
        out += "8";
    if (d[1] == "Sep")
        out += "9";
    if (d[1] == "Oct")
        out += "10";
    if (d[1] == "Nov")
        out += "11";
    if (d[1] == "Dec")
        out += "12";

    out += "-" + d[2] + " " + dt[1];
    return out;
}
