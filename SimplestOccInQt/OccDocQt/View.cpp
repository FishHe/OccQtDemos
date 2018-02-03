#include "View.h"
#include "ApplicationCommonWindow.h"

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QCursor>
#include <QFileInfo>
#include <QFileDialog>
#include <QMouseEvent>
#include <QRubberBand>
#include <QMdiSubWindow>
#include <QStyleFactory>

#include <Graphic3d_ExportFormat.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_TextureEnv.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_Manipulator.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Point.hxx>
#include <Geom_CartesianPoint.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <AIS_Axis.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <Geom_Axis1Placement.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <BRepTools_ReShape.hxx>
#include <TopExp.hxx>
#include <BRep_Builder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>


#include <OcctWindow.h>


// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftModifier

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlModifier

// for elastic bean selection
#define ValZWMin 1

static QCursor* defCursor = NULL;
static QCursor* handCursor = NULL;
static QCursor* panCursor = NULL;
static QCursor* globPanCursor = NULL;
static QCursor* zoomCursor = NULL;
static QCursor* rotCursor = NULL;

View::View(Handle(AIS_InteractiveContext) theContext, QWidget* parent)
	: QWidget(parent),
	myIsRaytracing(false),
	myIsShadowsEnabled(true),
	myIsReflectionsEnabled(false),
	myIsAntialiasingEnabled(false),
	myViewActions(0),
	myRaytraceActions(0),
	myBackMenu(NULL),
	myPolyPnts(NULL)
{
#if !defined(_WIN32) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX)) && QT_VERSION < 0x050000
	XSynchronize(x11Info().display(), true);
#endif
	myContext = theContext;

	myXmin = 0;
	myYmin = 0;
	myXmax = 0;
	myYmax = 0;
	myCurZoom = 0;
	myRectBand = 0;

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	myCurrentMode = CurAction3d_Nothing;
	myHlrModeIsOn = Standard_False;
	setMouseTracking(true);

	initViewActions();
	initDrawActions();
	initCursors();

	setBackgroundRole(QPalette::NoRole);//NoBackground );
										// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	init();
}

View::~View()
{
	delete myBackMenu;
}

void View::init()
{
	if (myView.IsNull())
		myView = myContext->CurrentViewer()->CreateView();

	Handle(OcctWindow) hWnd = new OcctWindow(this);
	myView->SetWindow(hWnd);
	if (!hWnd->IsMapped())
	{
		hWnd->Map();
	}
	myView->SetBackgroundColor(Quantity_NOC_BLACK);
	myView->MustBeResized();

	if (myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
	
	Handle(Geom_Axis2Placement) gAx2 = new Geom_Axis2Placement(gp_Ax2(gp_Pnt(0,0,0),gp_Dir(0,0,1),gp_Dir(1,0,0)));
	Handle(AIS_Trihedron) anAISTr = new AIS_Trihedron(gAx2);

	anAISTr->SetXAxisColor(Quantity_NOC_RED);
	anAISTr->SetYAxisColor(Quantity_NOC_GREEN);
	anAISTr->SetAxisColor(Quantity_NOC_BLUE1);
	anAISTr->SetSize(500);

	anAISTr->SetTextColor(Quantity_NOC_WHITE);
	
	myContext->Display(anAISTr, true);
}

void View::paintEvent(QPaintEvent *)
{
	//  QApplication::syncX();
	myView->Redraw();
}

void View::resizeEvent(QResizeEvent *)
{
	//  QApplication::syncX();
	if (!myView.IsNull())
	{
		myView->MustBeResized();
	}
}

void View::fitAll()
{
	myView->FitAll();
	myView->ZFitAll();
	myView->Redraw();
}

void View::fitArea()
{
	myCurrentMode = CurAction3d_WindowZooming;
}

void View::zoom()
{
	myCurrentMode = CurAction3d_DynamicZooming;
}

void View::pan()
{
	myCurrentMode = CurAction3d_DynamicPanning;
}

void View::rotation()
{
	myCurrentMode = CurAction3d_DynamicRotation;
}

void View::globalPan()
{
	// save the current zoom value
	myCurZoom = myView->Scale();
	// Do a Global Zoom
	myView->FitAll();
	// Set the mode
	myCurrentMode = CurAction3d_GlobalPanning;
}

void View::front()
{
	myView->SetProj(V3d_Yneg);
}

void View::back()
{
	myView->SetProj(V3d_Ypos);
}

void View::top()
{
	myView->SetProj(V3d_Zpos);
}

void View::bottom()
{
	myView->SetProj(V3d_Zneg);
}

void View::left()
{
	myView->SetProj(V3d_Xneg);
}

void View::right()
{
	myView->SetProj(V3d_Xpos);
}

void View::axo()
{
	myView->SetProj(V3d_XposYnegZpos);
}

void View::reset()
{
	myView->Reset();
}

void View::hlrOff()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myHlrModeIsOn = Standard_False;
	myView->SetComputedMode(myHlrModeIsOn);
	myView->Redraw();
	QApplication::restoreOverrideCursor();
}

void View::hlrOn()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myHlrModeIsOn = Standard_True;
	myView->SetComputedMode(myHlrModeIsOn);
	myView->Redraw();
	QApplication::restoreOverrideCursor();
}

void View::drawLine()
{
	myCurrentMode = CurrentAction3d_DrawLine;
}

void View::drawPoint()
{
	myCurrentMode = CurrentAction3d_DrawPoint;
}

void View::drawCube()
{
	myCurrentMode = CurrentAction3d_DrawCube0;
}

void View::drawPoly()
{
	myCurrentMode = CurrentAction3d_DrawPoly;
}

