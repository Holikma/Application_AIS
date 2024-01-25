#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AIS_Login.h"
#include "AIS_MainWindow.h"
#include <QMessageBox>

class AIS_Login : public QMainWindow{
    Q_OBJECT

    private:
        Ui::AIS_LoginClass ui;
        AIS_MainWindow* mainWindow = new AIS_MainWindow();
    
    private slots:
        void Check_Login();

    public:
        AIS_Login(QWidget *parent = nullptr);
        ~AIS_Login();
        void Set_Environment();
        void Set_Users();
       

};
