#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AIS_Login.h"
#include "AIS_MainWindow.h"


class AIS_Login : public QMainWindow{
    Q_OBJECT

    private:
        Ui::AIS_LoginClass ui;
        AIS_MainWindow* mainWindow = new AIS_MainWindow();
    private slots:


    public:
        AIS_Login(QWidget *parent = nullptr);
        ~AIS_Login();

};
