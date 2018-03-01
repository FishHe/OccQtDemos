#include "QtOcafMainWin.h"

//graphic driver
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>

//occ brep model api
#include <BRepPrimAPI_MakeBox.hxx>

//occ topo data structure
#include <TopoDS_Shape.hxx>

//ocaf function
#include <TFunction_Logbook.hxx>
#include <TFunction_Function.hxx>
#include <TFunction_DriverTable.hxx>
#include "TOcafFunction_BoxDriver.h"

//ocaf prs std
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>

//ocaf TNaming
#include <TNaming_NamedShape.hxx>

//occ interactive service
#include <AIS_Shape.hxx>

//ocaf doc file
#include <BinDrivers.hxx>
#include <XmlDrivers.hxx>

//ocaf data std
#include <TDataStd.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Integer.hxx>


QtOcafMainWin::QtOcafMainWin(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//initial occ viewer
	initOcaf();

	//test
	testMethord();
}

/////////////////////////////////////////
//initial occ viewer
/////////////////////////////////////////
void QtOcafMainWin::initOcaf()
{
	//initial ocaf app
	myApp = new TOcafApplication();
	BinDrivers::DefineFormat(myApp);
	XmlDrivers::DefineFormat(myApp);

	////dont clear meaning
	//try
	//{
	//	UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);
	//}
	//catch (Standard_Failure)
	//{
	//	AfxMessageBox(L"Fatal Error in units initialisation");
	//}

	//initial ocaf doc
	myApp->NewDocument("BinOcaf", myOcafDoc);

	//create a display connection
	Handle(Aspect_DisplayConnection) aDisplayConnection;
	//create a graphic driver
	Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
	myGraphicDriver = aGraphicDriver;

	//create a viewer
	myViewer = new V3d_Viewer(myGraphicDriver);
	TPrsStd_AISViewer::New(myOcafDoc->Main(), myViewer);
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();

	//create an interactive context
	TPrsStd_AISViewer::Find(myOcafDoc->Main(), myContext);
	myContext->SetDisplayMode(AIS_Shaded, true);

	//enable "undo/redo"
	//set the limit
	myOcafDoc->SetUndoLimit(999);

	//create an occ widget
	myOccWidget = new OccWidget(myContext, this);

	//add widget to the MainWindow
	setCentralWidget(myOccWidget);

}

void QtOcafMainWin::testMethord()
{
	//add a cube to the scene
	//TopoDS_Shape aShape = BRepPrimAPI_MakeBox(50, 50, 50).Solid();
	//Handle(AIS_Shape) anAisShape = new AIS_Shape(aShape);
	//myContext->Display(anAisShape, true);

	//get doc
	Handle(TDocStd_Document) D = GetOcafDoc();

	// Open a new command (for undo)
	D->NewCommand();

	//get the main lable
	auto MainLab = D->Main();

	// Create a new label in the data structure for the box
	TDF_Label L = TDF_TagSource::NewChild(MainLab);

	// Create the data structure : Set the dimensions, position and name attributes
	TDataStd_Real::Set(L.FindChild(1), 200);
	TDataStd_Real::Set(L.FindChild(2), 200);
	TDataStd_Real::Set(L.FindChild(3), 200);
	TDataStd_Real::Set(L.FindChild(4), 0);
	TDataStd_Real::Set(L.FindChild(5), 0);
	TDataStd_Real::Set(L.FindChild(6), 0);
	TDataStd_Name::Set(L, TCollection_ExtendedString(""));

	// Instanciate a TFunction_Function attribute connected to the current box driver
	// and attach it to the data structure as an attribute of the Box Label
	Handle(TFunction_Function) myFunction = TFunction_Function::Set(L, TOcafFunction_BoxDriver::GetID());

	// Set the log book
	Handle(TFunction_Logbook) log = TFunction_Logbook::Set(L);


	// Initialize and execute the box driver (look at the "Execute()" code)
	Handle(TFunction_Driver) myBoxDriver;
	// Find the TOcafFunction_BoxDriver in the TFunction_DriverTable using its GUID
	TFunction_DriverTable::Get()->FindDriver(TOcafFunction_BoxDriver::GetID(), myBoxDriver);
	myBoxDriver->Init(L); 
	myBoxDriver->Execute(log);

	// Get the TPrsStd_AISPresentation of the new box TNaming_NamedShape
	Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(L, TNaming_NamedShape::GetID());

	// Attach an integer attribute to L to memorize it's displayed
	TDataStd_Integer::Set(L, 1);
	// Display it
	prs->Display(true);

	myContext->UpdateCurrentViewer();
	

	// Close the command (for undo)
	D->CommitCommand();

	//modify the box

	// Open a new command (for undo)
	D->NewCommand();
}