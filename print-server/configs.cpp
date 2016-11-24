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


string Configs::readString(ifstream &is) const
{
    char line[255];
    bool isFind = false;
    string out = "";

    while (1) {
        is.getline(line, 255);
        if (line[0] != '#' || line[0] != '/' || line[0] != '\n')
            break;
    }

    for (const auto &sym : line) {
        if (sym == '=') {
            isFind = true;
            continue;
        }
        if (isFind && sym != ' ')
            out += sym;
    }
    return out;
}

int Configs::readInt(ifstream &is) const
{
    char line[255];
    bool isFind = false;
    string out = "";

    while (1) {
        is.getline(line, 255);
        if (line[0] != '#' || line[0] != '/' || line[0] != '\n')
            break;
    }

    for (const auto &sym : line) {
        if (sym == '=') {
            isFind = true;
            continue;
        }
        if (isFind && sym != ' ')
            out += sym;
    }
    return atoi(out.c_str());
}


void Configs::load(const string &filename)
{
    ifstream ifs;

    ifs.open(filename);
    if (!ifs.is_open())
        throw string("File not found.");

    try {
        sc.port = static_cast<unsigned>(readInt(ifs));
        sc.clients = static_cast<unsigned>(readInt(ifs));
    }
    catch (...) {
        ifs.close();
        throw string("Fail reading values.");
    }
    ifs.close();
}
