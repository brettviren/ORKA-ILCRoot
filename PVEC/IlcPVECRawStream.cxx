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

/* $Id: IlcPVECRawStream.cxx 14794 2006-08-08 16:33:17Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to PVEC digits in raw data.
///
/// It loops over all PVEC digits in the raw data given by the IlcRawReader.
/// The Next method goes to the next digit. If there are no digits left
/// it returns kFALSE.
/// Several getters provide information about the current digit.
/// usage: 
/// root > IlcRawReaderFile rawReader ; 
/// root > IlcPVECRawStream input(&rawReader) ; 
/// root > while (input.Next()) ..... 
///////////////////////////////////////////////////////////////////////////////

#include <TString.h>
#include <TSystem.h>

#include "IlcPVECRawStream.h"
#include "IlcRawReader.h"
#include "IlcPVECAltroMapping.h"

ClassImp(IlcPVECRawStream)


//_____________________________________________________________________________
IlcPVECRawStream::IlcPVECRawStream(IlcRawReader* rawReader) :
  IlcAltroRawStream(rawReader),
  fModule(-1),
  fPrevModule(-1),
  fRow(-1),
  fPrevRow(-1),
  fColumn(-1),
  fPrevColumn(-1),
  fGain(0)
{
// create an object to read PVEC raw digits

  SelectRawData("PVEC");

  TString path = gSystem->Getenv("ILC_ROOT");
  path += "/PVEC/mapping/RCU";
  TString path2;
  for(Int_t i = 0; i < 4; i++) {
    path2 = path;
    path2 += i;
    path2 += ".data";
    fMapping[i] = new IlcPVECAltroMapping(path2.Data());
  }

  SetNoAltroMapping(kFALSE);
}

//_____________________________________________________________________________
IlcPVECRawStream::IlcPVECRawStream(const IlcPVECRawStream& stream) :
  IlcAltroRawStream(stream),
  fModule(-1),
  fPrevModule(-1),
  fRow(-1),
  fPrevRow(-1),
  fColumn(-1),
  fPrevColumn(-1),
  fGain(0)
{  
  Fatal("IlcPVECRawStream", "copy constructor not implemented");
}

//_____________________________________________________________________________
IlcPVECRawStream& IlcPVECRawStream::operator = (const IlcPVECRawStream& 
					      /* stream */)
{
  Fatal("operator =", "assignment operator not implemented");
  return *this;
}

//_____________________________________________________________________________
IlcPVECRawStream::~IlcPVECRawStream()
{
// destructor

  for(Int_t i = 0; i < 4; i++) delete fMapping[i];
}

//_____________________________________________________________________________
void IlcPVECRawStream::Reset()
{
  // reset PVEC raw stream params
  IlcAltroRawStream::Reset();
  fModule = fPrevModule = fRow = fPrevRow = fColumn = fPrevColumn = -1;
  fGain = 0;
}

//_____________________________________________________________________________
Bool_t IlcPVECRawStream::Next()
{
  // Read next PVEC signal
  // Apply the PVEC altro mapping to get
  // the module,row and column indeces
  fPrevModule = fModule;
  fPrevRow = fRow;
  fPrevColumn = fColumn;
  if (IlcAltroRawStream::Next()) {
    if (IsNewHWAddress())
      ApplyAltroMapping();
    return kTRUE;
  }
  else
    return kFALSE;
}

//_____________________________________________________________________________
void IlcPVECRawStream::ApplyAltroMapping()
{
  // Take the DDL index, load
  // the corresponding altro mapping
  // object and fill the sector,row and pad indeces
  Int_t ddlNumber = GetDDLNumber();
  fModule = ddlNumber / 4;

  Int_t rcuIndex = ddlNumber % 4;

  Short_t hwAddress = GetHWAddress();
  fRow = fMapping[rcuIndex]->GetPadRow(hwAddress);
  fColumn = fMapping[rcuIndex]->GetPad(hwAddress);
  fGain = fMapping[rcuIndex]->GetSector(hwAddress);

}
