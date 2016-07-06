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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <tuple>
#include <openssl/md5.h>
#include "helper.h"

using namespace std;


MainWindow::MainWindow(const shared_ptr<IDatabase> &db, const shared_ptr<IConfigs> &cfg, const shared_ptr<ILog> &log, const shared_ptr<ITcpClient> &client, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), _db(std::move(db)), _cfg(std::move(cfg)), _log(std::move(log)),
    _client(std::move(client))
{
    ui->setupUi(this);
    QStringList labels;
    labels.push_back("id");
    labels.push_back("ФИО");
    ui->twResult->setHorizontalHeaderLabels(labels);
    ui->twResult->horizontalHeader()->resizeSection(0, 50);
    ui->twResult->horizontalHeader()->resizeSection(1, 720);

    QStringList labels2;
    labels2.push_back("id");
    labels2.push_back("ФИО");
    labels2.push_back("Время");
    labels2.push_back("Устройство");
    labels2.push_back("Код");
    ui->twDev->setHorizontalHeaderLabels(labels2);
    ui->twDev->horizontalHeader()->resizeSection(0, 40);
    ui->twDev->horizontalHeader()->resizeSection(1, 280);
    ui->twDev->horizontalHeader()->resizeSection(2, 150);
    ui->twDev->horizontalHeader()->resizeSection(3, 85);
    ui->twDev->horizontalHeader()->resizeSection(4, 310);

    ui->pushButton->setIcon(QIcon("icons\\search.png"));
    ui->pbPrint->setIcon(QIcon("icons\\printing.png"));
    this->setWindowIcon(QIcon("icons\\text.png"));

    ui->comboBox->addItem("CP1");
    ui->comboBox->addItem("CP2");
    ui->comboBox->addItem("CP3");
    ui->cbDevice->addItem("office");
    ui->cbDevice->addItem("production");
    ui->cbDevice->addItem("accounting");
    ui->pbPrint->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    const auto &dbc = _cfg->getDatabaseCfg();

    string lblUser = "";
    ui->pbPrint->setDisabled(false);
    try {
        _db->connect(dbc.ip, dbc.user, dbc.passwd, dbc.base);
    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        QMessageBox::critical(this, "Ошибка", "Ошибка соединения с базой данных", QMessageBox::Ok);
        return;
    }
    try {
        tuple<vector<ShortUserData>, bool> res = _db->checkSurname(ui->leSurname->text().toStdString());
        if (!get<1>(res)) {
            _db->close();
            QMessageBox::warning(this, "Поиск", "Данный пользователь не найден.", QMessageBox::Ok);
            return;
        }
        ui->twResult->setRowCount(0);
        for (const ShortUserData &user: get<0>(res)) {
            ui->twResult->setRowCount(ui->twResult->rowCount() + 1);

            QTableWidgetItem *item = new QTableWidgetItem(boost::lexical_cast<string>(user.id).c_str());
            item->setTextAlignment(Qt::AlignCenter);
            ui->twResult->setItem(ui->twResult->rowCount() - 1, 0, item);

            QTableWidgetItem *item2 = new QTableWidgetItem(user.name.c_str());
            ui->twResult->setItem(ui->twResult->rowCount() - 1, 1, item2);

            _userID = user.id;
            _userName = user.name;
        }

    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
    }
    try {
       tuple<vector<User>, bool> res = _db->checkUser(_userID);
       ui->twDev->setRowCount(0);
       lblUser = "";
       if (!get<1>(res)) {
           _db->close();
           _isExists = false;
           lblUser = "<font color=\"red\">Данный пользователь не отметился.</font>";
           ui->lbRes->setText(lblUser.c_str());
           QMessageBox::warning(this, "Поиск", "Данный пользователь не отметился.", QMessageBox::Ok);
           return;
       }
       for (const User &usr: get<0>(res)) {
           ui->twDev->setRowCount(ui->twDev->rowCount() + 1);
           QTableWidgetItem *item3 = new QTableWidgetItem(boost::lexical_cast<string>(usr.id).c_str());
           QTableWidgetItem *item4 = new QTableWidgetItem(usr.name.c_str());
           QTableWidgetItem *item5 = new QTableWidgetItem(usr.time.c_str());
           QTableWidgetItem *item6 = new QTableWidgetItem(usr.device.c_str());
           QTableWidgetItem *item7 = new QTableWidgetItem(usr.code.c_str());
           item3->setTextAlignment(Qt::AlignCenter);
           ui->twDev->setItem(ui->twDev->rowCount() - 1, 0, item3);
           ui->twDev->setItem(ui->twDev->rowCount() - 1, 1, item4);
           ui->twDev->setItem(ui->twDev->rowCount() - 1, 2, item5);
           ui->twDev->setItem(ui->twDev->rowCount() - 1, 3, item6);
           ui->twDev->setItem(ui->twDev->rowCount() - 1, 4, item7);
           lblUser = "<font color=\"green\">Данный пользователь отметился.</font>";
           _user = usr;
           _isExists = true;
       }
    }
    catch (const string& err) {
        _log->local(err, LOG_ERROR);
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
    }
    _db->close();
    ui->lbRes->setText(lblUser.c_str());
}

