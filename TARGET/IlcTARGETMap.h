#ifndef ILCTARGETMAP_H
#define ILCTARGETMAP_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */
/* $Id: IlcTARGETMap.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
////////////////////////////////////////////////
//  Map Class for TARGET.                        //
////////////////////////////////////////////////

#include <TObject.h>
#include <TArrayI.h>
typedef enum {kEmpty, kUsed, kUnused} FlagType;

//___________________________________________________________________________

class IlcTARGETMap : public TObject {

 public:
    virtual ~IlcTARGETMap() {}
    // Fill hits from list of digits into hit map
    virtual  void  FillMap()                                       =0;
    virtual  void  FillMap2()                                      =0;
    // Clear the map
    virtual  void  ClearMap()                                      =0;
    // Set a single hit
    virtual  void  SetHit(Int_t iz, Int_t ix, Int_t idigit)        =0;
    // Set threshold for the signal
    virtual  void  SetThreshold(Int_t)                             =0;
    virtual  void  SetThresholdArr(TArrayI)                        =0;
    // Delete a single hit
    virtual  void  DeleteHit(Int_t iz, Int_t ix)                   =0;
    // Flag a hit as used
    virtual  void  FlagHit(Int_t iz, Int_t ix)                     =0;    
    // Get index of hit in the list of digits
    virtual Int_t  GetHitIndex(Int_t iz, Int_t ix) const           =0;
    // Get pointer to digit
    virtual TObject * GetHit(Int_t iz, Int_t ix) const             =0;
    // Test hit status
    virtual FlagType TestHit(Int_t iz, Int_t ix)                   =0;
    // Get signal from map
    virtual Double_t  GetSignal(Int_t iz, Int_t ix) const          =0;

    ClassDef(IlcTARGETMap,1) //virtual base class for TARGET Hit/Digit Map

};

#endif	




