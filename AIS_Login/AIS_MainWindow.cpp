#include "AIS_MainWindow.h"



User::User(QString login, QString password, QString type) {
	Login = login;
	Password = password;
	Type = type;
}

Person::Person(QString name, QString surname, QString age) {
	Name = name;
	Surname = surname;
	Age = age;
}

Employee::Employee(QString login, QString password, QString type, QString name, QString surname, QString age, QString position, QVector<Subject*> subject ) : User(login, password, type), Person(name, surname, age) {
	Position = position;
    Teaching_Subjects = subject;
}

Student::Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject*> sub) : User(login, password, type), Person(name, surname, age) {
	Year = year;
    Enrolled_Subjects = sub;
}

void Student::Enroll_Subject(Subject* subject) {
	Enrolled_Subject* enrolled_subject = new Enrolled_Subject(subject->Get_Name(), subject->Get_Study_Year(), subject->Get_Type(), QString("0"), 0);
	Enrolled_Subjects.append(enrolled_subject);
}

Phd_Student::Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject*> teaching_subjects, QVector<Enrolled_Subject*> enrolled_subjects, QString position)
    : Student(login, password, type, name, surname, age, year, enrolled_subjects), Employee(login, password, type, name, surname, age, position, teaching_subjects) {};


AIS_MainWindow::AIS_MainWindow(QWidget* parent) : QMainWindow(parent) {
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
		QStringList fields = line.split(';');
		User* user =  new User(fields[0], fields[1], fields[2]);
		if (user->Get_Type() == "Student") {
			User* student = new Student(fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], QVector<Enrolled_Subject*>());
			QSharedPointer<User> userSharedPointer(student);
			Users.append(userSharedPointer);
			QStringList en_sub = fields[7].split(',');
			for (int i = 0; i < en_sub.size(); i++) {
				Subject* enrolled_sub = Get_Subject(en_sub[i]);
				static_cast<Student*>(student)->Enroll_Subject(enrolled_sub);
			};
			
		}
		delete user;
	}
	Print_Users();
	file.close();
}

void AIS_MainWindow::Load_Subjects(){
	QFile file("predmety_1.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	
	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split(',');
		Subject *subject = new Subject(fields[0], fields[1], fields[2]);
		Subjects.append(subject);
	}
	file.close();
}

void AIS_MainWindow::Print_Subjects() {
	for (int i = 0; i < Subjects.size(); i++) {
		qDebug() << Subjects[i]->Get_Name();
	}
}

Subject* AIS_MainWindow::Get_Subject(QString name) {
	for (int i = 0; i < Subjects.size(); i++) {
		if (Subjects[i]->Get_Name() == name) {
			return Subjects[i];
		}
	}
}

void AIS_MainWindow::Print_Users() {
	for (int i = 0; i < Users.size(); i++) {
		QSharedPointer<User> user = Users[i];
		qDebug() << user->Get_Name() << user->Get_Surname() << user->Get_Age() << user->Get_Login() << user->Get_Password() << user->Get_Type();
	}
}