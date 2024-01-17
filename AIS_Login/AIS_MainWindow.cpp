#include "AIS_MainWindow.h"

AIS_MainWindow::AIS_MainWindow(QWidget *parent) : QMainWindow(parent){
	ui.setupUi(this);
}

AIS_MainWindow::~AIS_MainWindow()
{}

void AIS_MainWindow::Load_Users() {
	QFile file("Users.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;


	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split(',');
	}
	file.close();
}

void AIS_MainWindow::Load_Subjects(){
	QFile file("predmety_1.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split(';');
		Subject subject = Subject(fields[0], fields[1], fields[2]);
		Subjects.append(subject);
	}
	Print_Subjects();
	file.close();
}

void AIS_MainWindow::Print_Users() {
	for (int i = 0; i < Get_Users().size(); i++) {
		qDebug() << Get_Users()[i]->Get_Name() << Get_Users()[i]->Get_Surname();
	}
}

void AIS_MainWindow::Print_Subjects() {
	for (int i = 0; i < Get_Subjects().size(); i++) {
		qDebug() << Get_Subjects()[i].Get_Name();
	}
}

User AIS_MainWindow::Get_User(QString login) {
	for (int i = 0; i < Get_Users().size(); i++) {
		if (Get_Users()[i]->Get_Name() == login) {
			return *Get_Users()[i];
		}
	}
}