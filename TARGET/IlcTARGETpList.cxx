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
/* $Id: IlcTARGETpList.cxx,v 1.2 2008/11/03 07:46:37 cgatto Exp $ */

//***********************************************************************
//
// It consist of a TClonesArray of 
// IlcTARGETpListItem objects
// This array can be accessed via 2 indexed
// it is used at digitization level by 
// all the 3 TARGET subdetectors
//
// ***********************************************************************
#include <iostream>
#include "IlcTARGETpList.h"
#include "IlcTARGETpListItem.h"


//______________________________________________________________________
TClonesArray* IlcTARGETpList::fa=0;
std::vector<int>* IlcTARGETpList::fRSDigit=0;
std::vector<int>* IlcTARGETpList::fRealSDigit=0;
ClassImp(IlcTARGETpList)
//______________________________________________________________________
IlcTARGETpList::IlcTARGETpList(){
    // Default constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A zeroed/empty IlcTARGETpList class.

    fNi = 0;
    fNj = 0;
    //    fa  = 0;
    fEntries=0;
}
//______________________________________________________________________
IlcTARGETpList::IlcTARGETpList(Int_t imax,Int_t jmax){
    // Standard constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A setup IlcTARGETpList class.

    fNi = imax;
    fNj = jmax;
    fEntries = 0;
    if(!fa) fa = new TClonesArray("IlcTARGETpListItem",1);
    if(!fRSDigit) fRSDigit=new std::vector<int>();
    if(!fRealSDigit)  fRealSDigit=new std::vector<int>();

    fEntries = fRSDigit->size();
}
//______________________________________________________________________
IlcTARGETpList::~IlcTARGETpList(){
    // Default destructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    a properly destroyed class

  if(fa){
    fa->Clear();
    delete fa;
    fa = 0;
  }
  if(fRSDigit) {
    delete fRSDigit;
    fRSDigit=0;
  }
  if(fRealSDigit){
    delete fRealSDigit;
    fRealSDigit=0;
  }
}