void View::extrude()
{
	auto firstObj = myContext->FirstSelectedObject();
	
	if (firstObj.IsNull()) return;

	Handle(Standard_Type) typeShape = firstObj->DynamicType();

	if (typeShape != STANDARD_TYPE(AIS_Shape)) return;

	auto hAISShape = Handle(AIS_Shape)::DownCast(firstObj);

	auto aShape = hAISShape->Shape();

	//for (TopExp_Explorer aFaceExplorer(myExtrudeShape, TopAbs_FACE);
	//	aFaceExplorer.More(); aFaceExplorer.Next())
	//{
	//	myExtrudeFace = TopoDS::Face(aFaceExplorer.Current());
	//}

	//for (TopExp_Explorer aWireExplorer(myExtrudeShape, TopAbs_ShapeEnum::TopAbs_WIRE);
	//	aWireExplorer.More(); aWireExplorer.Next())
	//{
	//	myExtrudeWire = TopoDS::Wire(aWireExplorer.Current());
	//}

	for (TopExp_Explorer aEDGEExplorer(aShape, TopAbs_ShapeEnum::TopAbs_WIRE);
		aEDGEExplorer.More(); aEDGEExplorer.Next())
	{
		myExtrudeWire = TopoDS::Wire(aEDGEExplorer.Current());
	}

	gp_Pnt barycenter;
	int vertexNum = 0;
	for (TopExp_Explorer aVertexExplorer(myExtrudeWire, TopAbs_ShapeEnum::TopAbs_VERTEX);
		aVertexExplorer.More(); aVertexExplorer.Next())
	{
		TopoDS_Vertex aVertex = TopoDS::Vertex(aVertexExplorer.Current());
		
		vertexNum++;
		auto ptnV = BRep_Tool::Pnt(aVertex);
		barycenter.Translate(gp_Vec(ptnV.XYZ()));
	}
	barycenter.Scale(gp_Pnt(),1.0 / vertexNum);

	myExtrudeCenter = barycenter;
	
	//Handle(Geom_Axis1Placement) anGA1P = new Geom_Axis1Placement(barycenter, gp_Dir(0,0,1.0));
	//Handle(AIS_Axis) anAISAxis = new AIS_Axis(anGA1P);

	//barycenter.Translate(gp_Vec(0, 0, 100));
	//Handle(Geom_Axis2Placement) anGA2P = new Geom_Axis2Placement(myExtrudeCenter, gp_Dir(0, 0, 1), gp_Dir(1, 0, 0));

	//Handle(AIS_Trihedron) anAISThd = new AIS_Trihedron(anGA2P);
	//myContext->Display(anAISThd, true);

	//TopoDS_Vertex v1 = BRepBuilderAPI_MakeVertex(gp_Pnt(0,0,0));
	//TopoDS_Vertex v2 = BRepBuilderAPI_MakeVertex(gp_Pnt(100,100,100));
	//TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(v1, v2);

	//TopoDS_Vertex v3 = BRepBuilderAPI_MakeVertex(gp_Pnt(0, 500, 500));

	//BRep_Builder builer;

	//TopoDS_Compound cpd;
	//
	//

	//auto testttt = TopExp::FirstVertex(edge,false);

	//ShapeBuild_ReShape aReShape;
	//aReShape.Replace(testttt, v3);
	//auto test2 = aReShape.Apply(edge);
	//

	//Handle(AIS_Shape) hEdge = new AIS_Shape(test2);
	//myContext->Display(hEdge, true);


	//manipulator
	{
		//Handle(AIS_Manipulator) anAISMnplt = new AIS_Manipulator(gp_Ax2(myExtrudeCenter, gp_Dir(0, 0, 1), gp_Dir(1, 0, 0)));

		//anAISMnplt->SetPart(0, AIS_ManipulatorMode::AIS_MM_Rotation, false);
		//anAISMnplt->SetPart(1, AIS_ManipulatorMode::AIS_MM_Rotation, false);
		//anAISMnplt->SetPart(2, AIS_ManipulatorMode::AIS_MM_Rotation, false);

		//anAISMnplt->SetPart(0, AIS_ManipulatorMode::AIS_MM_Scaling, false);
		//anAISMnplt->SetPart(1, AIS_ManipulatorMode::AIS_MM_Scaling, false);
		//anAISMnplt->SetPart(2, AIS_ManipulatorMode::AIS_MM_Scaling, false);

		//AIS_Manipulator::OptionsForAttach ofa;
		//ofa.SetEnableModes(false);

		//anAISMnplt->Attach(hAISShape, ofa);
		//anAISMnplt->EnableMode(AIS_MM_Translation);

		//gp_Trsf tr;
		//tr.SetTranslation(gp_Vec(0,0,500));

		//anAISMnplt->Transform(tr);
	}

	myCurrentMode = CurrentAction3d_Extrude;
}

void View::changeDisplayMode()
{
	if(myContext->DisplayMode()== AIS_DisplayMode::AIS_WireFrame)
	myContext->SetDisplayMode(AIS_DisplayMode::AIS_Shaded, true);
	else myContext->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame, true);
}

