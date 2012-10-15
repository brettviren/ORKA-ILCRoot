#ifndef ILCTARGETTRACKV2PID_H
#define ILCTARGETTRACKV2PID_H

#include <TMath.h>
#include <Riostream.h>
#include "IlcTARGETtrackV2.h"

//_____________________________________________________________________________
class IlcTARGETtrackV2Pid : public TObject {
public:
    IlcTARGETtrackV2Pid();
    virtual ~IlcTARGETtrackV2Pid(){}
protected:
    Float_t fWpi,fWk,fWp,fSignal,fMom;
    Float_t fPhi,fLam;
    Int_t   fPcode;
    
    Float_t fGSignal,fGMom,fGpx,fGpy,fGpz,fGx,fGy,fGz;
    Int_t   fGcode,fGlab,fFound;

    Float_t fQ1,fQ2,fQ3,fQ4,fQ5,fQ6;
    Float_t fD,fZ;
    
  ClassDef(IlcTARGETtrackV2Pid,2)  // TARGET trackV2 PID
};

#endif


