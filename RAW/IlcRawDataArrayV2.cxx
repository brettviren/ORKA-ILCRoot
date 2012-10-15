// Author: Cvetan Cheshkov 27/03/2007

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

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcRawDataArrayV2                                                    //
// A container object which is used in order to write the sub-detector  //
// raw-data payloads into a separate branches                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "IlcRawDataArrayV2.h"
#include "IlcRawData.h"


ClassImp(IlcRawDataArrayV2)

IlcRawDataArrayV2::IlcRawDataArrayV2():
fRawDataArray("IlcRawData",100),
fNAlloc(0)
{
  // Default constructor
}

IlcRawDataArrayV2::IlcRawDataArrayV2(Int_t n):
fRawDataArray("IlcRawData",n),
fNAlloc(0)
{
  // Default constructor
}

IlcRawDataArrayV2::~IlcRawDataArrayV2()
{
  fRawDataArray.Delete();
}

void IlcRawDataArrayV2::ClearData()
{
  fRawDataArray.Clear();
}

IlcRawData *IlcRawDataArrayV2::Add()
{
  Int_t nEntries = fRawDataArray.GetEntriesFast();
  if (nEntries < fNAlloc) {
    return (IlcRawData *)fRawDataArray[nEntries];
  }
  else {
    fNAlloc = nEntries + 1;
    return new (fRawDataArray[nEntries]) IlcRawData();
  }
}