void View::SetRaytracedShadows(bool theState)
{
	myView->ChangeRenderingParams().IsShadowEnabled = theState;

	myIsShadowsEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void View::SetRaytracedReflections(bool theState)
{
	myView->ChangeRenderingParams().IsReflectionEnabled = theState;

	myIsReflectionsEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void View::onRaytraceAction()
{
	QAction* aSentBy = (QAction*)sender();

	if (aSentBy == myRaytraceActions->at(ToolRaytracingId))
	{
		bool aState = myRaytraceActions->at(ToolRaytracingId)->isChecked();

		QApplication::setOverrideCursor(Qt::WaitCursor);
		if (aState)
			EnableRaytracing();
		else
			DisableRaytracing();
		QApplication::restoreOverrideCursor();
	}

	if (aSentBy == myRaytraceActions->at(ToolShadowsId))
	{
		bool aState = myRaytraceActions->at(ToolShadowsId)->isChecked();
		SetRaytracedShadows(aState);
	}

	if (aSentBy == myRaytraceActions->at(ToolReflectionsId))
	{
		bool aState = myRaytraceActions->at(ToolReflectionsId)->isChecked();
		SetRaytracedReflections(aState);
	}

	if (aSentBy == myRaytraceActions->at(ToolAntialiasingId))
	{
		bool aState = myRaytraceActions->at(ToolAntialiasingId)->isChecked();
		SetRaytracedAntialiasing(aState);
	}
}

void View::SetRaytracedAntialiasing(bool theState)
{
	myView->ChangeRenderingParams().IsAntialiasingEnabled = theState;

	myIsAntialiasingEnabled = theState;

	myContext->UpdateCurrentViewer();
}

void View::EnableRaytracing()
{
	if (!myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;

	myIsRaytracing = true;

	myContext->UpdateCurrentViewer();
}

void View::DisableRaytracing()
{
	if (myIsRaytracing)
		myView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;

	myIsRaytracing = false;

	myContext->UpdateCurrentViewer();
}

void View::updateToggled(bool isOn)
{
	QAction* sentBy = (QAction*)sender();

	if (!isOn)
		return;

	for (int i = ViewFitAllId; i < ViewHlrOffId; i++)
	{
		QAction* anAction = myViewActions->at(i);

		if ((anAction == myViewActions->at(ViewFitAreaId)) ||
			(anAction == myViewActions->at(ViewZoomId)) ||
			(anAction == myViewActions->at(ViewPanId)) ||
			(anAction == myViewActions->at(ViewGlobalPanId)) ||
			(anAction == myViewActions->at(ViewRotationId)))
		{
			if (anAction && (anAction != sentBy))
			{
				anAction->setCheckable(true);
				anAction->setChecked(false);
			}
			else
			{
				if (sentBy == myViewActions->at(ViewFitAreaId))
					setCursor(*handCursor);
				else if (sentBy == myViewActions->at(ViewZoomId))
					setCursor(*zoomCursor);
				else if (sentBy == myViewActions->at(ViewPanId))
					setCursor(*panCursor);
				else if (sentBy == myViewActions->at(ViewGlobalPanId))
					setCursor(*globPanCursor);
				else if (sentBy == myViewActions->at(ViewRotationId))
					setCursor(*rotCursor);
				else
					setCursor(*defCursor);

				sentBy->setCheckable(false);
			}
		}
	}
}

void View::initCursors()
{
	if (!defCursor)
		defCursor = new QCursor(Qt::ArrowCursor);
	if (!handCursor)
		handCursor = new QCursor(Qt::PointingHandCursor);
	if (!panCursor)
		panCursor = new QCursor(Qt::SizeAllCursor);
	if (!globPanCursor)
		globPanCursor = new QCursor(Qt::CrossCursor);
	if (!zoomCursor)
		zoomCursor = new QCursor(QPixmap(ApplicationCommonWindow::getResourceDir() + QString("/") + QObject::tr("ICON_CURSOR_ZOOM")));
	if (!rotCursor)
		rotCursor = new QCursor(QPixmap(ApplicationCommonWindow::getResourceDir() + QString("/") + QObject::tr("ICON_CURSOR_ROTATE")));
}

QList<QAction*>* View::getViewActions()
{
	initViewActions();
	return myViewActions;
}

QList<QAction*>* View::getRaytraceActions()
{
	initRaytraceActions();
	return myRaytraceActions;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* View::paintEngine() const
{
	return 0;
}

void View::initViewActions()
{
	if (myViewActions)
		return;

	myViewActions = new QList<QAction*>();
	QString dir = ApplicationCommonWindow::getResourceDir() + QString("/");
	QAction* a;

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_FITALL")), QObject::tr("MNU_FITALL"), this);
	a->setToolTip(QObject::tr("TBR_FITALL"));
	a->setStatusTip(QObject::tr("TBR_FITALL"));
	connect(a, SIGNAL(triggered()), this, SLOT(fitAll()));
	myViewActions->insert(ViewFitAllId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_FITAREA")), QObject::tr("MNU_FITAREA"), this);
	a->setToolTip(QObject::tr("TBR_FITAREA"));
	a->setStatusTip(QObject::tr("TBR_FITAREA"));
	connect(a, SIGNAL(triggered()), this, SLOT(fitArea()));

	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewFitAreaId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_ZOOM")), QObject::tr("MNU_ZOOM"), this);
	a->setToolTip(QObject::tr("TBR_ZOOM"));
	a->setStatusTip(QObject::tr("TBR_ZOOM"));
	connect(a, SIGNAL(triggered()), this, SLOT(zoom()));

	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewZoomId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_PAN")), QObject::tr("MNU_PAN"), this);
	a->setToolTip(QObject::tr("TBR_PAN"));
	a->setStatusTip(QObject::tr("TBR_PAN"));
	connect(a, SIGNAL(triggered()), this, SLOT(pan()));

	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewPanId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_GLOBALPAN")), QObject::tr("MNU_GLOBALPAN"), this);
	a->setToolTip(QObject::tr("TBR_GLOBALPAN"));
	a->setStatusTip(QObject::tr("TBR_GLOBALPAN"));
	connect(a, SIGNAL(triggered()), this, SLOT(globalPan()));

	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewGlobalPanId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_FRONT")), QObject::tr("MNU_FRONT"), this);
	a->setToolTip(QObject::tr("TBR_FRONT"));
	a->setStatusTip(QObject::tr("TBR_FRONT"));
	connect(a, SIGNAL(triggered()), this, SLOT(front()));
	myViewActions->insert(ViewFrontId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_BACK")), QObject::tr("MNU_BACK"), this);
	a->setToolTip(QObject::tr("TBR_BACK"));
	a->setStatusTip(QObject::tr("TBR_BACK"));
	connect(a, SIGNAL(triggered()), this, SLOT(back()));
	myViewActions->insert(ViewBackId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_TOP")), QObject::tr("MNU_TOP"), this);
	a->setToolTip(QObject::tr("TBR_TOP"));
	a->setStatusTip(QObject::tr("TBR_TOP"));
	connect(a, SIGNAL(triggered()), this, SLOT(top()));
	myViewActions->insert(ViewTopId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_BOTTOM")), QObject::tr("MNU_BOTTOM"), this);
	a->setToolTip(QObject::tr("TBR_BOTTOM"));
	a->setStatusTip(QObject::tr("TBR_BOTTOM"));
	connect(a, SIGNAL(triggered()), this, SLOT(bottom()));
	myViewActions->insert(ViewBottomId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_LEFT")), QObject::tr("MNU_LEFT"), this);
	a->setToolTip(QObject::tr("TBR_LEFT"));
	a->setStatusTip(QObject::tr("TBR_LEFT"));
	connect(a, SIGNAL(triggered()), this, SLOT(left()));
	myViewActions->insert(ViewLeftId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_RIGHT")), QObject::tr("MNU_RIGHT"), this);
	a->setToolTip(QObject::tr("TBR_RIGHT"));
	a->setStatusTip(QObject::tr("TBR_RIGHT"));
	connect(a, SIGNAL(triggered()), this, SLOT(right()));
	myViewActions->insert(ViewRightId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_AXO")), QObject::tr("MNU_AXO"), this);
	a->setToolTip(QObject::tr("TBR_AXO"));
	a->setStatusTip(QObject::tr("TBR_AXO"));
	connect(a, SIGNAL(triggered()), this, SLOT(axo()));
	myViewActions->insert(ViewAxoId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_ROTATION")), QObject::tr("MNU_ROTATION"), this);
	a->setToolTip(QObject::tr("TBR_ROTATION"));
	a->setStatusTip(QObject::tr("TBR_ROTATION"));
	connect(a, SIGNAL(triggered()), this, SLOT(rotation()));
	a->setCheckable(true);
	connect(a, SIGNAL(toggled(bool)), this, SLOT(updateToggled(bool)));
	myViewActions->insert(ViewRotationId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_RESET")), QObject::tr("MNU_RESET"), this);
	a->setToolTip(QObject::tr("TBR_RESET"));
	a->setStatusTip(QObject::tr("TBR_RESET"));
	connect(a, SIGNAL(triggered()), this, SLOT(reset()));
	myViewActions->insert(ViewResetId, a);

	QActionGroup* ag = new QActionGroup(this);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_HLROFF")), QObject::tr("MNU_HLROFF"), this);
	a->setToolTip(QObject::tr("TBR_HLROFF"));
	a->setStatusTip(QObject::tr("TBR_HLROFF"));
	connect(a, SIGNAL(triggered()), this, SLOT(hlrOff()));
	a->setCheckable(true);
	ag->addAction(a);
	myViewActions->insert(ViewHlrOffId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_VIEW_HLRON")), QObject::tr("MNU_HLRON"), this);
	a->setToolTip(QObject::tr("TBR_HLRON"));
	a->setStatusTip(QObject::tr("TBR_HLRON"));
	connect(a, SIGNAL(triggered()), this, SLOT(hlrOn()));

	a->setCheckable(true);
	ag->addAction(a);
	myViewActions->insert(ViewHlrOnId, a);

	a = new QAction(QPixmap(QObject::tr("ICON_TOOL_CHANGEDSPMODE")), QObject::tr("MNU_TOOL_CHANGEDSPMODE"), this);
	a->setToolTip(QObject::tr("TBR_CHANGEDSPMODE"));
	a->setStatusTip(QObject::tr("TBR_CHANGEDSPMODE"));
	connect(a, SIGNAL(triggered()), this, SLOT(changeDisplayMode()));
	myViewActions->insert(ViewChangeDisplayModeId, a);
}

void View::initRaytraceActions()
{
	if (myRaytraceActions)
		return;

	myRaytraceActions = new QList<QAction*>();
	QString dir = ApplicationCommonWindow::getResourceDir() + QString("/");
	QAction* a;

	a = new QAction(QPixmap(dir + QObject::tr("ICON_TOOL_RAYTRACING")), QObject::tr("MNU_TOOL_RAYTRACING"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_RAYTRACING"));
	a->setStatusTip(QObject::tr("TBR_TOOL_RAYTRACING"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolRaytracingId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_TOOL_SHADOWS")), QObject::tr("MNU_TOOL_SHADOWS"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_SHADOWS"));
	a->setStatusTip(QObject::tr("TBR_TOOL_SHADOWS"));
	a->setCheckable(true);
	a->setChecked(true);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolShadowsId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_TOOL_REFLECTIONS")), QObject::tr("MNU_TOOL_REFLECTIONS"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_REFLECTIONS"));
	a->setStatusTip(QObject::tr("TBR_TOOL_REFLECTIONS"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolReflectionsId, a);

	a = new QAction(QPixmap(dir + QObject::tr("ICON_TOOL_ANTIALIASING")), QObject::tr("MNU_TOOL_ANTIALIASING"), this);
	a->setToolTip(QObject::tr("TBR_TOOL_ANTIALIASING"));
	a->setStatusTip(QObject::tr("TBR_TOOL_ANTIALIASING"));
	a->setCheckable(true);
	a->setChecked(false);
	connect(a, SIGNAL(triggered()), this, SLOT(onRaytraceAction()));
	myRaytraceActions->insert(ToolAntialiasingId, a);
}

//create draw actions
void View::initDrawActions()
{
	OccDocQt * parentOccDocQt = (OccDocQt *)((QFrame *)parent()->parent());
	QToolBar* aToolBar = parentOccDocQt->addToolBar(tr("Draw Optiongs"));
	myDrawActions = new QList<QAction*>();

	QAction* aAction;

	aAction = new QAction(QPixmap(QObject::tr("ICON_TOOL_DRAWPOINT")), QObject::tr("MNU_TOOL_DRAWPOINT"), this);
	aAction->setToolTip(QObject::tr("TBR_DRAWPOINT"));
	aAction->setStatusTip(QObject::tr("TBR_DRAWPOINT"));
	connect(aAction, SIGNAL(triggered()), this, SLOT(drawPoint()));
	myDrawActions->insert(DrawPiontId, aAction);

	aAction = new QAction(QPixmap(QObject::tr("ICON_TOOL_DRAWLINE")), QObject::tr("MNU_TOOL_DRAWLINE"), this);
	aAction->setToolTip(QObject::tr("TBR_DRAWLINE"));
	aAction->setStatusTip(QObject::tr("TBR_DRAWLINE"));
	connect(aAction, SIGNAL(triggered()), this, SLOT(drawLine()));
	myDrawActions->insert(DrawLineId, aAction);

	aAction = new QAction(QPixmap(QObject::tr("ICON_TOOL_DRAWPOLY")), QObject::tr("MNU_TOOL_DRAWPOLY"), this);
	aAction->setToolTip(QObject::tr("TBR_DRAWPOLY"));
	aAction->setStatusTip(QObject::tr("TBR_DRAWPOLY"));
	connect(aAction, SIGNAL(triggered()), this, SLOT(drawPoly()));
	myDrawActions->insert(DrawPolyId, aAction);

	aAction = new QAction(QPixmap(QObject::tr("ICON_TOOL_DRAWCUBE")), QObject::tr("MNU_TOOL_DRAWCUBE"), this);
	aAction->setToolTip(QObject::tr("TBR_DRAWCUBE"));
	aAction->setStatusTip(QObject::tr("TBR_DRAWCUBE"));
	connect(aAction, SIGNAL(triggered()), this, SLOT(drawCube()));
	myDrawActions->insert(DrawCubeId, aAction);

	aAction = new QAction(QPixmap(QObject::tr("ICON_TOOL_EXTRUDE")), QObject::tr("MNU_TOOL_EXTRUDE"), this);
	aAction->setToolTip(QObject::tr("TBR_EXTRUDE"));
	aAction->setStatusTip(QObject::tr("TBR_EXTRUDE"));
	connect(aAction, SIGNAL(triggered()), this, SLOT(extrude()));
	myDrawActions->insert(ExtrudeId, aAction);

	aToolBar->addActions(*myDrawActions);
}


void View::onLButtonDblClick(const int nFlags, const QPoint point)
{
	switch (myCurrentMode)
	{
	case View::CurAction3d_Nothing:
		break;
	case View::CurAction3d_DynamicZooming:
		break;
	case View::CurAction3d_WindowZooming:
		break;
	case View::CurAction3d_DynamicPanning:
		break;
	case View::CurAction3d_GlobalPanning:
		break;
	case View::CurAction3d_DynamicRotation:
		break;
	case View::CurrentAction3d_DrawLine:
		break;
	case View::CurrentAction3d_DrawPoint:
		break;
	case View::CurrentAction3d_DrawCube0:
		break;
	case View::CurrentAction3d_DrawCube1:
		break;
	case View::CurrentAction3d_DrawCube2:
		break;
	case View::CurrentAction3d_DrawPoly:
	{
		BRepBuilderAPI_MakeWire aWire;
		
		for (int i = 0; i < myPolyPnts->length()-1; i++)
		{
			aWire.Add(BRepBuilderAPI_MakeEdge(myPolyPnts->at(i), myPolyPnts->at(i+1)));
		}
		aWire.Add(BRepBuilderAPI_MakeEdge(myPolyPnts->first(), myPolyPnts->last()));
		TopoDS_Shape aFace = BRepBuilderAPI_MakeFace(aWire);

		//BRepBuilderAPI_MakePolygon aPolyMaker = BRepBuilderAPI_MakePolygon();
		//for (int i = 0; i < myPolyPnts->length(); i++)
		//{
		//	aPolyMaker.Add(myPolyPnts->at(i));
		//}
		//aPolyMaker.Add(myPolyPnts->first());
		//TopoDS_Shape aPoly = aPolyMaker.Shape();

		if (!myCurrentShape.IsNull())  myContext->Remove(myCurrentShape,false);
		myCurrentShape = NULL;
		myContext->Display(new AIS_Shape(aFace), true);
		myCurrentMode = CurAction3d_Nothing;

		myPolyPnts = NULL;
	}
		break;
	default:
		break;
	}
}

void View::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
		onLButtonDown((e->buttons() | e->modifiers()), e->pos());
	else if (e->button() == Qt::MidButton)
		onMButtonDown(e->buttons() | e->modifiers(), e->pos());
	else if (e->button() == Qt::RightButton)
		onRButtonDown(e->buttons() | e->modifiers(), e->pos());
}

void View::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
		onLButtonUp(e->buttons(), e->pos());
	else if (e->button() == Qt::MidButton)
		onMButtonUp(e->buttons(), e->pos());
	else if (e->button() == Qt::RightButton)
		onRButtonUp(e->buttons(), e->pos());
}

void View::mouseMoveEvent(QMouseEvent* e)
{
	onMouseMove(e->buttons(), e->pos());
}

void View::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
		onLButtonDblClick((e->buttons() | e->modifiers()), e->pos());
}

void View::activateCursor(const CurrentAction3d mode)
{
	switch (mode)
	{
	case CurAction3d_DynamicPanning:
		setCursor(*panCursor);
		break;
	case CurAction3d_DynamicZooming:
		setCursor(*zoomCursor);
		break;
	case CurAction3d_DynamicRotation:
		setCursor(*rotCursor);
		break;
	case CurAction3d_GlobalPanning:
		setCursor(*globPanCursor);
		break;
	case CurAction3d_WindowZooming:
		setCursor(*handCursor);
		break;
	case CurAction3d_Nothing:
	default:
		setCursor(*defCursor);
		break;
	}
}

void View::onLButtonDown(const int/*Qt::MouseButtons*/ nFlags, const QPoint point)
{
	//  save the current mouse coordinate in min
	myXmin = point.x();
	myYmin = point.y();
	myXmax = point.x();
	myYmax = point.y();

	if (nFlags & CASCADESHORTCUTKEY)
	{
		myCurrentMode = CurAction3d_DynamicZooming;
	}
	else
	{
		switch (myCurrentMode)
		{
		case CurAction3d_Nothing:
			if (nFlags & MULTISELECTIONKEY)
				MultiDragEvent(myXmax, myYmax, -1);
			else
				DragEvent(myXmax, myYmax, -1);
			break;
		case CurAction3d_DynamicZooming:
			break;
		case CurAction3d_WindowZooming:
			break;
		case CurAction3d_DynamicPanning:
			break;
		case CurAction3d_GlobalPanning:
			break;
		case CurAction3d_DynamicRotation:
			if (myHlrModeIsOn)
			{
				myView->SetComputedMode(Standard_False);
			}
			myView->StartRotation(point.x(), point.y());
			break;
		case CurrentAction3d_DrawLine:
			break;
		case CurrentAction3d_DrawPoint:
		{
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmin, myYmin, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint,ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			//Handle(Geom_Point) anGPnt = new Geom_CartesianPoint(gp_Pnt(XPoint, YPoint, ZPoint));
			//Handle(AIS_Point) anAISPnt = new AIS_Point(anGPnt);
			//myContext->Display(anAISPnt, true);

			TopoDS_Shape aPoint = BRepBuilderAPI_MakeVertex(gp_Pnt(XPoint, YPoint, ZPoint));
			Handle(AIS_Shape) anAISShape = new AIS_Shape(aPoint);
			myContext->Display(anAISShape, true);
		}
			break;
		case CurrentAction3d_DrawCube0:
		{
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmin, myYmin, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			myCubePnt1 = gp_Pnt(XPoint, YPoint, ZPoint);
		}
			break;
		case CurrentAction3d_DrawCube1:
		{
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmin, myYmin, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			XPoint = XPoint == myCubePnt1.X() ? XPoint + FLT_EPSILON : XPoint;
			YPoint = YPoint == myCubePnt1.Y() ? YPoint + FLT_EPSILON : YPoint;
			ZPoint = ZPoint == myCubePnt1.Z() ? ZPoint + FLT_EPSILON : ZPoint;

			myCubePnt2 =gp_Pnt(XPoint, YPoint, ZPoint);
		}
			break;
		case CurrentAction3d_DrawCube2:
			break;
		case CurrentAction3d_DrawPoly:
		{
			if (myPolyPnts == NULL) myPolyPnts = new QList<gp_Pnt>();
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmin, myYmin, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			myPolyPnts->append(gp_Pnt(XPoint, YPoint, ZPoint));
		}
			break;
		case CurrentAction3d_Extrude:
		{
			////如果不为空 删除过去的
			//if (!myExtrudePrism.IsNull()) myContext->Remove(myExtrudePrism, false);
			myContext->Deactivate(myExtrudePrism);
			myExtrudePrism = NULL;
			myCurrentMode = CurAction3d_Nothing;
		}
			break;
		default:
			throw Standard_Failure("incompatible Current Mode");
			break;
		}
	}
	activateCursor(myCurrentMode);
}

void View::onMButtonDown(const int/*Qt::MouseButtons*/ nFlags, const QPoint /*point*/)
{
	if (nFlags & CASCADESHORTCUTKEY)
		myCurrentMode = CurAction3d_DynamicPanning;
	activateCursor(myCurrentMode);
}

void View::onRButtonDown(const int/*Qt::MouseButtons*/ nFlags, const QPoint point)
{
	if (nFlags & CASCADESHORTCUTKEY)
	{
		if (myHlrModeIsOn)
		{
			myView->SetComputedMode(Standard_False);
		}
		myCurrentMode = CurAction3d_DynamicRotation;
		myView->StartRotation(point.x(), point.y());
	}
	else
	{
		Popup(point.x(), point.y());
	}
	activateCursor(myCurrentMode);
}

void View::onLButtonUp(Qt::MouseButtons nFlags, const QPoint point)
{
	switch (myCurrentMode)
	{
	case CurAction3d_Nothing:
		if (point.x() == myXmin && point.y() == myYmin)
		{
			// no offset between down and up --> selectEvent
			myXmax = point.x();
			myYmax = point.y();
			if (nFlags & MULTISELECTIONKEY)
				MultiInputEvent(point.x(), point.y());
			else
				InputEvent(point.x(), point.y());
		}
		else
		{
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
			myXmax = point.x();
			myYmax = point.y();
			if (nFlags & MULTISELECTIONKEY)
				MultiDragEvent(point.x(), point.y(), 1);
			else
				DragEvent(point.x(), point.y(), 1);
		}
		break;
	case CurAction3d_DynamicZooming:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_WindowZooming:
		DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);//,LongDash);
		myXmax = point.x();
		myYmax = point.y();
		if ((abs(myXmin - myXmax) > ValZWMin) ||
			(abs(myYmin - myYmax) > ValZWMin))
			myView->WindowFitAll(myXmin, myYmin, myXmax, myYmax);
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_DynamicPanning:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_GlobalPanning:
		myView->Place(point.x(), point.y(), myCurZoom);
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurAction3d_DynamicRotation:
		myCurrentMode = CurAction3d_Nothing;
		noActiveActions();
		break;
	case CurrentAction3d_DrawLine:
	{
		myCurrentShape = NULL;
		noActiveActions();
	}
	break;
	case CurrentAction3d_DrawPoint:
		break;
	case CurrentAction3d_DrawCube0:
		myCurrentMode = CurrentAction3d_DrawCube1;
		break;
	case CurrentAction3d_DrawCube1:
		myCurrentMode = CurrentAction3d_DrawCube2;
		break;
	case CurrentAction3d_DrawCube2:
	{
		myCurrentMode = CurAction3d_Nothing;
		myCurrentShape = NULL;
		noActiveActions();
	}
		break;
	case CurrentAction3d_DrawPoly:
		break;
	case CurrentAction3d_Extrude:
		break;
	default:
		throw Standard_Failure(" incompatible Current Mode ");
		break;
	}
	activateCursor(myCurrentMode);
	ApplicationCommonWindow::getApplication()->onSelectionChanged();
}

void View::onMButtonUp(Qt::MouseButtons /*nFlags*/, const QPoint /*point*/)
{
	myCurrentMode = CurAction3d_Nothing;
	activateCursor(myCurrentMode);
}

void View::onRButtonUp(Qt::MouseButtons /*nFlags*/, const QPoint point)
{
	if (myCurrentMode == CurAction3d_Nothing)
		Popup(point.x(), point.y());
	else
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);
		// reset tyhe good Degenerated mode according to the strored one
		//   --> dynamic rotation may have change it
		if (myHlrModeIsOn)
		{
			myView->SetComputedMode(myHlrModeIsOn);
			myView->Redraw();
		}
		QApplication::restoreOverrideCursor();
		myCurrentMode = CurAction3d_Nothing;
	}
	activateCursor(myCurrentMode);
}

void View::onMouseMove(Qt::MouseButtons nFlags, const QPoint point)
{
	if (nFlags & Qt::LeftButton || nFlags & Qt::RightButton || nFlags & Qt::MidButton)
	{
		switch (myCurrentMode)
		{
		case CurAction3d_Nothing:
			myXmax = point.x();
			myYmax = point.y();
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
			if (nFlags & MULTISELECTIONKEY)
				MultiDragEvent(myXmax, myYmax, 0);
			else
				DragEvent(myXmax, myYmax, 0);
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_True);
			break;
		case CurAction3d_DynamicZooming:
			myView->Zoom(myXmax, myYmax, point.x(), point.y());
			myXmax = point.x();
			myYmax = point.y();
			break;
		case CurAction3d_WindowZooming:
			myXmax = point.x();
			myYmax = point.y();
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_False);
			DrawRectangle(myXmin, myYmin, myXmax, myYmax, Standard_True);
			break;
		case CurAction3d_DynamicPanning:
			myView->Pan(point.x() - myXmax, myYmax - point.y());
			myXmax = point.x();
			myYmax = point.y();
			break;
		case CurAction3d_GlobalPanning:
			break;
		case CurAction3d_DynamicRotation:
			myView->Rotation(point.x(), point.y());
			myView->Redraw();
			break;
		case CurrentAction3d_DrawLine:
		{
			if (!myCurrentShape.IsNull()) myContext->Remove(myCurrentShape, true);
			myXmax = point.x();
			myYmax = point.y();
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmin, myYmin, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;
			Standard_Real XPoint1, YPoint1, ZPoint1;
			ZPoint1 = 0;
			if (Vz != ZPoint1) t = (ZPoint1 - Z) / Vz;
			XPoint1 = X + t*Vx;
			YPoint1 = Y + t*Vy;

			X = 0, Y = 0, Z = 0, Vx = 0, Vy = 0, Vz = 0;
			myView->ConvertWithProj(myXmax, myYmax, X, Y, Z, Vx, Vy, Vz);
			t = 0;
			Standard_Real XPoint2, YPoint2, ZPoint2;
			ZPoint2 = 0;
			if (Vz != ZPoint2) t = (ZPoint2 - Z) / Vz;
			XPoint2 = X + t*Vx;
			YPoint2 = Y + t*Vy;

			if (XPoint1 != XPoint2 || YPoint1 != YPoint2 || ZPoint1 != ZPoint2)
			{
				TopoDS_Shape aLine = BRepBuilderAPI_MakeEdge(
					gp_Pnt(XPoint1, YPoint1, ZPoint1), gp_Pnt(XPoint2, YPoint2, ZPoint2));
				myCurrentShape = new AIS_Shape(aLine);
				myContext->Display(myCurrentShape, true);
			}
		}
			break;
		case CurrentAction3d_DrawPoint:
			break;
		case CurrentAction3d_DrawCube0:
			break;
		case CurrentAction3d_DrawCube1:
			break;
		case CurrentAction3d_DrawCube2:
			break;
		case CurrentAction3d_DrawPoly:
			break;
		case CurrentAction3d_Extrude:
			break;
		default:
			throw Standard_Failure("incompatible Current Mode");
			break;
		}
	}
	else
	{
		myXmax = point.x();
		myYmax = point.y();
		if (nFlags & MULTISELECTIONKEY)
			MultiMoveEvent(point.x(), point.y());
		else
			MoveEvent(point.x(), point.y());
		switch (myCurrentMode)
		{
		case View::CurrentAction3d_DrawCube0:
			break;
		case View::CurrentAction3d_DrawCube1:
		{
			if (!myCurrentShape.IsNull()) myContext->Remove(myCurrentShape, true);
			myXmax = point.x();
			myYmax = point.y();

			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmax, myYmax, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			XPoint = XPoint == myCubePnt1.X() ? XPoint + FLT_EPSILON : XPoint;
			YPoint = YPoint == myCubePnt1.Y() ? YPoint + FLT_EPSILON : YPoint;
			ZPoint = ZPoint == myCubePnt1.Z() ? ZPoint + FLT_EPSILON : ZPoint;

			myCubePnt2 = gp_Pnt(XPoint, YPoint, ZPoint);

			TopoDS_Shape aBox = BRepPrimAPI_MakeBox(myCubePnt1, myCubePnt2);
			myCurrentShape = new AIS_Shape(aBox);
			
			myContext->Display(myCurrentShape, true);
			myContext->Deactivate(myCurrentShape);
		}
			break;
		case View::CurrentAction3d_DrawCube2:
		{
			if (!myCurrentShape.IsNull()) myContext->Remove(myCurrentShape, true);
			myXmax = point.x();
			myYmax = point.y();

			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmax, myYmax, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			//current cursor proj to the ref plane
			auto projPnt3 = gp_Pnt(XPoint, YPoint, ZPoint);
			//distance 
			gp_Vec d(myCubePnt2, projPnt3);
			gp_Vec v(Vx, Vy, Vz);
			//angle<v,d>
			Standard_Real angleV_D = v.Angle(d);
			//project d to h
			Standard_Real d_to_h = d.Magnitude()*std::tan(angleV_D);

			//the 3rd point
			auto cubePnt3 = gp_Pnt(myCubePnt2.X(), myCubePnt2.Y(), d_to_h);

			TopoDS_Shape aBox = BRepPrimAPI_MakeBox(myCubePnt1, cubePnt3);
			myCurrentShape = new AIS_Shape(aBox);

			myContext->Display(myCurrentShape, true);
			myContext->Deactivate(myCurrentShape);
		}
			break;
		case CurrentAction3d_DrawPoly:
		{
			if (!myCurrentShape.IsNull()) myContext->Remove(myCurrentShape, true);
			if (myPolyPnts == NULL ) return;
			if (myPolyPnts->length() == 0) return;

			gp_Pnt lastPnt = myPolyPnts->last();

			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmax, myYmax, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			XPoint = XPoint == lastPnt.X() ? XPoint + FLT_EPSILON : XPoint;
			YPoint = YPoint == lastPnt.Y() ? YPoint + FLT_EPSILON : YPoint;
			ZPoint = ZPoint == lastPnt.Z() ? ZPoint + FLT_EPSILON : ZPoint;

			TopoDS_Wire aWire;
			for (int i=0;i<myPolyPnts->length()-1;i++)
			{
				TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(myPolyPnts->at(i), myPolyPnts->at(i+1));
				aWire = BRepBuilderAPI_MakeWire(aWire, anEdge);
			}
			TopoDS_Edge curEdge = BRepBuilderAPI_MakeEdge(myPolyPnts->last(), 
				gp_Pnt(XPoint, YPoint, ZPoint));
			aWire = BRepBuilderAPI_MakeWire(aWire, curEdge);

			myCurrentShape = new AIS_Shape(aWire);
			myContext->Display(myCurrentShape, true);
		}
			break;

		case CurrentAction3d_Extrude:
		{
			Standard_Real X, Y, Z, Vx, Vy, Vz;
			myView->ConvertWithProj(myXmax, myYmax, X, Y, Z, Vx, Vy, Vz);
			Standard_Real t = 0;

			Standard_Real XPoint, YPoint, ZPoint;
			ZPoint = 0;
			if (Vz != ZPoint) t = (ZPoint - Z) / Vz;
			XPoint = X + t*Vx;
			YPoint = Y + t*Vy;

			//current cursor proj to the ref plane
			auto projPnt = gp_Pnt(XPoint, YPoint, ZPoint);
			//distance 
			gp_Vec d(myExtrudeCenter, projPnt);
			gp_Vec v(Vx, Vy, Vz);
			//angle<v,d>
			Standard_Real angleV_D = v.Angle(d);
			//project d to h
			Standard_Real d_to_h = d.Magnitude()*std::tan(angleV_D);

			//Prism Vec
			gp_Vec prismVec(0, 0, d_to_h);

			//如果不为空 删除过去的
			if (!myExtrudePrism.IsNull()) myContext->Remove(myExtrudePrism,false);
			//建立柱体
			auto aPrism = BRepPrimAPI_MakePrism(myExtrudeWire, prismVec);
			myExtrudePrism = new AIS_Shape(aPrism);
			
			myContext->Display(myExtrudePrism, true);
			myContext->Deactivate(myExtrudePrism);

		}
			break;
		default:
			break;
		}
	}
}

void View::DragEvent(const int x, const int y, const int TheState)
{
	// TheState == -1  button down
	// TheState ==  0  move
	// TheState ==  1  button up

	static Standard_Integer theButtonDownX = 0;
	static Standard_Integer theButtonDownY = 0;

	if (TheState == -1)
	{
		theButtonDownX = x;
		theButtonDownY = y;
	}

	if (TheState == 1)
	{
		myContext->Select(theButtonDownX, theButtonDownY, x, y, myView, Standard_True);
		emit selectionChanged();
	}
}

void View::InputEvent(const int /*x*/, const int /*y*/)
{
	myContext->Select(Standard_True);
	emit selectionChanged();
}

void View::MoveEvent(const int x, const int y)
{
	myContext->MoveTo(x, y, myView, Standard_True);
}

void View::MultiMoveEvent(const int x, const int y)
{
	myContext->MoveTo(x, y, myView, Standard_True);
}

void View::MultiDragEvent(const int x, const int y, const int TheState)
{
	static Standard_Integer theButtonDownX = 0;
	static Standard_Integer theButtonDownY = 0;

	if (TheState == -1)
	{
		theButtonDownX = x;
		theButtonDownY = y;
	}
	if (TheState == 0)
	{
		myContext->ShiftSelect(theButtonDownX, theButtonDownY, x, y, myView, Standard_True);
		emit selectionChanged();
	}
}

void View::MultiInputEvent(const int /*x*/, const int /*y*/)
{
	myContext->ShiftSelect(Standard_True);
	emit selectionChanged();
}

void View::Popup(const int /*x*/, const int /*y*/)
{
	ApplicationCommonWindow* stApp = ApplicationCommonWindow::getApplication();
	QMdiArea* ws = ApplicationCommonWindow::getWorkspace();
	QMdiSubWindow* w = ws->activeSubWindow();
	if (myContext->NbSelected())
	{
		QList<QAction*>* aList = stApp->getToolActions();
		QMenu* myToolMenu = new QMenu(0);
		myToolMenu->addAction(aList->at(ApplicationCommonWindow::ToolWireframeId));
		myToolMenu->addAction(aList->at(ApplicationCommonWindow::ToolShadingId));
		myToolMenu->addAction(aList->at(ApplicationCommonWindow::ToolColorId));

		QMenu* myMaterMenu = new QMenu(myToolMenu);

		QList<QAction*>* aMeterActions = ApplicationCommonWindow::getApplication()->getMaterialActions();

		QString dir = ApplicationCommonWindow::getResourceDir() + QString("/");
		myMaterMenu = myToolMenu->addMenu(QPixmap(dir + QObject::tr("ICON_TOOL_MATER")), QObject::tr("MNU_MATER"));
		for (int i = 0; i < aMeterActions->size(); i++)
			myMaterMenu->addAction(aMeterActions->at(i));

		myToolMenu->addAction(aList->at(ApplicationCommonWindow::ToolTransparencyId));
		myToolMenu->addAction(aList->at(ApplicationCommonWindow::ToolDeleteId));
		addItemInPopup(myToolMenu);
		myToolMenu->exec(QCursor::pos());
		delete myToolMenu;
	}
	else
	{
		if (!myBackMenu)
		{
			myBackMenu = new QMenu(0);

			QAction* a = new QAction(QObject::tr("MNU_CH_BACK"), this);
			a->setToolTip(QObject::tr("TBR_CH_BACK"));
			connect(a, SIGNAL(triggered()), this, SLOT(onBackground()));
			myBackMenu->addAction(a);
			addItemInPopup(myBackMenu);

			a = new QAction(QObject::tr("MNU_CH_ENV_MAP"), this);
			a->setToolTip(QObject::tr("TBR_CH_ENV_MAP"));
			connect(a, SIGNAL(triggered()), this, SLOT(onEnvironmentMap()));
			a->setCheckable(true);
			a->setChecked(false);
			myBackMenu->addAction(a);
			addItemInPopup(myBackMenu);
		}

		myBackMenu->exec(QCursor::pos());
	}
	if (w)
		w->setFocus();
}

void View::addItemInPopup(QMenu* /*theMenu*/)
{
}

void View::DrawRectangle(const int MinX, const int MinY,
	const int MaxX, const int MaxY, const bool Draw)
{
	static Standard_Integer StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
	static Standard_Boolean m_IsVisible;

	StoredMinX = (MinX < MaxX) ? MinX : MaxX;
	StoredMinY = (MinY < MaxY) ? MinY : MaxY;
	StoredMaxX = (MinX > MaxX) ? MinX : MaxX;
	StoredMaxY = (MinY > MaxY) ? MinY : MaxY;

	QRect aRect;
	aRect.setRect(StoredMinX, StoredMinY, abs(StoredMaxX - StoredMinX), abs(StoredMaxY - StoredMinY));

	if (!myRectBand)
	{
		myRectBand = new QRubberBand(QRubberBand::Rectangle, this);
		myRectBand->setStyle(QStyleFactory::create("windows"));
		myRectBand->setGeometry(aRect);
		myRectBand->show();

		/*QPalette palette;
		palette.setColor(myRectBand->foregroundRole(), Qt::white);
		myRectBand->setPalette(palette);*/
	}

	if (m_IsVisible && !Draw) // move or up  : erase at the old position
	{
		myRectBand->hide();
		delete myRectBand;
		myRectBand = 0;
		m_IsVisible = false;
	}

	if (Draw) // move : draw
	{
		//aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
		m_IsVisible = true;
		myRectBand->setGeometry(aRect);
		//myRectBand->show();
	}
}

void View::noActiveActions()
{
	for (int i = ViewFitAllId; i < ViewHlrOffId; i++)
	{
		QAction* anAction = myViewActions->at(i);
		if ((anAction == myViewActions->at(ViewFitAreaId)) ||
			(anAction == myViewActions->at(ViewZoomId)) ||
			(anAction == myViewActions->at(ViewPanId)) ||
			(anAction == myViewActions->at(ViewGlobalPanId)) ||
			(anAction == myViewActions->at(ViewRotationId)))
		{
			setCursor(*defCursor);
			anAction->setCheckable(true);
			anAction->setChecked(false);
		}
	}
}

void View::onBackground()
{
	QColor aColor;
	Standard_Real R1;
	Standard_Real G1;
	Standard_Real B1;
	myView->BackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
	aColor.setRgb(R1 * 255, G1 * 255, B1 * 255);

	QColor aRetColor = QColorDialog::getColor(aColor);

	if (aRetColor.isValid())
	{
		R1 = aRetColor.red() / 255.;
		G1 = aRetColor.green() / 255.;
		B1 = aRetColor.blue() / 255.;
		myView->SetBackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
	}
	myView->Redraw();
}

void View::onEnvironmentMap()
{
	if (myBackMenu->actions().at(1)->isChecked())
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
			tr("All Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tga)"));

		const TCollection_AsciiString anUtf8Path(fileName.toUtf8().data());

		Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv(anUtf8Path);

		myView->SetTextureEnv(aTexture);
	}
	else
	{
		myView->SetTextureEnv(Handle(Graphic3d_TextureEnv)());
	}

	myView->Redraw();
}

bool View::dump(Standard_CString theFile)
{
	return myView->Dump(theFile);
}

Handle(V3d_View)& View::getView()
{
	return myView;
}

Handle(AIS_InteractiveContext)& View::getContext()
{
	return myContext;
}

View::CurrentAction3d View::getCurrentMode()
{
	return myCurrentMode;
}
