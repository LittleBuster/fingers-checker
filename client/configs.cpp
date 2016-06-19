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

#include "configs.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


void Configs::load(const string &filename)
{
    std::ifstream ifs;
    boost::property_tree::ptree pt;

    ifs.open(filename);
    if (!ifs.is_open())
        throw string("File not found.");

    try {
        boost::property_tree::read_json(ifs, pt);
    }
    catch (...) {
        ifs.close();
        throw string("Fail parsing json file.");
    }

    try {
        wc.username = pt.get<string>("Web.User");
        wc.passwd = pt.get<string>("Web.Passwd");
        wc.pages = pt.get<unsigned>("Web.Pages");
        wc.devices[0] = pt.get<string>("Web.Dev1");
        wc.devices[1] = pt.get<string>("Web.Dev2");
        wc.devices[2] = pt.get<string>("Web.Dev3");
        wc.printers[0] = pt.get<string>("Web.Print1");
        wc.printers[1] = pt.get<string>("Web.Print2");
        wc.printers[2] = pt.get<string>("Web.Print3");
        sc.ip = pt.get<string>("Server.IP");
        sc.port = pt.get<unsigned>("Server.Port");
        cc.interval = pt.get<unsigned>("Checker.Interval");
        tc.key = pt.get<string>("Telegram.Key");
        dbc.ip = pt.get<string>("Database.IP");
        dbc.user = pt.get<string>("Database.User");
        dbc.passwd = pt.get<string>("Database.Passwd");
        dbc.base = pt.get<string>("Database.Base");
    }
    catch (...) {
        ifs.close();
        throw string("Fail reading configs values.");
    }
    ifs.close();
}
