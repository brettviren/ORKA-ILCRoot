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

/* $Id: IlcTARGETdigit.cxx,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

////////////////////////////////////////////////
//  Digits classes for all TARGET detectors      //
//                                            //
//                                            //
////////////////////////////////////////////////

#include "IlcTARGETdigit.h"

//______________________________________________________________________
ClassImp(IlcTARGETdigit)
IlcTARGETdigit::IlcTARGETdigit(const Int_t *digits) {
  // Creates a real data digit object

  fCoord1       = digits[0];
  fCoord2       = digits[1];
  fSignal       = digits[2];
}
//______________________________________________________________________
void IlcTARGETdigit::Print(ostream *os) {
    //Standard output format for this class

    *os << fCoord1 <<","<< fCoord2 <<","<< fSignal;
}
//______________________________________________________________________
void IlcTARGETdigit::Read(istream *os) {
    //Standard input for this class

    *os >> fCoord1 >> fCoord2 >> fSignal;
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETdigit &source){
    // Standard output streaming function.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETdigit &source){
    // Standard output streaming function.

    source.Read(&os);
    return os;
}