//______________________________________________________________________
void IlcTARGETpList::ClearMap(){
    // Delete all IlcTARGETpListItems and zero TClonesArray.
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A zeroed IlcTARGETpList class.
 
    for (Int_t i=0;i<=GetNRealSDigit();i++) {
      if (GetpListItem(GetRealSDigitAt(i))!=0)
	(*fRSDigit)[GetRealSDigitAt(i)]=-1;
	//printf("sto cancellando questo %d\n",i);
	//fa->RemoveAt(GetRealSDigitAt(i));
    } 
    fa->Clear();
    fRealSDigit->clear();
    //    fEntries = 0;
//cout <<"Ho cancellato la mappa!"<<endl;
   // fa->Clear();
  //delete [] fa;
}
//______________________________________________________________________
void IlcTARGETpList::DeleteHit(Int_t i,Int_t j){
    // Delete a particular IlcTARGETpListItems.
    // Inputs:
    //    Int_t i   Row number
    //    Int_t j   Columns number
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t k = GetIndex(i,j);

    if((*fRSDigit)[k]>=0){
      fa->RemoveAt((*fRSDigit)[k]);
      (*fRSDigit)[k]=-1;
    } // end for i && if
    for (UInt_t ii=0;ii<fRealSDigit->size();ii++)
      if ((*fRealSDigit)[ii]==k) {
	(*fRealSDigit)[ii]=(*fRealSDigit)[fRealSDigit->size()-1];
	
      }
    fRealSDigit->pop_back();  
}
//______________________________________________________________________
IlcTARGETpList& IlcTARGETpList::operator=(const IlcTARGETpList &source){
    // = operator
    // Inputs:
    //    const IlcTARGETpList &source    A IlcTARGETpList object.
    // Outputs:
    //    none.
    // Return:
    //    A copied IlcTARGETpList object.

    if(this == &source) return *this;

/*    if(this->fRealSDigit!=0){ // if this->fa exists delete it first.
      fRealSDigit->Delete();
      delete fRealSDigit;
      fRealSDigit = 0;
      } // end if this->fa!=0
*/
    this->fNi = source.fNi;
    this->fNj = source.fNj;
    this->fEntries = source.fEntries;
    return *this;
}
//______________________________________________________________________
IlcTARGETpList::IlcTARGETpList(const IlcTARGETpList &source) : IlcTARGETMap(source){
    // Copy constructor

  fNi = source.fNi;
  fNj = source.fNj;
  fEntries = source.fEntries;
}
//______________________________________________________________________
void IlcTARGETpList::AddItemTo(Int_t fileIndex, IlcTARGETpListItem *pl) {
    // Adds the contents of pl to the list with track number off set given by
    // fileIndex.
    // Creates the IlcTARGETpListItem if needed.
    // Inputs:
    //    Int_t fileIndex      track number offset value
    //    IlcTARGETpListItem *pl  an IlcTARGETpListItem to be added to this class.
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t index = pl->GetIndex();
    if(!fa) {fa = new TClonesArray("IlcTARGETpListItem",1);}
    if(!fRSDigit) {fRSDigit=new std::vector<int>();fEntries = 0;}
    if(!fRealSDigit)  fRealSDigit=new std::vector<int>();
    
    if(index>=(int)fRSDigit->size()){
      if(index>=fNi*fNj) 
	IlcError(Form("Wrong index %i, bigger than number of pixel %i x %i",index,fNi,fNj));
      fEntries = fNi*fNj;  fRSDigit->resize(fEntries,-1);//    fa->Expand(fEntries);
    }
    if(GetpListItem(index)==0) { // most create IlcTARGETpListItem
      (*fRSDigit)[index]=fRealSDigit->size();
      new((*fa)[fRealSDigit->size()])IlcTARGETpListItem(-2,-1,pl->GetModule(),index,0.0);
      fRealSDigit->push_back(index);
    } // end if
    
    ((IlcTARGETpListItem*)(GetpListItem(index)))->AddTo( fileIndex,pl);


}
//______________________________________________________________________
void IlcTARGETpList::AddSignal(Int_t i,Int_t j,Int_t trk,Int_t ht,Int_t mod,
                       Double_t signal){
    // Adds a Signal value to the TClonesArray at i,j. 
    // Creates the IlcTARGETpListItem
    // if needed.
    // Inputs:
    //    Int_t i         Row number for this signal
    //    Int_t j         Column number for this signal
    //    Int_t trk       Track number creating this signal
    //    Int_t ht        Hit number creating this signal
    //    Int_t mod       The module where this signal is in
    //    Double_t signal The signal (ionization)
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t index = GetIndex(i,j);
    if (index<0) return;
    if(!fa) {fa = new TClonesArray("IlcTARGETpListItem",1);fEntries = 1;}
    if(!fRSDigit) {fRSDigit=new std::vector<int>();fEntries = 0;}
    if(!fRealSDigit)  fRealSDigit=new std::vector<int>();
    
    if(index>=(int)fRSDigit->size()){
      if(index>=fNi*fNj) 
	IlcError(Form("Wrong index %i, bigger than number of pixel %i x %i",index,fNi,fNj));
      fEntries = fNi*fNj;  fRSDigit->resize(fEntries,-1);//    fa->Expand(fEntries);
    }
    if(GetpListItem(index)==0){ // must create IlcTARGETpListItem
    //cout <<"Crea ed aggiunge segnale"<<endl;
      (*fRSDigit)[index]=fRealSDigit->size();
      new((*fa)[fRealSDigit->size()])IlcTARGETpListItem(trk,ht,mod,index,signal);
      fRealSDigit->push_back(index);
    }else{ // IlcTARGETpListItem exists, just add signal to it.
      GetpListItem(index)->AddSignal(trk,ht,mod,index,signal);
    } // end if



}
//______________________________________________________________________
void IlcTARGETpList::AddNoise(Int_t i,Int_t j,Int_t mod,Double_t noise){
    // Adds a noise value to the TClonesArray at i,j. 
    // Creates the IlcTARGETpListItem
    // if needed.
    // Inputs:
    //    Int_t i        Row number for this noise
    //    Int_t j        Column number for this noise
    //    Double_t noise The noise signal value.
    // Outputs:
    //    none.
    // Return:
    //    none.
    if (noise==0.0) return;
    Int_t index = GetIndex(i,j);
    if(!fa) {fa = new TClonesArray("IlcTARGETpListItem",1);fEntries = 1;}
    if(!fRSDigit) {fRSDigit=new std::vector<int>();fEntries = 0;}
    if(!fRealSDigit)  fRealSDigit=new std::vector<int>();
 //   printf("Dan_AddNoise: indice: %d\n",index);
    if(index>=(int)fRSDigit->size()){
      if(index>=fNi*fNj) 
	IlcError(Form("Wrong index %i, bigger than number of pixel %i x %i",index,fNi,fNj));
      fEntries = fNi*fNj;  fRSDigit->resize(fEntries,-1);//    fa->Expand(fEntries);
    }
    if(GetpListItem(index)==0){ // must create IlcTARGETpListItem
      (*fRSDigit)[index]=fRealSDigit->size();
      new((*fa)[fRealSDigit->size()]) IlcTARGETpListItem(mod,index,noise);
      fRealSDigit->push_back(index);
    }else{ // IlcTARGETpListItem exists, just add signal to it.
      GetpListItem(index)->AddNoise(mod,index,noise);
    } // end if

}
//______________________________________________________________________
void IlcTARGETpList::GetCell(Int_t index,Int_t &i,Int_t &j) const {
  // returns the i,j index numbers from the linearized index computed
  // with GetIndex
  if(index<0 || index>=fNi*fNj){
    Warning("GetCell","Index out of range 0<=index=%d<%d",
	    index,fNi*fNj);
    i=-1;j=-1;
    return;
  } // end if
  i = index/fNj;
  j = index - fNj*i;
  return;
}
//______________________________________________________________________
Int_t IlcTARGETpList::GetIndex(Int_t i, Int_t j) const {
 // returns the TClonesArray index for a given set of map indexes.
  if(i<0||i>=fNi || j<0||j>=fNj){
    Warning("GetIndex","Index out of range 0<i=%d<%d and 0<j=%d<%d",i,fNi,j,fNj);
    return -1;
  }
  else {
    return fNj*i+j;
  }
}
