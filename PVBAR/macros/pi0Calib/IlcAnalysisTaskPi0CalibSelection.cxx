/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Author: Boris Polishchuk                                               *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//---------------------------------------------------------------------------// 
//                                                                           //
// Fill histograms (one per cell) with two-cluster invariant mass            //
// using calibration coefficients of the previous iteration.                 //
// Histogram for a given cell is filled if the most energy of one cluster    //
// is deposited in this cell and the other cluster could be anywhere in PVBAR.//
//                                                                           //
//---------------------------------------------------------------------------//

#include "IlcAnalysisTaskPi0CalibSelection.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "IlcESDEvent.h"
#include "IlcPVBARPIDv1.h"
#include "IlcPVBAREsdCluster.h"
#include "TRefArray.h"
#include "TList.h"

ClassImp(IlcAnalysisTaskPi0CalibSelection)

IlcAnalysisTaskPi0CalibSelection::IlcAnalysisTaskPi0CalibSelection() :
IlcAnalysisTaskSE(),fOutputContainer(0x0),fPVBARGeo(0x0),fCalibData(0x0),
fHmgg(0x0),fEmin(0.),fLogWeight(4.5)
{
  //Default constructor.

  for(Int_t iMod=0; iMod<5; iMod++) {
    for(Int_t iX=0; iX<64; iX++) {
      for(Int_t iZ=0; iZ<56; iZ++) {
	fHmpi0[iMod][iX][iZ]=0;
      }
    } 
  }
  
}

IlcAnalysisTaskPi0CalibSelection::IlcAnalysisTaskPi0CalibSelection(const char* name) :
  IlcAnalysisTaskSE(name),fOutputContainer(0x0),fPVBARGeo(0x0),fCalibData(0x0),
  fHmgg(0x0),fEmin(0.),fLogWeight(4.5)
{
  //Named constructor which should be used.
  
  DefineOutput(1,TList::Class());
  
  for(Int_t iMod=0; iMod<5; iMod++) {
    for(Int_t iX=0; iX<64; iX++) {
      for(Int_t iZ=0; iZ<56; iZ++) {
	fHmpi0[iMod][iX][iZ]=0;
      }
    } 
  }

}

IlcAnalysisTaskPi0CalibSelection::~IlcAnalysisTaskPi0CalibSelection()
{
  //Destructor.
  
  if(fOutputContainer){
    fOutputContainer->Delete() ; 
    delete fOutputContainer ;
  }

  if(fCalibData) delete fCalibData;
  if(fPVBARGeo)   delete fPVBARGeo;
}

void IlcAnalysisTaskPi0CalibSelection::UserCreateOutputObjects()
{
  fOutputContainer = new TList();
  
  char hname[128], htitl[128];
  
  for(Int_t iMod=0; iMod<5; iMod++) {
    for(Int_t iX=0; iX<64; iX++) {
      for(Int_t iZ=0; iZ<56; iZ++) {
	snprintf(hname,128,"%d_%d_%d",iMod,iX,iZ);
	snprintf(htitl,128,"Two-gamma inv. mass for mod %d, cell (%d,%d)",iMod,iX,iZ);
	fHmpi0[iMod][iX][iZ] = new TH1F(hname,htitl,100,0.,300.);
	fOutputContainer->Add(fHmpi0[iMod][iX][iZ]);
      }
    }
  }

  fHmgg = new TH1F("hmgg","2-cluster invariant mass",100,0.,300.);
  fOutputContainer->Add(fHmgg);
  
  fCalibData = new IlcPVBARCalibData();
  fPVBARGeo =  IlcPVBARGeometry::GetInstance("ORKA") ;
}

