#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	Set_Environment();
	//connect(ui.Button_Submit, SIGNAL(clicked()), this, SLOT(Check_Login()));
}

AIS_Login::~AIS_Login()
{}

void AIS_Login::Set_Environment() {
	mainWindow->Load_Subjects();
	mainWindow->Load_Users();

}


void AIS_Login::Check_Login() {
	//QString username = ui.LineEdit_Username->text();
	//QString password = ui.LineEdit_Password->text();
	//if (mainWindow->Check_Login(username, password)) {
	//	mainWindow->show();
	//	this->close();
	//}
	//else {
	//	QMessageBox::warning(this, "Login", "Username or Password is incorrect");
	//}
}