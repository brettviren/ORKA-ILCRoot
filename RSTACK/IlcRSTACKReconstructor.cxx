/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcRSTACKReconstructor.cxx 54863 2012-02-28 12:10:29Z hristov $ */

//_________________________________________________________________________
//--
//-- Yves Schutz (SUBATECH) 
// Reconstruction class. Redesigned from the old IlcReconstructionner class and 
// derived from STEER/IlcReconstructor. 
// 
// --- ROOT system ---
#include "TGeoManager.h"
#include "TGeoMatrix.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcAltroMapping.h"
#include "IlcESDEvent.h"
#include "IlcESDCaloCluster.h"
#include "IlcESDCaloCells.h"
#include "IlcRSTACKReconstructor.h"
#include "IlcRSTACKClusterizerv1.h"
#include "IlcRSTACKTrackSegmentMakerv1.h"
#include "IlcRSTACKPIDv1.h"
#include "IlcRSTACKTracker.h"
#include "IlcRawReader.h"
#include "IlcRSTACKCalibData.h"
#include "IlcCDBEntry.h"
#include "IlcCDBManager.h"
#include "IlcRSTACKTrigger.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKDigit.h"
#include "IlcRSTACKTrackSegment.h"
#include "IlcRSTACKEmcRecPoint.h"
#include "IlcRSTACKRecParticle.h"
#include "IlcRSTACKRawFitterv0.h"
#include "IlcRSTACKRawFitterv1.h"
#include "IlcRSTACKRawFitterv2.h"
#include "IlcRSTACKRawFitterv3.h"
#include "IlcRSTACKRawFitterv4.h"
#include "IlcRSTACKRawDigiProducer.h"
#include "IlcRSTACKPulseGenerator.h"
#include "IlcRSTACKTriggerRawDigit.h"
#include "IlcRSTACKTriggerRawDigiProducer.h"
#include "IlcRSTACKTriggerParameters.h"

ClassImp(IlcRSTACKReconstructor)

Bool_t IlcRSTACKReconstructor::fgDebug = kFALSE ; 
TClonesArray*     IlcRSTACKReconstructor::fgDigitsArray = 0;   // Array of RSTACK digits
TObjArray*        IlcRSTACKReconstructor::fgEMCRecPoints = 0;   // Array of EMC rec.points
IlcRSTACKCalibData * IlcRSTACKReconstructor::fgCalibData  = 0 ;
TClonesArray*     IlcRSTACKReconstructor::fgTriggerDigits = 0;   // Array of RSTACK trigger digits

//____________________________________________________________________________
IlcRSTACKReconstructor::IlcRSTACKReconstructor() :
  fGeom(NULL),fClusterizer(NULL),fTSM(NULL),fPID(NULL),fTmpDigLG(NULL)
{
  // ctor
  fGeom          = IlcRSTACKGeometry::GetInstance("ORKA","");
  fClusterizer   = new IlcRSTACKClusterizerv1      (fGeom);
  fTSM           = new IlcRSTACKTrackSegmentMakerv1(fGeom);
  fPID           = new IlcRSTACKPIDv1              (fGeom);
  fTmpDigLG      = new TClonesArray("IlcRSTACKDigit",100);
  fgDigitsArray  = new TClonesArray("IlcRSTACKDigit",100);
  fgEMCRecPoints = new TObjArray(100) ;
  if (!fgCalibData)
    fgCalibData = new IlcRSTACKCalibData(-1); //use IlcCDBManager's run number
  
  fgTriggerDigits  = new TClonesArray("IlcRSTACKTriggerRawDigit",100);
  
  IlcInfo(Form("RSTACK bad channel map contains %d bad channel(s).\n",
               fgCalibData->GetNumOfEmcBadChannels()));
 
}

//____________________________________________________________________________
IlcRSTACKReconstructor::~IlcRSTACKReconstructor()
{
  // dtor
  delete fGeom;
  delete fClusterizer;
  delete fTSM;
  delete fPID;
  delete fTmpDigLG;
  delete fgDigitsArray;
  delete fgEMCRecPoints;
  delete fgTriggerDigits;
} 

