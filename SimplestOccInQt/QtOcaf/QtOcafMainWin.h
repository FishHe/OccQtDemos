#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_QtOcafMainWin.h"
#include "OccWidget.h"
#include "TOcafApplication.h"

#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <TCollection_ExtendedString.hxx>

class QtOcafMainWin : public QMainWindow
{
	Q_OBJECT

public:
	QtOcafMainWin(QWidget *parent = Q_NULLPTR);
	Handle(TOcafApplication) GetApp() { return myApp; }
	Handle(TDocStd_Document) GetOcafDoc() { return myOcafDoc; };
	Handle(Graphic3d_GraphicDriver) GetGraphicDriver() { return myGraphicDriver; }


private:
	Ui::QtOcafMainWinClass ui;

	//occ widget
	OccWidget*							myOccWidget;
	//occ graphics driver
	Handle(Graphic3d_GraphicDriver)		myGraphicDriver;
	//viewer
	Handle(V3d_Viewer)					myViewer;
	//interactive context
	Handle(AIS_InteractiveContext)		myContext;

	//ocaf app
	Handle(TOcafApplication) myApp;
	//ocaf doc
	Handle(TDocStd_Document) myOcafDoc;

	void initOcaf();
	void testMethord();

};
