#ifndef ILCDCHFAST_H
#define ILCDCHFAST_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   */
 /* See cxx source for full Copyright notice                               */

/* $Id: IlcDCHFast.h,v 1.8 2008/05/07 14:45:37 cgatto Exp $ */

////////////////////////////////////////////////
//  fast DCH cluster simulation               //
////////////////////////////////////////////////

#include <TObject.h>
#include "IlcDCHXTREL.h"
#include <map>
#include <vector>
#include "IlcDCHwireposition.h"
#include "IlcDCHclusterizer.h"


class IlcRunLoader;
class IlcDCHClustersArray;
class IlcDCHParam;
class IlcDCHwireposition;
class IlcDCHclusterizer;

class IlcDCHFast : public TObject {

public:
  IlcDCHFast(IlcDCHwireposition *lwireMatr=0x0, IlcDCHclusterizer *lclus=0x0);
  ~IlcDCHFast();
  
  void Hits2Clusters(IlcRunLoader* runLoader, Int_t iev, Bool_t CheckQuality=kFALSE) const;
  void Hits2ClustersOld(IlcRunLoader* runLoader, Int_t iev, Bool_t CheckQuality=kFALSE) const;

  void SetExact(bool exact=kTRUE){kExact=exact;}

 protected:

  
 // IlcDCHParam *param;   //!pointer to parameters - NOT OWNER
  IlcDCHXTREL *xtr;
  //IlcDCHParam *locparam;
  IlcDCHwireposition *wireMatr;
  IlcDCHclusterizer *clus;


  Bool_t localOpened;
Bool_t kExact;

  typedef std::pair<int,int> IntPair;
  typedef std::map<std::pair<IntPair,IntPair>, std::vector<std::vector<float> > > hitstype;

  ClassDef(IlcDCHFast,0)  // fast DCH cluster simulation
};

#endif
