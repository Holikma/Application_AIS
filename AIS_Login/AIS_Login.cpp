#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	Set_Environment();
	connect(ui.Submit, SIGNAL(clicked()), this, SLOT(Check_Login()));
	connect(mainWindow->Get_UI().Button_Logout, SIGNAL(clicked()), this, SLOT(Log_Out()));
	connect(ui.Line_Password, SIGNAL(returnPressed()), this, SLOT(Check_Login()));
}

AIS_Login::~AIS_Login()
{}

void AIS_Login::Set_Environment() {
	mainWindow->Load_Subjects();
	mainWindow->Load_Users();
	Set_Users();
}


void AIS_Login::Check_Login() {

	User* user = mainWindow->Get_User(ui.List_Users->currentItem()->text());
	QString password = ui.Line_Password->text();
	if (user->Get_Password() == password) {
		if (user->Get_Type() == "Student") {
			mainWindow->Set_Student_Ui(user);
		}
		else if (user->Get_Type() == "Teacher") {
			mainWindow->Set_Teacher_Ui(user);
		}
		else if (user->Get_Type() == "Admin") {
			mainWindow->Set_Admin_Ui(user);
		}
		else if (user->Get_Type() == "PhD_Student") {
			mainWindow->Set_PhD_Student_Ui(user);
		}
		mainWindow->show();
		this->close();
	}
	else {
		QMessageBox::warning(this, "Warning", "Wrong password!");
	}
}

void AIS_Login::Set_Users() {
	for (int i = 0; i < mainWindow->Get_Users().size(); i++) {
		ui.List_Users->addItem(mainWindow->Get_Users()[i]->Get_Login());
	}
}

void AIS_Login::Log_Out() {
	mainWindow->close();
	this->show();
}