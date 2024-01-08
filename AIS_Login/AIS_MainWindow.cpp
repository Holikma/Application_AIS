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
		switch (fields[0].toInt()) {
			case 1: {
				Student* student = new Student(fields[1], fields[2], fields[6]);
				student->Set_Personal_Info(fields[3], fields[4], fields[5]);
				Users.append(QSharedPointer<User>(student));
				break;
			}
			case 2: {
				Phd_Student* student = new Phd_Student(fields[1], fields[2], fields[6]);
				student->Set_Personal_Info(fields[3], fields[4], fields[5]);
				Users.append(QSharedPointer<User>(student));
				break;
			}
			case 3:{
				Teacher* teacher = new Teacher(fields[1], fields[2], fields[3]);
				Users.append(QSharedPointer<User>(teacher));
				break;
			}
			case 4:{
				Administrator* admin = new Administrator(fields[1], fields[2], fields[3]);
				Users.append(QSharedPointer<User>(admin));
				break;
			}
			default:
				break;
			}
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