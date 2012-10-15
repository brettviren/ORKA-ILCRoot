#ifndef ILCDCHV1_H
#define ILCDCHV1_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHv1.h,v 1.2 2009/02/04 14:02:32 tassiell Exp $ */

////////////////////////////////////////////////////////
//  Manager and hits classes for set:DCH version 1    //
////////////////////////////////////////////////////////

// Energy spectrum of the delta-rays 
// Double_t Ermilova(Double_t *x, Double_t *par);
// Double_t IntSpecGeant(Double_t *x, Double_t *par);
 
#include "IlcDCH.h"
#include "TClonesArray.h"
#include "TArrayS.h"

class TF1;
class TTree;
class TFile;
class IlcDCHsim;

//_____________________________________________________________________________
class IlcDCHv1 : public IlcDCH {

 public:

  IlcDCHv1();
  IlcDCHv1(const char *name, const char *title);
  virtual ~IlcDCHv1();

  virtual void       Copy(TObject &trd) const;
  virtual void       CreateGeometry();
  virtual void       CreateMaterials();
//  virtual void       CreateTRhit(Int_t det);
  virtual Int_t      IsVersion() const          { return 1; };
  virtual void       StepManager();
  virtual void       Init();

          void       StepManagerErmilova();
          void       StepManagerGeant();
          void       StepManagerFixedStep();
          void       SelectStepManager(Int_t t);
          void       SetStepSize(Double_t s)    { fStepSize = s; };

//          void       SetSensPlane(Int_t iplane = 0);
//          void       SetSensChamber(Int_t ichamber = 0);
//          void       SetSensSector(Int_t isector);
//          void       SetSensSector(Int_t isector, Int_t nsector);

          void       SetTR(Bool_t kTRUE)	{ fTRon = kTRUE; };

          Int_t      GetSensPlane() const	{ return fSensPlane;	   };
          Int_t      GetSensChamber() const	{ return fSensChamber;     };
          Int_t      GetSensSector() const	{ return fSensSector;	   };
          Int_t      GetSensSectorRange() const { return fSensSectorRange; };
 
	   Bool_t     GetTR() const		 { return fTRon;	    };
//           IlcDCHsim *GetDCHsim() const          { return fTR;              };

 protected:

  void        *StepManagerEntity();

  Int_t        fSensSelect;             // Switch to select only parts of the detector
  Int_t        fSensPlane;              // Sensitive detector plane
  Int_t        fSensChamber;            // Sensitive detector chamber
  Int_t        fSensSector;             // Sensitive detector sector 
  Int_t        fSensSectorRange;        // Sensitive detector range

  Bool_t       fTRon;                   // Switch for TR simulation
//  IlcDCHsim   *fTR;                     // TR simulator
  Int_t        fTypeOfStepManager;      // Type of Step Manager.
  Double_t     fStepSize;               // Used for the fixed step size

 private:
  IlcDCHv1(const IlcDCHv1 &trd);
  IlcDCHv1 &operator=(const IlcDCHv1 &trd);
  Int_t        oldtrk;
  IlcDCHhit *lastaddedhit;
  Bool_t       exiting;
  ULong_t      fExitingPrev_det;

  Double_t     BetheBloch(Double_t bg);
  Double_t     BetheBlochGeant(Double_t bg);
  void 	       Stepping();
 

//  TF1         *fDeltaE;                 // Energy distribution of the delta-electrons (Ermilova)
//  TF1         *fDeltaG;                 // Energy distribution of the
	                                // Delta-electrons (GEANT) for StepManagerGeant
  Float_t      fTrackLength0;         	// Save the track length at chamber entrance  
  Int_t	       fPrimaryTrackPid;        // Save the id of the primary track  
  ULong_t      fPrev_det;
  
