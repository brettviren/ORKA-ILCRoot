#ifndef ILCTARGETSDIGITIZATION_H
#define ILCTARGETSDIGITIATION_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
 
/*
  $Id: IlcTARGETsDigitize.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
 */
 
#include <TTask.h>
 
class TString;
class IlcTARGET;
 
class IlcTARGETsDigitize : public TTask{
 public:
    IlcTARGETsDigitize(); // default constructor
    IlcTARGETsDigitize(const char *filename); // standard constructor
    virtual ~IlcTARGETsDigitize();//Destructor
    virtual Bool_t Init();
    virtual void Exec(const Option_t *opt="SPD");
 private:
    Bool_t InitSDig();  // Standard SDigitization initilization.
 private:
//    TFile   *fFile;    //! pointer to the file contatining the hits and
//                       // and will contain the SDigits
    
    Bool_t  fDet[3];   //! logical specifing which detectors to reconstruct.
    Bool_t  fInit;     //! True if Init was sucessfull, else false.
    Int_t   fEnt;      //! Number of events to processevent index.
    Int_t   fEnt0;     //! first event to process, default 0.
    IlcTARGET  *fTARGET;     //! Local pointer to TARGET class.
    IlcRunLoader* fRunLoader;//!Run Loader 
    ClassDef(IlcTARGETsDigitize,1) // Task to SDigitize TARGET from Hits.
 
};
#endif
