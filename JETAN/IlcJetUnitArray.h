#ifndef ILCJETUNITARRAY_H
#define ILCJETUNITARRAY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 *  *  * See cxx source for full Copyright notice     */

/* $Id: IlcJetUnitArray.h 50548 2011-07-12 07:10:28Z kleinb $ */

// Class description : Unit red as input by jet finder algorithm to store
//                     the physical characteristics of a particle
//
// Author: magali.estienne@subatech.in2p3.fr
//
// Unit used by jet finder algorithm
//
//

#include <Riostream.h> 
#include <TObject.h>
#include <TRefArray.h>
#include <TMath.h>
#include "IlcESDtrack.h"
#include "IlcAODTrack.h"
#include "IlcVParticle.h"
#include "IlcESDCaloCells.h"
#include "IlcAODCaloCells.h"
#include "IlcESDCaloCluster.h"
#include "IlcAODCaloCluster.h"
#include "IlcJetFinderTypes.h"

class TList;
class TVector3;

class IlcJetUnitArray : public TObject
{
 public:
  IlcJetUnitArray();
  IlcJetUnitArray(Int_t absId, Int_t esdId, Float_t eta, Float_t phi, Float_t en, Float_t Deta, Float_t Dphi, 
		  IlcJetFinderUnitDetectorFlagType_t det, IlcJetFinderUnitFlagType_t inout, IlcJetFinderUnitCutFlagType_t cut, 
		  IlcJetFinderUnitCutFlagType_t cut2, IlcJetFinderUnitSignalFlagType_t signal, Float_t mass, Int_t clusId);
  IlcJetUnitArray(Int_t absId, Int_t esdId, Float_t eta, Float_t phi, Float_t en, Float_t px, Float_t py, Float_t pz, 
		  Float_t Deta, Float_t Dphi, IlcJetFinderUnitDetectorFlagType_t det, IlcJetFinderUnitFlagType_t inout, 
		  IlcJetFinderUnitCutFlagType_t cut, Float_t mass, Int_t clusId);
  ~IlcJetUnitArray();

  // Setter
  void SetUnitEnergy(Float_t energy)  {fUnitEnergy = energy;}
  void SetUnitEta(Float_t eta)        {fUnitEta = eta;} 
  void SetUnitPhi(Float_t phi)        {fUnitPhi = phi;}
  void SetUnitPx(Float_t px)          {fUnitPx = px;}
  void SetUnitPy(Float_t py)          {fUnitPy = py;}
  void SetUnitPz(Float_t pz)          {fUnitPz = pz;}
  void SetUnitDeta(Float_t deta)      {fUnitDeta = deta;} 
  void SetUnitDphi(Float_t dphi)      {fUnitDphi = dphi;}
  void SetUnitID(Int_t id)            {fUnitID = id;}
  void SetUnitTrackID(Int_t esdid)    {fUnitTrackID = esdid;}
  void SetUnitEntries(Int_t num)      {fUnitNum = num;}
  void SetUnitClusterID(Int_t id)     {fUnitClusterID = id;}
  void SetUnitFlag(IlcJetFinderUnitFlagType_t flag)    
  { 
    fUnitFlag = flag; 
  } 
  void SetUnitCutFlag(IlcJetFinderUnitCutFlagType_t cutFlag)    
  { 
    fUnitCutFlag = cutFlag; 
  } 
  void SetUnitCutFlag2(IlcJetFinderUnitCutFlagType_t cutFlag)
  {
    fUnitCutFlag2 = cutFlag;
  }
  void SetUnitSignalFlag(IlcJetFinderUnitSignalFlagType_t signalFlag)    
  { 
    fUnitSignalFlag = signalFlag; 
  } 
  void SetUnitDetectorFlag(IlcJetFinderUnitDetectorFlagType_t detectorflag)    
  { 
    fUnitDetectorFlag = detectorflag; 
  } 
  void SetUnitSignalFlagC(Bool_t init, IlcJetFinderUnitSignalFlagType_t flag);
  void SetUnitSignalFlagN(Bool_t init, IlcJetFinderUnitSignalFlagType_t flag);
  void SetUnitMass(Float_t mass) {fUnitMass = mass;}
  void SetUnitTrackRef(TRefArray* trackref)     {fUnitTrackRef = trackref;}
  void SetUnitCellRef(TRefArray* cellref)       {fUnitCellRef = cellref;}
  void SetUnitClusterRef(TRefArray* clusterref) {fUnitClusterRef = clusterref;}

