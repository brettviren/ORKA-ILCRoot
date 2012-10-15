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
/* 
   4x4 TRU represented by position and amplitude.
   Author: Boris Polishchuk (Boris.Polishchuk@cern.ch)
*/

// --- IlcRoot header files ---
#include "IlcRSTACKTriggerRawDigit.h"

ClassImp(IlcRSTACKTriggerRawDigit)

IlcRSTACKTriggerRawDigit::IlcRSTACKTriggerRawDigit() : 
IlcDigitNew(),fMod(-1),fXIdx(-1),fZIdx(-1),fTRURow(-1),fBranch(-1)
{
  //Default constructor.
}

IlcRSTACKTriggerRawDigit::IlcRSTACKTriggerRawDigit(Int_t module, Int_t xIdx, Int_t zIdx, Int_t TRURow, Int_t branch, Int_t amp) :
  IlcDigitNew(),fMod(module),fXIdx(xIdx),fZIdx(zIdx),fTRURow(TRURow),fBranch(branch)
{
  fAmp = amp;
}

IlcRSTACKTriggerRawDigit::IlcRSTACKTriggerRawDigit(const IlcRSTACKTriggerRawDigit & tdigit) :
  IlcDigitNew(tdigit),fMod(tdigit.fMod),fXIdx(tdigit.fXIdx),fZIdx(tdigit.fZIdx),
  fTRURow(tdigit.fTRURow),fBranch(tdigit.fBranch) 
{
  fAmp = tdigit.fAmp;
}

IlcRSTACKTriggerRawDigit& IlcRSTACKTriggerRawDigit::operator=(const IlcRSTACKTriggerRawDigit & tdigit)
{
  if (&tdigit != this) {
    IlcDigitNew::operator=(tdigit);
    fMod = tdigit.fMod;
    fXIdx = tdigit.fXIdx;
    fZIdx = tdigit.fZIdx;
    fTRURow = tdigit.fTRURow;
    fBranch = tdigit.fBranch; 
  }

  return *this;
}

void IlcRSTACKTriggerRawDigit::GetModXZ(Int_t& mod, Int_t& modX, Int_t& modZ)
{
  //Return 4x4 region bottom left cell position.

  Int_t kN2x2XPrTRURow = 8;
  Int_t kN2x2ZPrBranch = 14;
  
  modX = (fXIdx + fTRURow * kN2x2XPrTRURow)*2;
  modZ = (fZIdx + fBranch * kN2x2ZPrBranch)*2;
  mod = fMod;
  
}
