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
#include "configs.h"


int main(void)
{
    auto log = make_shared<Log>();
    auto cfg = make_shared<Configs>();
    auto checker = make_shared<Checker>(log, cfg);

    auto app = make_shared<App>(checker, log, cfg);
    return app->start();
}
