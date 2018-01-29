#include "SimplestOccInQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SimplestOccInQt w;
	w.show();
	return a.exec();
}
