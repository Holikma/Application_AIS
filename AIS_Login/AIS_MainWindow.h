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
		QString Get_Type() { return Type; };
};

class Enrolled_Subject : public Subject {
	private:
		QString Mark;
		int Attempts;
	public:
		Enrolled_Subject(QString name, QString study_year, QString type, QString mark, int attempts) : Subject(name, study_year, type) { Mark = mark; Attempts = attempts; };
};


class User {
	private:
		QString Login;
		QString Password;
		QString Type;
	public:
		User(QString name, QString surname, QString age);
		QString Get_Login() { return Login; };
		QString Get_Password() { return Password; };
		QString Get_Type() { return Type; };
		virtual QString Get_Name() { return ""; };
		virtual QString Get_Surname() { return ""; };
		virtual QString Get_Age() { return ""; };




};

class Person {
	private:
		QString Name;
		QString Surname;
		QString Age;
	public:
		Person() {};
		Person(QString name, QString surname, QString age);
		QString Get_Name() { return Name; };
		QString Get_Surname() { return Surname; };
		QString Get_Age() { return Age; };
};

class Employee : public User, public virtual Person {
	private:
		QString Position;
		QVector<Subject*> Teaching_Subjects;
	public:
		Employee(QString login, QString password, QString type, QString name, QString surname, QString age, QString position, QVector<Subject*> teaching_subjects);
		QString Get_Position() { return Position; };
		QString Get_Name() override { return Person::Get_Name(); };
		QString Get_Surname() override { return Person::Get_Surname(); };
		QString Get_Age() override { return Person::Get_Age(); };


};

class Student : public User, public virtual Person {
	private:
		QString Year;
		QVector<Enrolled_Subject*> Enrolled_Subjects;
	public:
		Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject*> enrolled_subjects);
		QString Get_Year() { return Year; };
		QVector<Enrolled_Subject*> Get_Enrolled_Subjects() { return Enrolled_Subjects; };
		void Enroll_Subject(Subject* subject);
		QString Get_Name() override { return Person::Get_Name(); };
		QString Get_Surname() override { return Person::Get_Surname(); };
		QString Get_Age() override { return Person::Get_Age(); };
};

class Phd_Student : public Student, public Employee {
	public:
		Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject*> teaching_subjects, QVector<Enrolled_Subject*> enrolled_subjects, QString position);
};

class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<QSharedPointer<User>> Users;
		QVector<Subject*> Subjects;
		QMap<Subject, QVector<Student>> Enrolled_Students;

	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();
		void Load_Users();
		void Print_Users();
		void Load_Subjects();
		void Print_Subjects();
		Subject* Get_Subject(QString name);
		QVector<Subject*> Get_Subjects() { return Subjects; };
		QVector<QSharedPointer<User>> Get_Users() { return Users; };

};
//name;password;role;first_name;last_name;age;year/role;en_subjects;te_subjects