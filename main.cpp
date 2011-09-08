#include <QtGui/QApplication>
#include "MainWindow.h"

#include <SLExceptions.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SLFormat::Exceptions::GenerateExceptions();
	SLFormat::Exceptions::GenerateDevExceptions();

	MainWindow w;
	w.show();

	return a.exec();
}