//____________________________________________________________________________
void IlcRSTACKReconstructor::Reconstruct(TTree* digitsTree, TTree* clustersTree) const
{
  // 'single-event' local reco method called by IlcReconstruction; 
  // Only the clusterization is performed,; the rest of the reconstruction is done in FillESD because the track
  // segment maker needs access to the IlcESDEvent object to retrieve the tracks reconstructed by 
  // the global tracking.

  fClusterizer->InitParameters();
  fClusterizer->SetInput(digitsTree);
  fClusterizer->SetOutput(clustersTree);
  if ( Debug() ) 
    fClusterizer->Digits2Clusters("deb all") ; 
  else 
    fClusterizer->Digits2Clusters("") ;
}

//____________________________________________________________________________
void IlcRSTACKReconstructor::FillESD(TTree* digitsTree, TTree* clustersTree, 
				   IlcESDEvent* esd) const
{
  // This method produces RSTACK rec-particles,
  // then it creates IlcESDtracks out of them and
  // write tracks to the ESD


  // do current event; the loop over events is done by IlcReconstruction::Run()
  fTSM->SetESD(esd) ; 
  fTSM->SetInput(clustersTree);
  if ( Debug() ) 
    fTSM->Clusters2TrackSegments("deb all") ;
  else 
    fTSM->Clusters2TrackSegments("") ;
  
  fPID->SetInput(clustersTree, fTSM->GetTrackSegments()) ; 
  fPID->SetESD(esd) ; 
  if ( Debug() ) 
    fPID->TrackSegments2RecParticles("deb all") ;
  else 
    fPID->TrackSegments2RecParticles("") ;

  TClonesArray *recParticles  = fPID->GetRecParticles();
  Int_t nOfRecParticles = recParticles->GetEntriesFast();
  
  IlcDebug(2,Form("%d rec. particles, option %s",nOfRecParticles,GetOption()));
  
  // Read digits array

  TBranch *branch = digitsTree->GetBranch("RSTACK");
  if (!branch) { 
    IlcError("can't get the branch with the RSTACK digits !");
    return;
  }
  branch->SetAddress(&fgDigitsArray);
  branch->GetEntry(0);

  // Get the clusters array

  TBranch *emcbranch = clustersTree->GetBranch("RSTACKEmcRP");
  if (!emcbranch) { 
    IlcError("can't get the branch with the RSTACK EMC clusters !");
    return;
  }

  emcbranch->SetAddress(&fgEMCRecPoints);
  emcbranch->GetEntry(0);
  
  
  // Trigger
  
  TBranch *tbranch = digitsTree->GetBranch("TRSTACK");
  if (tbranch) { 
    
    tbranch->SetAddress(&fgTriggerDigits);
    tbranch->GetEntry(0);
    
    IlcESDCaloTrigger* trgESD = esd->GetCaloTrigger("RSTACK");
    
    if (trgESD) {
      trgESD->Allocate(fgTriggerDigits->GetEntriesFast());
      
      for (Int_t i = 0; i < fgTriggerDigits->GetEntriesFast(); i++) {
	IlcRSTACKTriggerRawDigit* tdig = (IlcRSTACKTriggerRawDigit*)fgTriggerDigits->At(i);
	
	Int_t mod,modX,modZ;
	tdig->GetModXZ(mod,modX,modZ);
	
	const Int_t relId[4] = {5-mod,0,modX+1,modZ+1};
	Int_t absId;
	
	fGeom->RelToAbsNumbering(relId,absId);
	trgESD->Add(mod,absId,tdig->GetAmp(),0.,(Int_t*)NULL,0,0,0);
      }
    }  
  }
  
//   //#########Calculate trigger and set trigger info###########

//   IlcRSTACKTrigger tr ;
//   //   tr.SetPatchSize(1);//create 4x4 patches
//   tr.SetSimulation(kFALSE);
//   tr.Trigger(fgDigitsArray);
  
//   Float_t maxAmp2x2  = tr.Get2x2MaxAmplitude();
//   Float_t maxAmpnxn  = tr.GetnxnMaxAmplitude();
//   Float_t ampOutOfPatch2x2  = tr.Get2x2AmpOutOfPatch() ;
//   Float_t ampOutOfPatchnxn  = tr.GetnxnAmpOutOfPatch() ;

//   Int_t iSM2x2      = tr.Get2x2SuperModule();
//   Int_t iSMnxn      = tr.GetnxnSuperModule();
//   Int_t iCrystalPhi2x2 = tr.Get2x2CrystalPhi();
//   Int_t iCrystalPhinxn = tr.GetnxnCrystalPhi();
//   Int_t iCrystalEta2x2 = tr.Get2x2CrystalEta();
//   Int_t iCrystalEtanxn = tr.GetnxnCrystalEta();

//   IlcDebug(2, Form("Trigger 2x2 max amp %f, out amp %f, SM %d, iphi %d ieta %d",  
// 		   maxAmp2x2, ampOutOfPatch2x2, iSM2x2,iCrystalPhi2x2, iCrystalEta2x2));
//   IlcDebug(2, Form("Trigger 4x4 max amp %f , out amp %f, SM %d, iphi %d, ieta %d",
// 		   maxAmpnxn, ampOutOfPatchnxn, iSMnxn,iCrystalPhinxn, iCrystalEtanxn));

//   // Attention! RSTACK modules in order to calculate AbsId need to be 1-5 not 0-4 as returns trigger.
//   Int_t iRelId2x2 []= {iSM2x2+1,0,iCrystalPhi2x2,iCrystalEta2x2};
//   Int_t iAbsId2x2 =-1;
//   Int_t iRelIdnxn []= {iSMnxn+1,0,iCrystalPhinxn,iCrystalEtanxn};
//   Int_t iAbsIdnxn =-1;
//   TVector3    pos2x2(-1,-1,-1);
//   TVector3    posnxn(-1,-1,-1);
//   fGeom->RelToAbsNumbering(iRelId2x2, iAbsId2x2);
//   fGeom->RelToAbsNumbering(iRelIdnxn, iAbsIdnxn);
//   fGeom->RelPosInIlc(iAbsId2x2, pos2x2);
//   fGeom->RelPosInIlc(iAbsIdnxn, posnxn);

//   TArrayF triggerPosition(6);
//   triggerPosition[0] = pos2x2(0) ;   
//   triggerPosition[1] = pos2x2(1) ;   
//   triggerPosition[2] = pos2x2(2) ;  
//   triggerPosition[3] = posnxn(0) ;   
//   triggerPosition[4] = posnxn(1) ;   
//   triggerPosition[5] = posnxn(2) ;  

//   TArrayF triggerAmplitudes(4);
//   triggerAmplitudes[0] = maxAmp2x2 ;   
//   triggerAmplitudes[1] = ampOutOfPatch2x2 ;    
//   triggerAmplitudes[2] = maxAmpnxn ;   
//   triggerAmplitudes[3] = ampOutOfPatchnxn ;   

//   //esd->SetRSTACKTriggerCells(triggerPosition);
//   esd->AddRSTACKTriggerPosition(triggerPosition);
//   esd->AddRSTACKTriggerAmplitudes(triggerAmplitudes);
  

  //########################################
  //############# Fill CaloCells ###########
  //########################################

  Int_t nDigits = fgDigitsArray->GetEntries();
  Int_t idignew = 0 ;
  IlcDebug(1,Form("%d digits",nDigits));

  const Int_t knEMC = fGeom->GetNModules()*fGeom->GetNPhi()*fGeom->GetNZ();
  IlcESDCaloCells &phsCells = *(esd->GetPHOSCells());
  phsCells.CreateContainer(nDigits);
  phsCells.SetType(IlcESDCaloCells::kPHOSCell);

  // Add to CaloCells only EMC digits with non-zero energy 
  for (Int_t idig = 0 ; idig < nDigits ; idig++) {
    const IlcRSTACKDigit * dig = (const IlcRSTACKDigit*)fgDigitsArray->At(idig);
    if(dig->GetId() <= knEMC && 
       Calibrate(dig->GetEnergy(),dig->GetId()) > GetRecoParam()->GetEMCMinE() ){
      phsCells.SetCell(idignew,dig->GetId(), Calibrate(dig->GetEnergy(),dig->GetId()),
                                             CalibrateT(dig->GetTime(),dig->GetId()));   
      idignew++;
    }
  }
  phsCells.SetNumberOfCells(idignew);
  phsCells.Sort();

  //########################################
  //############## Fill CaloClusters #######
  //########################################

  for (Int_t recpart = 0 ; recpart < nOfRecParticles ; recpart++) {
    IlcRSTACKRecParticle  *rp    = static_cast<IlcRSTACKRecParticle*>(recParticles->At(recpart));
    if (Debug()) 
      rp->Print();
    // Get track segment and EMC rec.point associated with this rec.particle
    IlcRSTACKTrackSegment *ts    = static_cast<IlcRSTACKTrackSegment *>(fTSM->GetTrackSegments()
								    ->At(rp->GetRSTACKTSIndex()));

    IlcRSTACKEmcRecPoint  *emcRP = static_cast<IlcRSTACKEmcRecPoint *>(fgEMCRecPoints->At(ts->GetEmcIndex()));
    IlcESDCaloCluster   *ec    = new IlcESDCaloCluster() ; 
    
    Float_t xyz[3];
    for (Int_t ixyz=0; ixyz<3; ixyz++) 
      xyz[ixyz] = rp->GetPos()[ixyz];
    
    IlcDebug(2,Form("Global position xyz=(%f,%f,%f)",xyz[0],xyz[1],xyz[2]));
   
    // Create cell lists

    Int_t     cellMult   = emcRP->GetDigitsMultiplicity();
    Int_t    *digitsList = emcRP->GetDigitsList();
    Float_t  *rpElist    = emcRP->GetEnergiesList() ;
    UShort_t *absIdList  = new UShort_t[cellMult];
    Double_t *fracList   = new Double_t[cellMult];

    for (Int_t iCell=0; iCell<cellMult; iCell++) {
      IlcRSTACKDigit *digit = static_cast<IlcRSTACKDigit *>(fgDigitsArray->At(digitsList[iCell]));
      absIdList[iCell] = (UShort_t)(digit->GetId());
      if (digit->GetEnergy() > 0)
 	fracList[iCell] = rpElist[iCell]/(Calibrate(digit->GetEnergy(),digit->GetId()));
      else
 	fracList[iCell] = 0;
    }

    //Primaries
    Int_t  primMult  = 0;
    Int_t *primList =  emcRP->GetPrimaries(primMult);

    Float_t energy=0.;
    if (GetRecoParam()->EMCEcore2ESD())
      energy = emcRP->GetCoreEnergy();
    else
      energy = rp->Energy();
    //Apply nonlinearity correction
    if(GetRecoParam()->GetEMCEnergyCorrectionOn())
      energy=CorrectNonlinearity(energy) ;

    // fills the ESDCaloCluster
    ec->SetType(IlcVCluster::kPHOSNeutral);
    ec->SetPosition(xyz);                       //rec.point position in MARS
    ec->SetE(energy);                           //total or core particle energy
    ec->SetDispersion(emcRP->GetDispersion());  //cluster dispersion
    ec->SetPID(rp->GetPID()) ;            //array of particle identification
    ec->SetM02(emcRP->GetM2x()) ;               //second moment M2x
    ec->SetM20(emcRP->GetM2z()) ;               //second moment M2z
    ec->SetNExMax(emcRP->GetNExMax());          //number of local maxima
    ec->SetEmcCpvDistance(ts->GetCpvDistance("r")); //Only radius, what about separate x,z????
    ec->SetTrackDistance(ts->GetCpvDistance("x"),ts->GetCpvDistance("z")); 
    ec->SetChi2(-1);                     //not yet implemented
    ec->SetTOF(emcRP->GetTime());               //Time of flight - already calibrated in EMCRecPoint

    //Cells contributing to clusters
    ec->SetNCells(cellMult);
    ec->SetCellsAbsId(absIdList);
    ec->SetCellsAmplitudeFraction(fracList);

    //Distance to the nearest bad crystal
    ec->SetDistanceToBadChannel(emcRP->GetDistanceToBadCrystal()); 
  
    //Array of MC indeces
    TArrayI arrayPrim(primMult,primList);
    ec->AddLabels(arrayPrim);
    
    //Matched ESD track
    TArrayI arrayTrackMatched(1);
    arrayTrackMatched[0]= ts->GetTrackIndex();
    ec->AddTracksMatched(arrayTrackMatched);
    
    Int_t index = esd->AddCaloCluster(ec);

    //Set pointer to this cluster in ESD track
    Int_t nt=esd->GetNumberOfTracks();
    for (Int_t itr=0; itr<nt; itr++) {
      IlcESDtrack *esdTrack=esd->GetTrack(itr);
      if(!esdTrack->IsPHOS())
        continue ;
      if(esdTrack->GetPHOScluster()==-recpart){ //we store negative cluster number
        esdTrack->SetPHOScluster(index) ;
//no garatie that only one track matched this cluster
//      break ;
      }
    }
 
    delete ec;   
    delete [] fracList;
    delete [] absIdList;
  }
  fgDigitsArray ->Clear();
  fgEMCRecPoints->Clear("C");
  recParticles  ->Clear();

  //Store RSTACK misalignment matrixes
  FillMisalMatrixes(esd) ;

}

