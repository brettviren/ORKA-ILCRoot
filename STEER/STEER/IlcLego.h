#ifndef ILCLEGO_H
#define ILCLEGO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLego.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//    Utility class to compute and draw Radiation Length Map                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
class TH2F;
class IlcLegoGenerator;
class TClonesArray;

class IlcLego : public TNamed  {

public:
  IlcLego();
  IlcLego(const char *title, Int_t ntheta,Float_t themin,
	  Float_t themax, Int_t nphi, Float_t phimin,
	  Float_t phimax,Float_t rmin,Float_t rmax,Float_t zmax);
  IlcLego(const char *title, IlcLegoGenerator* generator);
  IlcLego(const IlcLego &lego);
  virtual ~IlcLego();
  virtual void  StepManager();
  virtual void  BeginEvent();
  virtual void  FinishEvent();
  virtual void  FinishRun();
  virtual IlcLego &operator=(const IlcLego &lego) 
  {lego.Copy(*this);return(*this);}

private:
  void Copy(TObject &lego) const;
  void DumpVolumes();
  
private:
   IlcLegoGenerator *fGener;     //Lego generator
   Float_t    fTotRadl;          //!Total Radiation length
   Float_t    fTotAbso;          //!Total absorption length
   Float_t    fTotGcm2;          //!Total G/CM2 traversed
   TH2F      *fHistRadl;         //Radiation length map 
   TH2F      *fHistAbso;         //Interaction length map
   TH2F      *fHistGcm2;         //g/cm2 length map
   TH2F      *fHistReta;         //Radiation length map as a function of eta
   TClonesArray *fVolumesFwd;    //!Volume sequence forward
   TClonesArray *fVolumesBwd;    //!Volume sequence backward   
   Int_t      fStepBack;         //!Flag for backstepping
   Int_t      fStepsBackward;    //!Counts steps forward
   Int_t      fStepsForward;     //!Counts steps backward
   Int_t      fErrorCondition;   //!Error condition flag
   Int_t      fDebug;            // Debug Flag
   Bool_t     fStopped;          //!Scoring has been stopped 
   
  ClassDef(IlcLego,1) //Utility class to compute and draw Radiation Length Map

};


#endif











