/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcTARGETRawStreamSSDv1.cxx,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to beam test TARGET SSD digits in raw data.
//  Modified by Enrico Fragiacomo, October 2004, for beamtest analysis
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStreamSSDv1.h"
#include "IlcRawReader.h"

ClassImp(IlcTARGETRawStreamSSDv1)


IlcTARGETRawStreamSSDv1::IlcTARGETRawStreamSSDv1(IlcRawReader* rawReader) :
  IlcTARGETRawStreamSSD(rawReader)
{
// create an object to read TARGET SSD raw digits

  fADC =0;
  fADModule=0;
  fRawReader->Select(3);
  fRawReader->SelectEquipment(17,102,102);
}


Bool_t IlcTARGETRawStreamSSDv1::Next()
{
// read the next raw digit
// returns kFALSE if there is no digit left

  Int_t seq=0;

  fPrevModuleID = fModuleID;

  if (!fRawReader->ReadNextInt(fData)) return kFALSE;

  fADModule = (fData >> 27) & 0x0000000F;
  fADC       = (fData >> 23) & 0x0000000F;

  // seq 0 (first 768 strips) or 1 are obtained from fCoord2
  fCoord2    = (fData >> 12) & 0x000007FF;
  seq    = (fCoord2 >= 768) ? 1 : 0; 
  if(seq) fCoord2 = 1535 - fCoord2;

  if((fCoord2<0)||(fCoord2>=768)) return kFALSE;

  // ADModule 2 -> layer 5
  // fCoord1 is set according to the cabling map
  if(fADModule==2) {
    if ((fADC==0)&&(seq==0)) {fModuleID = 10; fCoord1=1;}
    else if ((fADC==0)&&(seq==1)) {fModuleID = 11; fCoord1=1;}
    else if ((fADC==1)&&(seq==0)) {fModuleID = 11; fCoord1=0;}
    else if ((fADC==1)&&(seq==1)) {fModuleID = 10; fCoord1=0;}
  }
  // ADModule 6 -> layer 6
  // fCoord1 is set according to the cabling map
  else if(fADModule==6) {
    if ((fADC==0)&&(seq==0)) {fModuleID = 12; fCoord1=0;}
    else if ((fADC==0)&&(seq==1)) {fModuleID = 13; fCoord1=0;}
    else if ((fADC==1)&&(seq==0)) {fModuleID = 13; fCoord1=1;}
    else if ((fADC==1)&&(seq==1)) {fModuleID = 12; fCoord1=1;}
  }

  fSignal    = (fData & 0x00000FFF);

  if(fSignal>=2048) fSignal-=4096;

  return kTRUE;
}

