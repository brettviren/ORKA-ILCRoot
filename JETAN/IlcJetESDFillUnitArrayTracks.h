#ifndef ILCJETESDFILLUNITARRAYTRACKS_H
#define ILCJETESDFILLUNITARRAYTRACKS_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// Jet Fill Unit Array 
// Called by ESD Reader for jet analysis
// Author: Magali Estienne (magali.estienne@subatech.in2p3.fr)
//---------------------------------------------------------------------

#include "IlcJetFillUnitArray.h"
#include "IlcJetESDReaderHeader.h"

class IlcEMCALGeometry;
class IlcJetReader;
class IlcJetESDReader;

class IlcJetESDFillUnitArrayTracks : public IlcJetFillUnitArray
{
 public: 
  IlcJetESDFillUnitArrayTracks();
  IlcJetESDFillUnitArrayTracks(IlcESDEvent *fESD);
  virtual ~IlcJetESDFillUnitArrayTracks();
  
  // Setter
  void SetHadCorrector(IlcJetHadronCorrection* const corr) {fHadCorr = corr;}
  void SetApplyMIPCorrection(Bool_t const val) {fApplyMIPCorrection = val;}
  void SetESD(IlcESDEvent* const esd) {fESD = esd;}
  void SetGrid0(IlcJetGrid* const grid0)                   {fGrid0 = grid0;}
  void SetGrid1(IlcJetGrid* const grid1)                   {fGrid1 = grid1;}
  void SetGrid2(IlcJetGrid* const grid2)                   {fGrid2 = grid2;}
  void SetGrid3(IlcJetGrid* const grid3)                   {fGrid3 = grid3;}
  void SetGrid4(IlcJetGrid* const grid4)                   {fGrid4 = grid4;}

  // Getter
  Int_t         GetHadCorrection() const {return fApplyMIPCorrection;}
  Int_t         GetMult() const {return fNTracks;}
  Int_t         GetMultCut() const {return fNTracksCut;}

  // Other
  void Exec(Option_t* const option);

 protected:
  Int_t                       fNumUnits;           // Number of units in the unit object array (same as num towers in EMCAL)
  IlcJetHadronCorrection      *fHadCorr;           // Pointer to Hadron Correction Object
  Bool_t                      fApplyMIPCorrection; // Apply MIP or not ? Exclusive with fApplyFractionHadronicCorrection
  IlcESDEvent                 *fESD;               // ESD
  IlcJetGrid                  *fGrid0;             // Grid used for dead zones definition
  IlcJetGrid                  *fGrid1;             // Grid used for dead zones definition
  IlcJetGrid                  *fGrid2;             // Grid used for dead zones definition
  IlcJetGrid                  *fGrid3;             // Grid used for dead zones definition
  IlcJetGrid                  *fGrid4;             // Grid used for dead zones definition

 private:
  IlcJetESDFillUnitArrayTracks(const IlcJetESDFillUnitArrayTracks &det);
  IlcJetESDFillUnitArrayTracks &operator=(const IlcJetESDFillUnitArrayTracks &det);
  void InitParameters();

  ClassDef(IlcJetESDFillUnitArrayTracks,1) 
};

#endif
