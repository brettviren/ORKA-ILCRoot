#ifndef IlcOADBTriggerAnalysis_H
#define IlcOADBTriggerAnalysis_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     OADB container for filling scheme information (BX ids, name ...)
//     Author: Michele Floris, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>
#include "TMap.h"
#include "TObjString.h"


class IlcOADBTriggerAnalysis : public TNamed {

 public :
  IlcOADBTriggerAnalysis();
  IlcOADBTriggerAnalysis(char* name);
  virtual ~IlcOADBTriggerAnalysis();
  //  void Init();
  
  // Getters
  Float_t GetZDCCutRefSumCorr()     { return fZDCCutRefSumCorr;     }      
  Float_t GetZDCCutRefDeltaCorr()   { return fZDCCutRefDeltaCorr;   }   
  Float_t GetZDCCutSigmaSumCorr()   { return fZDCCutSigmaSumCorr;   }   
  Float_t GetZDCCutSigmaDeltaCorr() { return fZDCCutSigmaDeltaCorr; }  
  // Setters
  void SetZDCCorrParameters(Float_t sumCorr, Float_t deltaCorr, Float_t sigmaSumCorr, Float_t sigmaDeltaCorr) 
  { fZDCCutRefSumCorr = sumCorr; fZDCCutRefDeltaCorr = deltaCorr; fZDCCutSigmaSumCorr = sigmaSumCorr; fZDCCutSigmaDeltaCorr = sigmaDeltaCorr;}
  // Browse
  virtual Bool_t	IsFolder() const { return kTRUE; }
  void Browse(TBrowser *b);
  // Print
  virtual void	Print(Option_t* option = "") const;

 private :

  Float_t fZDCCutRefSumCorr;      // Corrected ZDC time cut configuration
  Float_t fZDCCutRefDeltaCorr;    // Corrected ZDC time cut configuration
  Float_t fZDCCutSigmaSumCorr;    // Corrected ZDC time cut configuration
  Float_t fZDCCutSigmaDeltaCorr;  // Corrected ZDC time cut configuration  

  IlcOADBTriggerAnalysis(const IlcOADBTriggerAnalysis& cont);  // not implemented
  IlcOADBTriggerAnalysis& operator=(const IlcOADBTriggerAnalysis& cont); // not implemented


  ClassDef(IlcOADBTriggerAnalysis, 1);
};

#endif
