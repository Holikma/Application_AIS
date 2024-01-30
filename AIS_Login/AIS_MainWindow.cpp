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
	Enrolled_Subject* enrolled_subject = new Enrolled_Subject(subject->Get_Name(), subject->Get_Study_Year(), subject->Get_Type(), subject->Get_Teacher(), { "-", "-", "-"}, 0, false);
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
	connect(ui.Button_Enroll, SIGNAL(clicked()), this, SLOT(Enroll_Subject()));
	connect(ui.Button_Teach, SIGNAL(clicked()), this, SLOT(Teach_Subject()));
	connect(ui.List_Teaching_Subjects, &QTableWidget::itemClicked, this, [this]() {List_Enrolled_Students(ui.List_Teaching_Subjects);});
	connect(ui.List_Teaching_Subjects, &QTableWidget::itemClicked, this, [this]() {List_Awaiting_Exam_Students(Get_Subject(ui.List_Teaching_Subjects->item(ui.List_Teaching_Subjects->currentRow(), 0)->text())); });
	connect(ui.Button_Sign_for_Exam, SIGNAL(clicked()), this, SLOT(Sign_for_Exam()));
	connect(ui.Button_Upload_UI, SIGNAL(clicked()), this, SLOT(Load_New_Subjects()));
	connect(ui.Button_Grade, SIGNAL(clicked()), this, SLOT(Grade_Student()));
}

AIS_MainWindow::~AIS_MainWindow()
{}

void AIS_MainWindow::Load_Users() {
	QFile file("Users_out.txt");
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
					Update_Database(Get_Subject(en_sub[i]), dynamic_cast<Student*>(student));
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
					Update_Database(Get_Subject(en_sub[i]), dynamic_cast<Student*>(phd_student));
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
		Subject* subject = new Subject(fields[0], fields[1], fields[2], "");
		if (fields.size() > 3) {
			subject->Set_Teacher(fields[3]);
		}
		Subjects.append(subject);
	}
	file.close();
}

void AIS_MainWindow::Load_Database() {
	for (int i = 0; i < Get_Users().size(); i++) {
		if (Users[i]->Get_Type() != "Student") {
			continue;
		}
		Student* student = dynamic_cast<Student*>(Users[i].data());
		QVector<Enrolled_Subject*> enrolledSubjects = student->Get_Enrolled_Subjects();
		for (Subject* subject : enrolledSubjects) {
			if (Database.contains(subject)) {
				Database[subject].append(student);
			}
			else {
				QVector<Student*> students;
				students.append(student);
				Database.insert(subject, students);
			}
		}
	}
}

void AIS_MainWindow::Print_Subjects() {
	for (int i = 0; i < Subjects.size(); i++) {
		qDebug() << Subjects[i];
	}
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

void AIS_MainWindow::Print_Database() {
	for (Subject* subject : Database.keys()) {
		qDebug() << subject->Get_Name();
		for (Student* student : Database[subject]) {
			qDebug() << student->Get_Name() << student->Get_Surname();
		}
		qDebug() << "----------------------------------";
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
	ui.TabWidget->setCurrentIndex(0);
	ui.Line_Current_User->setText(user->Get_Login());
	ui.Line_Current_User->setEnabled(false);
	Load_Marks_from_File();
	List_Subjects(user);
	List_Enrolled_Subjects(user);
	Set_Personal_Info(user);
	List_Signed_for_Exam_Subjects(user);

}

void AIS_MainWindow::Set_Teacher_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, false);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, false);
	ui.TabWidget->setCurrentIndex(1);
	ui.Line_Current_User->setText(user->Get_Login());
	ui.Line_Current_User->setEnabled(false);
	Load_Marks_from_File();
	List_Subjects_to_Teach();
	Set_Personal_Info(user);
	List_Teaching_Subjects(user);
	List_Enrolled_Subjects(user);
	List_Signed_for_Exam_Subjects(user);
}

