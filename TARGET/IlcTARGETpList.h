#ifndef ILCTARGETPLIST_H
#define ILCTARGETPLIST_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */

// ***********************************************************************
//
// It consist of a TClonesArray of 
// IlcTARGETpListItem objects
// This array can be accessed via 2 indexed
// it is used at digitization level by 
// all the 3 TARGET subdetectors
//
// ***********************************************************************
#include "TArrayI.h"
#include "TClonesArray.h"
#include "IlcTARGETMap.h"
#include "IlcTARGETpListItem.h"
#include <vector>
#include "IlcLog.h"

class IlcTARGETpList: public IlcTARGETMap {

 public:
    // Default Constructor
    IlcTARGETpList();
    // Standard Constructor
    IlcTARGETpList(Int_t imax,Int_t jmax);
    // Class destrutor
    virtual ~IlcTARGETpList();
    // Copy constructor
    IlcTARGETpList(const IlcTARGETpList &source);
    // = Operator
    virtual IlcTARGETpList& operator=(const IlcTARGETpList &source);
    // Returns the max mape index values
    void GetMaxMapIndex(Int_t &ni,Int_t &nj) const {ni=fNi;nj=fNj;return;}
    // returns the max index value.
    Int_t GetMaxIndex() const {return fNi*fNj;}
    // returns the largest non-zero entry kept in the array fa.
    Int_t GetEntries() const {return fEntries;}
    // returns the max number of track/hit entries per cell.
    Int_t GetNEntries() const {return IlcTARGETpListItem::GetMaxKept();}
    // for a given TClonesArray index it returns the corresponding map index
    void  GetMapIndex(Int_t index,Int_t &i,Int_t &j) const {
	i = 0;j =0;
	i = (index/fNj);j = index - fNj*(i);
	if(i<0||i>=fNi || j<0||j>=fNj){i=-1;j=-1; return;}
    }
    // Returns the signal+noise for a give map coordinate
    Double_t GetSignal(Int_t index)  {
	if(GetpListItem(index)==0) return 0.0;
	return GetpListItem(index)->GetSumSignal();
    }
    // Returns the signal+noise for a give map coordinate
    virtual Double_t GetSignal(Int_t i,Int_t j) const  {
	if(GetpListItem(i,j)==0) return 0.0;
	return GetpListItem(i,j)->GetSumSignal();
    }
    // Returns the signal only for a give map coordinate
    Double_t GetSignalOnly(Int_t i,Int_t j)const  {
	if(GetpListItem(i,j)==0) return 0.0;
	return GetpListItem(i,j)->GetSignal();
    }
    // Returns the noise for a give map coordinate
    Double_t GetNoise(Int_t i,Int_t j) const {
	if(GetpListItem(i,j)==0) return 0.0;
	return GetpListItem(i,j)->GetNoise();
    }
    // returns the track number which generated the signal at a given map
    // coordinate. If there is no signal or only noise, then -2 is returned.
    // k is the track rank number.
    Double_t GetTSignal(Int_t i,Int_t j,Int_t k) const {
	if(GetpListItem(i,j)==0) return 0.0;
	return GetpListItem(i,j)->GetSignal(k);
    }
    // returns the track number which generated the signal at a given map
    // coordinate. If there is no signal or only noise, then -2 is returned.
    // k is the track rank number.
    Int_t GetTrack(Int_t i,Int_t j,Int_t k) const {
	if(GetpListItem(i,j)==0) return -2;
	return GetpListItem(i,j)->GetTrack(k);
    }
    // returns the hit number which generated the signal at a given map
    // coordinate. If there is no signal or only noise, then -2 is returned.
    // k is the hit rank number.
    Int_t GetHit(Int_t i,Int_t j,Int_t k) const {
        if(GetpListItem(i,j)==0) return -2;
	return GetpListItem(i,j)->GetHit(k);
    }
    // returns the number of Signal values
    Int_t GetNSignals(Int_t i,Int_t j) {
      Int_t index = GetIndex(i,j);
      if(index>=(int)fRSDigit->size()){
	if(index>=fNi*fNj) 
	  IlcError(Form("Wrong index %i, bigger than number of pixel %i x %i",index,fNi,fNj));
	fEntries = fNi*fNj;  fRSDigit->resize(fEntries,-1);//    fa->Expand(fEntries);
      }
      if(GetpListItem(i,j)==0) return 0;
      return GetpListItem(i,j)->GetNsignals();
    }
    // Adds the contents of pl to the list with track number off set given by
    // fileIndex.
    virtual void AddItemTo(Int_t fileIndex, IlcTARGETpListItem *pl);
    // Adds a Signal value to the map. Creating and expanding arrays as needed.
    void AddSignal(Int_t i,Int_t j,Int_t trk,Int_t ht,Int_t mod,Double_t sig);
    // Adds a Noise value to the map. Creating and expanding arrays as needed.
    void AddNoise(Int_t i,Int_t j,Int_t mod,Double_t noise);
    // Delete all IlcTARGETpListItems and zero the TClonesArray
    virtual void ClearMap();
    // Delete a particular IlcTARGETpListItem in the TClonesArray.
    virtual void DeleteHit(Int_t i,Int_t j);
    // returns hit index in TClonesArray
    virtual Int_t GetHitIndex(Int_t i,Int_t j) const {return GetIndex(i,j);}
    // returns "hit" IlcTARGETpListItem as a TObject.
    TObject * GetHit(Int_t i,Int_t j) const {return (TObject*)GetpListItem(i,j);}
    // tests hit status.
    virtual FlagType TestHit(Int_t i,Int_t j){if(GetpListItem(i,j)==0) return kEmpty;
    else if(GetSignal(i,j)<=0) return kUnused; else return kUsed;}
    // Returns the pointer to the TClonesArray of pList Items
    TClonesArray * GetpListItems(){return fa;}
    // returns the pList Item stored in the TClonesArray
    IlcTARGETpListItem* GetpListItem(Int_t index) const{
      if((*fRSDigit)[index]<0) return 0;
      return (IlcTARGETpListItem*) fa->UncheckedAt((*fRSDigit)[index]);
    }
    // returns the pList Item stored in the TObject array
    IlcTARGETpListItem* GetpListItem(Int_t i,Int_t j) const {
      return GetpListItem(GetIndex(i,j));
    }
    Int_t GetNRealSDigit() const {return fRealSDigit->size()-1;}
    Int_t GetRealSDigitAt(Int_t i) { return (*fRealSDigit)[i];}

