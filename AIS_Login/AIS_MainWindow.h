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
};

class Person {
	private:
		QString Name;
		QString Surname;
		QString Age;
	public:
		Person() {};
		Person(QString name, QString surname, QString age);
};

class Employee : public User, public virtual Person {
	private:
		QString Position;
		QVector<Subject> Teaching_Subjects;
	public:
		Employee(QString login, QString password, QString type, QString name, QString surname, QString age, QString position, QVector<Subject> teaching_subjects);
};

class Student : public User, public virtual Person {
	private:
		QString Year;
		QVector<Enrolled_Subject> Enrolled_Subjects;
	public:
		Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject> enrolled_subjects);
};

class Phd_Student : public Student, public Employee {
	public:
		Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject> teaching_subjects, QVector<Enrolled_Subject> enrolled_subjects, QString position);
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

};
