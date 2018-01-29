#pragma once

#include "OccDocQt.h"

#include <QObject>
#include <QList>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>

class ApplicationCommonWindow;

class DocumentCommon
	: public QObject
{
	Q_OBJECT
public:
	DocumentCommon(const int, ApplicationCommonWindow*);
	~DocumentCommon();

	ApplicationCommonWindow*       getApplication();
	Handle(AIS_InteractiveContext) getContext();
	void                           removeView(OccDocQt*);
	void                           removeViews();
	int                            countOfWindow();
	void                           fitAll();

protected:
	virtual OccDocQt*                   createNewOccDocQt();

signals:
	void                           selectionChanged();
	void                           sendCloseDocument(DocumentCommon*);

	public slots:
	virtual void                   onCloseView(OccDocQt*);
	virtual void                   onCreateNewView();
	virtual void                   onMaterial();
	virtual void                   onMaterial(int);
	virtual void                   onDelete();

	void                           onWireframe();
	void                           onShading();
	void                           onColor();
	void                           onTransparency();
	void                           onTransparency(int);

private:
	Handle(V3d_Viewer)             Viewer(
		const Standard_Real theViewSize,
		const V3d_TypeOfOrientation theViewProj,
		const Standard_Boolean theComputedMode,
		const Standard_Boolean theDefaultComputedMode);

protected:
	ApplicationCommonWindow*       myApp;
	QList<OccDocQt*>              myViews;
	Handle(V3d_Viewer)             myViewer;
	Handle(AIS_InteractiveContext) myContext;
	int                            myIndex;
	int                            myNbViews;
};

