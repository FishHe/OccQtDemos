#pragma once

#include "ui_SimplestOccInQt.h"
#include "OccWidget.h"

#include <QtWidgets/QMainWindow>

#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>

class SimplestOccInQt : public QMainWindow
{
	Q_OBJECT

public:
	SimplestOccInQt(QWidget *parent = Q_NULLPTR);

private:
	Ui::SimplestOccInQtClass ui;

	//occ widget
	OccWidget*							myOccWidget;
	//viewer
	Handle(V3d_Viewer)					myViewer;
	//interactive context
	Handle(AIS_InteractiveContext)		myContext;
};