void MainWindow::on_pbPrint_clicked()
{
    SendData sdata;
    auto sc = _cfg->getPrintServerCfg();
    const auto &dbc = _cfg->getDatabaseCfg();

    if (!_isExists) {
        string hStr = "";
        string outHash = "";
        MD5_CTX ctx;
        unsigned char uhash[MD5_DIGEST_LENGTH];
        const auto &dt = boost::posix_time::second_clock::local_time();
        hStr = boost::lexical_cast<string>(_userID) + boost::lexical_cast<string>(dt.date()) + _userName;

        try {
            _db->connect(dbc.ip, dbc.user, dbc.passwd, dbc.base);
        }
        catch (const string &err) {
            _log->local(err, LOG_ERROR);
            QMessageBox::critical(this, "Ошибка", "Ошибка соединения с базой данных", QMessageBox::Ok);
            return;
        }
        int retVal = MD5_Init(&ctx);
        if (!retVal) {
            _db->close();
            _log->local("Ошибка инициализации MD5.", LOG_ERROR);
            QMessageBox::critical(this, "Ошибка", "Ошибка инициализации MD5.", QMessageBox::Ok);
            return;
        }

        retVal = MD5_Update(&ctx, reinterpret_cast<const void *>(hStr.c_str()), hStr.size());
        if (!retVal) {
            _db->close();
            _log->local("Ошибка обновления MD5.", LOG_ERROR);
            QMessageBox::critical(this, "Ошибка", "Ошибка обновления MD5.", QMessageBox::Ok);
            return;
        }
        retVal = MD5_Final(uhash, &ctx);
        if (!retVal) {
            _db->close();
            _log->local("Ошибка генерации MD5.", LOG_ERROR);
            QMessageBox::critical(this, "Ошибка", "Ошибка генерации MD5.", QMessageBox::Ok);
            return;
        }
        /*
         * Converting hash
         */
        for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
            char part[3];
            sprintf(part, "%02X", uhash[i]);
            outHash += string(part);
        }
        try {
            _db->addUser(_userID, _userName, ui->cbDevice->currentText().toStdString(),
                         dateTimeToNum(boost::posix_time::second_clock::local_time()), outHash);
        }
        catch (const string &err) {
            _log->local(err, LOG_ERROR);
            _db->close();
            QMessageBox::critical(this, "Ошибка", ("Adding user: " + err).c_str(), QMessageBox::Ok);
        }
        try {
            _client->connect(sc.ip, sc.port);
        }
        catch (const string &err) {
            _log->local(err, LOG_ERROR);
            _client->close();
            QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
            return;
        }
        try {
            strcpy(sdata.shortName, _userName.c_str());
            strcpy(sdata.printer, ui->comboBox->currentText().toStdString().c_str());
            strcpy(sdata.time, dateTimeToNum(boost::posix_time::second_clock::local_time()).c_str());
            strcpy(sdata.hash, outHash.c_str());
            sdata.uid = _userID;
            _client->send(&sdata, sizeof(sdata));
        }
        catch (const string &err) {
            _log->local(err, LOG_ERROR);
            _client->close();
            QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
            return;
        }
        _client->close();
        QMessageBox::information(this, "Готово", "Чек распечатан.", QMessageBox::Ok);
        return;
    }
    try {
        _client->connect(sc.ip, sc.port);
    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        _client->close();
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
        return;
    }
    try {
        strcpy(sdata.shortName, _user.name.c_str());
        strcpy(sdata.printer, ui->comboBox->currentText().toStdString().c_str());
        strcpy(sdata.time, _user.time.c_str());
        strcpy(sdata.hash, _user.code.c_str());
        sdata.uid = _user.id;
        _client->send(&sdata, sizeof(sdata));
    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        _client->close();
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
        return;
    }
    _client->close();
    QMessageBox::information(this, "Готово", "Чек распечатан.", QMessageBox::Ok);

}

void MainWindow::on_pushButton_2_clicked()
{
    const auto &dbc = _cfg->getDatabaseCfg();
    try {
        _db->connect(dbc.ip, dbc.user, dbc.passwd, dbc.base);
    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
        return;
    }

    try {
        unsigned count = _db->getMonthCount();
        ui->lbCount->setText(boost::lexical_cast<string>(count).c_str());
    }
    catch (const string &err) {
        _log->local(err, LOG_ERROR);
        _db->close();
        QMessageBox::critical(this, "Ошибка", err.c_str(), QMessageBox::Ok);
        return;
    }
    _db->close();
}
