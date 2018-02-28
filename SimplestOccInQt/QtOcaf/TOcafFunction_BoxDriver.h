#pragma once

#include <TFunction_Driver.hxx>

class TOcafFunction_BoxDriver :public TFunction_Driver
{
public:
	static const Standard_GUID& GetID();
	TOcafFunction_BoxDriver();
	virtual  void Validate(Handle(TFunction_Logbook)& log) const;
	virtual  Standard_Boolean MustExecute(const Handle(TFunction_Logbook)& log) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(TOcafFunction_BoxDriver, TFunction_Driver)
};

