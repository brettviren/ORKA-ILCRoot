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
#include "IlcTARGETMapA1.h"
#include "IlcTARGETsegmentation2.h"
#include "IlcTARGETdigit.h"

////////////////////////////////////////////////////////////////////////
//  Map Class for TARGET. Implementation A1. In this implementation, the //
// 2 dimensional (iz,ix) map is filled with integers values. For each //
// cell a corresponding TObject, a hit, can also be stored.           //
//  The detector geometry is accessed via the that detectors          //
// segmentation class and stored here for conveniance.                //
////////////////////////////////////////////////////////////////////////

ClassImp(IlcTARGETMapA1)
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(){
    // default constructor

    fSegmentation = 0;
    fNpz          = 0;
    fNpx          = 0;
    fMaxIndex     = 0;
    fHitMap       = 0;
    fObjects      = 0;
    fNobjects     = 0;
    fMapThreshold = 0;
    fMapThresholdArr = 0;
}
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg){
    //constructor

    fSegmentation = seg;
    fNpz          = fSegmentation->Npz(lay);
    fNpx          = fSegmentation->Npx(lay);
    fMaxIndex     = fNpz*fNpx+fNpx;             // 2 halves of detector
    fHitMap       = new Int_t[fMaxIndex];
    fObjects      = 0;
    fNobjects     = 0;
    fMapThreshold = 0;
    fMapThresholdArr = 0;
    ClearMap();
}
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *obj){
    //constructor

    fNobjects     = 0;
    fSegmentation = seg;
    fNpz          = fSegmentation->Npz(lay);
    fNpx          = fSegmentation->Npx(lay);
    fMaxIndex     = fNpz*fNpx+fNpx;             // 2 halves of detector
    fHitMap       = new Int_t[fMaxIndex];
    fObjects      =  obj;
    if(fObjects) fNobjects = fObjects->GetEntriesFast();
    fMapThreshold = 0;
    fMapThresholdArr = 0;
    ClearMap();
}
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *obj, Int_t thr){
    //constructor

    fNobjects     = 0;
    fSegmentation = seg;
    fNpz          = fSegmentation->Npz(lay);
    fNpx          = fSegmentation->Npx(lay);
    fMaxIndex     = fNpz*fNpx+fNpx;             // 2 halves of detector
    fHitMap       = new Int_t[fMaxIndex];
    fObjects      =  obj;
    if(fObjects) fNobjects = fObjects->GetEntriesFast();
    fMapThreshold = thr;
    ClearMap();
}
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *obj, TArrayI thr){
    //constructor

    fNobjects     = 0;
    fSegmentation = seg;
    fNpz          = fSegmentation->Npz(lay);
    fNpx          = fSegmentation->Npx(lay);
    fMaxIndex     = fNpz*fNpx+fNpx;             // 2 halves of detector
    fHitMap       = new Int_t[fMaxIndex];
    fObjects      =  obj;
    if(fObjects) fNobjects = fObjects->GetEntriesFast();
    fMapThreshold = 0;
    fMapThresholdArr = thr;
    ClearMap();
}