//____________________________________________________________________________
IlcTracker* IlcRSTACKReconstructor::CreateTracker() const
{
  // creates the RSTACK tracker
  return new IlcRSTACKTracker();
}

//____________________________________________________________________________
void  IlcRSTACKReconstructor::ConvertDigits(IlcRawReader* rawReader, TTree* digitsTree) const
{
  // Converts raw data to
  // RSTACK digits
  // Works on a single-event basis
  rawReader->Reset() ; 

  IlcRSTACKRawFitterv0 * fitter ;

  const TObjArray* maps = IlcRSTACKRecoParam::GetMappings();
  if(!maps) IlcFatal("Cannot retrieve ALTRO mappings!!");

  IlcAltroMapping *mapping[20];
  for(Int_t i = 0; i < 20; i++) {
    mapping[i] = (IlcAltroMapping*)maps->At(i);
  }

  if      (strcmp(GetRecoParam()->EMCFitterVersion(),"v0")==0) 
    fitter=new IlcRSTACKRawFitterv0();
  else if (strcmp(GetRecoParam()->EMCFitterVersion(),"v1")==0) 
    fitter=new IlcRSTACKRawFitterv1();
  else if (strcmp(GetRecoParam()->EMCFitterVersion(),"v2")==0) 
    fitter=new IlcRSTACKRawFitterv2();
  else if (strcmp(GetRecoParam()->EMCFitterVersion(),"v3")==0) 
    fitter=new IlcRSTACKRawFitterv3();
  else
    fitter=new IlcRSTACKRawFitterv4();

  fitter->SubtractPedestals(GetRecoParam()->EMCSubtractPedestals());
  fitter->SetAmpOffset     (GetRecoParam()->GetGlobalAltroOffset());
  fitter->SetAmpThreshold  (GetRecoParam()->GetGlobalAltroThreshold());

  TClonesArray *digits = new TClonesArray("IlcRSTACKDigit",1);
  digits->SetName("DIGITS");
  Int_t bufsize = 32000;
  digitsTree->Branch("RSTACK", &digits, bufsize);

  IlcRSTACKRawDigiProducer rdp(rawReader,mapping);

  rdp.SetEmcMinAmp(GetRecoParam()->GetEMCRawDigitThreshold()); // in ADC
  rdp.SetCpvMinAmp(GetRecoParam()->GetCPVMinE());
  rdp.SetSampleQualityCut(GetRecoParam()->GetEMCSampleQualityCut());
  rdp.MakeDigits(digits,fTmpDigLG,fitter);

  delete fitter ;

  TClonesArray *tdigits = new TClonesArray("IlcRSTACKTriggerRawDigit",1);
  tdigits->SetName("TDIGITS");
  digitsTree->Branch("TRSTACK", &tdigits, bufsize);  

  rawReader->Reset();
  IlcRSTACKTriggerRawDigiProducer tdp(rawReader);
  
  IlcRSTACKTriggerParameters* parameters = (IlcRSTACKTriggerParameters*)IlcRSTACKRecoParam::GetTriggerParameters();
  
  tdp.SetTriggerParameters(parameters);
  tdp.ProcessEvent(tdigits);
  
  if (IlcLog::GetGlobalDebugLevel() == 1) {
    Int_t modMax=-111;
    Int_t colMax=-111;
    Int_t rowMax=-111;
    Float_t eMax=-333;
    //!!!for debug!!!
    
    Int_t relId[4];
    for(Int_t iDigit=0; iDigit<digits->GetEntries(); iDigit++) {
      IlcRSTACKDigit* digit = (IlcRSTACKDigit*)digits->At(iDigit);
      if(digit->GetEnergy()>eMax) {
	fGeom->AbsToRelNumbering(digit->GetId(),relId);
	eMax=digit->GetEnergy();
	modMax=relId[0];
	rowMax=relId[2];
	colMax=relId[3];
      }
    }
    
    IlcDebug(1,Form("Digit with max. energy:  modMax %d colMax %d rowMax %d  eMax %f\n\n",
		    modMax,colMax,rowMax,eMax));
  }
  
  digitsTree->Fill();
  
  digits->Delete();
  delete digits;
  
  tdigits->Delete();
  delete tdigits;
}
//==================================================================================
Float_t IlcRSTACKReconstructor::Calibrate(Float_t amp, Int_t absId)const{
  // Calibrate EMC digit, i.e. multiply its Amp by a factor read from CDB

  const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;

  //Determine rel.position of the cell absolute ID
  Int_t relId[4];
  geom->AbsToRelNumbering(absId,relId);
  Int_t module=relId[0];
  Int_t row   =relId[2];
  Int_t column=relId[3];
  if(relId[1]){ //CPV
    Float_t calibration = fgCalibData->GetADCchannelCpv(module,column,row);
    return amp*calibration ;
  }
  else{ //EMC
    Float_t calibration = fgCalibData->GetADCchannelEmc(module,column,row);
    return amp*calibration ;
  }
}
//==================================================================================
Float_t IlcRSTACKReconstructor::CalibrateT(Float_t time, Int_t absId)const{
  // Calibrate EMC digit, i.e. multiply its Amp by a factor read from CDB

  const IlcRSTACKGeometry *geom = IlcRSTACKGeometry::GetInstance() ;

  //Determine rel.position of the cell absolute ID
  Int_t relId[4];
  geom->AbsToRelNumbering(absId,relId);
  Int_t module=relId[0];
  Int_t row   =relId[2];
  Int_t column=relId[3];
  if(relId[1]){ //CPV
    return 0. ;
  }
  else{ //EMC
    time += fgCalibData->GetTimeShiftEmc(module,column,row);
    return time ;
  }
}
//==================================================================================
void IlcRSTACKReconstructor::FillMisalMatrixes(IlcESDEvent* esd)const{
  //Store RSTACK matrixes in ESD Header

  //Check, if matrixes was already stored
  for(Int_t mod=0 ;mod<5; mod++){
    if(esd->GetPHOSMatrix(mod)!=0)
      return ;
  }

  //Create and store matrixes
  if(!gGeoManager){
    IlcError("Can not store misal. matrixes: no gGeoManager! \n") ;
    return ;
  }
  //Note, that owner of copied marixes will be header
  char path[255] ;
  TGeoHMatrix * m ;
  for(Int_t mod=0; mod<5; mod++){
    snprintf(path,255,"/ILCM_1/RSTACK_%d",mod+1) ; //In Geometry modules numbered 1,2,.,5
    if (gGeoManager->cd(path)){
      m = gGeoManager->GetCurrentMatrix() ;
      esd->SetPHOSMatrix(new TGeoHMatrix(*m),mod) ;
    }
    else{
      esd->SetPHOSMatrix(NULL,mod) ;
    }
  }

}
//==================================================================================
Float_t IlcRSTACKReconstructor::CorrectNonlinearity(Float_t en){

  //For backward compatibility, if no RecoParameters found
  if(!GetRecoParam()){
    return 0.0241+1.0504*en+0.000249*en*en ;
  }

  if(strcmp(GetRecoParam()->GetNonlinearityCorrectionVersion(),"NoCorrection")==0){
    return en ;
  }
  if(strcmp(GetRecoParam()->GetNonlinearityCorrectionVersion(),"Gustavo2005")==0){
    const Float_t *par=GetRecoParam()->GetNonlinearityParams() ;
    return par[0]+par[1]*en + par[2]*en*en ;
  }
  if(strcmp(GetRecoParam()->GetNonlinearityCorrectionVersion(),"Henrik2010")==0){
     const Float_t *par=GetRecoParam()->GetNonlinearityParams() ;
     return en*(par[0]+par[1]*TMath::Exp(-en*par[2]))*(1.+par[3]*TMath::Exp(-en*par[4]))*(1.+par[6]/(en*en+par[5])) ;
  }
  //For backward compatibility
  if(strcmp(GetRecoParam()->GetNonlinearityCorrectionVersion(),"")==0){
    return 0.0241+1.0504*en+0.000249*en*en ;
  }
  return en ;
}