  // Getter
  Float_t     GetUnitEnergy() const            {return fUnitEnergy;}
  Float_t     GetUnitEta() const               {return fUnitEta;}
  Float_t     GetUnitPhi() const               {return fUnitPhi;} 
  Float_t     GetUnitPx() const                {return fUnitPx;}
  Float_t     GetUnitPy() const                {return fUnitPy;}
  Float_t     GetUnitPz() const                {return fUnitPz;}
  Float_t     GetUnitDeta() const              {return fUnitDeta;}
  Float_t     GetUnitDphi() const              {return fUnitDphi;}         
  Int_t       GetUnitID() const                {return fUnitID;}
  Int_t       GetUnitTrackID() const           {return fUnitTrackID;}
  Int_t       GetUnitEntries() const           {return fUnitNum;}
  Int_t       GetUnitClusterID() const         {return fUnitClusterID;}
  Float_t     GetUnitMass() const              {return fUnitMass;}
  Bool_t      GetUnitSignalFlagC(Int_t ind, IlcJetFinderUnitSignalFlagType_t &flagc);
  Bool_t      GetUnitSignalFlagN(Int_t ind, IlcJetFinderUnitSignalFlagType_t &flagn);
  TRefArray*  GetUnitTrackRef() const          {return fUnitTrackRef;}
  TRefArray*  GetUnitCellRef() const           {return fUnitCellRef;}
  TRefArray*  GetUnitClusterRef() const        {return fUnitClusterRef;}

  Float_t     EtaToTheta(Float_t arg) const;
  void        ClearUnitTrackRef();
  void        ClearUnitCellRef();

  IlcJetFinderUnitFlagType_t GetUnitFlag() const     
  {
	  return fUnitFlag;
  }
  IlcJetFinderUnitCutFlagType_t GetUnitCutFlag() const     
  {
	  return fUnitCutFlag;
  }
  IlcJetFinderUnitCutFlagType_t GetUnitCutFlag2() const
  {
          return fUnitCutFlag2;
  }
  IlcJetFinderUnitSignalFlagType_t GetUnitSignalFlag() const     
  {
	  return fUnitSignalFlag;
  }
  IlcJetFinderUnitDetectorFlagType_t GetUnitDetectorFlag() const     
  {
	  return fUnitDetectorFlag;
  }

 protected:
  IlcJetUnitArray(const IlcJetUnitArray& rUnit);
  IlcJetUnitArray& operator = (const IlcJetUnitArray& rhs);
  Bool_t operator>  (const IlcJetUnitArray* unit1) const;
  Bool_t operator<  (const IlcJetUnitArray* unit1) const;
  Bool_t operator== (const IlcJetUnitArray* unit1) const;

  Float_t                            fUnitEnergy;       // Energy (Pt,et) of the unit 
  Float_t                            fUnitEta;          // Eta of the unit
  Float_t                            fUnitPhi;          // Phi of the unit
  Float_t                            fUnitDeta;         // Delta Eta of the unit
  Float_t                            fUnitDphi;         // Delta Phi of the unit
  Int_t                              fUnitID;           // ID of the unit
  Int_t                              fUnitTrackID;      // ID of a given charged track 
  Int_t                              fUnitNum;          // Number of units
  Int_t                              fUnitClusterID;    // ID for clusters
  IlcJetFinderUnitFlagType_t         fUnitFlag;         // Flag of the unit
  IlcJetFinderUnitCutFlagType_t      fUnitCutFlag;      // Cut flag of the unit in the tpc
  IlcJetFinderUnitCutFlagType_t      fUnitCutFlag2;     // Cut flag of the unit in the emcal
  IlcJetFinderUnitSignalFlagType_t   fUnitSignalFlag;   // Signal flag of the unit
  IlcJetFinderUnitDetectorFlagType_t fUnitDetectorFlag; // Detector flag of the unit
  Float_t                            fUnitPx;           // Px of charged track
  Float_t                            fUnitPy;           // Py of charged track
  Float_t                            fUnitPz;           // Pz of charged track
  Float_t                            fUnitMass;         // Mass of particle
  vector< IlcJetFinderUnitSignalFlagType_t >   fVc;     //|| added for background studies
  vector< IlcJetFinderUnitSignalFlagType_t >   fVn;     //|| added for background studies
  TRefArray*                         fUnitTrackRef;     //! pointer to array of references to esd tracks
  TRefArray*                         fUnitCellRef;      //! pointer to array of references to esd cells
  TRefArray*                         fUnitClusterRef;   //! pointer to array of references to esd clusters

  ClassDef(IlcJetUnitArray,2)

};

#endif

