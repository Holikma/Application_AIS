#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"
#include <QFile>


class Subject {
	private:
		QString Name;
		int Study_Year;
		QString Study_Type;
};

class Enrolled_Subject : public Subject {
	private:
		QString Mark;
		int Attempts;
};

class User {
	private:
		QString Login;
		QString Password;

	public:
		User(QString login, QString password);
		void Get_Info();
};

class Student : public User {
	private:
		QMap<QString, QString> Personal_Info;
		QVector<Enrolled_Subject> Enrolled_Subjects;
		int Year;

	public:
		Student(QString login, QString password, QMap<QString, QString> personal, QString access, int year, QVector<Subject> enrolled_subjects);
		void Get_Enrolled_Subjects();
		void Enroll_Subject(Subject subject);
};

class Phd_Student : public Student {
	private:
		QVector<Subject> Teaching_Subjects;

	public:

};

class Employee : public User {
	private:
		QString Position;
	public:
		void Get_Position();
};

class Teacher : public Employee {
	private:
		QVector<Subject> Teaching_Subjects;
	public:
		void modify_marks(Student student, Subject subject, QString Mark);
};

class Administrator : public Employee {
	private:
		QVector<User> Users;
	public:
		void Add_User(User user);
		void Delete_User(User user);
		void Modify_User(User user);
		void Update_Subjects();
};
class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<User> Users;
		QVector<Subject> Subjects;


	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();

		void Load_Users();
		QVector<User> Get_Users() { return Users; };
		User Get_User(int index) { return Users[index]; };

};
