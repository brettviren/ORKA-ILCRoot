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
/* $Id: IlcPVECDATreeDigit.cxx 35905 2009-10-25 19:35:32Z kharlov $ */

// --
// --
// Implementation for TTree output in PVEC DA
// for calibrating energy by pi0 and MIP.
// --
// -- Author: Hisayuki Torii (Hiroshima Univ.)
// --
#include <iostream>
#include <math.h>
#include <Rtypes.h>
#include "IlcPVECDATreeDigit.h"

ClassImp(IlcPVECDATreeDigit)
//------------------------------------------------------------------------
bool IlcPVECDATreeDigit::IsNeighbor(const IlcPVECDATreeDigit& digit) const{
  // Check wether the given digit is neighboring to this digit.
  // Return true if yes.
  if( fabs(this->GetRow() - digit.GetRow()) < 2 && fabs(this->GetCol() - digit.GetCol()) < 2 ){
    return true;
  }
  return false;
}
//------------------------------------------------------------------------
void IlcPVECDATreeDigit::Print(Option_t *opt) const
{
  // Print out
  std::cout<<" IlcPVECDATreeDigit:: "<<opt<<" E="<<fEnergy<<" (row,col)=("
	   <<GetRow()<<","<<GetCol()<<") absid="<<fAbsId<<std::endl;
}
//------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const IlcPVECDATreeDigit& digit){
  // Print out
  //std::cout<<" IlcPVECDATreeDigit:: E="<<digit.GetEnergy()<<" (row,col)=("
  //<<digit.GetRow()<<","<<digit.GetCol()<<") absid="<<digit.GetAbsId()<<std::endl;
  out<<" IlcPVECDATreeDigit:: E="<<digit.fEnergy<<" (row,col)=("
  <<(int)(digit.fAbsId/56)<<","<<digit.fAbsId%56<<") absid="<<digit.fAbsId;
  return out;
}
//------------------------------------------------------------------------
