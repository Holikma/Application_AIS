#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	Set_Environment();
	connect(ui.Button_Submit, SIGNAL(clicked()), this, SLOT(Check_Login()));
}

AIS_Login::~AIS_Login()
{}

void AIS_Login::Set_Environment() {
	mainWindow->Load_Users();
	mainWindow->Load_Subjects();
	for (int i = 0; i < mainWindow->Get_Users().size(); i++) {
		ui.Box_Users->addItem(mainWindow->Get_Users()[i]->Get_Login());
	}

}

void AIS_Login::Check_Login() {
	User us = mainWindow->Get_User(ui.Box_Users->currentText());
	if (us.Get_Password() == ui.Line_Password->text()) {
		mainWindow->show();
		this->close();
	}
	else {
		QMessageBox::warning(this, "Error", "Wrong password");
	}
		
}
