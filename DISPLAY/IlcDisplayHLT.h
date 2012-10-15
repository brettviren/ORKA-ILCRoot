#ifndef IlcDISPLAYHLT_H
#define IlcDISPLAYHLT_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC HLT DISPLAY CLASS                                             //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>

class TPolyMarker3D;

class IlcDisplayHLT{
  //This classes is an interface to the HLT data
  //For the moment only for TPC, for adding modules there is two choices:
  //1) add the function LoadHLT[module](Int_t) and update the function LoadHLT
  //2) or inherit your class from IlcDisplayHLT and overload LoadHLT

 public:

  IlcDisplayHLT();
  virtual ~IlcDisplayHLT();

  virtual void  LoadHLT(const char *name,Int_t e);//Load L3 datas whose belong to detector name and from the event e
  virtual void  LoadHLTTPC(Int_t nevent);
  virtual void  Draw();

 private:
  TPolyMarker3D *fPoints; //fPoints[i]=set of cluster coordinates in detector i;
  Int_t         fNb; // Number of HLT clusters
  char          **fName; //fName[i]=name of the detector i 

 ClassDef(IlcDisplayHLT,0);
};

#endif
