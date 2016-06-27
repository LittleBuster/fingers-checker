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
    strncpy(sdata.shortName, _name.c_str(), 50);
    strncpy(sdata.printer, _printer.c_str(), 10);
    sdata.uid = _uID;
    strncpy(sdata.time, _time.c_str(), 20);

    /*
     * Generate hash
     */
    string outHash = "";
    string inTxt = boost::lexical_cast<string>(_uID) + _name + _printer + _devName + _time;
    unsigned char result[MD5_DIGEST_LENGTH];

    MD5(reinterpret_cast<const unsigned char *>(inTxt.c_str()), inTxt.size(), result);
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
        char s[5];
        memset(s, 0x00, 5);
        sprintf(s, "%02X", result[i]);
        outHash += string(s);
    }
    strncpy(sdata.hash, outHash.c_str(), 32);
cout << outHash << endl;
    return outHash;
}

void PrintClient::sendData()
{
    send(&sdata, sizeof(sdata));
}
