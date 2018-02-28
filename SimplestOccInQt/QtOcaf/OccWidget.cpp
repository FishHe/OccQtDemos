#include "OccWidget.h"
#include "OccWindow.h"

#include <QPushButton>

OccWidget::OccWidget(Handle(AIS_InteractiveContext) aContext, QWidget* parent)
	:QWidget(parent),
	myContext(aContext),
	myXmin(0),
	myXmax(0),
	myYmin(0),
	myYmax(0)
{
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setMouseTracking(true);
	setBackgroundRole(QPalette::NoRole);
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	//create the view.
	myView = myContext->CurrentViewer()->CreateView();

	//create an occ window
	Handle(OccWindow) hWnd = new OccWindow(this);

	//bind occ window to the view
	myView->SetWindow(hWnd);

	myView->SetBackgroundColor(Quantity_NOC_GRAY38);
	myView->MustBeResized();
}


OccWidget::~OccWidget()
{
}

void OccWidget::paintEvent(QPaintEvent *)
{
	//  QApplication::syncX();
	myView->Redraw();
}

void OccWidget::resizeEvent(QResizeEvent *)
{
	//  QApplication::syncX();
	if (!myView.IsNull())
	{
		myView->MustBeResized();
	}
}

void OccWidget::mouseMoveEvent(QMouseEvent *e)
{
	onMouseMove(e->buttons(), e->pos());
}

void OccWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		onLeftButtonDown(e->buttons(),e->pos());
	}
}


void OccWidget::onMouseMove(Qt::MouseButtons nFlags, QPoint point)
{
	OccCtxMove(point.x(), point.y());
}

void OccWidget::onLeftButtonDown(Qt::MouseButtons nFlags, QPoint point)
{
	OccCtxSelect(point.x(), point.y());
}

void OccWidget::OccCtxMove(const int x, const int y)
{
	myContext->MoveTo(x, y, myView, Standard_True);
}

void OccWidget::OccCtxSelect(const int x, const int y)
{
	myContext->Select(Standard_True);
}



QPaintEngine* OccWidget::paintEngine() const
{
	return 0;
}