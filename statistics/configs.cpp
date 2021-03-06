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
        psc.ip = pt.get<string>("PrintServer.IP");
        psc.port = pt.get<unsigned>("PrintServer.Port");
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
