#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	Set_Environment();
	connect(ui.Submit, SIGNAL(clicked()), this, SLOT(Check_Login()));
}

AIS_Login::~AIS_Login()
{}

void AIS_Login::Set_Environment() {
	mainWindow->Load_Subjects();
	mainWindow->Load_Users();
	Set_Users();
}


void AIS_Login::Check_Login() {
	QString  username = ui.List_Users->currentItem()->text();
	if (username == mainWindow->Get_User(username)->Get_Login()) {
		mainWindow->show();
		this->close();
	}
}

void AIS_Login::Set_Users() {
	for (int i = 0; i < mainWindow->Get_Users().size(); i++) {
		ui.List_Users->addItem(mainWindow->Get_Users()[i]->Get_Login());
	}
}