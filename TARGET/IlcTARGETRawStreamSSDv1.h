#ifndef ILCTARGETRAWSTREAMSSDV1_H
#define ILCTARGETRAWSTREAMSSDV1_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETRawStreamSSDv1.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
///
/// This class provides access to TARGET SSD digits in raw data.
///
///////////////////////////////////////////////////////////////////////////////

#include "IlcTARGETRawStreamSSD.h"

class IlcRawReader;


class IlcTARGETRawStreamSSDv1: public IlcTARGETRawStreamSSD {
  public :
    IlcTARGETRawStreamSSDv1(IlcRawReader* rawReader);
    virtual ~IlcTARGETRawStreamSSDv1() {};

    virtual Bool_t   Next();

    Int_t            GetADC() const {return fADC;}
    Int_t            GetADModule() const {return fADModule;}

  protected :

  Int_t  fADModule;               // FEROM module
  Int_t  fADC;                    // ADC within the ADModule
         

    ClassDef(IlcTARGETRawStreamSSDv1, 0) // class for reading beam test TARGET digits
};

#endif