//______________________________________________________________________
IlcTARGETMapA1::~IlcTARGETMapA1(){
    //destructor

    if(fHitMap) delete[] fHitMap;
}
//_________________________________________________________________________
IlcTARGETMapA1& IlcTARGETMapA1::operator=(const IlcTARGETMapA1 &source) {
    //    Assignment operator

    if(&source == this) return *this;

    this->fNpx          = source.fNpx;
    this->fNpz          = source.fNpz;
    this->fObjects      = source.fObjects;
    this->fNobjects     = source.fNobjects;
    this->fMaxIndex     = source.fMaxIndex;
    this->fHitMap       = source.fHitMap;
    this->fMapThreshold = source.fMapThreshold;
    this->fMapThresholdArr = source.fMapThresholdArr;
    return *this;
}
//______________________________________________________________________
IlcTARGETMapA1::IlcTARGETMapA1(const IlcTARGETMapA1 &source) : IlcTARGETMap(source){
    //     Copy Constructor

    *this = source;
}
//______________________________________________________________________
void IlcTARGETMapA1::ClearMap(){
    //clear array

    memset(fHitMap,0,sizeof(int)*fMaxIndex);
}
//______________________________________________________________________
void IlcTARGETMapA1::SetArray(TObjArray *obj){
    // set array of objects

    fObjects =  obj;
    if (fObjects) fNobjects = fObjects->GetEntriesFast();
}
//______________________________________________________________________
Int_t IlcTARGETMapA1::CheckedIndex(Int_t iz, Int_t ix) const {
    //check boundaries and return an index in array
    Int_t index=fNpx*iz+ix;

    //if (index > fMaxIndex) {
    if (index > fMaxIndex || index < 0) {
	printf("\n \n \n Try to read/write outside array !!!!"
	       " \n \n %d %d %d %d %d %d \n",iz,ix,fMaxIndex,index,fNpz,fNpx);
	// force crash
	return  -1;
    } else {
	return index;
    } // end if index>max or < 0
}
//______________________________________________________________________
void  IlcTARGETMapA1::FillMap(){
    // fill array with digits indices

    Int_t ndigits = fObjects->GetEntriesFast();
    if (!ndigits) return;

    IlcTARGETdigit *dig;
    for (Int_t ndig=0; ndig<ndigits; ndig++) {
	dig = (IlcTARGETdigit*)fObjects->UncheckedAt(ndig);
	if(dig->GetSignal() > fMapThreshold) {
	    SetHit(dig->GetCoord1(),dig->GetCoord2(),ndig);
	} // end if fSignal > fMapthreshold
    } // end for ndig
}
//______________________________________________________________________
void  IlcTARGETMapA1::FillMap2(){
    // fill array with digits indices

    Int_t ndigits = fObjects->GetEntriesFast();
    if (!ndigits) return;

    IlcTARGETdigit *dig;
    for (Int_t ndig=0; ndig<ndigits; ndig++) {
	dig = (IlcTARGETdigit*)fObjects->UncheckedAt(ndig);
	if(dig->GetSignal() > fMapThresholdArr[dig->GetCoord1()]) {
	    SetHit(dig->GetCoord1(),dig->GetCoord2(),ndig);
	} // end if fSignal > fMapthreshold
    } // end for ndig
}

//______________________________________________________________________
void  IlcTARGETMapA1::SetHit(Int_t iz, Int_t ix, Int_t idigit){
    // set the digit index at a certain position in array

    fHitMap[CheckedIndex(iz, ix)]=idigit+1;
}
//______________________________________________________________________
void IlcTARGETMapA1::DeleteHit(Int_t iz, Int_t ix){
    // delete an entry in array

    fHitMap[CheckedIndex(iz, ix)]=0;
}
//______________________________________________________________________
void IlcTARGETMapA1::FlagHit(Int_t iz, Int_t ix){
    // flag an entry in array

    fHitMap[CheckedIndex(iz, ix)] = -TMath::Abs(fHitMap[CheckedIndex(iz, ix)]);
}
//______________________________________________________________________
Int_t IlcTARGETMapA1::GetHitIndex(Int_t iz, Int_t ix) const {
    // return the digit index from a specific entry in array

    return TMath::Abs(fHitMap[CheckedIndex(iz, ix)])-1;
}
//______________________________________________________________________
TObject* IlcTARGETMapA1::GetHit(Int_t iz, Int_t ix) const {
    // return the pointer to the digit 

    Int_t index=GetHitIndex(iz,ix);
    // Force crash if index does not exist ! 
    return (index <0) ? 0 : fObjects->UncheckedAt(GetHitIndex(iz,ix));
}
//______________________________________________________________________
Double_t IlcTARGETMapA1::GetSignal(Int_t iz, Int_t ix) const{
    // get a pad signal
    Double_t signal;

    IlcTARGETdigit *dig = (IlcTARGETdigit*)GetHit(iz,ix);
    if(dig) signal=(Double_t)dig->GetSignal();
    else signal=0.;
    return signal;
}
//______________________________________________________________________
FlagType IlcTARGETMapA1::TestHit(Int_t iz, Int_t ix)  {
    // check whether the digit has already been flagged

    if (CheckedIndex(iz, ix) < 0) return kEmpty;
    Int_t inf=fHitMap[CheckedIndex(iz, ix)]; 
    if (inf < 0) {
	return kUsed;
    } else if (inf == 0) {
	return kEmpty;
    } else {
	return kUnused;
    } // end if inf
}

