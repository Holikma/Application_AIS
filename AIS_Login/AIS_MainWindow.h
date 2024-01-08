#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"
#include <QFile>


class Subject {
	private:
		QString Name;
		int Study_Year;
	public:
		Subject(QString name, int study_year) { Name = name; Study_Year = study_year; };
		QString Get_Name() { return Name; };
		int Get_Study_Year() { return Study_Year; };
		
};

class Enrolled_Subject : public Subject {
	private:
		QString Mark;
		int Attempts;
	public:
		Enrolled_Subject(QString name, int study_year, QString mark, int attempts) : Subject(name, study_year) { Mark = mark; Attempts = attempts; };
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
};

class Student : public User {
	private:
		QMap<QString, QString> Personal_Info;
		QVector<Enrolled_Subject> Enrolled_Subjects;
		QString Year;

	public:
		Student(QString login, QString password, QMap<QString, QString> personal_info, QString year): User(login, password), Personal_Info(personal_info), Year(year) {};
		QVector<Enrolled_Subject> Get_Enrolled_Subjects() { return Enrolled_Subjects; };
		void Enroll_Subject(Subject subject) { Get_Enrolled_Subjects().push_back(Enrolled_Subject(subject.Get_Name(), subject.Get_Study_Year(), "0", 0)); };
		QString Get_Name() override { return Personal_Info["Name"]; };
};

class Phd_Student : public Student {
	private:
		QVector<Subject> Teaching_Subjects;
	public:
		void Modify_Marks(Student student, Subject subject, QString Mark) {}

};

class Employee : public User {
	private:
		QString Position;
	public:
		QString Get_Position() { return Position; };
};

class Teacher : public Employee {
	private:
		QVector<Subject> Teaching_Subjects;
	public:
		void Modify_Marks(Student student, Subject subject, QString Mark);
};

class Administrator : public Employee {
	private:
		QVector<User> Users;
	public:
		void Modify_User(User user);
		void Update_Subjects();
};
class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<User*> Users;
		QVector<Subject> Subjects;
		QMap<Subject, QVector<Student>> Enrolled_Students;
	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();
		void Load_Users();
		QVector<User*> Get_Users() { return Users; };
};
