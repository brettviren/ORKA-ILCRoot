#ifndef ILCTARGETRECONSTRUCTION_H
#define ILCTARGETRECONSTRUCTION_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
 
/*
  $Id: IlcTARGETreconstruction.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
 */
/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Class for TARGET RecPoint reconstruction                               //
//                                                                     //
////////////////////////////////////////////////////////////////////////

#include <TTask.h>

class IlcRunLoader;
class TString;

class IlcTARGETreconstruction : public TTask{
 public:
    IlcTARGETreconstruction(); // default constructor
    IlcTARGETreconstruction(const char *filename); // standard constructor
    IlcTARGETreconstruction(IlcRunLoader *rl); // standard constructor
    IlcTARGETreconstruction(const IlcTARGETreconstruction& rec);
    IlcTARGETreconstruction& operator=(const IlcTARGETreconstruction &source);
    virtual ~IlcTARGETreconstruction();//Destructor
    virtual Bool_t Init();
    virtual void Exec(const Option_t *opt="SPD");
    virtual void SetOutputFile(TString filename);
 private:
    Bool_t InitRec();  // Standard Reconstrution initilization.
 private:
    Bool_t  fDet[3];   //! logical specifing which detectors to reconstruct.
    Bool_t  fInit;     //! True if Init was sucessfull, else false.
    Int_t   fEnt;      //! Number of events to processevent index.
    Int_t   fEnt0;     //! first event to process, default 0.
    IlcTARGETDetTypeRec *fDetTypeRec; //!TARGET obj. for reconstruction
    Bool_t  fDfArp;    //! if True then delete fRunLoader in destructor.
    IlcTARGETgeom*   fTARGETgeom;//! TARGET geometry
    IlcTARGETLoader *fLoader; //! TARGET loader
    IlcRunLoader* fRunLoader;//!Run Loader
 
    ClassDef(IlcTARGETreconstruction,3) // Task to Reconstruct TARGET from Digits.

};
#endif
