#pragma once

#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class OccWidget :
	public QWidget
{
	Q_OBJECT
public:
	OccWidget(Handle(AIS_InteractiveContext) aContext, QWidget* parent);
	~OccWidget();

	virtual QPaintEngine*         paintEngine() const;

protected:
	virtual void                  paintEvent(QPaintEvent*);
	virtual void                  resizeEvent(QResizeEvent*);

private:
	Handle(V3d_View)				myView;
	Handle(AIS_InteractiveContext)	myContext;

	Standard_Integer myXmin;
	Standard_Integer myYmin;
	Standard_Integer myXmax;
	Standard_Integer myYmax;
};

