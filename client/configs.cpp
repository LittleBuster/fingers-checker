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
        wc.devIps[0] = pt.get<string>("Web.DevIP1");
        wc.devIps[1] = pt.get<string>("Web.DevIP2");
        wc.devIps[2] = pt.get<string>("Web.DevIP3");
        wc.devNames[0] = pt.get<string>("Web.DevName1");
        wc.devNames[1] = pt.get<string>("Web.DevName2");
        wc.devNames[2] = pt.get<string>("Web.DevName3");
        wc.printIps[0] = pt.get<string>("Web.PrintIP1");
        wc.printIps[1] = pt.get<string>("Web.PrintIP2");
        wc.printIps[2] = pt.get<string>("Web.PrintIP3");
        wc.printNames[0] = pt.get<string>("Web.PrintName1");
        wc.printNames[1] = pt.get<string>("Web.PrintName2");
        wc.printNames[2] = pt.get<string>("Web.PrintName3");
        sc.ip = pt.get<string>("Server.IP");
        sc.port = pt.get<unsigned>("Server.Port");
        cc.interval = pt.get<unsigned>("Checker.Interval");
        cc.devInterval = pt.get<unsigned>("Checker.DeviceInterval");
        tc.id = pt.get<unsigned>("Telegram.ChatID");
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
