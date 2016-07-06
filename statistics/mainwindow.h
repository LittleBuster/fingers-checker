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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "configs.h"
#include "tcpclient.h"
#include "log.h"
#include <memory>

using namespace std;
namespace Ui { class MainWindow; }

typedef struct {
    unsigned uid;
    char shortName[50];
    char printer[10];
    char time[40];
    char hash[32];
} SendData;


class IMainWindow
{
public:
    virtual void show() = 0;
};


class MainWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    const shared_ptr<IDatabase> _db;
    const shared_ptr<IConfigs> _cfg;
    const shared_ptr<ILog> _log;
    const shared_ptr<ITcpClient> _client;
    User _user;
    bool _isExists;
    unsigned _userID;
    string _userName;

public:
    explicit MainWindow(const shared_ptr<IDatabase> &db, const shared_ptr<IConfigs> &cfg, const shared_ptr<ILog> &log,
                        const shared_ptr<ITcpClient> &client,
                        QWidget *parent = 0);
    ~MainWindow();

    virtual void show() override final { QMainWindow::show(); }
private slots:
    void on_pushButton_clicked();
    void on_pbPrint_clicked();
    void on_pushButton_2_clicked();
};


#endif
