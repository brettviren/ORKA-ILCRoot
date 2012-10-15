#ifndef ILCTARGETMAPA1_H
#define ILCTARGETMAPA1_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */
/* $Id: IlcTARGETMapA1.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
////////////////////////////////////////////////////////////////////////
//  Map Class for TARGET. Implementation A1. In this implementation, the //
// 2 dimensional (iz,ix) map is filled with integers values. For each //
// cell a corresponding TObject, a hit, can also be stored.           //
////////////////////////////////////////////////////////////////////////
#include "IlcTARGETMap.h"
#include "TArrayI.h"

class IlcTARGETsegmentation2;
class TObjArray;

class IlcTARGETMapA1 : public IlcTARGETMap{

 public:
    IlcTARGETMapA1(); // default creator
    // Standard reator using only a segmentation class
    IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg);
    // Standard reator using only a segmentation class and pointer to digits
    IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *dig);
    // Standard reator using only a segmentation class and pointer to digits
    // and a threshold value
    IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *dig, Int_t thr);
    IlcTARGETMapA1(Int_t lay,IlcTARGETsegmentation2 *seg, TObjArray *dig, TArrayI thr);
    // Copy Operator
    IlcTARGETMapA1(const IlcTARGETMapA1 &source);
    // Assignment operator
    IlcTARGETMapA1& operator=(const IlcTARGETMapA1 &source);
    // Distructor
    virtual ~IlcTARGETMapA1();
    // Fill hits from list of digits into hit map
    virtual  void  FillMap();
    virtual  void  FillMap2();
    // Clear the hit map
    virtual  void  ClearMap();    
    // Set a single hit
    virtual  void  SetHit(Int_t iz, Int_t ix, Int_t idigit);
    // Set threshold for the signal
    virtual  void  SetThreshold(Int_t thresh) {fMapThreshold=thresh;}
    virtual  void  SetThresholdArr(TArrayI th) {fMapThresholdArr=th;} 
    // Delete a single hit
    virtual  void  DeleteHit(Int_t iz, Int_t ix);
    // Get index of hit in the list of digits
    virtual Int_t  GetHitIndex(Int_t iz, Int_t ix) const ;
    // Get pointer to digit
    virtual TObject* GetHit(Int_t iz, Int_t ix) const;
    // Flag a hit as used
    virtual  void  FlagHit(Int_t iz, Int_t ix);
    // Test hit status
    virtual FlagType TestHit(Int_t iz, Int_t ix);
    // Get signal from map
    virtual Double_t  GetSignal(Int_t iz, Int_t ix) const;
    // Get max index inmap
    Int_t   MaxIndex() const  {return fMaxIndex;}
    // Set the array of objects
    void SetArray(TObjArray *obj);

 protected:
    // Check index
    Int_t   CheckedIndex(Int_t iz, Int_t ix) const;

    // Data members
    IlcTARGETsegmentation2 *fSegmentation;   // segmentation class
    Int_t fNpx;                          // fNpx
    Int_t fNpz;                          // fNpz
    TObjArray  *fObjects;                // object
    Int_t fNobjects;                     // number of objects
    Int_t fMaxIndex;                     // max index in map
    TArrayI fMapThresholdArr;            // array with thresholds
 private:
    Int_t *fHitMap;                      //! [fMaxIndex]
    Int_t fMapThreshold;                 // signal threshold (ADC)

    ClassDef(IlcTARGETMapA1,2)  // Implements Hit/Digit Map 
};

#endif	

