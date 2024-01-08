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


		qDebug() << fields[0];
		if (fields[0] == "1") {
			QMap<QString, QString> temp;
			temp.insert("name", fields[3]);
			temp.insert("surname", fields[4]);
			temp.insert("Age", fields[5]);
			Users.append(new Student(fields[1], fields[2], temp, fields[6]));
			qDebug() << "name: " << Users[0]->Get_Name();
		}
	}
	file.close();
	qDebug() << "Users: ";
	for (int i = 0; i < Get_Users().size(); i++) {
		qDebug() << Get_Users()[i]->Get_Name();
	}
}
