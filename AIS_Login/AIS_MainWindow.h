#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"

class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;


	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();

};