void AIS_MainWindow::Set_Admin_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, false);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, true);
	ui.TabWidget->setCurrentIndex(1);
	ui.Line_Current_User->setText(user->Get_Login());
	ui.Line_Current_User->setEnabled(false);
	Set_Personal_Info(user);
	List_Subjects_to_Teach();
}

void AIS_MainWindow::Set_PhD_Student_Ui(User* user) {
	ui.TabWidget->setTabVisible(0, true);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, false);
	ui.TabWidget->setCurrentIndex(0);
	ui.Line_Current_User->setText(user->Get_Login());
	ui.Line_Current_User->setEnabled(false);
	Load_Marks_from_File();
	Set_Personal_Info(user);
	List_Subjects_to_Teach();
	List_Subjects(user);
	List_Teaching_Subjects(user);
	List_Enrolled_Subjects(user);
	List_Signed_for_Exam_Subjects(user);
}

void AIS_MainWindow::Set_Personal_Info(User* user) {
	ui.Line_Login_Info->setText(user->Get_Login());
	ui.Line_Login_Info->setEnabled(false);

	ui.Line_Password_Info->setText(user->Get_Password());
	ui.Line_Password_Info->setEnabled(false);

	ui.Line_Name->setText(user->Get_Name());
	ui.Line_Name->setEnabled(false);

	ui.Line_Surname->setText(user->Get_Surname());
	ui.Line_Surname->setEnabled(false);

	ui.Line_Age->setText(user->Get_Age());
	ui.Line_Age->setEnabled(false);

	ui.Line_Type->setText(user->Get_Type());
	ui.Line_Type->setEnabled(false);
}

void AIS_MainWindow::List_Subjects(User* user) {
	ui.List_Subjects->clear();
	ui.List_Subjects->setRowCount(0);
	ui.List_Subjects->setColumnCount(2);
	ui.List_Subjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Subjects->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Subjects->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Subjects->setShowGrid(false);
	ui.List_Subjects->verticalHeader()->hide();
	ui.List_Subjects->horizontalHeader()->hide();
	ui.List_Subjects->setColumnWidth(0, 300);
	ui.List_Subjects->setColumnWidth(1, 150);
	for (int i = 0; i < Subjects.size(); i++) {
		if (Subjects[i]->Get_Study_Year() == user->Get_Year()) {
			int newRow = ui.List_Subjects->rowCount();
			ui.List_Subjects->insertRow(newRow);
			ui.List_Subjects->setItem(newRow, 0, new QTableWidgetItem(Subjects[i]->Get_Name()));
			ui.List_Subjects->setItem(newRow, 1, new QTableWidgetItem(Subjects[i]->Get_Teacher()));
		}
	}
}

void AIS_MainWindow::Generate_Report() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QFile file(current_user->Get_Login() + "_Report.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	out << "Report for " << current_user->Get_Name() << " " << current_user->Get_Surname() << "\n";
	out << "----------------------------------\n";
	out << "Enrolled Subjects:\n";
	for (int i = 0; i < current_user->Get_Enrolled_Subjects().size(); i++) {
		out << current_user->Get_Enrolled_Subjects()[i]->Get_Name() << "\t" << current_user->Get_Enrolled_Subjects()[i]->Get_Mark(0) << "\t" << current_user->Get_Enrolled_Subjects()[i]->Get_Mark(1) << "\t" << current_user->Get_Enrolled_Subjects()[i]->Get_Mark(2) << "\n";
	}
	out << "----------------------------------\n";
	file.close();
	QMessageBox::information(this, "Report", "Report generated successfully!");
}

void AIS_MainWindow::closeEvent(QCloseEvent* event) {
	Save_Users_to_File();
	Save_Subjects_to_File();
	Save_Marks_to_File();
	Reset_UI();
}