  TFile		*fHitWires;		//file for hitting wire probability study
  TTree		*fHitWiresData;		//data of hit wires
  Int_t         oldtrkWS;
  Int_t         oldevWS;
  Int_t         currentev;
  Int_t         currenttrk;
  Int_t		tmpNHits;
  Int_t		tmpLast;
  Int_t		tmpFist_;
  Int_t		tmpLast_;
  TClonesArray	*WSDataBlk;
  TArrayI	*tmpIdSL;
  TArrayI	*tmpIdRing;
  TArrayI	*tmpIdAbsRing;
  TArrayS	*tmpIsSense;

  ClassDef(IlcDCHv1,1)

};

class IlcDCHHitWireStat : public TObject {
  public:
    IlcDCHHitWireStat()		{
      TckId=-1;
      Nev=-1;
      NHits=0;
      IdSL = new Int_t[0];
      IdRing = new Int_t[0];
      IdAbsRing = new Int_t[0];
      IsSense = new Short_t[0];
    }
    ~IlcDCHHitWireStat()	{
      if (IdSL) delete IdSL;
      if (IdRing) delete IdRing;
      if (IdAbsRing) delete IdAbsRing;
      if (IsSense) delete IsSense;     
    }

  public:
    void	SetTrckId	(Int_t lTckId)		{ TckId = lTckId; }
    void	SetNev		(Int_t lNev)		{ Nev = lNev; }
    void	SetNHits	(Int_t lNHits)		{ NHits = lNHits; }
    void	SetIdSL		(TArrayI *lIdSL)	{
      Int_t tmpSize = lIdSL->GetSize();
      IdSL = new Int_t[tmpSize];
      memcpy(IdSL,lIdSL->GetArray(),tmpSize*sizeof(Int_t));
    }
    void	SetIdSL		(Int_t *lIdSL)		{ IdSL = lIdSL; }
    void	SetIdRing	(TArrayI *lIdRing)	{
      Int_t tmpSize = lIdRing->GetSize();
      IdRing = new Int_t[tmpSize];
      memcpy(IdRing,lIdRing->GetArray(),tmpSize*sizeof(Int_t));
    }
    void	SetIdRing	(Int_t *lIdRing)	{ IdRing = lIdRing; }
    void	SetIdAbsRing	(TArrayI *lIdAbsRing)	{
      Int_t tmpSize = lIdAbsRing->GetSize();
      IdAbsRing = new Int_t[tmpSize];
      memcpy(IdAbsRing,lIdAbsRing->GetArray(),tmpSize*sizeof(Int_t));
    }
    void	SetIdAbsRing	(Int_t *lIdAbsRing)	{ IdAbsRing = lIdAbsRing; }
    void	SetIsSense	(TArrayS *lIsSense)	{
      Int_t tmpSize = lIsSense->GetSize();
      IsSense = new Short_t[tmpSize];
      memcpy(IsSense,lIsSense->GetArray(),tmpSize*sizeof(Short_t));
    }
    void	SetIsSense	(Short_t *lIsSense)	{ IsSense = lIsSense; }
    Int_t	GetTrckId	()	{ return TckId;}
    Int_t	GetNev		()	{ return Nev;}
    Int_t	GetNHits	()	{ return NHits;}
    Int_t	*GetIdSL	()	{ return IdSL;}
    Int_t	*GetIdRing	()	{ return IdRing;}
    Int_t	*GetIdAbsRing	()	{ return IdAbsRing;}
    Short_t	*GetIsSense	()	{ return IsSense;}
 private:
    Int_t	TckId;          //Track label
    Int_t	Nev;		//Event number
    Int_t	NHits;		//number of hits for track
    Int_t	*IdSL;		//[NHits]  super layer id
    Int_t	*IdRing;	//[NHits]  ring id
    Int_t	*IdAbsRing;	//[NHits]  abdolute ring id
    Short_t	*IsSense;	//[NHits]  flag for field type: 0=sense 1=field
  ClassDef(IlcDCHHitWireStat,1)

};

#endif
