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

//-----------------------------------------------------------------
//           Implementation of the Event Time class
//           for the Event Data Summary class
//           This class contains the Event Time
//           as estimated by the TOF combinatorial algorithm
// Origin: A.De Caro, decaro@sa.infn.it
//-----------------------------------------------------------------

//---- standard headers ----
#include "Riostream.h"
//---- Root headers --------
#include "TArrayF.h"
#include "TArrayI.h"
//---- IlcRoot headers -----
#include "IlcTOFHeader.h"


ClassImp(IlcTOFHeader)

//--------------------------------------------------------------------------
IlcTOFHeader::IlcTOFHeader() :
  TObject(),
  fDefaultEventTimeValue(0.),
  fDefaultEventTimeRes(0.),
  fNbins(0),
  fEventTimeValues(0),
  fEventTimeRes(0),
  fNvalues(0),
  fTOFtimeResolution(0.),
  fT0spread(0.)
{
  //
  // Default Constructor
  //

}

//--------------------------------------------------------------------------
IlcTOFHeader::IlcTOFHeader(Float_t defEvTime, Float_t defResEvTime,
			   Int_t nDifPbins, Float_t *times, Float_t *res,
			   Int_t *nPbin, Float_t tofTimeRes, Float_t t0spread) :
  TObject(),
  fDefaultEventTimeValue(defEvTime),
  fDefaultEventTimeRes(defResEvTime),
  fNbins(nDifPbins),
  fEventTimeValues(0),
  fEventTimeRes(0),
  fNvalues(0),
  fTOFtimeResolution(tofTimeRes),
  fT0spread(t0spread)
{
  //
  // Constructor for TOF header
  //

  if (fNbins>0) {
    fEventTimeValues = new TArrayF(fNbins);
    fEventTimeRes = new TArrayF(fNbins);
    fNvalues = new TArrayI(fNbins);
    for (Int_t ii=0; ii<fNbins; ii++) {
      fEventTimeValues->SetAt(times[ii],ii);
      fEventTimeRes->SetAt(res[ii],ii);
      fNvalues->SetAt(nPbin[ii],ii);
    }
  }

}

//--------------------------------------------------------------------------
IlcTOFHeader::IlcTOFHeader(const IlcTOFHeader &source):
  TObject(source),
  fDefaultEventTimeValue(source.fDefaultEventTimeValue),
  fDefaultEventTimeRes(source.fDefaultEventTimeRes),
  fNbins(source.fNbins),
  fEventTimeValues(0),
  fEventTimeRes(0),
  fNvalues(0),
  fTOFtimeResolution(source.fTOFtimeResolution),
  fT0spread(source.fT0spread)
{
  //
  // Copy constructor
  //

  if (fNbins>0) {
    fEventTimeValues = new TArrayF(fNbins);
    fEventTimeRes = new TArrayF(fNbins);
    fNvalues = new TArrayI(fNbins);
    for(Int_t i=0;i<fNbins;i++) {
      (*fEventTimeValues)[i]=source.fEventTimeValues->At(i);
      (*fEventTimeRes)[i]=source.fEventTimeRes->At(i);
      (*fNvalues)[i]=source.fNvalues->At(i);
    }
  }


}
//--------------------------------------------------------------------------
IlcTOFHeader &IlcTOFHeader::operator=(const IlcTOFHeader &source){
  //
  // assignment operator
  //
  if(&source != this){
    TObject::operator=(source);

    fDefaultEventTimeValue=source.fDefaultEventTimeValue;
    fDefaultEventTimeRes=source.fDefaultEventTimeRes;
    fNbins=source.fNbins;
    fTOFtimeResolution=source.fTOFtimeResolution;
    fT0spread=source.fT0spread;

    if (fNbins>0) {
      fEventTimeValues = new TArrayF(fNbins);
      fEventTimeRes = new TArrayF(fNbins);
      fNvalues = new TArrayI(fNbins);
      for(Int_t i=0;i<fNbins;i++) {
	(*fEventTimeValues)[i]=source.fEventTimeValues->At(i);
	(*fEventTimeRes)[i]=source.fEventTimeRes->At(i);
	(*fNvalues)[i]=source.fNvalues->At(i);
      }
    } else {
      fEventTimeValues = 0;
      fEventTimeRes = 0;
      fNvalues = 0;
    }

  }
  return *this;
}
//--------------------------------------------------------------------------
void IlcTOFHeader::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if (this==&obj) return;
  IlcTOFHeader *robj = dynamic_cast<IlcTOFHeader*>(&obj);
  if (!robj) return; // not an IlcTOFHeader
  *robj = *this;

}

//--------------------------------------------------------------------------
IlcTOFHeader::~IlcTOFHeader()
{

  fNbins = 0;
  if (fEventTimeValues) {
    delete fEventTimeValues;
    fEventTimeValues=0;
  }
  if (fEventTimeRes) {
    delete fEventTimeRes;
    fEventTimeRes=0;
  }
  if (fNvalues) {
    delete fNvalues;
    fNvalues=0;
  }

}

//--------------------------------------------------------------------------
void IlcTOFHeader::SetNbins(Int_t nbins)
{
  //
  //
  //

  fNbins=nbins;
  if (!fEventTimeValues)
    fEventTimeValues = new TArrayF(nbins);
  else
    fEventTimeValues->Set(nbins);

  if (!fEventTimeRes)
    fEventTimeRes = new TArrayF(nbins);
  else
    fEventTimeRes->Set(nbins);

  if (!fNvalues)
    fNvalues = new TArrayI(nbins);
  else
    fNvalues->Set(nbins);

}
//--------------------------------------------------------------------------
void IlcTOFHeader::SetEventTimeValues(TArrayF *arr)
{
  //
  //
  //

  fNbins=arr->GetSize();

  if (!fEventTimeValues)
    fEventTimeValues = new TArrayF(fNbins);
  else
    fEventTimeValues->Set(fNbins);

  for (Int_t ii=0; ii<fNbins; ii++)
    fEventTimeValues->SetAt(arr->GetAt(ii),ii);

}
//--------------------------------------------------------------------------
void IlcTOFHeader::SetEventTimeRes(TArrayF *arr)
{
  //
  //
  //

  fNbins=arr->GetSize();

  if (!fEventTimeRes)
    fEventTimeRes = new TArrayF(fNbins);
  else
    fEventTimeRes->Set(fNbins);

  for (Int_t ii=0; ii<fNbins; ii++)
    fEventTimeRes->SetAt(arr->GetAt(ii),ii);

}
//--------------------------------------------------------------------------
void IlcTOFHeader::SetNvalues(TArrayI *arr)
{
  //
  //
  //

  fNbins=arr->GetSize();

  if (!fNvalues)
    fNvalues = new TArrayI(fNbins);
  else
    fNvalues->Set(fNbins);

  for (Int_t ii=0; ii<fNbins; ii++)
    fNvalues->SetAt(arr->GetAt(ii),ii);

}