void AIS_MainWindow::Reset_UI() {
	ui.Line_Login_Info->clear();
	ui.Line_Password_Info->clear();
	ui.Line_Name->clear();
	ui.Line_Surname->clear();
	ui.Line_Age->clear();
	ui.Line_Type->clear();
	ui.List_Subjects->clear();
	ui.List_Subjects_to_Teach->clear();
	ui.List_Enrolled_Students->clear();
	ui.List_Enrolled_Subjects->clear();
	ui.List_Teaching_Subjects->clear();
	ui.List_Subjects_for_Grading->clear();
	ui.List_Awaiting_Students_for_Exam->clear();
	ui.Line_Current_User->clear();
	ui.Line_Current_User->setEnabled(true);
	ui.TabWidget->setTabVisible(0, true);
	ui.TabWidget->setTabVisible(1, true);
	ui.TabWidget->setTabVisible(2, true);
	ui.TabWidget->setTabVisible(3, true);
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

void AIS_MainWindow::Enroll_Subject() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QString subject_name = ui.List_Subjects->item(ui.List_Subjects->currentRow(), 0)->text();
	Subject* subject = Get_Subject(subject_name);
	for(int i = 0; i < current_user->Get_Enrolled_Subjects().size(); i++) {
		if (current_user->Get_Enrolled_Subjects()[i]->Get_Name() == subject->Get_Name()) {
			return;
		}
	}
	current_user->Enroll_Subject(subject);
	List_Enrolled_Subjects(current_user);
	Update_Database(subject, dynamic_cast<Student*>(current_user));
}

void AIS_MainWindow::List_Enrolled_Subjects(User* user) {
	ui.List_Enrolled_Subjects->setRowCount(user->Get_Enrolled_Subjects().size());
	ui.List_Enrolled_Subjects->setColumnCount(7);
	ui.List_Enrolled_Subjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Enrolled_Subjects->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Enrolled_Subjects->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Enrolled_Subjects->setShowGrid(false);
	ui.List_Enrolled_Subjects->verticalHeader()->hide();
	ui.List_Enrolled_Subjects->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.List_Enrolled_Subjects->resizeColumnsToContents();
	ui.List_Enrolled_Subjects->setHorizontalHeaderLabels(QStringList() << "Name of the Enrolled Subject" << "Study Year" << "Type" << "First Attempt" << "Second Attempt" << "Third Attempt" << "Attempts Left");
	for (int i = 0; i < user->Get_Enrolled_Subjects().size(); i++) {
		ui.List_Enrolled_Subjects->setItem(i, 0, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Name()));
		ui.List_Enrolled_Subjects->setItem(i, 1, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Study_Year()));
		ui.List_Enrolled_Subjects->setItem(i, 2, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Type()));
		ui.List_Enrolled_Subjects->setItem(i, 3, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Mark(0)));
		ui.List_Enrolled_Subjects->setItem(i, 4, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Mark(1)));
		ui.List_Enrolled_Subjects->setItem(i, 5, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Mark(2)));
		ui.List_Enrolled_Subjects->setItem(i, 6, new QTableWidgetItem(QString::number(user->Get_Enrolled_Subjects()[i]->Get_Attempts())));
	}
}

void AIS_MainWindow::List_Subjects_to_Teach() {
	ui.List_Subjects_to_Teach->clear();
	ui.List_Subjects_to_Teach->setRowCount(0);
	ui.List_Subjects_to_Teach->setColumnCount(2);
	ui.List_Subjects_to_Teach->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Subjects_to_Teach->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Subjects_to_Teach->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Subjects_to_Teach->setShowGrid(false);
	ui.List_Subjects_to_Teach->verticalHeader()->hide();

	ui.List_Subjects_to_Teach->setHorizontalHeaderLabels(QStringList() << "Name" << "Teacher");
	ui.List_Subjects_to_Teach->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < Get_Subjects().size(); i++) {
		int newRow = ui.List_Subjects_to_Teach->rowCount();
		ui.List_Subjects_to_Teach->insertRow(newRow);
		ui.List_Subjects_to_Teach->setItem(newRow, 0, new QTableWidgetItem(Subjects[i]->Get_Name()));
		ui.List_Subjects_to_Teach->setItem(newRow, 1, new QTableWidgetItem(Subjects[i]->Get_Teacher()));
	}
}

