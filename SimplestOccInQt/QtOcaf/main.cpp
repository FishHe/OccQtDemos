#include "QtOcafMainWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtOcafMainWin w;
	w.show();
	return a.exec();
}
