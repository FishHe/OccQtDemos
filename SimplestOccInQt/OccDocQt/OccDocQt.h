#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OccDocQt.h"

class OccDocQt : public QMainWindow
{
	Q_OBJECT

public:
	OccDocQt(QWidget *parent = Q_NULLPTR);

private:
	Ui::OccDocQtClass ui;
};