void IlcAnalysisTaskPi0CalibSelection::UserExec(Option_t* /* option */)
{
  //Analysis per event.
  
  IlcVEvent* event = InputEvent();
  IlcESDEvent* esd = static_cast<IlcESDEvent*>(event) ;

  Double_t v[3] ; //vertex ;
  esd->GetVertex()->GetXYZ(v) ;
  TVector3 vtx(v);
  printf("Vertex: (%.3f,%.3f,%.3f)\n",vtx.X(),vtx.Y(),vtx.Z());
 
  Int_t runNum = esd->GetRunNumber();
  printf("Run number: %d\n",runNum);
  
  for(Int_t mod=0; mod<5; mod++)
    fPVBARGeo->SetMisalMatrix(esd->GetPHOSMatrix(mod),mod) ;
  
  Float_t logWeight = fLogWeight;
  printf("Will use logWeight %.3f .\n",logWeight);
  
  IlcPVBARPIDv1 pid;

  Int_t mod1 = -1;
  TLorentzVector p1;
  TLorentzVector p2;
  TLorentzVector p12;
  Int_t relid[4] ;
  Int_t maxId;

  TRefArray * caloClustersArr  = new TRefArray();
  esd->GetPHOSClusters(caloClustersArr);
  
  const Int_t kNumberOfPVBARClusters   = caloClustersArr->GetEntries() ;
  printf("CaloClusters: %d\n", kNumberOfPVBARClusters);

  // PVBAR cells
  IlcESDCaloCells *phsCells = esd->GetPHOSCells();
  
  // loop over PVBAR clusters
  for(Int_t iClu=0; iClu<kNumberOfPVBARClusters; iClu++) {
    
    IlcESDCaloCluster *c1 = (IlcESDCaloCluster *) caloClustersArr->At(iClu);
    if(!c1->IsPHOS()) continue; // EMCAL cluster!

    Float_t E1_i = c1->E();   // cluster energy before correction
    if(E1_i<fEmin) continue;

    IlcPVBAREsdCluster clu1(*c1);
    clu1.Recalibrate(fCalibData, phsCells);
    clu1.EvalAll(logWeight,vtx);
    clu1.EnergyCorrection() ;

    clu1.GetMomentum(p1,v);

    MaxEnergyCellPos(phsCells,&clu1,maxId);
    fPVBARGeo->AbsToRelNumbering(maxId, relid);

    mod1 = relid[0]-1;        // module
    Int_t iX = relid[2]-1;    // cluster X-coord
    Int_t iZ = relid[3]-1;    // cluster Z-coord
    Float_t E1_ii = clu1.E(); // cluster energy after correction
    
    for (Int_t jClu=iClu; jClu<kNumberOfPVBARClusters; jClu++) {
      IlcESDCaloCluster *c2 = (IlcESDCaloCluster *) caloClustersArr->At(jClu);
      if(!c2->IsPHOS())   continue; // EMCAL cluster!
      if(c2->IsEqual(c1)) continue;

      Float_t E2_i = c2->E();
      if(E2_i<fEmin) continue;
      
      IlcPVBAREsdCluster clu2(*c2);
      clu2.Recalibrate(fCalibData, phsCells);
      clu2.EvalAll(logWeight,vtx);
      clu2.EnergyCorrection() ;
        
      clu2.GetMomentum(p2,v);
      Float_t E2_ii = clu2.E();

      p12 = p1+p2;

      fHmgg->Fill(p12.M()*1000); 
      fHmpi0[mod1][iX][iZ]->Fill(p12.M()*1000);
      
      printf("Mass in (mod%d,%d,%d): %.3f GeV  E1_i=%f E1_ii=%f  E2_i=%f E2_ii=%f\n",
	     mod1,iX,iZ,p12.M(),E1_i,E1_ii,E2_i,E2_ii);
    }
    
  } // end of loop over PVBAR clusters
  
  delete caloClustersArr;
  PostData(1,fOutputContainer);
}

void IlcAnalysisTaskPi0CalibSelection::MaxEnergyCellPos(IlcESDCaloCells *cells, IlcESDCaloCluster* clu, Int_t& maxId)
{
  //For a given CaloCluster calculates the absId of the cell 
  //with maximum energy deposit.
  
  Double_t eMax = -111;
  
  for (Int_t iDig=0; iDig< clu->GetNCells(); iDig++) {
    Int_t cellAbsId = clu->GetCellAbsId(iDig);
    Double_t eCell = cells->GetCellAmplitude(cellAbsId)*clu->GetCellAmplitudeFraction(iDig);
    if(eCell>eMax)  { 
      eMax = eCell; 
      maxId = cellAbsId;
    }
  }

}

void IlcAnalysisTaskPi0CalibSelection::SetCalibCorrections(IlcPVBARCalibData* cdata)
{
  //Set new correction factors (~1) to calibration coefficients, delete previous.

  if(fCalibData) delete fCalibData;
  fCalibData = cdata;

}
