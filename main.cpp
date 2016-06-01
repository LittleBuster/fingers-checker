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

#include "app.h"
#include "log.h"
#include "database.h"
#include <memory>
#include <QApplication>


int main(int argc, char *argv[])
{
    auto qapp = make_shared<QApplication>(argc, argv);
    auto log = make_shared<Log>();
    auto db = make_shared<Database>();
    auto checker = make_shared<Checker>(db, log);

    auto app = make_shared<App>(checker);
    return app->start();
}