    // Fill pList from digits. Not functional yet
    virtual void FillMap(){NotImplemented("FillMap");}
    virtual void FillMap2() {NotImplemented("FillMap2");}
    // Sets threshold for significance. Not of relavance in this case.
    virtual void SetThreshold(Int_t /* i */){NotImplemented("SetThreshold");}
    virtual void SetThresholdArr(TArrayI /*thr*/) {NotImplemented("SetThresholdArr");}    
    // Sets a single hit. Not of relavance in this case.
    virtual void SetHit(Int_t /* i */,Int_t /* j */,Int_t /* k */){NotImplemented("SetHit");}
    // Flags a hit. Not of relavence in this case.
    virtual void FlagHit(Int_t /* i */,Int_t /* j */){NotImplemented("FlagHit");}
    virtual void GetCell(Int_t index,Int_t &i,Int_t &j) const;

 private:

// private methods
    Int_t GetIndex(Int_t i,Int_t j) const;
    void NotImplemented(const char *method) const {if(gDebug>0)
         Warning(method,"This method is not implemented for this class");}
// data members
    Int_t     fNi,fNj;   // The max index in i,j.
    static TClonesArray *fa;       // array of pList items
    Int_t     fEntries; // keepts track of the number of non-zero entries.
    static std::vector<int>* fRealSDigit;
    static std::vector<int>* fRSDigit; // map of pixel to pListItem
    

    ClassDef(IlcTARGETpList,5) // list of signals and track numbers
};	
#endif
