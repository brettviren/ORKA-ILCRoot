

// Last update: October 2th 2009 

#include "IlcESDACORDE.h"

ClassImp(IlcESDACORDE)

IlcESDACORDE::IlcESDACORDE():TObject()
{
 //Default constructor
	for(Int_t i=0;i<60;i++)
	{
		fACORDEBitPattern[i] = 0;
	}
}


IlcESDACORDE::IlcESDACORDE(const IlcESDACORDE &o)
  :TObject(o)

{	
	//Default constructor
	for(Int_t i=0;i<60;i++)
	{
		fACORDEBitPattern[i] = o.fACORDEBitPattern[i];
	}
}


IlcESDACORDE::IlcESDACORDE(Bool_t* MACORDEBitPattern):TObject()
{

	//Constructor

	for(Int_t i=0;i<60;i++)
	{
		fACORDEBitPattern[i] = MACORDEBitPattern[i];
	}
}

IlcESDACORDE& IlcESDACORDE::operator=(const IlcESDACORDE& o)
{
// Copy Constructor
	if(this==&o)return *this;
	TObject::operator=(o);

	// Assignment operator
	for(Int_t i=0; i<60; i++)
	{
		fACORDEBitPattern[i] = o.fACORDEBitPattern[i];
	}
	
	return *this;
}


Bool_t IlcESDACORDE::GetHitChannel(Int_t i) const
{
	return fACORDEBitPattern[i];
}

void IlcESDACORDE::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcESDACORDE *robj = dynamic_cast<IlcESDACORDE*>(&obj);
  if(!robj)return; // not an IlcESDACRDE
  *robj = *this;

}


