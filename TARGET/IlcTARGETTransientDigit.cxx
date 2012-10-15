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

#include <TObjArray.h>
#include "IlcTARGETTransientDigit.h"

///////////////////////////////////////////////////////////////////
//                                                               //
// Class used internally by IlcTARGETsimulationSDD
// for SDD digitisation
// It is not currently used any longer
// The methods in ALiTARGETsimulationSDD using it are currently commented out
//                                                               //
///////////////////////////////////////////////////////////////////

ClassImp(IlcTARGETTransientDigit)

//______________________________________________________________________
IlcTARGETTransientDigit::IlcTARGETTransientDigit(Float_t phys,const Int_t *digits): 
    IlcTARGETdigitSDD(phys,digits) {
    // Creates a digit object in a list of digits to be updated

    fTrackList   = new TObjArray;  
}
//__________________________________________________________________________
IlcTARGETTransientDigit::IlcTARGETTransientDigit(const IlcTARGETTransientDigit &source):
 IlcTARGETdigitSDD(source){
    // Copy Constructor 
    if(&source == this) return;
    this->fTrackList = source.fTrackList;
    return;
}
//_________________________________________________________________________
IlcTARGETTransientDigit& IlcTARGETTransientDigit::operator=(
    const IlcTARGETTransientDigit &source) {
    // Assignment operator
    if(&source == this) return *this;
    this->fTrackList = source.fTrackList;
    return *this;
}
//______________________________________________________________________
void IlcTARGETTransientDigit::Print(ostream *os){
    //Standard output format for this class

    IlcTARGETdigitSDD::Print(os);
}
//______________________________________________________________________
void IlcTARGETTransientDigit::Read(istream *os){
    //Standard input for this class

    IlcTARGETdigitSDD::Read(os);
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETTransientDigit &source){
    // Standard output streaming function.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETTransientDigit &source){
    // Standard output streaming function.

    source.Read(&os);
    return os;
}