void AIS_MainWindow::List_Teaching_Subjects(User* user) {
	ui.List_Teaching_Subjects->clear();
	ui.List_Teaching_Subjects->setRowCount(user->Get_Teaching_Subjects().size());
	ui.List_Teaching_Subjects->setColumnCount(2);
	ui.List_Teaching_Subjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Teaching_Subjects->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Teaching_Subjects->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Teaching_Subjects->setShowGrid(false);
	ui.List_Teaching_Subjects->verticalHeader()->hide();
	ui.List_Teaching_Subjects->horizontalHeader()->hide();
	ui.List_Teaching_Subjects->setHorizontalHeaderLabels(QStringList() << "Name" << "Teacher");
	ui.List_Teaching_Subjects->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < user->Get_Teaching_Subjects().size(); i++) {
		ui.List_Teaching_Subjects->setItem(i, 0, new QTableWidgetItem(user->Get_Teaching_Subjects()[i]->Get_Name()));
		ui.List_Teaching_Subjects->setItem(i, 1, new QTableWidgetItem(user->Get_Teaching_Subjects()[i]->Get_Teacher()));
	}
}

void AIS_MainWindow::List_Enrolled_Students(QTableWidget* sourceList) {
	ui.List_Enrolled_Students->clear();
	ui.List_Enrolled_Students->setRowCount(0);
	ui.List_Enrolled_Students->setColumnCount(1);
	ui.List_Enrolled_Students->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Enrolled_Students->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Enrolled_Students->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Enrolled_Students->setShowGrid(false);
	ui.List_Enrolled_Students->verticalHeader()->hide();
	ui.List_Enrolled_Students->horizontalHeader()->hide();
	Subject* subject = Get_Subject(sourceList->item(sourceList->currentRow(), 0)->text());
	for (int i = 0; i < Database[subject].size(); i++) {
		int newRow = ui.List_Enrolled_Students->rowCount();
		ui.List_Enrolled_Students->insertRow(newRow);
		ui.List_Enrolled_Students->setItem(newRow, 0, new QTableWidgetItem(Database[subject][i]->Get_Name() + " " + Database[subject][i]->Get_Surname()));
	}
}

void AIS_MainWindow::Teach_Subject() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QString subject_name = ui.List_Subjects_to_Teach->item(ui.List_Subjects_to_Teach->currentRow(), 0)->text();
	Subject* subject = Get_Subject(subject_name);
	if (subject->Get_Teacher() != "") {
		return;
	}
	subject->Set_Teacher(current_user->Get_Name() + " " + current_user->Get_Surname());
	current_user->Teach_Subject(subject);
	List_Enrolled_Subjects(current_user);
	List_Teaching_Subjects(current_user);
	List_Subjects_to_Teach();
}

void AIS_MainWindow::Update_Database(Subject* subject,Student* student) {
	if (Database.contains(subject)) {
		Database[subject].append(student);
	}
	else {
		QVector<Student*> students;
		students.append(student);
		Database.insert(subject,students);
	}
}

void AIS_MainWindow::Sign_for_Exam() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QString subject_name = ui.List_Enrolled_Subjects->item(ui.List_Enrolled_Subjects->currentRow(), 0)->text();
	Subject* subject = Get_Subject(subject_name);
	for (int i = 0; i < current_user->Get_Enrolled_Subjects().size(); i++) {
		if (current_user->Get_Enrolled_Subjects()[i]->Get_Name() == subject_name && current_user->Get_Enrolled_Subjects()[i]->Get_Attempts() < 3) {
			current_user->Get_Enrolled_Subjects()[i]->Set_Signed_for_Exam(true);
		}
	}
	List_Signed_for_Exam_Subjects(current_user);
}

