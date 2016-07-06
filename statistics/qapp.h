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

#ifndef QAPP_H
#define QAPP_H

#include <QApplication>


class IQApp
{
public:
    virtual int exec() = 0;
};


class QApp: public QApplication, public IQApp
{
public:
    QApp(int argc, char *argv[]);

    virtual int exec() override final;
};


#endif
