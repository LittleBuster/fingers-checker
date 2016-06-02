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

#include "printer.h"
#include <QTextDocument>
#include <QPainter>
#include <QApplication>


void Printer::init(const string &printerName)
{
    _printer.setPrinterName(printerName.c_str());
}

void Printer::print(const string &name, unsigned id, const string &data)
{
    QPainter painter;
    painter.begin(&_printer);
    painter.setPen( Qt::black );

    QPoint point = QPoint( 10, 20 );
    painter.drawText( point, "You can draw text from a point..." );
    painter.drawLine( point+QPoint(-5, 0), point+QPoint(5, 0) );
    painter.drawLine( point+QPoint(0, -5), point+QPoint(0, 5) );

    QRect rect = QRect(10, 30, 180, 20);
    painter.drawText( rect, Qt::AlignCenter, "...or you can draw it inside a rectangle." );
    painter.drawRect( rect );

    rect.translate( 0, 30 );
    QFont font = QApplication::font();
    font.setPixelSize( rect.height() );
    painter.setFont( font );

    painter.drawText( rect, Qt::AlignRight, "Right." );
    painter.drawText( rect, Qt::AlignLeft, "Left." );
    painter.drawRect( rect );

    rect.translate( 0, rect.height()+10 );
    rect.setHeight( QFontMetrics( font ).height() );

    painter.drawText( rect, Qt::AlignRight, "Right." );
    painter.drawText( rect, Qt::AlignLeft, "Left." );
    painter.drawRect( rect );

    QTextDocument doc;
    doc.setHtml( "<p>A QTextDocument can be used to present formatted text "
                 "in a nice way.</p>"
                 "<p align=center>It can be <b>formatted</b> "
                 "<font size=+2>in</font> <i>different</i> ways.</p>"
                 "<p>The text can be really long and contain many "
                 "paragraphs. It is properly wrapped and such...</p>" );

    rect.translate( 0, rect.height()+10 );
    rect.setHeight( 160 );
    doc.setTextWidth( rect.width() );
    painter.save();
    painter.translate( rect.topLeft() );
    doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );
    painter.restore();
    painter.drawRect( rect );

    rect.translate( 0, 160 );
    rect.setHeight( doc.size().height()-160 );
    painter.setBrush( Qt::gray );
    painter.drawRect( rect );
    painter.end();
}
