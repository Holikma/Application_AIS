#include "AIS_MainWindow.h"

User::User(QString login, QString password, QString type) {
	Login = login;
	Password = password;
	Type = type;
}

Person::Person(QString login, QString password, QString type, QString name, QString surname, QString age) : User(login, password, type) {
	Name = name;
	Surname = surname;
	Age = age;
}

Employee::Employee(QString login, QString password, QString type, QString name, QString surname, QString age, QString position, QVector<Subject*> subject ) : Person(login, password, type, name, surname, age) {
	Position = position;
    Teaching_Subjects = subject;
}

Student::Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject*> sub) : Person(login, password, type, name, surname, age) {
	Year = year;
    Enrolled_Subjects = sub;
}

Phd_Student::Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject*> teaching_subjects, QVector<Enrolled_Subject*> enrolled_subjects)
	: Student(login, password, type, name, surname, age, year, enrolled_subjects) {
	Teaching_Subjects = teaching_subjects;
};

void Student::Enroll_Subject(Subject* subject) {
	Enrolled_Subject* enrolled_subject = new Enrolled_Subject(subject->Get_Name(), subject->Get_Study_Year(), subject->Get_Type(), subject->Get_Has_Teacher(), "-", 0);
	Enrolled_Subjects.append(enrolled_subject);
}

void Phd_Student::Teach_Subject(Subject* subject) {
	Teaching_Subjects.append(subject);
}

void Employee::Teach_Subject(Subject* subject) {
	Teaching_Subjects.append(subject);
}

AIS_MainWindow::AIS_MainWindow(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);
	connect(ui.Button_Generate_Report, SIGNAL(clicked()), this, SLOT(Generate_Report()));
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
			if (fields.size() > 7) {
				QStringList en_sub = fields[7].split(',');
				for (int i = 0; i < en_sub.size(); i++) {
					student->Enroll_Subject(Get_Subject(en_sub[i]));
				}
			}
			QSharedPointer<User> userSharedPointer(student);
			Users.append(userSharedPointer);
		}

		else if(user->Get_Type() == "PhD_Student") {
			User* phd_student = new Phd_Student(fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], QVector<Subject*>(), QVector<Enrolled_Subject*>());
			if (fields.size() > 7) {
				QStringList te_sub = fields[7].split(',');
				for (int i = 0; i < te_sub.size(); i++) {
					phd_student->Teach_Subject(Get_Subject(te_sub[i]));
				}
			}
			if (fields.size() > 8) {
				QStringList en_sub = fields[8].split(',');
				for (int i = 0; i < en_sub.size(); i++) {
					phd_student->Enroll_Subject(Get_Subject(en_sub[i]));
				}
			}
			QSharedPointer<User> userSharedPointer(phd_student);
			Users.append(userSharedPointer);
		}

		else if(user->Get_Type() == "Teacher") {
			User* employee = new Employee(fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], QVector<Subject*>());
			if (fields.size() > 7) {
				QStringList te_sub = fields[7].split(',');
				for (int i = 0; i < te_sub.size(); i++) {
					employee->Teach_Subject(Get_Subject(te_sub[i]));
				}
			}
			QSharedPointer<User> userSharedPointer(employee);
			Users.append(userSharedPointer);
		}
		else if(user->Get_Type() == "Admin"){
			User* admin = new Employee(fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], QVector<Subject*>());
			if (fields.size() > 7) {
				QStringList te_sub = fields[7].split(',');
				for (int i = 0; i < te_sub.size(); i++) {
					admin->Teach_Subject(Get_Subject(te_sub[i]));
				}
			}
			QSharedPointer<User> userSharedPointer(admin);
			Users.append(userSharedPointer);
		}
		else {
			continue;
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
		Subject *subject = new Subject(fields[0], fields[1], fields[2], false);
		Subjects.append(subject);
	}
	file.close();
}

void AIS_MainWindow::Print_Subjects() {
	for (int i = 0; i < Subjects.size(); i++) {
		qDebug() << Subjects[i];
	}
}

Subject* AIS_MainWindow::Get_Subject(QString name) {
	for (int i = 0; i < Subjects.size(); i++) {
		if (Subjects[i]->Get_Name() == name) {
			return Subjects[i];
		}
	}
	return nullptr;
}

void AIS_MainWindow::Print_Users() {
	for (int i = 0; i < Users.size(); i++) {
		QSharedPointer<User> user = Users[i];
		qDebug() << "Personal info: " << user->Get_Name() << user->Get_Surname() << user->Get_Age();
		qDebug() << "Credentials: " << user->Get_Login() << user->Get_Password() << user->Get_Type();
		qDebug() << "Enrolled Subjects: " << user->Get_Enrolled_Subjects();
		qDebug() << "Teaching Subjects:" << user->Get_Teaching_Subjects();
		if(user->Get_Type() == "Student") {
			qDebug() << "Year: " << user->Get_Year();
		}
		else if (user->Get_Type() == "Teacher" || user->Get_Type() == "Admin") {
			qDebug() << "Position: " << user->Get_Position();
		}
		else if (user->Get_Type() == "PhD_Student") {
			qDebug() << "Year: " << user->Get_Year();
		}
		qDebug() << "----------------------------------";
	}
}

