#ifndef ILCJETHADRONCORRECTION_H
#define ILCJETHADRONCORRECTION_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */

/* $Id: IlcJetHadronCorrection.h 17838 2007-04-05 16:30:21Z morsch $ */

//_________________________________________________________________________
//                  
//*-- Author: Aleksei Pavlinov (WSU)
// This pure abstract class which defines only interface
// 
#include "TNamed.h"

class IlcJetHadronCorrection : public TNamed {

  public:
    IlcJetHadronCorrection();
    IlcJetHadronCorrection(const char *name, const char *title);
    virtual ~IlcJetHadronCorrection() {} 

  // Add for particle
    virtual Double_t GetEnergy(Double_t pmom, Double_t eta, Int_t gid)=0; 

    ClassDef(IlcJetHadronCorrection,1) // Hadron correction for EMC (abstract class)
};

#endif // ILCJETHADRONCORRECTION_H