void IlcRSTACKReconstructor::readTRUParameters(IlcRSTACKTriggerParameters* parameters) const
{
  //Read trigger parameters.

  TString path(gSystem->Getenv("ILC_ROOT"));
  path += "/RSTACK/macros/Trigger/OCDB/";
  
  for (Int_t mod = 2; mod < 5; ++mod) { // module
    for (Int_t tru = 0; tru < 4; tru++) { // tru row
      for (Int_t branch = 0; branch < 2; branch++) { // branch
	
	// Open the Appropriate pedestal file
	TString fileName = path;
	fileName += "pedestal_m";
	fileName = fileName += mod;
	fileName+="_r";
	fileName+=tru;
	fileName+="_b";
	fileName+=branch;
	fileName+=".dat";
	std::ifstream instream;
	instream.open(fileName.Data());
	
	// Read pedestals from file
	if( ! instream.is_open() )
	  Printf("E-TRUPedestals: could not open %s", fileName.Data());
	else
	  {
	    Int_t ped[112];
	    
	    char ch_s[36];
	    char *ch_s_p = ch_s;
	    //Int_t nlines = 0;
	    
	    Int_t t_ped_0 =0;
	    Int_t t_ped_1 =0;
	    Int_t t_ped_2 =0;
	    
	    for(Int_t n=0; n<112; n++)
	      {
		instream.getline(ch_s_p,36);
		if (ch_s_p[23]<='9' && ch_s_p[23]>='0')
		  {
		    t_ped_0 = ch_s_p[23]-'0';
		  }
		else if (ch_s_p[23]>='A' && ch_s_p[23]<='Z')
		  {
		    t_ped_0 = ch_s_p[23]-'A'+10;
		    
		  }
		  
		if (ch_s_p[22]<='9' && ch_s_p[22]>='0')
		  {
		    t_ped_1 = ch_s_p[22]-'0';
		  }
		else if (ch_s_p[22]<='Z' && ch_s_p[22]>='A')
		  {
		    t_ped_1 = ch_s_p[22]-'A'+10;
		  }
		
		if (ch_s_p[21]<='9' && ch_s_p[21]>='0')
		  {
		    t_ped_2 = ch_s_p[21]-'0';
		  }
		else if (ch_s_p[21]<='Z' && ch_s_p[21]>='A')
		  {
		    t_ped_2 = ch_s_p[21]-'A'+10;
		  }
		
		ped[n]=t_ped_2*256+t_ped_1*16+t_ped_0;
		
		
	      }
	    for (Int_t xrow = 0; xrow < 8; xrow++){
	      for (Int_t zcol = 0; zcol < 14; zcol++){
		Int_t pedestal = ped[zcol*8+xrow];
		
		if( pedestal < 612 && pedestal > 412 ) // resonable
		  parameters->SetTRUPedestal(pedestal, mod, tru, branch, xrow, zcol);
		else // unresonable
		  continue;
	      }
	    }
	  } // Ends read of pedestals from branch from file.
	instream.close();
      }// end branch
    }// end tru
    
  }// end for mod
}




