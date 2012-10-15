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
// IlcRawDataArray                                                      //
// A container object which is used in order to write the sub-detector  //
// raw-data payloads into a separate branches                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>

#include "IlcRawDataArray.h"


ClassImp(IlcRawDataArray)

IlcRawDataArray::IlcRawDataArray():
fRawDataArray(NULL)
{
  // Default constructor
}

IlcRawDataArray::IlcRawDataArray(Int_t n):
fRawDataArray(new TObjArray(n))
{
  // Default constructor
}

IlcRawDataArray::~IlcRawDataArray()
{
  if (fRawDataArray) delete fRawDataArray;
}

void IlcRawDataArray::ClearData()
{
  if (fRawDataArray) fRawDataArray->Clear();
}

void IlcRawDataArray::Add(IlcRawData *data)
{
  if (fRawDataArray)
    fRawDataArray->Add((TObject *)data);
  else
    Error("Add", "TObjArray is not initialized! Cannot add raw-data payload!");
}