void AIS_MainWindow::List_Signed_for_Exam_Subjects(User* user) {
	ui.List_Subjects_for_Grading->clear();
	ui.List_Subjects_for_Grading->setRowCount(0);
	ui.List_Subjects_for_Grading->setColumnCount(1);
	ui.List_Subjects_for_Grading->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Subjects_for_Grading->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Subjects_for_Grading->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Subjects_for_Grading->setShowGrid(false);
	ui.List_Subjects_for_Grading->verticalHeader()->hide();
	ui.List_Subjects_for_Grading->horizontalHeader()->hide();
	ui.List_Subjects_for_Grading->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < user->Get_Enrolled_Subjects().size(); i++) {
		if (user->Get_Enrolled_Subjects()[i]->Get_Signed_for_Exam() == 1) {
			int newRow = ui.List_Subjects_for_Grading->rowCount();
			ui.List_Subjects_for_Grading->insertRow(newRow);
			ui.List_Subjects_for_Grading->setItem(newRow, 0, new QTableWidgetItem(user->Get_Enrolled_Subjects()[i]->Get_Name()));
		}
	}
}

void AIS_MainWindow::Save_Marks_to_File() {
	for (int i = 0; i < Users.size(); i++) {
		User* user = Users[i].data();
		QFile file("Grades/" + user->Get_Login() + "_grades.csv");
		if (user->Get_Type() == "Student" || user->Get_Type() == "PhD_Student") {
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				return;
			QTextStream out(&file);
			QVector<Enrolled_Subject*> En_Sub = user->Get_Enrolled_Subjects();
			for (int i = 0; i < user->Get_Enrolled_Subjects().size(); i++) {
				out << En_Sub[i]->Get_Name() << ";" << En_Sub[i]->Get_Marks()[0] << ";" << En_Sub[i]->Get_Marks()[1] << ";" << En_Sub[i]->Get_Marks()[2] << ";" << En_Sub[i]->Get_Attempts() << ";" << En_Sub[i]->Get_Signed_for_Exam();
				if (i != user->Get_Enrolled_Subjects().size() - 1) {
					out << "\n";
				}
			}
		}
		file.close();
	}
}

void AIS_MainWindow::Load_Marks_from_File() {
	for (int i = 0; i < Users.size(); i++) {
		User* user = Users[i].data();
		if (user->Get_Type() == "Student" || user->Get_Type() == "PhD_Student") {
			QFile file("Grades/" + user->Get_Login() + "_grades.csv");
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
				return;
			QTextStream in(&file);
			while (!in.atEnd()) {
				QString line = in.readLine();
				QStringList fields = line.split(';');
				for (int i = 0; i < user->Get_Enrolled_Subjects().size(); i++) {
					if (user->Get_Enrolled_Subjects()[i]->Get_Name() == fields[0]) {
						user->Get_Enrolled_Subjects()[i]->Set_Marks({ fields[1], fields[2], fields[3] });
						user->Get_Enrolled_Subjects()[i]->Set_Attempts(fields[4].toInt());
						user->Get_Enrolled_Subjects()[i]->Set_Signed_for_Exam(fields[5].toInt());
					}
				}
			}
			file.close();
		}
	}
}

void AIS_MainWindow::Load_New_Subjects() {
	for (int i = 0; i < Subjects.size(); i++) {
		delete Subjects[i];
	}
	Subjects.clear();
	for (QSharedPointer<User> user : Users) {
		if (user->Get_Type() == "Student" || user->Get_Type() == "PhD_Student") {
			user->Clear_Enrolled_Subjects();
		}
		if (user->Get_Type() == "Teacher" || user->Get_Type() == "Admin" || user->Get_Type() == "PhD_Student") {
			user->Clear_Teaching_Subjects();
		}
	}

	QFile file("predmety_2.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList fields = line.split(',');
		Subject* subject = new Subject(fields[0], fields[1], fields[2], "");
		if (fields.size() > 3) {
			subject->Set_Teacher(fields[3]);
		}
		Subjects.append(subject);
	}
	file.close();
	List_Subjects_to_Teach();
	
}

void AIS_MainWindow::Save_Subjects_to_File() {
	QFile file("predmety_1.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	for (int i = 0; i < Subjects.size(); i++) {
		out << Subjects[i]->Get_Name() << "," << Subjects[i]->Get_Study_Year() << "," << Subjects[i]->Get_Type();
		if (Subjects[i]->Get_Teacher() != "") {
			out << "," << Subjects[i]->Get_Teacher();
		}
		if (i != Subjects.size() - 1) {
			out << "\n";
		}
	}
	file.close();
}

