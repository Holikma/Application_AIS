#include "AIS_MainWindow.h"

void User::Get_Info(){
	qDebug() << "Login: " << this->Login;
	qDebug() << "Password: " << this->Password;
	qDebug() << "Access: " << this->Access;
	for (auto i = Personal.begin(); i != Personal.end(); i++){
		qDebug() << i.key() << ": " << i.value();
	}
}

void Student::Get_Enrolled_Subjects(){
	for (auto i = Enrolled_Subjects.begin(); i != Enrolled_Subjects.end(); i++){
		qDebug() << i;
	}
}

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
	QString line = in.readLine();
	file.close();
}
