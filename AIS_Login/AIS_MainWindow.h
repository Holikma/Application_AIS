#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"
#include <QFile>


class Subject {
	private:
		QString Name;
		QString Study_Year;
		QString Type;
	public:
		Subject(QString name, QString study_year, QString type) { Name = name; Study_Year = study_year; Type = type; };
		QString Get_Name() { return Name; };
		QString Get_Study_Year() { return Study_Year; };
		
};

class Enrolled_Subject : public Subject {
	private:
		QString Mark;
		int Attempts;
	public:
		Enrolled_Subject(QString name, QString study_year, QString type, QString mark, int attempts) : Subject(name, study_year, type) { Mark = mark; Attempts = attempts; };
		QString Get_Mark() { return Mark; };
		int Get_Attempts() { return Attempts; };
};

class User {
	private:
		QString Login;
		QString Password;

	public:
		User(QString login, QString password) { Login = login; Password = password; };
		virtual QString Get_Name() { return ""; };
		virtual QString Get_Surname() { return ""; };
		QString Get_Login() { return Login; };
		QString Get_Password() { return Password; };
};

class Student : public User {
	private:
		QMap<QString, QString> Personal_Info;
		QVector<Enrolled_Subject> Enrolled_Subjects;
		QString Year;

	public:
		Student(QString login, QString password, QString year) : User(login, password), Year(year) {
			Personal_Info = QMap<QString, QString>{ {"name", ""},{"Age", "0"}, {"surname", ""}};};
		QVector<Enrolled_Subject> Get_Enrolled_Subjects() { return Enrolled_Subjects; };
		//void Enroll_Subject(Subject subject) { Get_Enrolled_Subjects().push_back(Enrolled_Subject(subject.Get_Name(), subject.Get_Study_Year(), "0", 0)); };
		QString Get_Name() override { return (Personal_Info)["Name"]; };
		QString Get_Surname() { return (Personal_Info)["Surname"]; };
		void Set_Personal_Info(QString name, QString surname, QString age) { (Personal_Info)["Name"] = name; (Personal_Info)["Surname"] = surname; (Personal_Info)["Age"] = age; };
};

class Phd_Student : public Student {
	private:
		QVector<Subject> Teaching_Subjects;
	public:
		Phd_Student(QString login, QString password, QString year) : Student(login, password, year) {};
		void Modify_Marks(Student student, Subject subject, QString Mark) {};

};

class Employee : public User {
	private:
		QString Position;
	public:
		Employee(QString login, QString password, QString position) : User(login, password), Position(position) {};
		QString Get_Position() { return Position; };
};

class Teacher : public Employee {
	private:
		QVector<Subject> Teaching_Subjects;
	public:
		Teacher(QString login, QString password, QString position) : Employee(login, password, position) {};
		void Modify_Marks(Student student, Subject subject, QString Mark);
};

class Administrator : public Employee {
	private:
		QVector<User> Users;
	public:
		Administrator(QString login, QString password, QString position) : Employee(login, password, position) {};
		void Modify_User(User user);
		void Update_Subjects();
};
class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<QSharedPointer<User>> Users;
		QVector<Subject> Subjects;
		QMap<Subject, QVector<Student>> Enrolled_Students;

	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();
		void Load_Users();
		void Load_Subjects();
		QVector<QSharedPointer<User>> Get_Users() { return Users; };
		QVector<Subject> Get_Subjects() { return Subjects; };
		User Get_User(QString login);
		void Print_Users();
		void Print_Subjects();
};