User* AIS_MainWindow::Get_User(QString login) {
	for (int i = 0; i < Users.size(); i++) {
		if (Users[i]->Get_Login() == login) {
			return Users[i].data();
		}
	}
	return nullptr;
}

void AIS_MainWindow::Set_Student_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, true);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, false);
	ui.TabWidget->setTabVisible(3, false);
	Table_Subjects_Available(user);
	ui.Line_Current_User->setText(user->Get_Name() + " " + user->Get_Surname());
	ui.Line_Current_User->setEnabled(false);

}

void AIS_MainWindow::Set_Teacher_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, false);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, false);
	ui.Line_Current_User->setText(user->Get_Name() + " " + user->Get_Surname());
	ui.Line_Current_User->setEnabled(false);


}

void AIS_MainWindow::Set_Admin_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, false);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, true);
	ui.Line_Current_User->setText(user->Get_Name() + " " + user->Get_Surname());
	ui.Line_Current_User->setEnabled(false);
}

void AIS_MainWindow::Set_PhD_Student_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, true);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, false);
	ui.Line_Current_User->setText(user->Get_Name() + " " + user->Get_Surname());
	ui.Line_Current_User->setEnabled(false);
}

void AIS_MainWindow::Table_Subjects_Available(User* user) {
	ui.Table_Subjects_Available->setRowCount(Subjects.size());
	ui.Table_Subjects_Available->setColumnCount(3);
	ui.Table_Subjects_Available->setHorizontalHeaderLabels(QStringList() << "Name" << "Study Year" << "Type");
	for (int i = 0; i < Subjects.size(); i++) {
		ui.Table_Subjects_Available->setItem(i, 0, new QTableWidgetItem(Subjects[i]->Get_Name()));
		ui.Table_Subjects_Available->setItem(i, 1, new QTableWidgetItem(Subjects[i]->Get_Study_Year()));
		ui.Table_Subjects_Available->setItem(i, 2, new QTableWidgetItem(Subjects[i]->Get_Type()));
	}
}

void AIS_MainWindow::Generate_Report() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QFile file("User_Report.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
}

void AIS_MainWindow::closeEvent(QCloseEvent* event) {
	QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Do you want to save your changes before exiting?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	if (reply == QMessageBox::Save) {
		Save_Users_to_File();
	}
}

void AIS_MainWindow::Save_Users_to_File() {
	QFile file("Users_out.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	for (int i = 0; i < Users.size(); i++) {
		QSharedPointer<User> user = Users[i];
		out << user->Get_Login() << ";" << user->Get_Password() << ";" << user->Get_Type() << ";" << user->Get_Name() << ";" << user->Get_Surname() << ";" << user->Get_Age() << ";";
		if (user->Get_Type() == "Student") {
			out << user->Get_Year();
			if (user->Get_Enrolled_Subjects().size() > 0) {
				out << ";";
				for (int j = 0; j < user->Get_Enrolled_Subjects().size(); j++) {
					if (j == user->Get_Enrolled_Subjects().size() - 1) {
						out << user->Get_Enrolled_Subjects()[j]->Get_Name();
					}
					else {
						out << user->Get_Enrolled_Subjects()[j]->Get_Name() << ",";
					}
				}
			}
		}
		else if (user->Get_Type() == "PhD_Student") {
			out << user->Get_Year();
			if (user->Get_Teaching_Subjects().size() > 0) {
				out << ";";
				for (int j = 0; j < user->Get_Teaching_Subjects().size(); j++) {
					if (j == user->Get_Teaching_Subjects().size() - 1) {
						out << user->Get_Teaching_Subjects()[j]->Get_Name();
					}
					else {
						out << user->Get_Teaching_Subjects()[j]->Get_Name() << ",";
					}
				}
			}
			if (user->Get_Enrolled_Subjects().size() > 0) {
				out << ";";
				for (int j = 0; j < user->Get_Enrolled_Subjects().size(); j++) {
					if (j == user->Get_Enrolled_Subjects().size() - 1) {
						out << user->Get_Enrolled_Subjects()[j]->Get_Name();
					}
					else {
						out << user->Get_Enrolled_Subjects()[j]->Get_Name() << ",";
					}
				}
			}
		}
		else if (user->Get_Type() == "Teacher") {
			out << user->Get_Position();
			if (user->Get_Teaching_Subjects().size() > 0) {
				out << ";";
				for (int j = 0; j < user->Get_Teaching_Subjects().size(); j++) {
					if (j == user->Get_Teaching_Subjects().size() - 1) {
						out << user->Get_Teaching_Subjects()[j]->Get_Name();
					}
					else {
						out << user->Get_Teaching_Subjects()[j]->Get_Name() << ",";
					}
				}
			}
		}
		else if (user->Get_Type() == "Admin") {
			out << user->Get_Position();
			if (user->Get_Teaching_Subjects().size() > 0) {
				out << ";";
				for (int j = 0; j < user->Get_Teaching_Subjects().size(); j++) {
					if (j == user->Get_Teaching_Subjects().size() - 1) {
						out << user->Get_Teaching_Subjects()[j]->Get_Name();
					}
					else {
						out << user->Get_Teaching_Subjects()[j]->Get_Name() << ",";
					}
				}
			}
		}
		if (i != Users.size() - 1) {
			out << "\n";
		}
	}
}