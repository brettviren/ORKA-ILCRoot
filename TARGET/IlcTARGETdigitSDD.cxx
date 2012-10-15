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

#include <IlcTARGETdigitSDD.h>
#include <IlcTARGETCalibrationSDD.h>
#include <IlcTARGETresponseSDD.h>
#include <TArrayI.h>
#include <TArrayF.h>

///////////////////////////////////////////////////////////////////
//                                                               //
// Class defining the digit object
// for SDD
// Inherits from IlcTARGETdigit
//                                                               //
///////////////////////////////////////////////////////////////////

ClassImp(IlcTARGETdigitSDD)


//______________________________________________________________________
IlcTARGETdigitSDD::IlcTARGETdigitSDD():IlcTARGETdigit(){
    // default constructor, zero coordinates and set array
    // elements to clearly unphysical values. A value of 0 may
    // be a vilcde track of hit number.
    Int_t i;

    for(i=0;i<fgkSsdd;i++) fTracks[i] = -3;
    for(i=0;i<fgkSsdd;i++) fHits[i]   = -1;
    fPhysics = 0;
    for(i=0;i<fgkSsdd;i++) fTcharges[i] = 0;
    SetSignalExpanded(-1000);
}
//________________________________________________________________________
IlcTARGETdigitSDD::IlcTARGETdigitSDD(Float_t phys,const Int_t *digits): IlcTARGETdigit(digits){
 
   // Creates a simulated SDD digit object to be updated

    fPhysics = phys;
    SetSignalExpanded(-1000);
}

//________________________________________________________________________
void IlcTARGETdigitSDD::InitObject(Float_t phys,const Int_t *tracks,
			   const Int_t *hits,const Float_t *charges){

  // Protected function used by standard constructors
  fPhysics = phys;
  for(Int_t i=0; i<fgkSsdd; i++) {
    fTcharges[i] = charges[i];
    fTracks[i]   = tracks[i];
    fHits[i]     = hits[i];
  }
}
 
//_____________________________________________________________________________
IlcTARGETdigitSDD::IlcTARGETdigitSDD(Float_t phys,const Int_t *digits,
			       const Int_t *tracks,const Int_t *hits,
			       const Float_t *charges):IlcTARGETdigit(digits){

// standard constructor
  InitObject(phys,tracks,hits,charges);
  SetSignalExpanded(-1000);
}
//_____________________________________________________________________________
IlcTARGETdigitSDD::IlcTARGETdigitSDD( Float_t phys,const Int_t *digits,
    const Int_t *tracks,const Int_t *hits,const Float_t *charges, Int_t sige): IlcTARGETdigit(digits) {

  //constructor setting also fSignalExpanded
  InitObject(phys,tracks,hits,charges);
  SetSignalExpanded(sige);
}

//_____________________________________________________________________________
IlcTARGETdigitSDD::IlcTARGETdigitSDD( Float_t phys,const Int_t *digits,
    const Int_t *tracks,const Int_t *hits,const Float_t *charges,
    IlcTARGETCalibrationSDD* resp): IlcTARGETdigit(digits) {

  //constructor setting fSignalExpanded through IlcTARGETCalibrationSDD
  InitObject(phys,tracks,hits,charges);
  IlcTARGETresponseSDD* pd = (IlcTARGETresponseSDD*)resp->GetResponse();
  SetSignalExpanded(pd->Convert8to10(digits[2]));
}

//______________________________________________________________________
Int_t IlcTARGETdigitSDD::GetListOfTracks(TArrayI &t,TArrayF &c){

    // Fills the TArrayI t with the tracks found in fTracks removing
    // duplicated tracks, summing up their charge, and ordering the tracks
    // by the charge contributed to this digit. It will return
    // the number of tracks and fill the remaining elements to the array
    // t with -1.
    // Inputs:
    //   TArrayI  &t Reference to a TArrayI to contain the list of
    //               nonduplicated track numbers.
    //   TArrayF  &c Reference to a TArrayF to contain the summed charge
    //               contributed by each track.
    // Output:
    //   TArrayI  &t The input array filled with the nonduplicated track
    //               numbers.
    //   TArrayF  &c The input array filled with the summed charge 
    //               contributed by the corresponding track in the array t.
    // Return:
    //   Int_t The number of none -1 entries in the TArrayI t.
    Int_t nt = t.GetSize();
    nt = TMath::Min(nt,c.GetSize());
    Int_t nth = this->GetNTracks();
    Int_t n = 0,i,j;
    Bool_t inlist = kFALSE;

    t.Reset(-1); // -1 array.
    c.Reset(0.0); // zero array.
    for(i=0;i<nth;i++) {
	if(this->GetTrack(i) == -1) continue;
	inlist = kFALSE;
	for(j=0;j<n;j++)if(this->GetTrack(i) == t.At(j)){
	    inlist = kTRUE;
	    c.AddAt(this->GetCharge(i)+c.At(j),j);
	} // end for j/end if
	if(!inlist){ // add to end of list
	    t.AddAt(this->GetTrack(i),n);
	    c.AddAt(this->GetCharge(i),n);
	    if(n<nt) n++;
	} // end if
    } // end for i

    // Now lets sort the TArrays according to the charge. This algorithm
    // is based on the method from Chapter 8 section 1 Straight Insertion
    // sort. Wiliam H. Press, Saul A. Teukolsky, William T. Vetterling
    // and Brian P. Flannery, "Numerical Recipeis in C, The Art of Scientific
    // Computing", second Edition page 330 (1997).
    Int_t   tr;
    Float_t ch;
    for(i=0;i<n;i++){
	tr = t.At(i);
	ch = c.At(i);
	j = i-1;
	while(j>-1 && c.At(j)>ch){
	    t.AddAt(t.At(j+1),j);
	    c.AddAt(c.At(j+1),j);
	    j--;
	} // end while
	t.AddAt(tr,j+1);
	c.AddAt(ch,j+1);
    } // end for i
    //
    return n;
}


//______________________________________________________________________
void IlcTARGETdigitSDD::Print(ostream *os){


  //Standard output format for this class
    Int_t i;

    IlcTARGETdigit::Print(os);
    *os <<","<< fPhysics;
    for(i=0; i<fgkSsdd; i++) *os <<","<< fTcharges[i];
    for(i=0; i<fgkSsdd; i++) *os <<","<< fTracks[i];
    for(i=0; i<fgkSsdd; i++) *os <<","<< fHits[i];
    *os <<","<< fSignalExpanded;
}
//______________________________________________________________________
void IlcTARGETdigitSDD::Read(istream *os){

  //Standard input for this class
  Int_t i;

    IlcTARGETdigit::Read(os);
    *os >>fPhysics;
    for(i=0; i<fgkSsdd; i++) *os >> fTcharges[i];
    for(i=0; i<fgkSsdd; i++) *os >> fTracks[i];
    for(i=0; i<fgkSsdd; i++) *os >> fHits[i];
    *os >>fSignalExpanded;
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETdigitSDD &source){

  // Standard output streaming function.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETdigitSDD &source){
  
  // Standard output streaming function.

    source.Read(&os);
    return os;
}
