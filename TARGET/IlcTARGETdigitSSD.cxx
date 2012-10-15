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

#include <IlcTARGETdigitSSD.h>
#include <TArrayI.h>

///////////////////////////////////////////////////////////////////
//                                                               //
// Class defining the digit object
// for SSD
// Inherits from IlcTARGETdigit
//                                                               //
///////////////////////////////////////////////////////////////////

ClassImp(IlcTARGETdigitSSD)

//______________________________________________________________________
IlcTARGETdigitSSD::IlcTARGETdigitSSD():IlcTARGETdigit(){
    // default constructor
    Int_t i;

    for(i=0; i<fgkSssd; i++) fTracks[i] = -3;
    for(i=0; i<fgkSssd; i++) fHits[i] = -1;
}
//__________________________________________________________________________
IlcTARGETdigitSSD::IlcTARGETdigitSSD(const Int_t *digits):IlcTARGETdigit(digits){
    // Creates a real SSD digit object
}
//_____________________________________________________________________________
IlcTARGETdigitSSD::IlcTARGETdigitSSD(const Int_t *digits,const Int_t *tracks,
			       const Int_t *hits):IlcTARGETdigit(digits){
    // Creates a simulated SSD digit object

    for(Int_t i=0; i<fgkSssd; i++) {
	fTracks[i] = tracks[i];
	fHits[i]   = hits[i];
    } // end for i
}
//______________________________________________________________________
Int_t IlcTARGETdigitSSD::GetListOfTracks(TArrayI &t){
    // Fills the TArrayI t with the tracks found in fTracks removing
    // duplicated tracks, but otherwise in the same order. It will return
    // the number of tracks and fill the remaining elements to the array
    // t with -1.
    // Inputs:
    //   TArrayI  &t Reference to a TArrayI to contain the list of
    //               nonduplicated track numbers.
    // Output:
    //   TArrayI  &t The input array filled with the nonduplicated track
    //               numbers.
    // Return:
    //   Int_t The number of none -1 entries in the TArrayI t.
    Int_t nt = t.GetSize();
    Int_t nth = this->GetNTracks();
    Int_t n = 0,i,j;
    Bool_t inlist = kFALSE;

    t.Reset(-1); // -1 array.
    for(i=0;i<nth;i++) {
	if(this->GetTrack(i) == -1) continue;
	inlist = kFALSE;
	for(j=0;j<n;j++)if(this->GetTrack(i) == t.At(j)) inlist = kTRUE;
	if(!inlist){ // add to end of list
	    t.AddAt(this->GetTrack(i),n);
	    if(n<nt) n++;
	} // end if
    } // end for i
    return n;
}
//______________________________________________________________________
void IlcTARGETdigitSSD::Print(ostream *os){
    //Standard output format for this class
    Int_t i;

    IlcTARGETdigit::Print(os);
    for(i=0; i<fgkSssd; i++) *os <<","<< fTracks[i];
    for(i=0; i<fgkSssd; i++) *os <<","<< fHits[i];
}
//______________________________________________________________________
void IlcTARGETdigitSSD::Read(istream *os){
    //Standard input for this class
    Int_t i;

    IlcTARGETdigit::Read(os);
    for(i=0; i<fgkSssd; i++) *os >> fTracks[i];
    for(i=0; i<fgkSssd; i++) *os >> fHits[i];
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETdigitSSD &source){
    // Standard output streaming function.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETdigitSSD &source){
    // Standard output streaming function.

    source.Read(&os);
    return os;
}
