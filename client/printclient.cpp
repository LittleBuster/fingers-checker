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

#include "printclient.h"
#include <openssl/md5.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


string PrintClient::genSendData()
{
    string sname;
    vector<string> parts;
    boost::split(parts, _name, boost::is_any_of(" "));
    sname = parts[0] + " " + parts[1][0] + ". " + parts[2][0] + ".";

    strncpy(sdata.shortName, sname.c_str(), 50);
    strncpy(sdata.printer, _printer.c_str(), 10);
    sdata.uid = _uID;

    vector<string> datetime;
    boost::split(datetime, _time, boost::is_any_of(" "));
    strncpy(sdata.date, datetime[0].c_str(), 20);
    strncpy(sdata.time, datetime[1].c_str(), 20);

    /*
     * Generate hash
     */
    string outHash = "";
    string outHash2 = "";
    string inTxt = boost::lexical_cast<string>(_uID) + _name + _printer + _devName + _time;
    unsigned char result[MD5_DIGEST_LENGTH];
 
    MD5((unsigned char *)inTxt.c_str(), inTxt.size(), result);
    for (size_t i = 0; i < 8; i++) {
        char s[5];
        sprintf(s, "%02X", result[i]);
        outHash += string(s);
    }
    strncpy(sdata.hash1, outHash.c_str(), 8);

    for (size_t i = 8; i < 16; i++) {
        char s[5];
        sprintf(s, "%02X", result[i]);
        outHash2 += string(s);
    }
    strncpy(sdata.hash2, outHash2.c_str(), 8);

    return outHash + outHash2;
}

void PrintClient::sendData()
{
    send(&sdata, sizeof(sdata));
}
