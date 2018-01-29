#include "OccDocQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OccDocQt w;
	w.show();
	return a.exec();
}
