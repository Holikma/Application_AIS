#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	mainWindow->Load_Users();
	for (int i = 0; i < mainWindow->Get_Users().size(); i++) {
		ui.Box_Users->addItem(mainWindow->Get_Users()[i]->Get_Login());
	}	
}

AIS_Login::~AIS_Login()
{}
