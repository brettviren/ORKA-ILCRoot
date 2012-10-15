/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This class represents the value(s) of a the LHC DPs at a given timestamp   //
// The variuos measurement that occurred at the same timestamp are associated //
// to the same timestamp.                                                     //  
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "IlcDCSArray.h"
#include "IlcLog.h"

//#include "TTimeStamp.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>
#include <TMath.h>

ClassImp(IlcDCSArray)
	
IlcDCSArray::IlcDCSArray() :
		TObject(),
		fType(kInvalid),
		fnentries(0),
		fBool(0x0),
		fChar(0x0),
		fInt(0x0),
		fUInt(0x0),
		fFloat(0x0),
		fStringArray(0x0),
		fTimeStamp(-1.),
		fDouble(0x0)
{
	//
	// default constructor
	//
}

//--------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, Bool_t* value, Double_t timeStamp) : 
	TObject(),
	fType(kBool),
	fnentries(nentries),
	fBool(new Bool_t[fnentries]),
	fChar(0x0),
	fInt(0x0),
	fUInt(0x0),
	fFloat(0x0),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for Bool
	//
	
	for (Int_t i = 0; i<fnentries; i++){
		fBool[i] = value[i];
	}
}

//--------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, Char_t* value, Double_t timeStamp) :
	TObject(),
	fType(kChar),
	fnentries(nentries),
	fBool(0x0),
	fChar(new Char_t[fnentries]),
	fInt(0x0),
	fUInt(0x0),
	fFloat(0x0),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for Char
	//
	for (Int_t i = 0; i<fnentries; i++){
		fChar[i] = value[i];
	}
}

//-------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, Int_t* value, Double_t timeStamp) :
	TObject(),
	fType(kInt),
	fnentries(nentries),
	fBool(0x0),
	fChar(0x0),
	fInt(new Int_t[fnentries]),
	fUInt(0x0),
	fFloat(0x0),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for Int
	//
	for (Int_t i = 0; i<fnentries; i++){
		fInt[i] = value[i];
	}
}

//-------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, UInt_t* value, Double_t timeStamp) :
	TObject(),
	fType(kUInt),
	fnentries(nentries),
	fBool(0x0),
	fChar(0x0),
	fInt(0x0),
	fUInt(new UInt_t[fnentries]),
	fFloat(0x0),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for UInt
	//

	for (Int_t i = 0; i<fnentries; i++){
		fUInt[i] = value[i];
	}
}

//-------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, Float_t* value, Double_t timeStamp) :
	TObject(),
	fType(kFloat),
	fnentries(nentries),
	fBool(0x0),
	fChar(0x0),
	fInt(0x0),
	fUInt(0x0),
	fFloat(new Float_t[fnentries]),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for Float
	//

	for (Int_t i = 0; i<fnentries; i++){
		fFloat[i] = value[i];
	}
}
//-------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, Double_t* value, Double_t timeStamp) :
	TObject(),
	fType(kDouble),
	fnentries(nentries),
	fBool(0x0),
	fChar(0x0),
	fInt(0x0),
	fUInt(0x0),
	fFloat(0x0),
	fStringArray(0x0),
	fTimeStamp(timeStamp),
	fDouble(new Double_t[fnentries])
{
	//
	// constructor for Double
	//

	for (Int_t i = 0; i<fnentries; i++){
		fDouble[i] = value[i];
	}
}

//------------------------------------------------------------------------
IlcDCSArray::IlcDCSArray(Int_t nentries, TObjArray* value, Double_t timeStamp) :
	TObject(),
	fType(kString),
	fnentries(nentries),
	fBool(0x0),
	fChar(0x0),
	fInt(0x0),
	fUInt(0x0),
	fFloat(0x0),
	fStringArray(new TObjArray()),
	fTimeStamp(timeStamp),
	fDouble(0x0)
{
	//
	// constructor for String
	//

	fStringArray->SetOwner(1);
	for (Int_t i = 0; i<fnentries; i++){
		TObjString* strobj = new TObjString();
		strobj->SetString(((TObjString*)value->At(i))->String());
		fStringArray->Add(strobj);
	}
}

//-----------------------------------------------------------------------
IlcDCSArray::~IlcDCSArray()
{
	//
	// destructor
	//

	if (fBool){
		delete [] fBool;
		fBool = 0x0;
	}
	if (fChar){
		delete [] fChar;
		fChar = 0x0;
	}
	if (fUInt){
		delete [] fUInt;
		fUInt = 0x0;
	}
	if (fInt){
		delete [] fInt;
		fInt = 0x0;
	}
	if (fFloat){
		delete [] fFloat;
		fFloat = 0x0;
	}
	if (fStringArray!=0x0){
		delete fStringArray;
		fStringArray = 0x0;
	}
	if (fDouble){
		delete [] fDouble;
		fDouble = 0x0;
	}
}

//-----------------------------------------------------------------------
void IlcDCSArray::Init()
{
	//
	// init helper, that initializes everything to 0
	//
	
	fType = kInvalid;
	
	fnentries = 0;
	fBool = 0x0;
	fChar = 0x0;
	fInt = 0x0;
	fUInt = 0x0;
	fFloat = 0x0;
	fDouble = 0x0;
	fStringArray = 0x0;
	
	fTimeStamp = -1.;
}

