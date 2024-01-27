#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"
#include <QFile>


class Subject {
	protected:
		QString Name;
		QString Study_Year;
		QString Type;
		bool Has_Teacher;
	public:
		Subject(QString name, QString study_year, QString type, bool has_teacher) { Name = name; Study_Year = study_year; Type = type; Has_Teacher = has_teacher; };
		QString Get_Name() { return this->Name; };
		QString Get_Study_Year() { return this->Study_Year; };
		QString Get_Type() { return this->Type; };
		bool Get_Has_Teacher() { return this->Has_Teacher; };
		void Set_Has_Teacher(bool has_teacher) { this->Has_Teacher = has_teacher; };
};

class Enrolled_Subject : public Subject {
	private:
		QString Mark;
		int Attempts;
	public:
		Enrolled_Subject(QString name, QString study_year, QString type, bool has_teacher, QString mark, int attempts) : Subject(name, study_year, type, has_teacher) { Mark = mark; Attempts = attempts; };
};

class User {
	protected:
		QString Login;
		QString Password;
		QString Type;
	public:
		User() {};
		User(QString login, QString password, QString type);
		QString Get_Login() { return Login; };
		QString Get_Password() { return Password; };
		QString Get_Type() { return Type; };
		virtual QString Get_Name() { return ""; };
		virtual QString Get_Surname() { return ""; };
		virtual QString Get_Age() { return ""; };
		virtual QString Get_Year() { return ""; };
		virtual QVector<Enrolled_Subject*> Get_Enrolled_Subjects() { return QVector<Enrolled_Subject*>(); };
		virtual QVector<Subject*> Get_Teaching_Subjects() { return QVector<Subject*>(); };
		virtual void Enroll_Subject(Subject* subject) {};
		virtual void Teach_Subject(Subject* subject) {};
};

class Person : public User{
	protected:
		QString Name;
		QString Surname;
		QString Age;
	public:
		Person() {};
		Person(QString login, QString password, QString type, QString name, QString surname, QString age);
		QString Get_Name() override { return Name; };
		QString Get_Surname() override { return Surname; };
		QString Get_Age() override { return Age; };
};

class Employee : public Person {
	protected:
		QString Position;
		QVector<Subject*> Teaching_Subjects;
	public:
		Employee(QString login, QString password, QString type, QString name, QString surname, QString age, QString position, QVector<Subject*> teaching_subjects);
		QString Get_Position() { return Position; };
		QVector<Subject*> Get_Teaching_Subjects() { return Teaching_Subjects; };
		void Teach_Subject(Subject* subject) override;
};

class Student : public Person {
	protected:
		QString Year;
		QVector<Enrolled_Subject*> Enrolled_Subjects;
	public:
		Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject*> enrolled_subjects);
		void Enroll_Subject(Subject* subject) override;
		QString Get_Year() { return Year; };
		QVector<Enrolled_Subject*> Get_Enrolled_Subjects() override { return Enrolled_Subjects; };
};

class Phd_Student : public Student {
	private:
		QVector<Subject*> Teaching_Subjects;
	public:
		void Teach_Subject(Subject* subject) override;
		QVector<Subject*> Get_Teaching_Subjects() override { return Teaching_Subjects; };
		Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject*> teaching_subjects, QVector<Enrolled_Subject*> enrolled_subjects);	
};

class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<QSharedPointer<User>> Users;
		QVector<Subject*> Subjects;
		QMap<Subject*, QVector<Student*>> Database;

	private slots:
		

	public:
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();
		Ui::AIS_MainWindowClass Get_UI() { return ui; };
		void Load_Users();
		void Print_Users();
		void Load_Subjects();
		void Print_Subjects();
		Subject* Get_Subject(QString name);
		User* Get_User(QString login);
		QVector<Subject*> Get_Subjects() { return Subjects; };
		QVector<QSharedPointer<User>> Get_Users() { return Users; };
		void Set_Student_Ui(User* user);
		void Set_Teacher_Ui(User* user);
		void Set_PhD_Student_Ui(User* user);
		void Set_Admin_Ui(User* user);
		void Table_Subjects_Available(User* user);

};
//name;password;role;first_name;last_name;age;year/role;en_subjects;te_subjects