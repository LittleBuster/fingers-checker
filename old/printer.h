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

#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <string>
#include <QtPrintSupport/QPrinter>

using namespace std;


class IPrinter
{
public:
    virtual void init(const string &printerName)=0;
    virtual void print(const string &name, unsigned id, const string& data)=0;
};


class Printer: public IPrinter
{
private:
    QPrinter _printer;

public:
    void init(const string &printerName);

    void print(const string &name, unsigned id, const string& data);
};


#endif
