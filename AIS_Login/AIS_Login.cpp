#include "AIS_Login.h"

AIS_Login::AIS_Login(QWidget *parent) : QMainWindow(parent){
    ui.setupUi(this);
	mainWindow->Load_Users();

	
}

AIS_Login::~AIS_Login()
{}
