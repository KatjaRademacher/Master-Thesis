#include "qthesismainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QThesisMainWindow w;
	w.show();
	return a.exec();
}
