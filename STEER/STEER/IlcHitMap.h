#ifndef ILCHITMAP_H
#define ILCHITMAP_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHitMap.h 50615 2011-07-16 23:19:19Z hristov $ */

#include <TObject.h>

typedef enum {kEmpty, kUsed, kUnused} FlagType;

class IlcHitMap :
public TObject {
 public:
    // Virtual destructor -- it is necessary
    virtual ~IlcHitMap() {}
    // Fill hits from list of digits into hit map
    virtual  void  FillHits()                                      =0;
    // Clear the hit map
    virtual  void  Clear(const char *opt = "")                     =0;
    // Set a single hit
    virtual  void  SetHit(Int_t ix, Int_t iy, Int_t idigit)        =0;
    // Delete a single hit
    virtual  void  DeleteHit(Int_t ix, Int_t iy)                   =0;
    // Get index of hit in the list of digits
    virtual Int_t  GetHitIndex(Int_t ix, Int_t iy) const           =0;
    // Get pointer to digit
    virtual TObject * GetHit(Int_t ix, Int_t iy) const             =0;
    // Flag a hit as used
    virtual void   FlagHit(Int_t ix, Int_t iy)                     =0;
    // Validate
    virtual  Bool_t ValidateHit(Int_t ix, Int_t iy)                =0;
     // Test hit status
    virtual FlagType TestHit(Int_t ix, Int_t iy)                   =0;
    
    ClassDef(IlcHitMap,2) //virtual base class for HitMap
};
#endif	











