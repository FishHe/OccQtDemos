#include "SimplestOccInQt.h"

#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>

SimplestOccInQt::SimplestOccInQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//create a display connection
	Handle(Aspect_DisplayConnection) aDisplayConnection;
	//create a graphic driver
	Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);

	//create a viewer
	myViewer = new V3d_Viewer(aGraphicDriver);
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();

	//create an interactive context
	myContext = new AIS_InteractiveContext(myViewer);
	myContext->SetDisplayMode(AIS_Shaded, true);

	//create an occ widget
	myOccWidget = new OccWidget(myContext, this);

	//add widget to the MainWindow
	setCentralWidget(myOccWidget);

	//add a cube to the scene
	TopoDS_Shape aShape = BRepPrimAPI_MakeBox(200, 200, 200).Solid();
	Handle(AIS_Shape) anAisShape = new AIS_Shape(aShape);
	myContext->Display(anAisShape,true);
}