void AIS_MainWindow::List_Awaiting_Exam_Students(Subject* subject) {
	ui.List_Awaiting_Students_for_Exam->clear();
	ui.List_Awaiting_Students_for_Exam->setRowCount(0);
	ui.List_Awaiting_Students_for_Exam->setColumnCount(1);
	ui.List_Awaiting_Students_for_Exam->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.List_Awaiting_Students_for_Exam->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.List_Awaiting_Students_for_Exam->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.List_Awaiting_Students_for_Exam->setShowGrid(false);
	ui.List_Awaiting_Students_for_Exam->verticalHeader()->hide();
	ui.List_Awaiting_Students_for_Exam->horizontalHeader()->hide();
	for (int i = 0; i < Database[subject].size(); i++) {
		for (int j = 0; j < Database[subject][i]->Get_Enrolled_Subjects().size(); j++) {
			if (Database[subject][i]->Get_Enrolled_Subjects()[j]->Get_Name() == subject->Get_Name()) {
				if (Database[subject][i]->Get_Enrolled_Subjects()[j]->Get_Signed_for_Exam()) {
					int newRow = ui.List_Awaiting_Students_for_Exam->rowCount();
					ui.List_Awaiting_Students_for_Exam->insertRow(newRow);
					ui.List_Awaiting_Students_for_Exam->setItem(newRow, 0, new QTableWidgetItem(Database[subject][i]->Get_Name() + " " + Database[subject][i]->Get_Surname()));
				}
			}
		}
	}
}

void AIS_MainWindow::Grade_Student() {
	User* current_user = Get_User(ui.Line_Current_User->text());
	QString subject_name = ui.List_Teaching_Subjects->item(ui.List_Teaching_Subjects->currentRow(), 0)->text();
	Subject* subject = Get_Subject(subject_name);
	QString student_name = ui.List_Awaiting_Students_for_Exam->item(ui.List_Awaiting_Students_for_Exam->currentRow(), 0)->text();
	QMainWindow* grade_window = new QMainWindow();
	QWidget* central_widget = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	QLabel* label = new QLabel("Grade: ");
	QComboBox* gradeComboBox = new QComboBox();

	QStringList gradeList;
	gradeList << "A" << "B" << "C" << "D" << "E" << "Fx";
	gradeComboBox->addItems(gradeList);
	QPushButton* button = new QPushButton("Grade");
	layout->addWidget(label);
	layout->addWidget(gradeComboBox);
	layout->addWidget(button);
	central_widget->setLayout(layout);
	grade_window->setCentralWidget(central_widget);
	grade_window->show();
	connect(button, &QPushButton::clicked, this, [this, grade_window, gradeComboBox, current_user, subject, student_name]() {
		QString selectedGrade = gradeComboBox->currentText();
		for (int i = 0; i < Database[subject].size(); i++) {
			if (Database[subject][i]->Get_Name() + " " + Database[subject][i]->Get_Surname() == student_name) {
				for (int j = 0; j < Database[subject][i]->Get_Enrolled_Subjects().size(); j++) {
					if (Database[subject][i]->Get_Enrolled_Subjects()[j]->Get_Name() == subject->Get_Name()) {
						Database[subject][i]->Get_Enrolled_Subjects()[j]->Set_Mark(Database[subject][i]->Get_Enrolled_Subjects()[j]->Get_Attempts(), selectedGrade);
						Database[subject][i]->Get_Enrolled_Subjects()[j]->Set_Attempts(Database[subject][i]->Get_Enrolled_Subjects()[j]->Get_Attempts() + 1);
						Database[subject][i]->Get_Enrolled_Subjects()[j]->Set_Signed_for_Exam(false);
						grade_window->close();
						List_Awaiting_Exam_Students(subject);
						return;
					}
				}
			}
		}
	});
}