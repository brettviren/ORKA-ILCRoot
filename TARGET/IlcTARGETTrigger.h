#ifndef ILCTARGETTRIGGER_H
#define ILCTARGETTRIGGER_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETTrigger.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

#include "IlcTriggerDetector.h"

class IlcTARGETgeom;

////////////////////////////////////////////////////////////////////////
//
// Version 1
// Modified by D. Elia, C. Jorgensen
// March 2006
//
// Version 0
// Written by J. Conrad, E. Lopez Torres
// October 2005
//
// IlcTARGETTrigger: implementation of the SPD Fast-OR based triggers.
//
////////////////////////////////////////////////////////////////////////

class IlcTARGETTrigger : public IlcTriggerDetector
{
 public:
                   IlcTARGETTrigger();   // constructor
        virtual   ~IlcTARGETTrigger(){}  // destructor
   virtual void    CreateInputs();
   virtual void    Trigger();

private:

   Int_t fGlobalFOThreshold;         // minimum number of FOs to fire Global FO trigger
   Int_t fHighMultFOThreshold;       // minimum number of FOs to fire High Mult FO trigger

   void MultiplicityTriggers(TObjArray* digDet, TTree* treeD, IlcTARGETgeom* geom);
//   void GeometryTriggers(TObjArray* digDet, TTree* treeD, IlcTARGETgeom* geom);
   void GeometryTriggers();

  ClassDef( IlcTARGETTrigger, 1 )  // TARGET SPD Trigger Detector class
};

#endif
