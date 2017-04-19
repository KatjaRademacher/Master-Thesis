/**
* main.cpp just creates the main window and starts the application
*
* @autor: Katja Rademacher
* @date: 29.11.2016
*/

#include "qthesismainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QThesisMainWindow w;
	w.show();
	return a.exec();
}
