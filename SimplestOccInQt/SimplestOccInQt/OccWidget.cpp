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

	myView->SetBackgroundColor(Quantity_NOC_BLUE1);
	myView->MustBeResized();

	//auto button = new QPushButton(this);
	//auto c =this->children();
	//c.push_back(button);
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

QPaintEngine* OccWidget::paintEngine() const
{
	return 0;
}