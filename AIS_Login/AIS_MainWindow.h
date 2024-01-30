#pragma once

#include <QMainWindow>
#include "ui_AIS_MainWindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTableWidget>
#include <QComboBox>


class Subject {
	protected:
		QString Name;
		QString Study_Year;
		QString Type;
		QString Teacher;
	public:
		Subject(QString name, QString study_year, QString type, QString teacher) { Name = name; Study_Year = study_year; Type = type; Teacher = teacher; };
		QString Get_Name() { return this->Name; };
		QString Get_Study_Year() { return this->Study_Year; };
		QString Get_Type() { return this->Type; };
		QString Get_Teacher() { return this->Teacher; };
		void Set_Teacher(QString teacher) { this->Teacher = teacher; };
};

class Enrolled_Subject : public Subject {
	private:
		QVector<QString> Marks;
		int Attempts;
		bool Signed_for_Exam; 
	public:
		Enrolled_Subject(QString name, QString study_year, QString type, QString teacher, QVector<QString> mark, int attempts, bool signed_for_exam) : Subject(name, study_year, type, teacher) { Marks = mark; Attempts = attempts; Signed_for_Exam = signed_for_exam; };
		QVector<QString> Get_Marks() { return Marks; };
		int Get_Attempts() { return Attempts; };
		bool Get_Signed_for_Exam() { return Signed_for_Exam; };
		void Set_Marks(QVector<QString> mark) { Marks = mark; };
		void Set_Mark(int index, QString mark) { Marks[index] = mark; };
		void Set_Attempts(int attempts) { Attempts = attempts; };
		QString Get_Mark(int index) { return Marks[index]; };
		void Set_Signed_for_Exam(bool signed_for_exam) { Signed_for_Exam = signed_for_exam; };
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
		virtual QString Get_Position() { return ""; };
		virtual void Clear_Enrolled_Subjects() {};
		virtual void Clear_Teaching_Subjects() {};
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
		QString Get_Position() override { return Position; };
		QVector<Subject*> Get_Teaching_Subjects() { return Teaching_Subjects; };
		void Teach_Subject(Subject* subject) override;
		void Clear_Teaching_Subjects() override { Teaching_Subjects.clear(); };
};

class Student : public Person {
	protected:
		QString Year;
		QVector<Enrolled_Subject*> Enrolled_Subjects;
	public:
		Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Enrolled_Subject*> enrolled_subjects);
		void Enroll_Subject(Subject* subject) override;
		QString Get_Year() override { return Year; };
		QVector<Enrolled_Subject*> Get_Enrolled_Subjects() override { return Enrolled_Subjects; };
		void Clear_Enrolled_Subjects() override { Enrolled_Subjects.clear(); };

};

class Phd_Student : public Student {
	private:
		QVector<Subject*> Teaching_Subjects;
	public:
		Phd_Student(QString login, QString password, QString type, QString name, QString surname, QString age, QString year, QVector<Subject*> teaching_subjects, QVector<Enrolled_Subject*> enrolled_subjects);	
		void Teach_Subject(Subject* subject) override;
		QVector<Subject*> Get_Teaching_Subjects() override { return Teaching_Subjects; };
		void Clear_Teaching_Subjects() override { Teaching_Subjects.clear(); };
};

class AIS_MainWindow : public QMainWindow{
	Q_OBJECT

	private:
		Ui::AIS_MainWindowClass ui;
		QVector<QSharedPointer<User>> Users;
		QVector<Subject*> Subjects;
		QMap<Subject*, QVector<Student*>> Database;

	private slots:
		void Generate_Report();
		void Enroll_Subject();
		void Teach_Subject();
		void List_Enrolled_Students(QTableWidget* sourceList);
		void Sign_for_Exam();
		void Load_New_Subjects();
		void List_Awaiting_Exam_Students(Subject* subject);
		void Grade_Student();

	public:
		void closeEvent(QCloseEvent* event) override;
		AIS_MainWindow(QWidget *parent = nullptr);
		~AIS_MainWindow();
		Ui::AIS_MainWindowClass Get_UI() { return ui; };
		void Load_Users();
		void Load_Subjects();
		void Load_Database();
		void Print_Users();
		void Print_Subjects();
		void Print_Database();
		User* Get_User(QString login);
		Subject* Get_Subject(QString name);
		QVector<Subject*> Get_Subjects() { return Subjects; };
		QVector<QSharedPointer<User>> Get_Users() { return Users; };
		void Set_Student_Ui(User* user);
		void Set_Teacher_Ui(User* user);
		void Set_PhD_Student_Ui(User* user);
		void Set_Admin_Ui(User* user);
		void Set_Personal_Info(User* user);
		void List_Subjects(User* user);
		void List_Enrolled_Subjects(User* user);
		void List_Subjects_to_Teach();
		void List_Teaching_Subjects(User* user);
		void Save_Users_to_File();
		void Update_Database(Subject* subject, Student* Student);
		void Reset_UI();
		void List_Signed_for_Exam_Subjects(User* user);
		void Save_Marks_to_File();
		void Load_Marks_from_File();
		void Save_Subjects_to_File();
		
		
};
//name;password;role;first_name;last_name;age;year/role;en_subjects;te_subjects