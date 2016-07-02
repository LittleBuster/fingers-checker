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

#include "printserver.h"
#include <QTextDocument>
#include <QPainter>
#include <QApplication>
#include <iostream>
#include <QDebug>
#include <fstream>

using namespace std;

void PrintServer::newSession(shared_ptr<ITcpClient> client)
{
    RecvData rdata;
    string h1 = "";
    string h2 = "";

    try {
        client->recv(&rdata, sizeof(rdata));
    }
    catch(const string &err) {
        _mtx.lock();
        _log->local("Error receiving data", LOG_ERROR);
        _mtx.unlock();
    }

    QString str(rdata.shortName);
    QStringList slist = str.split(' ');
    QString out = slist[0] + " " + QString(slist[1][0]) + "." + QString(slist[2][0]) + ".";

    for (size_t i = 0; i < 16; i++)
        h1 += rdata.hash[i];
    for (size_t i = 16; i < 32; i++)
        h2 += rdata.hash[i];

    qDebug() << "ID: " << rdata.uid;
    qDebug() << "Name: " << out;
    qDebug() << "Date: " << rdata.time;
    qDebug() << "Printer: " << rdata.printer;
    qDebug() << "Hash1: " << h1.c_str();
    qDebug() << "Hash2: " << h2.c_str();

    try {
        _mtx.lock();
        printTicket(out, rdata.printer, rdata.time, h1, h2);
        _mtx.unlock();
    }
    catch (...) {
       _log->local("Error printing ticket.", LOG_ERROR);
    }
}

void PrintServer::acceptError() const
{
    _log->local("Fail accepting new client.", LOG_ERROR);
}

PrintServer::PrintServer(const shared_ptr<ILog> &log)
{
    _log = log;
}

void PrintServer::printTicket(const QString &shortName, const string &printName, const string &time, const string &hash, const string &hash2)
{
    _printer.setPrinterName(printName.c_str());
    QPainter painter;
    painter.begin(&_printer);
    painter.setPen( Qt::black );    
    QFont font = QApplication::font();
    font.setPixelSize(18);

    painter.setFont(font);
    QRect rect = QRect(5, 5, 265, 40);
    painter.drawText(rect, Qt::AlignCenter, "Талон на питание\nООО ПКФ «Строп»" );
    rect.translate(0, 1);

    QTextDocument doc;
    doc.setDefaultFont(font);
    doc.setHtml( QString("<p>===================<br>ФИО: " + shortName + "<br>Сумма: 100 руб.<br>===================<br>"
                 "Дата: " + QString(time.c_str()) + "<br>"
                 "Код: " + QString(hash.c_str()) + "<br>" +
                 QString(("     " + hash2).c_str()) +
                 "</p>"));

    rect.translate( 0, rect.height()+10 );
    rect.setHeight( 230 );
    doc.setTextWidth( rect.width() );
    painter.save();
    painter.translate( rect.topLeft() );
    doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );
    painter.restore();
    painter.end();
}
