#include "TOcafApplication.h"

#include <TFunction_DriverTable.hxx>

#include "TOcafFunction_BoxDriver.h"

IMPLEMENT_STANDARD_RTTIEXT(TOcafApplication, TDocStd_Application)

TOcafApplication::TOcafApplication()
{
	//add a boxdriver
	// Instanciate a TOcafFunction_BoxDriver and add it to the TFunction_DriverTable
	TFunction_DriverTable::Get()->AddDriver(TOcafFunction_BoxDriver::GetID(),
		new TOcafFunction_BoxDriver());

	//// Instanciate a TOcafFunction_Cyl  Driver and add it to the TFunction_DriverTable
	//TFunction_DriverTable::Get()->AddDriver(TOcafFunction_CylDriver::GetID(),
	//	new TOcafFunction_CylDriver());

	//// Instanciate a TOcafFunction_CutDriver and add it to the TFunction_DriverTable
	//Handle(TOcafFunction_CutDriver) myCutDriver = new TOcafFunction_CutDriver();
	//TFunction_DriverTable::Get()->AddDriver(TOcafFunction_CutDriver::GetID(),
	//	new TOcafFunction_CutDriver());

}
