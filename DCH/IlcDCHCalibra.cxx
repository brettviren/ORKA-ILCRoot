/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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
 
/* $Id: IlcDCHCalibra.cxx,v 1.2 2009/12/11 10:50:32 vitomeg Exp $ */ 
 
///////////////////////////////////////////////////////////////////////////////// 
//                                                                              
// IlcDCHCalibra                                                                
//                                                                              
// This class is for the DCH calibration of the relative gain factor, the drift velocity, 
// the time 0 and the pad response function.         
// It can be used for the calibration per chamber but also per group of pads and eventually per pad. 
// The user has to choose with the functions SetNz and SetNrphi the precision of the calibration.  
//Begin_Html 
/* 
<br> 
<CENTER> 
<TABLE border=1> 
<TR><TD><center>Nz</center></TD><TD><center> 0 </center></TD><TD><center> 1 </center></TD><TD><center> 2 </center></TD><TD><center> 3 </center></TD><TD><center> 4 </center></TD></TR> 
<TR><TD><CENTER>group of row pads per detector</CENTER></TD><TD><CENTER>1</CENTER></TD><TD><CENTER>2</CENTER></TD><TD><CENTER>4</CENTER></TD><TD><CENTER>6(chamb2)<br> 8(others chambers)</CENTER></TD><TD><CENTER>12 (chamb2)<br> 16 (chamb0)</CENTER></TD></TR> 
<TR><TD><CENTER>row pads per group</CENTER></TD><TD><CENTER>12 (chamb2)<br> 16 (chamb0)</CENTER></TD><TD><CENTER>6 (chamb2)<br> 8 (chamb0)</CENTER></TD><TD><CENTER>3 (chamb2)<br> 4 (chamb0)</CENTER></TD><TD><CENTER>2</CENTER></TD><TD><CENTER>1</CENTER></TD></TR> 
<TR><TD><CENTER>~distance [cm]</CENTER></TD><TD><CENTER>106 (chamb2)<br> 130 (chamb0)</CENTER></TD><TD><CENTER>53 (chamb2)<br> 65 (chamb0)</CENTER></TD><TD><CENTER>26.5 (chamb2)<br> 32.5 (chamb0)</CENTER></TD><TD><CENTER>17 (chamb2)<br> 17 (chamb0)</CENTER></TD><TD><CENTER>9 (chamb2)<br> 9 (chamb0)</CENTER></TD></TR> 
<CAPTION>In the z direction</CAPTION> 
</TABLE> 
</CENTER> 
<CENTER> 
<br> 
<TABLE border=1> 
<TR><TD><center>Nrphi</center></TD><TD><center> 0 </center></TD><TD><center> 1 </center></TD><TD><center> 2 </center></TD><TD><center> 3 </center></TD><TD><center> 4 </center></TD><TD><center> 5 </center></TD><TD><center> 6 </center></TD></TR> 
<TR><TD><CENTER>group of col pads per detector</CENTER></TD><TD><CENTER>1</CENTER></TD><TD><CENTER>2</CENTER></TD><TD><CENTER>4</CENTER></TD><TD><CENTER>8</CENTER></TD><TD><CENTER>16</CENTER></TD><TD><center>36</center></TD><TD><center>144</center></TD></TR> 
<TR><TD><CENTER>col pads per group</CENTER></TD><TD><CENTER>144</CENTER></TD><TD><CENTER>72</CENTER></TD><TD><CENTER>36</CENTER></TD><TD><CENTER>18</CENTER></TD><TD><CENTER>9</CENTER></TD><TD><center>4</center></TD><TD><center>1</center></TD></TR> 
<TR><TD><CENTER>~distance [cm]</CENTER></TD><TD><CENTER>113.4</CENTER></TD><TD><CENTER>56.7</CENTER></TD><TD><CENTER>25.3</CENTER></TD><TD><CENTER>14.3</CENTER></TD><TD><CENTER>7.25</CENTER></TD><TD><center>3.2</center></TD><TD><center>0.8</center></TD></TR> 
<CAPTION>In the rphi direction</CAPTION> 
</TABLE> 
</CENTER> 
<br> 
*/ 
//End_Html  
// 
// Fill histograms or vectors 
//---------------------------- 
//    
// 2D Histograms (Histo2d) or vectors (Vector2d), then converted in Trees, will be filled 
// from RAW DATA in a run or from reconstructed DCH tracks during the offline tracking  
// in the function "FollowBackProlongation" (IlcDCHtracker) 
// Per default the functions to fill are off.                                    
// 
//Begin_Html 
/* 
Example of 2D histos for the relative gain (left) and the drift velocity (right) calibration of the sector 13 <br> 
<center> 
<img src="./gif/2dhisto.gif" width="600" height="350"><br> 
</center> 
*/ 
//End_Html     
// 
// Fit the histograms to find the coefficients  
//--------------------------------------------- 
// 
// These 2D histograms or vectors (first converted in 1D histos) will be fitted   
// if they have enough entries, otherwise the (default) value of the choosen database  
// will be put. For the relative gain calibration the resulted factors will be globally  
// normilczed to the gain factors of the choosen database. It unables to precise      
// previous calibration procedure. 
// The function SetDebug enables the user to see:                                      
// _fDebug = 0: nothing, only the values are written in the tree if wanted 
// _fDebug = 1: a comparaison of the coefficients found and the default values  
//              in the choosen database. 
//              fCoef , histogram of the coefs as function of the calibration group number 
//              fDelta , histogram of the relative difference of the coef with the default 
//                        value in the database as function of the calibration group number 
//              fError , dirstribution of this relative difference 
// _fDebug = 2: only the fit of the choosen calibration group fFitVoir (SetFitVoir) 
// _fDebug = 3: The coefficients in the choosen detector fDet (SetDet) as function of the 
//              pad row and col number 
// _fDebug = 4; The coeffcicients in the choosen detector fDet (SetDet) like in the 3 but with 
//              also the comparaison histograms of the 1 for this detector 
// 
//Begin_Html 
/* 
Example of fCoef for the relative gain calibration of the sector 13 <br> 
<center> 
<img src="./gif/coef.gif"  width="400" height="460"> 
</center><br> 
Example of fDelta (right) and fError (left) for the relative gain calibration of the sector 13 <br> 
<center> 
<img src="./gif/detlaerror.gif"  width="550" height="380"><br> 
</center> 
*/ 
//End_Html  
//                             
// Author: 
//   R. Bailhache (R.Bailhache@gsi.de) 
//                             
////////////////////////////////////////////////////////////////////////////////////// 
 
#include <TTree.h> 
#include <TLine.h> 
#include <TH1I.h> 
#include <TStyle.h> 
#include <TProfile2D.h> 
#include <TFile.h> 
#include <TCanvas.h> 
#include <TGraphErrors.h> 
#include <TObjArray.h> 
#include <TChain.h> 
#include <TH1.h> 
#include <TH1I.h> 
#include <TH1F.h> 
#include <TF1.h> 
#include <TH2F.h> 
#include <TAxis.h> 
#include <TStopwatch.h> 
#include <TMath.h> 
#include <TLegend.h> 
#include <TDirectory.h> 
#include <TROOT.h> 
 
#include "IlcLog.h" 
#include "IlcCDBManager.h" 
#include "IlcRun.h" 
#include "IlcRunLoader.h" 
#include "IlcLoader.h" 
#include "IlcRawReaderFile.h" 
#include "IlcRawReader.h" 
 
#include "IlcDCHCalibra.h" 
#include "IlcDCHcalibDB.h" 
#include "IlcDCHCommonParam.h" 
#include "IlcDCHmcmTracklet.h" 
#include "IlcDCHpadPlane.h" 
#include "IlcDCHcluster.h" 
#include "IlcDCHtrack.h" 
#include "IlcDCHdigit.h" 
#include "IlcDCHdigitsManager.h" 
#include "IlcDCH.h" 
#include "IlcDCHgeometry.h" 
#include "./Cal/IlcDCHCalROC.h" 
#include "./Cal/IlcDCHCalPad.h" 
#include "./Cal/IlcDCHCalDet.h" 
#include "IlcDCHrawData.h" 
 
ClassImp(IlcDCHCalibra) 
 
IlcDCHCalibra* IlcDCHCalibra::fgInstance = 0; 
Bool_t IlcDCHCalibra::fgTerminated = kFALSE; 
 
//_____________singleton implementation_________________________________________________ 
IlcDCHCalibra *IlcDCHCalibra::Instance() 
{ 
  // 
  // Singleton implementation 
  // 
 
  if (fgTerminated != kFALSE) { 
    return 0; 
  } 
 
  if (fgInstance == 0) { 
    fgInstance = new IlcDCHCalibra(); 
  } 
 
  return fgInstance; 
 
} 
 
//______________________________________________________________________________________ 
void IlcDCHCalibra::Terminate() 
{ 
  // 
  // Singleton implementation 
  // Deletes the instance of this class 
  // 
 
  fgTerminated = kTRUE; 
 
  if (fgInstance != 0) { 
    delete fgInstance; 
    fgInstance = 0; 
  } 
 
} 
 
//______________________________________________________________________________________ 
IlcDCHCalibra::IlcDCHCalibra() 
  :TObject() 
  ,fMITracking(kFALSE) 
  ,fMcmTracking(kFALSE) 
  ,fMcmCorrectAngle(kFALSE) 
  ,fCH2dOn(kFALSE) 
  ,fPH2dOn(kFALSE) 
  ,fPRF2dOn(kFALSE) 
  ,fHisto2d(kFALSE) 
  ,fVector2d(kFALSE) 
  ,fRelativeScale(0) 
  ,fCountRelativeScale(0) 
  ,fRelativeScaleAuto(kFALSE) 
  ,fThresholdDigit(0) 
  ,fThresholdClusterPRF1(0.0) 
  ,fThresholdClusterPRF2(0.0) 
  ,fCenterOfflineCluster(kFALSE) 
  ,fTraMaxPad(kFALSE) 
  ,fWriteNameCoef(0) 
  ,fWriteName(0) 
  ,fFitPHOn(kFALSE) 
  ,fFitPHPeriode(0) 
  ,fBeginFitCharge(0.0) 
  ,fRangeFitPRF(0.0) 
  ,fMeanChargeOn(kFALSE) 
  ,fFitChargeBisOn(kFALSE) 
  ,fT0Shift(0.0) 
  ,fAccCDB(kFALSE) 
  ,fNumberFit(0) 
  ,fStatisticMean(0.0) 
  ,fDebug(0) 
  ,fFitVoir(0) 
  ,fPRF(0) 
  ,fGain(0) 
  ,fT0(0) 
  ,fVdrift(0) 
  ,fVdriftDetector(0) 
  ,fVdriftPad(0x0) 
  ,fT0Detector(0) 
  ,fT0Pad(0x0) 
  ,fPRFDetector(0) 
  ,fPRFPad(0x0) 
  ,fCoefCH(0x0) 
  ,fDetectorIlcDCHtrack(kFALSE) 
  ,fChamberIlcDCHtrack(-1) 
  ,fDetectorPreviousTrack(-1) 
  ,fGoodTrack(kTRUE) 
  ,fAmpTotal(0x0) 
  ,fPHPlace(0x0) 
  ,fPHValue(0x0) 
  ,fNumberClusters(0) 
  ,fProcent(0.0) 
  ,fDifference(0) 
  ,fNumberTrack(0) 
  ,fDeltaPRF(0) 
  ,fErrorPRF(0) 
  ,fCoefPRFDB(0) 
  ,fTimeMax(0) 
  ,fSf(0.0) 
  ,fScaleFitFactor(0.0) 
  ,fMinEntries(0) 
  ,fEntriesCurrent(0) 
  ,fL3P0(0.0) 
  ,fL3P2(0.0) 
  ,fG3P2(0.0) 
  ,fVectorPH(0) 
  ,fPlaPH(0) 
  ,fNumberBinCharge(0) 
  ,fVectorCH(0) 
  ,fPlaCH(0) 
  ,fVectorFitCH(0) 
  ,fNumberBinPRF(0) 
  ,fVectorPRF(0) 
  ,fPlaPRF(0) 
  ,fPH2d(0x0) 
  ,fPRF2d(0x0) 
  ,fCH2d(0x0) 
  ,fRebin(0) 
{ 
  // 
  // Default constructor 
  // 
 
  for (Int_t i = 0; i < 3; i++) { 
    fNz[i]    = 0; 
    fNrphi[i] = 0; 
  } 
 
  for (Int_t k = 0; k < 3; k++) { 
    fNtotal[k]    = 0; 
    fDetChamb2[k] = 0; 
    fDetChamb0[k] = 0; 
  } 
 
  // Write 
  for (Int_t i = 0; i < 3; i++) { 
    fWriteCoef[i] = kFALSE; 
    fWrite[i]     = kFALSE; 
  } 
 
  // Debug Mode 
  for (Int_t k = 0; k < 3; k++) { 
    fDet[k] = 0; 
  } 
 
  for (Int_t i = 0; i < 2; i++) { 
    fPhd[i] = 0.0; 
  } 
  fPhd[3] = 0.0; 
 
  // Init 
  Init(); 
   
} 
 
//______________________________________________________________________________________ 
IlcDCHCalibra::IlcDCHCalibra(const IlcDCHCalibra &c) 
  :TObject(c) 
  ,fMITracking(kFALSE) 
  ,fMcmTracking(kFALSE) 
  ,fMcmCorrectAngle(kFALSE) 
  ,fCH2dOn(kFALSE) 
  ,fPH2dOn(kFALSE) 
  ,fPRF2dOn(kFALSE) 
  ,fHisto2d(kFALSE) 
  ,fVector2d(kFALSE) 
  ,fRelativeScale(0) 
  ,fCountRelativeScale(0) 
  ,fRelativeScaleAuto(kFALSE) 
  ,fThresholdDigit(0) 
  ,fThresholdClusterPRF1(0.0) 
  ,fThresholdClusterPRF2(0.0) 
  ,fCenterOfflineCluster(kFALSE) 
  ,fTraMaxPad(kFALSE) 
  ,fWriteNameCoef(0) 
  ,fWriteName(0) 
  ,fFitPHOn(kFALSE) 
  ,fFitPHPeriode(0) 
  ,fBeginFitCharge(0.0) 
  ,fRangeFitPRF(0.0) 
  ,fMeanChargeOn(kFALSE) 
  ,fFitChargeBisOn(kFALSE) 
  ,fT0Shift(0.0) 
  ,fAccCDB(kFALSE) 
  ,fNumberFit(0) 
  ,fStatisticMean(0.0) 
  ,fDebug(0) 
  ,fFitVoir(0) 
  ,fPRF(0) 
  ,fGain(0) 
  ,fT0(0) 
  ,fVdrift(0) 
  ,fVdriftDetector(0) 
  ,fVdriftPad(0x0) 
  ,fT0Detector(0) 
  ,fT0Pad(0x0) 
  ,fPRFDetector(0) 
  ,fPRFPad(0x0) 
  ,fCoefCH(0x0) 
  ,fDetectorIlcDCHtrack(kFALSE) 
  ,fChamberIlcDCHtrack(-1) 
  ,fDetectorPreviousTrack(-1) 
  ,fGoodTrack(kTRUE) 
  ,fAmpTotal(0x0) 
  ,fPHPlace(0x0) 
  ,fPHValue(0x0) 
  ,fNumberClusters(0) 
  ,fProcent(0.0) 
  ,fDifference(0) 
  ,fNumberTrack(0) 
  ,fDeltaPRF(0) 
  ,fErrorPRF(0) 
  ,fCoefPRFDB(0) 
  ,fTimeMax(0) 
  ,fSf(0.0) 
  ,fScaleFitFactor(0.0) 
  ,fMinEntries(0) 
  ,fEntriesCurrent(0) 
  ,fL3P0(0.0) 
  ,fL3P2(0.0) 
  ,fG3P2(0.0) 
  ,fVectorPH(0) 
  ,fPlaPH(0) 
  ,fNumberBinCharge(0) 
  ,fVectorCH(0) 
  ,fPlaCH(0) 
  ,fVectorFitCH(0) 
  ,fNumberBinPRF(0) 
  ,fVectorPRF(0) 
  ,fPlaPRF(0) 
  ,fPH2d(0x0) 
  ,fPRF2d(0x0) 
  ,fCH2d(0x0) 
  ,fRebin(0) 
{ 
  // 
  // Copy constructor 
  // 
 
} 
 
//____________________________________________________________________________________ 
IlcDCHCalibra::~IlcDCHCalibra() 
{ 
  // 
  // IlcDCHCalibra destructor 
  // 
 
  ClearHistos(); 
  ClearTree(); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::Destroy()  
{ 
  // 
  // Delete instance  
  // 
 
  if (fgInstance) { 
    delete fgInstance; 
    fgInstance = 0x0; 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ClearHistos()  
{ 
  // 
  // Delete the histos 
  // 
 
  if (fPH2d) { 
    delete fPH2d; 
    fPH2d  = 0x0; 
  } 
  if (fCH2d) { 
    delete fCH2d; 
    fCH2d  = 0x0; 
  } 
  if (fPRF2d) { 
    delete fPRF2d; 
    fPRF2d = 0x0; 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ClearTree()  
{ 
  // 
  // Delete the trees 
  // 
   
  if (fPRF) { 
    delete fPRF; 
    fPRF    = 0x0; 
  } 
  if (fGain) { 
    delete fGain; 
    fGain   = 0x0; 
  } 
  if (fT0) { 
    delete fT0; 
    fT0     = 0x0; 
  } 
  if (fVdrift) { 
    delete fVdrift; 
    fVdrift = 0x0; 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::Init()  
{ 
  // 
  // Init some default values 
  // 
 
  // How to fill the 2D 
  fThresholdDigit       = 5; 
  fThresholdClusterPRF1 = 2.0; 
  fThresholdClusterPRF2 = 20.0; 
   
  // Store the Info 
  fNumberBinCharge      = 100; 
  fNumberBinPRF         = 20; 
   
  // Write 
  fWriteName            = "DCH.calibration.root"; 
  fWriteNameCoef        = "DCH.coefficient.root"; 
   
  // Fit 
  fFitPHPeriode         = 1; 
  fBeginFitCharge       = 3.5; 
  fRangeFitPRF          = 0.5; 
  fMinEntries           = 800; 
  fT0Shift              = 0.143397; 
   
  // Internal variables 
   
  // Fill the 2D histos in the offline tracking 
  fDetectorPreviousTrack = -1; 
  fChamberIlcDCHtrack    = -1; 
  fGoodTrack             = kTRUE; 
 
  fProcent               = 6.0; 
  fDifference            = 17; 
  fNumberClusters        = 18; 
  fNumberTrack           = 0; 
  fNumberUsedCh[0]       = 0; 
  fNumberUsedCh[1]       = 0; 
  fNumberUsedPh[0]       = 0; 
  fNumberUsedPh[1]       = 0; 
   
  // Variables in the loop 
  for (Int_t k = 0; k < 4; k++) { 
    fChargeCoef[k] = 1.0; 
    fVdriftCoef[k] = 1.5; 
    fT0Coef[k]     = -1.0; 
  } 
  for (Int_t i = 0; i < 2; i++) { 
    fPRFCoef[i]    = -1.0; 
  } 
   
  // Pad calibration 
  for (Int_t i = 0; i < 3; i++) { 
    fRowMin[i]    = -1; 
    fRowMax[i]    = -1; 
    fColMax[i]    = -1; 
    fColMin[i]    = -1; 
    fNnZ[i]       = -1; 
    fNnRphi[i]    = -1; 
    fNfragZ[i]    = -1; 
    fNfragRphi[i] = -1; 
    fXbins[i]     = -1; 
  } 
   
  // Local database to be changed 
  fRebin = 1; 
   
} 
 
//____________Functions fit Online CH2d________________________________________ 
Bool_t IlcDCHCalibra::FitCHOnline(TH2I *ch) 
{ 
  // 
  // Fit the 1D histos, projections of the 2D ch on the Xaxis, for each 
  // calibration group normilczed the resulted coefficients (to 1 normally) 
  // and write the results in a tree 
  // 
    
  // Number of Xbins (detectors or groups of pads) 
  TAxis   *xch     = ch->GetXaxis(); 
  Int_t    nbins   = xch->GetNbins(); 
  TAxis   *yph     = ch->GetYaxis(); 
  Int_t    nybins  = yph->GetNbins(); 
  Double_t lowedge = xch->GetBinLowEdge(1); 
  Double_t upedge  = xch->GetBinUpEdge(xch->GetNbins()); 
  if (!InitFit(nbins,lowedge,upedge,0)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // For memory 
  if (fVectorCH) { 
    fVectorCH->Clear(); 
  } 
  if (fPlaCH) { 
    fPlaCH->Clear(); 
  } 
   
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(0); 
   
  // Beginning of the loop betwwen dect1 and dect2 
  for (Int_t idect = fDect1[0]; idect < fDect2[0]; idect++) { 
 
    TH1I *projch = (TH1I *) ch->ProjectionY("projch",idect+1,idect+1,(Option_t *)"e"); 
    projch->SetDirectory(0); 
 
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,0); 
     
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect, 0); 
     
    // Number of entries for this calibration group 
    Double_t nentries = 0.0; 
    for (Int_t k = 0; k < nybins; k++) { 
      nentries += ch->GetBinContent(ch->GetBin(idect+1,k+1)); 
    } 
       
    // Rebin and statistic stuff 
    // Rebin 
    if (fRebin > 1) { 
      projch = ReBin((TH1I *) projch); 
    } 
    // This detector has not enough statistics or was off 
    if (nentries < fMinEntries) { 
      // Fill with the default infos 
      NotEnoughStatistic(idect,0); 
      // Memory!!! 
      if (fDebug != 2) { 
	delete projch; 
      } 
      continue; 
    } 
     
    // Statistics of the group fitted 
    IlcInfo(Form("For the group number %d there are %f stats",idect,nentries)); 
    fStatisticMean += nentries; 
    fNumberFit++;  
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitChargeBisOn) {  
      FitBisCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
    } 
     
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefChargeDB(); 
    } 
    // Fill Infos Fit 
    FillInfosFit(idect,0); 
     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projch; 
    } 
     
  } // Boucle object 
 
  // Normierungcharge 
  if (fDebug != 2) { 
    NormierungCharge(); 
  } 
   
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorCH(); 
  } 
  
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    PlotCH(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)) { 
    PlotCHDB(); 
  } 
 
  // Mean Statistic 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean/fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
   
  // Write the things! 
  ConvertVectorFitCHTree(); 
  if (fWriteCoef[0]) { 
    WriteFitInfos(0);        
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online CH2d________________________________________ 
Bool_t IlcDCHCalibra::FitCHOnline() 
{ 
  // 
  // Reconstruct a 1D histo from the vectorCH for each calibration group, 
  // fit the histo, normilczed the resulted coefficients (to 1 normally) 
  // and write the results in a tree 
  // 
 
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,0)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
  
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(0); 
 
  // Beginning of the loop between dect1 and dect2 
  for (Int_t idect = fDect1[0]; idect < fDect2[0]; idect++) { 
     
    // Search if the group is in the VectorCH 
    Int_t place = SearchInVector(idect,0); 
        
    // Is in 
    TH1F *projch = 0x0; 
    TString name("CH"); 
    name += idect; 
    if (place != -1) { 
      // Variable 
      IlcDCHCTInfo *fCHInfo = new IlcDCHCTInfo(); 
      // Retrieve 
      fCHInfo = ((IlcDCHCTInfo *) fVectorCH->At(place)); 
      projch  = ConvertVectorCTHisto(fCHInfo,(const char *) name); 
      projch->SetDirectory(0); 
      delete fCHInfo; 
    } 
 
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,0); 
     
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,0); 
     
    // Number of entries 
    Double_t nentries = 0.0; 
    if (projch) { 
      for (Int_t k = 0; k < fNumberBinCharge; k++) { 
        nentries += projch->GetBinContent(k+1); 
      } 
    } 
   
    // Rebin and statistic stuff 
    // Rebin 
    if ((fRebin >  1) &&  
        (place != -1)) { 
      projch = ReBin((TH1F *) projch); 
    } 
 
    // This detector has not enough statistics or was not found in VectorCH 
    if ((place == -1) ||  
        ((place != -1) &&  
         (nentries < fMinEntries))) { 
       
      // Fill with the default infos 
      NotEnoughStatistic(idect,0); 
 
      // Memory!!! 
      if (fDebug != 2) { 
	delete projch; 
      }      
       
      continue; 
 
    } 
 
    // Statistic of the histos fitted 
    IlcInfo(Form("For the group number %d there are %f stats",idect,nentries)); 
    fNumberFit++; 
    fStatisticMean += nentries; 
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
     
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitChargeBisOn) {  
      FitBisCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
    } 
     
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefChargeDB(); 
    } 
     
    // Fill Infos Fit 
    FillInfosFit(idect,0);  
     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projch; 
    } 
     
  } // Boucle object 
 
  // Normierungcharge 
  if (fDebug != 2) { 
    NormierungCharge(); 
  } 
   
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorCH(); 
  } 
   
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)){ 
    PlotCH(); 
  } 
  if((fDebug == 4) ||  
     (fDebug == 3)){ 
    PlotCHDB(); 
  } 
   
  // Mean Statistics 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
   
  // Write the things! 
  ConvertVectorFitCHTree(); 
  if (fWriteCoef[0]) { 
    WriteFitInfos(0);       
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online CH2d________________________________________ 
Bool_t IlcDCHCalibra::FitCHOnline(TTree *tree) 
{ 
  // 
  // Look if the calibration group can be found in the tree, if yes take the 
  // histo, fit it, normilczed the resulted coefficients (to 1 normally) and 
  // write the results in a tree 
  // 
    
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,0)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
   
  // For memory 
  if (fVectorCH) { 
    fVectorCH->Clear(); 
  } 
  if (fPlaCH) { 
    fPlaCH->Clear(); 
  } 
    
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(0); 
  TH1F      *projch      = 0x0; 
  tree->SetBranchAddress("histo",&projch); 
  TObjArray *vectorplace = ConvertTreeVector(tree); 
 
  // Beginning of the loop between dect1 and dect2 
  for (Int_t idect = fDect1[0]; idect < fDect2[0]; idect++) { 
     
    //Search if the group is in the VectorCH 
    Int_t place = SearchInTreeVector(vectorplace,idect); 
     
    // Is in 
    if (place != -1) { 
      // Variable 
      tree->GetEntry(place); 
    } 
     
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,0); 
     
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,0); 
     
    // Number of entries 
    Double_t nentries = 0.0; 
    if (projch) { 
      for (Int_t k = 0; k < projch->GetXaxis()->GetNbins(); k++) { 
        nentries += projch->GetBinContent(k+1); 
      } 
    }     
 
    // Rebin and statistic stuff 
    // Rebin 
    if ((fRebin  >  1) &&  
        (place  != -1)) { 
      projch = ReBin((TH1F *) projch); 
    } 
 
    // This detector has not enough statistics or was not found in VectorCH 
    if((place == -1) ||  
       ((place    !=          -1) &&  
        (nentries  < fMinEntries))) { 
       
      // Fill with the default infos 
      NotEnoughStatistic(idect,0); 
       
      continue; 
 
    } 
     
    // Statistics of the group fitted 
    IlcInfo(Form("For the group number %d there are %f stats",idect,nentries)); 
    fNumberFit++; 
    fStatisticMean += nentries; 
    
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
 
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitChargeBisOn) {  
      FitBisCH((TH1 *) projch,(Int_t) (idect-fDect1[0])); 
    } 
 
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefChargeDB(); 
    }     
 
    // Fill Infos Fit 
    FillInfosFit(idect,0);  
     
  } // Boucle object 
 
  // Normierungcharge 
  if (fDebug != 2) { 
    NormierungCharge(); 
  } 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorCH(); 
  } 
  
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)){ 
    PlotCH(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)){ 
    PlotCHDB(); 
  } 
 
  // Mean Statistic 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
 
  // Write the things! 
  ConvertVectorFitCHTree(); 
  if (fWriteCoef[0]) { 
    WriteFitInfos(0);       
  } 
 
  return kTRUE; 
   
} 
 
//________________functions fit Online PH2d____________________________________ 
Bool_t IlcDCHCalibra::FitPHOnline(TProfile2D *ph) 
{ 
  // 
  // Take the 1D profiles (average pulse height), projections of the 2D PH 
  // on the Xaxis, for each calibration group 
  // Fit or use the slope of the average pulse height to reconstruct the 
  // drift velocity write the results in a tree 
  // A first calibration of T0 is also made  using the same method (slope method) 
  // 
  
  // Number of Xbins (detectors or groups of pads) 
  TAxis   *xph     = ph->GetXaxis(); 
  TAxis   *yph     = ph->GetYaxis(); 
  Int_t    nbins   = xph->GetNbins(); 
  Int_t    nybins  = yph->GetNbins(); 
  Double_t lowedge = xph->GetBinLowEdge(1); 
  Double_t upedge  = xph->GetBinUpEdge(xph->GetNbins()); 
  if (!InitFit(nbins,lowedge,upedge,1)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // For memory 
  if (fVectorPH) { 
    fVectorPH->Clear(); 
  } 
  if (fPlaPH) { 
    fPlaPH->Clear(); 
  } 
 
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(1); 
   
  // Beginning of the loop 
  for (Int_t idect = fDect1[1]; idect < fDect2[1]; idect++) { 
       
    TH1D *projph = (TH1D *) ph->ProjectionY("projph",idect+1,idect+1,(Option_t *) "e"); 
    projph->SetDirectory(0);  
 
    // Number of entries for this calibration group 
    Double_t nentries = 0; 
    for (Int_t k = 0; k < nybins; k++) { 
      nentries += ph->GetBinEntries(ph->GetBin(idect+1,k+1)); 
    } 
   
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,1); 
 
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,1); 
     
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if (nentries  < fMinEntries) { 
       
      // Fill with the default values 
      NotEnoughStatistic(idect,1);      
       
      // Memory!!! 
      if (fDebug != 2) { 
	delete projph; 
      } 
            
      continue; 
 
    } 
     
    // Statistics of the histos fitted 
    IlcInfo(Form("For the group number %d there are %f stats",idect,nentries)); 
    fNumberFit++; 
    fStatisticMean += nentries; 
     
    // Calcul of "real" coef 
    CalculVdriftCoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
    CalculT0CoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPente((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
 
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitPHOn) {  
      FitPH((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
    } 
      
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefVdriftDB(); 
      FillCoefT0DB(); 
    } 
 
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,1); 
     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projph; 
    } 
   
  } // Boucle object 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPH(); 
    ErrorT0(); 
  } 
  
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    PlotPH(); 
    PlotT0(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)) { 
    PlotPHDB(); 
    PlotT0DB(); 
  } 
 
  // Mean Statistic 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
 
  // Write the things! 
  if(fWriteCoef[1]) { 
    WriteFitInfos(1); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online PH2d________________________________________ 
Bool_t IlcDCHCalibra::FitPHOnline() 
{ 
  // 
  // Reconstruct the average pulse height from the vectorPH for each 
  // calibration group 
  // Fit or use the slope of the average pulse height to reconstruct the 
  // drift velocity write the results in a tree 
  // A first calibration of T0 is also made  using the same method (slope method) 
  // 
    
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,1)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(1); 
 
  // Beginning of the loop 
  for (Int_t idect = fDect1[1]; idect < fDect2[1]; idect++) { 
 
    // Search if the group is in the VectorCH 
    Int_t place = SearchInVector(idect,1); 
     
    // Is in 
    TH1F    *projph = 0x0; 
    TString name("PH"); 
    name += idect; 
    if (place != -1) { 
      // Variable 
      IlcDCHPInfo *fPHInfo = new IlcDCHPInfo(); 
      // Retrieve 
      fPHInfo = (IlcDCHPInfo *) fVectorPH->At(place); 
      projph  = CorrectTheError((TGraphErrors *) ConvertVectorPHisto(fPHInfo,(const char *) name)); 
      projph->SetDirectory(0); 
      delete fPHInfo; 
    } 
 
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,1); 
     
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,1); 
 
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if ((place == -1) ||  
        ((place           !=          -1) &&  
         (fEntriesCurrent <  fMinEntries))) { 
       
      // Fill with the default values 
      NotEnoughStatistic(idect,1); 
 
      // Memory!!! 
      if (fDebug != 2) { 
	delete projph; 
      } 
 
      continue; 
 
    } 
 
    // Statistic of the histos fitted 
    IlcInfo(Form("For the group number %d there are %d stats",idect,fEntriesCurrent)); 
    fNumberFit++; 
    fStatisticMean += fEntriesCurrent; 
 
    // Calcul of "real" coef 
    CalculVdriftCoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
    CalculT0CoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
 
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPente((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
 
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitPHOn) {  
      FitPH((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
    } 
   
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefVdriftDB(); 
      FillCoefT0DB(); 
    } 
     
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,1); 
     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projph; 
    } 
     
  } // Boucle object 
   
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPH(); 
    ErrorT0(); 
  } 
  
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    PlotPH(); 
    PlotT0(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)) { 
    PlotPHDB(); 
    PlotT0DB(); 
  } 
   
  // Mean Statistic 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
   
  // Write the things! 
  if (fWriteCoef[1]) { 
    WriteFitInfos(1); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online PH2d________________________________________ 
Bool_t IlcDCHCalibra::FitPHOnline(TTree *tree) 
{ 
  // 
  // Look if the calibration group can be found in the tree, if yes take the 
  // histo, fit it, and write the results in a tree 
  // A first calibration of T0 is also made  using the same method (slope method) 
  // 
    
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,1)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // For memory 
  if (fVectorPH) { 
    fVectorPH->Clear(); 
  } 
  if (fPlaPH) { 
    fPlaPH->Clear(); 
  } 
 
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(1); 
  TGraphErrors *projphtree  = 0x0; 
  tree->SetBranchAddress("histo",&projphtree); 
  TObjArray    *vectorplace = ConvertTreeVector(tree); 
   
  // Beginning of the loop 
  for (Int_t idect = fDect1[1]; idect < fDect2[1]; idect++) { 
 
    // Search if the group is in the VectorCH 
    Int_t place = SearchInTreeVector(vectorplace,idect); 
     
    TH1F *projph = 0x0; 
    // Is in 
    if (place != -1) { 
      // Variable 
      tree->GetEntry(place); 
      projph = CorrectTheError(projphtree); 
    } 
     
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,1); 
 
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,1); 
 
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if((place == -1) ||  
       ((place           !=         -1) &&  
        (fEntriesCurrent < fMinEntries))) { 
       
      // Fill with the default values 
      NotEnoughStatistic(idect,1); 
       
      // Memory!!! 
      if (fDebug != 2) { 
	delete projph; 
      } 
       
      continue; 
 
    } 
     
    // Statistics of the histos fitted 
    IlcInfo(Form("For the group number %d there are %d stats",idect,fEntriesCurrent)); 
    fNumberFit++; 
    fStatisticMean += fEntriesCurrent; 
 
    // Calcul of "real" coef 
    CalculVdriftCoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
    CalculT0CoefMean(fCountDet[1],(Int_t) (idect - fDect1[1])); 
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPente((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
    // Method fit bis 
    // idect is egal for fDebug = 0 and 2, only to fill the hist 
    if (fFitPHOn) {  
      FitPH((TH1 *) projph,(Int_t) (idect - fDect1[1])); 
    } 
   
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefVdriftDB(); 
      FillCoefT0DB(); 
    } 
 
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,1); 
 
    // Memory!!! 
    if (fDebug != 2) { 
      delete projph; 
    } 
        
  } // Boucle object 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPH(); 
    ErrorT0(); 
  } 
  
  // Plot 
  // 0 no plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)){ 
    PlotPH(); 
    PlotT0(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)){ 
    PlotPHDB(); 
    PlotT0DB(); 
  } 
 
  // Mean Statistics 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
   
  // Write the things! 
  if (fWriteCoef[1]) { 
    WriteFitInfos(1); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online PRF2d_______________________________________ 
Bool_t IlcDCHCalibra::FitPRFOnline(TProfile2D *prf) 
{ 
  // 
  // Take the 1D profiles (pad response function), projections of the 2D PRF 
  // on the Xaxis, for each calibration group 
  // Fit with a gaussian to reconstruct the sigma of the pad response function 
  // write the results in a tree 
  // 
 
  // Number of Xbins (detectors or groups of pads) 
  TAxis   *xprf    = prf->GetXaxis(); 
  TAxis   *yprf    = prf->GetYaxis(); 
  Int_t    nybins  = yprf->GetNbins(); 
  Int_t    nbins   = xprf->GetNbins(); 
  Double_t lowedge = xprf->GetBinLowEdge(1); 
  Double_t upedge  = xprf->GetBinUpEdge(xprf->GetNbins()); 
  if (!InitFit(nbins,lowedge,upedge,2)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // For memory 
  if (fVectorPRF) { 
    fVectorPRF->Clear(); 
  } 
  if (fPlaPRF) { 
    fPlaPRF->Clear(); 
  } 
   
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(2); 
   
  // Beginning of the loop 
  for (Int_t idect = fDect1[2]; idect < fDect2[2]; idect++) { 
 
    TH1D *projprf = (TH1D *) prf->ProjectionY("projprf",idect+1,idect+1,(Option_t *) "e"); 
    projprf->SetDirectory(0); 
     
    // Number of entries for this calibration group 
    Double_t nentries = 0; 
    for (Int_t k = 0; k < nybins; k++) { 
      nentries += prf->GetBinEntries(prf->GetBin(idect+1,k+1)); 
    } 
     
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,2); 
     
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,2); 
     
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if (nentries < fMinEntries) { 
       
      // Fill with the default values 
      NotEnoughStatistic(idect,2); 
       
      // Memory! 
      if (fDebug != 2) { 
	delete projprf; 
      } 
       
      continue; 
 
    } 
     
    // Statistics of the histos fitted 
    IlcInfo(Form("For the group number %d there are %f stats",idect,nentries)); 
    fNumberFit++; 
    fStatisticMean += nentries; 
     
    // Calcul of "real" coef 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      CalculPRFCoefMean(fCountDet[2],(Int_t) (idect - fDect1[2])); 
    } 
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPRF((TH1 *) projprf,(Int_t) (idect - fDect1[2])); 
     
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefPRFDB(); 
    } 
 
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,2); 
     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projprf; 
    } 
     
  } // Boucle object 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPRF(); 
  } 
  
  // Plot 
  // No plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    PlotPRF(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)){ 
    PlotPRFDB(); 
  } 
 
  // Mean Statistic 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
 
  // Write the things! 
  if (fWriteCoef[2]) { 
    WriteFitInfos(2); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online PRF2d_______________________________________ 
Bool_t IlcDCHCalibra::FitPRFOnline(TTree *tree) 
{ 
  // 
  // Look if the calibration group can be found in the tree, if yes take 
  // the histo, fit it, and write the results in a tree 
  // 
   
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,2)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // For memory 
  if (fVectorPRF) { 
    fVectorPRF->Clear(); 
  } 
  if (fPlaPRF) { 
    fPlaPRF->Clear(); 
  } 
 
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(2); 
  TGraphErrors *projprftree = 0x0; 
  tree->SetBranchAddress("histo",&projprftree); 
  TObjArray    *vectorplace = ConvertTreeVector(tree); 
 
  // Beginning of the loop 
  for (Int_t idect = fDect1[2]; idect < fDect2[2]; idect++) { 
 
    // Search if the group is in the VectorCH 
    Int_t place = SearchInTreeVector(vectorplace,idect); 
     
    // Is in    
    TH1F *projprf = 0x0; 
    if (place != -1) { 
      // Variable 
      tree->GetEntry(place); 
      projprf = CorrectTheError(projprftree); 
    } 
 
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,2); 
 
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,2); 
 
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if ((place == -1) || 
        ((place           !=          -1) && 
         (fEntriesCurrent  < fMinEntries))) { 
       
      // Fill with the default values 
      NotEnoughStatistic(idect,2); 
       
      // Memory!!! 
      if (fDebug != 2) { 
	delete projprf; 
      } 
       
      continue; 
 
    } 
 
    // Statistics of the histos fitted 
    IlcInfo(Form("For the group number %d there are %d stats",idect,fEntriesCurrent)); 
    fNumberFit++; 
    fStatisticMean += fEntriesCurrent; 
         
    // Calcul of "real" coef 
    if ((fDebug == 1) ||  
        (fDebug == 4)){ 
      CalculPRFCoefMean(fCountDet[2],(Int_t) (idect - fDect1[2])); 
    } 
     
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPRF((TH1 *) projprf,(Int_t) (idect - fDect1[2])); 
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefPRFDB(); 
    } 
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,2); 
 
    // Memory!!! 
    if (fDebug != 2) { 
      delete projprf; 
    }                 
     
  } // Boucle object 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPRF(); 
  } 
   
  // Plot 
  // No plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)){ 
    PlotPRF(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)){ 
    PlotPRFDB(); 
  } 
   
  // Mean Statistics 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
    fStatisticMean = fStatisticMean / fNumberFit; 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
 
  // Write the things! 
  if (fWriteCoef[2]) { 
    WriteFitInfos(2); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions fit Online PRF2d_______________________________________ 
Bool_t IlcDCHCalibra::FitPRFOnline() 
{ 
  // 
  // Reconstruct the 1D histo (pad response function) from the vectorPRD for 
  // each calibration group 
  // Fit with a gaussian to reconstruct the sigma of the pad response function 
  // write the results in a tree 
  // 
   
  // Number of Xbins (detectors or groups of pads) 
  if (!InitFit(0,0,0,2)) { 
    return kFALSE; 
  } 
  fStatisticMean = 0.0; 
  fNumberFit     = 0; 
 
  // Init fCountDet and fCount 
  InitfCountDetAndfCount(2); 
 
  // Beginning of the loop 
  for (Int_t idect = fDect1[2]; idect < fDect2[2]; idect++) { 
 
    // Search if the group is in the VectorCH 
    Int_t place = SearchInVector(idect,2); 
     
    // Is in 
    TH1F   *projprf = 0x0; 
    TString name("PRF"); 
    name += idect; 
    if (place != -1) { 
      // Variable 
      IlcDCHPInfo *fPRFInfo = new IlcDCHPInfo(); 
      // Retrieve 
      fPRFInfo = (IlcDCHPInfo *) fVectorPRF->At(place); 
      projprf = CorrectTheError((TGraphErrors *) ConvertVectorPHisto(fPRFInfo,(const char *)name)); 
      projprf->SetDirectory(0); 
      delete fPRFInfo; 
    } 
 
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    UpdatefCountDetAndfCount(idect,2); 
 
    // Reconstruction of the row and pad group: rowmin, row max ... 
    ReconstructFitRowMinRowMax(idect,2); 
 
    // Rebin and statistic stuff 
    // This detector has not enough statistics or was off 
    if ((place == -1) || 
        ((place           != -1) && 
         (fEntriesCurrent < fMinEntries))) { 
 
      // Fill with the default values 
      NotEnoughStatistic(idect,2); 
 
      // Memory 
      if (fDebug != 2) { 
	delete projprf; 
      } 
 
      continue; 
 
    } 
 
    // Statistic of the histos fitted 
    IlcInfo(Form("For the group number %d there are %d stats", idect,fEntriesCurrent)); 
    fNumberFit++; 
    fStatisticMean += fEntriesCurrent; 
 
    // Calcul of "real" coef 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      CalculPRFCoefMean(fCountDet[2],(Int_t) (idect-fDect1[2])); 
    } 
 
    // Method Mean and fit 
    // idect is egal for fDebug = 0 and 2, only to fill the hist  
    FitPRF((TH1 *) projprf,(Int_t) (idect-fDect1[2])); 
    // Visuilcse the detector for fDebug 3 or 4 
    // Here is the reconstruction of the pad and row group is used! 
    if (fDebug >= 3) { 
      FillCoefPRFDB(); 
    } 
    // Fill the tree if end of a detector or only the pointer to the branch!!! 
    FillInfosFit(idect,2); 
                     
    // Memory!!! 
    if (fDebug != 2) { 
      delete projprf; 
    } 
     
  } // Boucle object 
 
  // Error 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    ErrorPRF(); 
  } 
  
  // Plot 
  // No plot, 1 and 4 error plot, 3 and 4 DB plot 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    PlotPRF(); 
  } 
  if ((fDebug == 4) ||  
      (fDebug == 3)) { 
    PlotPRFDB(); 
  } 
 
  // Mean Statistics 
  if (fNumberFit > 0) { 
    IlcInfo(Form("There is a mean statistic of: %d",(Int_t) fStatisticMean / fNumberFit)); 
  } 
  else { 
    IlcInfo("There is no fit!"); 
  } 
 
  // Write the things! 
  if (fWriteCoef[2]) { 
    WriteFitInfos(2); 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
Bool_t IlcDCHCalibra::Init2Dhistos() 
{ 
  // 
  // For the offline tracking 
  // This function will be called in the function IlcReconstruction::Run()  
  // Init the calibration mode (Nz, Nrphi), the 2D histograms if fHisto2d = kTRUE,  
  // 
 
  // DB Setting 
  // Get cal 
  IlcDCHcalibDB *cal = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
 
  // Some parameters 
  fTimeMax = cal->GetNumberOfTimeBins(); 
  fSf      = cal->GetSamplingFrequency(); 
  if (fRelativeScaleAuto) { 
    fRelativeScale = 0; 
  } 
  else { 
    fRelativeScale = 20; 
  } 
 
  // Create the 2D histos corresponding to the pad groupCalibration mode 
  if (fCH2dOn) { 
 
    IlcInfo(Form("The pad calibration mode for the relative gain calibration: Nz %d, and Nrphi %d" 
                ,fNz[0] 
                ,fNrphi[0])); 
     
    // Calcul the number of Xbins 
    fNtotal[0] = 0; 
    ModePadCalibration(2,0); 
    ModePadFragmentation(0,2,0,0); 
    fDetChamb2[0] = fNfragZ[0] * fNfragRphi[0]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the chamber 2: %d",fDetChamb2[0])); 
    } 
    fNtotal[0] += 6 * 18 * fDetChamb2[0]; 
    ModePadCalibration(0,0); 
    ModePadFragmentation(0,0,0,0); 
    fDetChamb0[0] = fNfragZ[0] * fNfragRphi[0]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the other chamber 0: %d",fDetChamb0[0])); 
    } 
    fNtotal[0] += 6 * 4 * 18 * fDetChamb0[0]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[0])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreateCH2d(fNtotal[0]); 
    } 
    if (fVector2d) { 
      fVectorCH = new TObjArray(); 
      fPlaCH    = new TObjArray(); 
    } 
 
    // Variable 
    fAmpTotal = new Float_t[TMath::Max(fDetChamb2[0],fDetChamb0[0])]; 
    for (Int_t k = 0; k < TMath::Max(fDetChamb2[0],fDetChamb0[0]); k++) { 
      fAmpTotal[k] = 0.0; 
    }  
 
  } 
 
  if (fPH2dOn) { 
 
    IlcInfo(Form("The pad calibration mode for the drift velocity calibration: Nz %d, and Nrphi %d" 
                ,fNz[1] 
                ,fNrphi[1])); 
     
    // Calcul the number of Xbins 
    fNtotal[1] = 0; 
    ModePadCalibration(2,1); 
    ModePadFragmentation(0,2,0,1); 
    fDetChamb2[1] = fNfragZ[1]*fNfragRphi[1]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the chamber 2: %d",fDetChamb2[1])); 
    } 
    fNtotal[1] += 6 * 18 * fDetChamb2[1]; 
    ModePadCalibration(0,1); 
    ModePadFragmentation(0,0,0,1); 
    fDetChamb0[1] = fNfragZ[1] * fNfragRphi[1]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the chamber 0: %d",fDetChamb0[1])); 
    } 
    fNtotal[1] += 6 * 4 * 18 * fDetChamb0[1]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[1])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreatePH2d(fNtotal[1]); 
    } 
    if (fVector2d) { 
      fVectorPH = new TObjArray(); 
      fPlaPH    = new TObjArray(); 
    } 
    
    // Variable 
    fPHPlace = new Short_t[fTimeMax]; 
    for (Int_t k = 0; k < fTimeMax; k++) { 
      fPHPlace[k] = -1; 
    }  
    fPHValue = new Float_t[fTimeMax]; 
    for (Int_t k = 0; k < fTimeMax; k++) { 
      fPHValue[k] = -1.0; 
    } 
 
  } 
 
  if (fPRF2dOn) { 
 
    IlcInfo(Form("The pad calibration mode for the PRF calibration: Nz %d, and Nrphi %d" 
                ,fNz[2] 
                ,fNrphi[2])); 
     
    // Calcul the number of Xbins 
    fNtotal[2] = 0; 
    ModePadCalibration(2,2); 
    ModePadFragmentation(0,2,0,2); 
    fDetChamb2[2] = fNfragZ[2] * fNfragRphi[2]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the chamber 2: %d",fDetChamb2[2])); 
    } 
    fNtotal[2] += 6 * 18 * fDetChamb2[2]; 
    ModePadCalibration(0,2); 
    ModePadFragmentation(0,0,0,2); 
    fDetChamb0[2] = fNfragZ[2] * fNfragRphi[2]; 
    if (fDebug == 4) { 
      IlcInfo(Form("For the chamber 0: %d",fDetChamb0[2])); 
    } 
    fNtotal[2] += 6 * 4 * 18 * fDetChamb0[2]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[2])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreatePRF2d(fNtotal[2]); 
    } 
    if (fVector2d) { 
      fVectorPRF = new TObjArray(); 
      fPlaPRF    = new TObjArray(); 
    } 
   
  } 
 
  return kTRUE; 
 
} 
 
//____________Functions for filling the histos in the code_____________________ 
 
//____________Offine tracking in the IlcDCHtracker_____________________________ 
Bool_t IlcDCHCalibra::ResetTrack() 
{ 
  // 
  // For the offline tracking 
  // This function will be called in the function 
  // IlcDCHtracker::FollowBackPropagation() at the beginning  
  // Reset the parameter to know we have a new DCH track 
  // 
   
  fDetectorIlcDCHtrack = kFALSE; 
  return kTRUE; 
 
} 
 
//____________Offline tracking in the IlcDCHtracker____________________________ 
Bool_t IlcDCHCalibra::UpdateHistograms(IlcDCHcluster *cl, IlcDCHtrack *t) 
{ 
  // 
  // For the offline tracking 
  // This function will be called in the function 
  // IlcDCHtracker::FollowBackPropagation() in the loop over the clusters 
  // of DCH tracks  
  // Fill the 2D histos or the vectors with the info of the clusters at 
  // the end of a detectors if the track is "good" 
  // 
 
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam"); 
    return kFALSE; 
  } 
 
  // Get the parameter object 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
  
  // Localisation of the detector 
  Int_t detector = cl->GetDetector(); 
  Int_t chamber  = GetChamber(detector); 
  Int_t plane    = GetPlane(detector); 
 
  // Fill the infos for the previous clusters if not the same 
  // detector anymore or if not the same track 
  if (((detector != fDetectorPreviousTrack) || (!fDetectorIlcDCHtrack)) &&  
      (fDetectorPreviousTrack != -1)) { 
 
    fNumberTrack++;    
     
    // If the same track, then look if the previous detector is in 
    // the same plane, if yes: not a good track 
    if (fDetectorIlcDCHtrack &&  
        (GetPlane(detector) <= GetPlane(fDetectorPreviousTrack))) { 
      fGoodTrack = kFALSE; 
    } 
 
    // Fill only if the track doesn't touch a masked pad or doesn't 
    // appear in the middle (fGoodTrack) 
    if (fGoodTrack) { 
 
      // Gain calibration 
      if (fCH2dOn) { 
	FillTheInfoOfTheTrackCH(); 
      } 
 
      // PH calibration 
      if (fPH2dOn) { 
	FillTheInfoOfTheTrackPH();     
      } 
     
    } // if a good track 
     
    ResetfVariables(); 
    
  } // Fill at the end the charge 
   
  // Calcul the position of the detector 
  if (detector != fDetectorPreviousTrack) { 
    LocalisationDetectorXbins(detector); 
  } 
 
  // Reset the good track for the PRF 
  Bool_t good = kTRUE; 
   
  // Localisation of the cluster 
  Double_t pos[3] = { 0.0, 0.0, 0.0 }; 
  pos[0] = cl->GetX(); 
  pos[1] = cl->GetY(); 
  pos[2] = cl->GetZ(); 
  Int_t    time   = cl->GetLocalTimeBin(); 
   
  // Reset the detector 
  fDetectorPreviousTrack = detector; 
  fDetectorIlcDCHtrack   = kTRUE; 
   
  // Position of the cluster 
  IlcDCHpadPlane *padplane = parCom->GetPadPlane(plane,chamber); 
  Int_t    row        = padplane->GetPadRowNumber(pos[2]); 
  Double_t offsetz    = padplane->GetPadRowOffset(row,pos[2]); 
  Double_t offsettilt = padplane->GetTiltOffset(offsetz); 
  Int_t    col        = padplane->GetPadColNumber(pos[1] + offsettilt,offsetz); 
   
  // See if we are not near a masked pad 
  if (!IsPadOn(detector,col,row)) { 
    good       = kFALSE; 
    fGoodTrack = kFALSE; 
  } 
 
  if (col > 0) { 
    if (!IsPadOn(detector,col-1,row)) { 
      fGoodTrack = kFALSE; 
      good       = kFALSE; 
    } 
  } 
 
  if (col < 143) { 
    if (!IsPadOn(detector,col+1,row)) { 
      fGoodTrack = kFALSE; 
      good       = kFALSE; 
    } 
  } 
 
  // Row of the cluster and position in the pad groups 
  Int_t posr[3] = { 0, 0, 0 }; 
  if ((fCH2dOn)  && (fNnZ[0] != 0)) { 
    posr[0] = (Int_t) row / fNnZ[0]; 
  } 
  if ((fPH2dOn)  && (fNnZ[1] != 0)) { 
    posr[1] = (Int_t) row / fNnZ[1]; 
  } 
  if ((fPRF2dOn) && (fNnZ[2] != 0)) { 
    posr[2] = (Int_t) row / fNnZ[2]; 
  }   
       
  // Col of the cluster and position in the pad groups 
  Int_t posc[3] = { 0, 0, 0 }; 
  if ((fCH2dOn)  && (fNnRphi[0] != 0)) { 
    posc[0] = (Int_t) col / fNnRphi[0]; 
  } 
  if ((fPH2dOn)  && (fNnRphi[1] != 0)) { 
    posc[1] = (Int_t) col / fNnRphi[1]; 
  } 
  if ((fPRF2dOn) && (fNnRphi[2] != 0)) { 
    posc[2] = (Int_t) col / fNnRphi[2]; 
  } 
 
  // Charge in the cluster 
  // For the moment take the abs 
  Float_t  q       = TMath::Abs(cl->GetQ()); 
  Short_t *signals = cl->GetSignals(); 
 
  // Correction due to the track angle 
  Float_t correction    = 1.0; 
  Float_t normilcsation = 6.67; 
  if ((q >0) && (t->GetNdedx() > 0)) { 
    correction = t->GetClusterdQdl((t->GetNdedx() - 1)) / (q * normilcsation); 
  } 
 
  // Fill the fAmpTotal with the charge 
  if (fCH2dOn) { 
    if (!fTraMaxPad){  
      fAmpTotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += q * correction; 
    } 
    else { 
      fAmpTotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += ((Float_t) signals[3]) * correction; 
    } 
  } 
 
  // Fill the fPHPlace and value 
  if (fPH2dOn) { 
    fPHPlace[time] = posc[1]*fNfragZ[1]+posr[1]; 
    if (!fTraMaxPad) { 
      fPHValue[time] = q * correction; 
    } 
    else { 
      fPHValue[time] = ((Float_t) signals[3]) * correction; 
    } 
  } 
 
  // Fill direct the PRF 
  if ((fPRF2dOn) && (good)) { 
 
    Float_t yminus  = 0.0; 
    Float_t xcenter = 0.0; 
    Float_t ycenter = 0.0; 
    Float_t ymax    = 0.0; 
    Bool_t  echec   = kFALSE; 
     
    if ((cl->From3pad()) && (!cl->IsUsed())) {  
          
      // Center 3 balanced 
      if ((((Float_t) signals[3]) > fThresholdClusterPRF2) &&  
          (((Float_t) signals[2]) > fThresholdClusterPRF2) &&  
          (((Float_t) signals[4]) > fThresholdClusterPRF2) &&  
          (((Float_t) signals[1]) < fThresholdClusterPRF1) &&  
          (((Float_t) signals[5]) < fThresholdClusterPRF1) &&  
          (q * correction > 130.0)) { 
	// Col correspond to signals[3] 
	if (fCenterOfflineCluster) { 
          xcenter = cl->GetCenter(); 
	} 
	else { 
	  // Security of the denomiateur is 0 
	  if ((((Float_t) (((Float_t) signals[3]) * ((Float_t) signals[3]))) /  
                           ((Float_t) (((Float_t) signals[2]) * ((Float_t) signals[4])))) != 1.0) { 
	    xcenter = 0.5 * (TMath::Log((Float_t) (((Float_t) signals[4]) / ((Float_t) signals[2])))) 
                          / (TMath::Log(((Float_t) (((Float_t) signals[3]) * ((Float_t) signals[3])))  
                                      / ((Float_t) (((Float_t) signals[2]) * ((Float_t) signals[4]))))); 
	  } 
	  else { 
            xcenter = -100.0; 
	  } 
	} 
	if ((xcenter > -0.5) && (xcenter < 0.5)) { 
	  ycenter = (Float_t) (((Float_t) signals[3])  
                            / (((Float_t) signals[2]) + ((Float_t) signals[3]) + (((Float_t) signals[4])))); 
	  yminus  = (Float_t) (((Float_t) signals[2])  
                            / (((Float_t) signals[2]) + ((Float_t) signals[3]) + (((Float_t) signals[4])))); 
	  ymax    = (Float_t) (((Float_t) signals[4])  
                            / (((Float_t) signals[2]) + ((Float_t) signals[3]) + (((Float_t) signals[4])))); 
	  if ((ycenter > 0.485) &&  
              (TMath::Abs(((Float_t) signals[2]) + ((Float_t) signals[3]) + (((Float_t) signals[4])) - q) < 8.0)) { 
            echec = kTRUE; 
	  } 
	} 
      } 
       
      // Fill only if it is in the drift region! 
      if ((((Float_t) (((Float_t) time) / fSf)) > 0.3) && (echec)) { 
	if (fHisto2d) { 
	  fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),xcenter,ycenter); 
	  if (xcenter < 0.0) { 
            fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),-(xcenter+1.0),yminus); 
	  } 
	  if (xcenter > 0.0) { 
            fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),1.0-xcenter,ymax); 
	  } 
	} 
	if (fVector2d) { 
	  UpdateVectorPRF(fXbins[2]+posc[2]*fNfragZ[2]+posr[2],xcenter,ycenter); 
	  if (xcenter < 0.0) { 
            UpdateVectorPRF(fXbins[2]+posc[2]*fNfragZ[2]+posr[2],-(xcenter+1.0),yminus); 
	  } 
	  if (xcenter > 0.0) { 
            UpdateVectorPRF(fXbins[2]+posc[2]*fNfragZ[2]+posr[2],1.0-xcenter,ymax); 
	  } 
	} 
      } // If in the drift region 
 
    } // Cluster isole 
 
  } // PRF2dOn	 
   
  return kTRUE; 
   
} 
 
//____________Online trackling in IlcDCHtrigger________________________________ 
Bool_t IlcDCHCalibra::UpdateHistogramcm(IlcDCHmcmTracklet *trk) 
{ 
  // 
  // For the tracking 
  // This function will be called in the function IlcDCHtrigger::TestTracklet 
  // before applying the pt cut on the tracklets  
  // Fill the infos for the tracklets fTrkTest if the tracklets is "good" 
  // 
   
  // Localisation of the Xbins involved 
  Int_t idect = trk->GetDetector(); 
  LocalisationDetectorXbins(idect); 
 
  // Get the parameter object 
  IlcDCHcalibDB *cal = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
    
  // Reset 
  ResetfVariables(); 
 
  // Row of the tracklet and position in the pad groups 
  Int_t row     = trk->GetRow(); 
  Int_t posr[3] = { 0, 0, 0 }; 
  if ((fCH2dOn)  && (fNnZ[0] != 0)) { 
    posr[0] = (Int_t) row / fNnZ[0]; 
  } 
  if ((fPH2dOn)  && (fNnZ[1] != 0)) { 
    posr[1] = (Int_t) row / fNnZ[1]; 
  } 
  if ((fPRF2dOn) && (fNnZ[2] != 0)) { 
    posr[2] = (Int_t) row / fNnZ[2]; 
  } 
  
  // Eventuelle correction due to track angle in z direction 
  Float_t correction = 1.0; 
  if (fMcmCorrectAngle) { 
    Float_t z = trk->GetRowz(); 
    Float_t r = trk->GetTime0(); 
    correction = r / TMath::Sqrt((r*r+z*z)); 
  } 
 
  // Boucle sur les clusters 
  // Condition on number of cluster: don't come from the middle of the detector 
  if (trk->GetNclusters() >= fNumberClusters) { 
 
    for (Int_t icl = 0; icl < trk->GetNclusters(); icl++) { 
 
      Float_t amp[3] = { 0.0, 0.0, 0.0 }; 
      Int_t   time   = trk->GetClusterTime(icl); 
      Int_t   col    = trk->GetClusterCol(icl); 
             
      amp[0] = trk->GetClusterADC(icl)[0] * correction; 
      amp[1] = trk->GetClusterADC(icl)[1] * correction; 
      amp[2] = trk->GetClusterADC(icl)[2] * correction; 
            
      if ((amp[0] < 0.0) ||  
          (amp[1] < 0.0) ||  
          (amp[2] < 0.0)) { 
        continue; 
      } 
 
      // Col of cluster and position in the pad groups 
      Int_t posc[3] = { 0, 0, 0 }; 
      if ((fCH2dOn)  && (fNnRphi[0] != 0)) { 
        posc[0] = (Int_t) col / fNnRphi[0]; 
      } 
      if ((fPH2dOn)  && (fNnRphi[1] != 0)) { 
        posc[1] = (Int_t) col / fNnRphi[1]; 
      } 
      if ((fPRF2dOn) && (fNnRphi[2] != 0)) { 
        posc[2] = (Int_t) col / fNnRphi[2]; 
      } 
 
      // See if we are not near a masked pad 
      Bool_t good = kTRUE; 
      if (!IsPadOn(idect,col,row)) { 
	fGoodTrack = kFALSE; 
	good       = kFALSE; 
      } 
 
      if (col >   0) { 
	if (!IsPadOn(idect,col-1,row)) { 
	  fGoodTrack = kFALSE; 
	  good       = kFALSE; 
	} 
      } 
       
      if (col < 143) { 
	if (!IsPadOn(idect,col+1,row)) { 
	  fGoodTrack = kFALSE; 
	  good       = kFALSE; 
	} 
      } 
 
      // Total spectrum 
      if (fPH2dOn) { 
        fPHPlace[time] = posc[1] * fNfragZ[1] + posr[1]; 
      } 
 
      if (!fTraMaxPad) { 
	if (fCH2dOn) { 
          fAmpTotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += (Float_t) (amp[0]+amp[1]+amp[2]); 
	} 
	if (fPH2dOn) { 
          fPHValue[time] = (Float_t) (amp[0]+amp[1]+amp[2]); 
	} 
      } 
      else { 
	if (fCH2dOn) { 
          fAmpTotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += (Float_t) amp[1]; 
	} 
	if (fPH2dOn) { 
          fPHValue[time] = amp[1]; 
	} 
      } 
             
      // Fill PRF direct 
      if (fPRF2dOn && good) { 
	if ((amp[0] > fThresholdClusterPRF2) &&  
            (amp[1] > fThresholdClusterPRF2) &&  
            (amp[2] > fThresholdClusterPRF2) &&  
            ((amp[0]+amp[1]+amp[2]) > 130.0)) { 
	  // Security of the denomiateur is 0 
	  if ((((Float_t) (((Float_t) amp[1]) * ((Float_t) amp[1])))  
             / ((Float_t) (((Float_t) amp[0]) * ((Float_t) amp[2])))) != 1.0) { 
	    Float_t xcenter = 0.5 * (TMath::Log(amp[2] / amp[0])) 
                                  / (TMath::Log((amp[1]*amp[1]) / (amp[0]*amp[2]))); 
	    Float_t ycenter = amp[1] / (amp[0] + amp[1] + amp[2]); 
	    if ((xcenter > -0.5) &&  
                (xcenter <  0.5) &&  
                (ycenter >  0.485)) { 
	      Float_t yminus = amp[0] / (amp[0]+amp[1]+amp[2]); 
	      Float_t ymax   = amp[2] / (amp[0]+amp[1]+amp[2]); 
	      // Fill only if it is in the drift region! 
	      if (((Float_t) time / fSf) > 0.3) { 
		if (fHisto2d) { 
		  fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),xcenter,ycenter); 
		  if (xcenter < 0.0) { 
                    fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),-(xcenter+1.0),yminus); 
		  } 
		  if (xcenter > 0.0) { 
                    fPRF2d->Fill((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]+0.5),(1.0-xcenter),ymax); 
		  } 
		} 
		if (fVector2d) { 
		  UpdateVectorPRF((fXbins[2]+posc[2]*fNfragZ[2]+posr[2]),xcenter,ycenter); 
		  if (xcenter < 0.0) { 
                    UpdateVectorPRF(fXbins[2]+posc[2]*fNfragZ[2]+posr[2],-(xcenter+1.0),yminus); 
		  } 
		  if (xcenter > 0.0) { 
                    UpdateVectorPRF(fXbins[2]+posc[2]*fNfragZ[2]+posr[2],(1.0-xcenter),ymax); 
		  } 
		} 
	      }  
	    } 
	  } 
	} 
      } 
       
    } // Boucle clusters 
     
    // Fill the charge 
    if (fCH2dOn && fGoodTrack) { 
      FillTheInfoOfTheTrackCH(); 
    } 
 
    // PH calibration 
    if (fPH2dOn && fGoodTrack) { 
      FillTheInfoOfTheTrackPH();	 
    } 
         
  } // Condition on number of clusters 
 
  return kTRUE; 
   
} 
 
//____________Functions for seeing if the pad is really okey___________________ 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::SetModeCalibrationFromTObject(TObject *object, Int_t i) 
{ 
  // 
  // Set fNz[i] and fNrphi[i] of the IlcDCHCalibra::Instance() 
  // corresponding to the given TObject 
  // 
 
  const char *nametitle = object->GetTitle(); 
 
  // Some patterns 
  const Char_t *patternz0    = "Nz0"; 
  const Char_t *patternz1    = "Nz1"; 
  const Char_t *patternz2    = "Nz2"; 
  const Char_t *patternz3    = "Nz3"; 
  const Char_t *patternz4    = "Nz4"; 
  const Char_t *patternrphi0 = "Nrphi0"; 
  const Char_t *patternrphi1 = "Nrphi1"; 
  const Char_t *patternrphi2 = "Nrphi2"; 
  const Char_t *patternrphi3 = "Nrphi3"; 
  const Char_t *patternrphi4 = "Nrphi4"; 
  const Char_t *patternrphi5 = "Nrphi5"; 
  const Char_t *patternrphi6 = "Nrphi6"; 
 
  UShort_t testz    = 0; 
  UShort_t testrphi = 0; 
 
  // Nz mode 
  if (strstr(nametitle,patternz0)) { 
    testz++; 
    fNz[i] = 0; 
  } 
  if (strstr(nametitle,patternz1)) { 
    testz++; 
    fNz[i] = 1; 
  } 
  if (strstr(nametitle,patternz2)) { 
    testz++; 
    fNz[i] = 2; 
  } 
  if (strstr(nametitle,patternz3)) { 
    testz++; 
    fNz[i] = 3; 
  } 
  if (strstr(nametitle,patternz4)) { 
    testz++; 
    fNz[i] = 4; 
  } 
 
  // Nrphi mode 
  if (strstr(nametitle,patternrphi0)) { 
    testrphi++; 
    fNrphi[i] = 0; 
  } 
  if (strstr(nametitle,patternrphi1)) { 
    testrphi++; 
    fNrphi[i] = 1; 
  } 
  if (strstr(nametitle,patternrphi2)) { 
    testrphi++; 
    fNrphi[i] = 2; 
  } 
  if (strstr(nametitle,patternrphi3)) { 
    testrphi++; 
    fNrphi[i] = 3; 
  } 
  if (strstr(nametitle,patternrphi4)) { 
    testrphi++; 
    fNrphi[i] = 4; 
  } 
  if (strstr(nametitle,patternrphi5)) { 
    testrphi++; 
    fNrphi[i] = 5; 
  } 
  if (strstr(nametitle,patternrphi6)) { 
    testrphi++; 
    fNrphi[i] = 6; 
  } 
  
  // Look if all is okey 
  if ((testz    == 1) &&  
      (testrphi == 1)) { 
    return kTRUE; 
  } 
  else { 
    fNrphi[i] = 0; 
    fNz[i]    = 0; 
    return kFALSE; 
  } 
   
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::IsPadOn(Int_t detector, Int_t col, Int_t row) const 
{ 
  // 
  // Look in the choosen database if the pad is On. 
  // If no the track will be "not good" 
  // 
 
  // Get the parameter object 
  IlcDCHcalibDB *cal = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
   
  Int_t npads  = 18; 
  Int_t colmcm = (Int_t) col / npads; 
   
  if (!cal->IsChamberInstalled(detector)     ||  
       cal->IsChamberMasked(detector)        ||  
       cal->IsMCMMasked(detector,colmcm,row) ||  
       cal->IsPadMasked(detector,col,row)) { 
    return kFALSE; 
  } 
  else { 
    return kTRUE; 
  } 
   
} 
 
//____________Functions for plotting the 2D____________________________________ 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::Plot2d() 
{ 
  // 
  // Plot the 2D histos  
  // 
  
  if (fCH2dOn) { 
    PlotCH2d(); 
  } 
  if (fPH2dOn) { 
    PlotPH2d(); 
  } 
  if (fPRF2dOn) { 
    PlotPRF2d(); 
  } 
 
} 
 
//____________Writing the 2D___________________________________________________ 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::Write2d() 
{ 
  // 
  // Write the 2D histograms or the vectors converted in trees in the file 
  // "DCH.calibration.root"  
  // 
   
  TFile *fout = TFile::Open(fWriteName,"RECREATE"); 
  // Check if the file could be opened 
  if (!fout || !fout->IsOpen()) { 
    IlcInfo("No File found!"); 
    return kFALSE; 
  } 
  IlcInfo(Form("Numbertrack: %d Numberusedch[0]: %d, Numberusedch[1]: %d Numberusedph[0]: %d, Numberusedph[1]: %d" 
              ,fNumberTrack 
              ,fNumberUsedCh[0] 
              ,fNumberUsedCh[1] 
              ,fNumberUsedPh[0] 
              ,fNumberUsedPh[1])); 
   
  TStopwatch stopwatch; 
  stopwatch.Start(); 
  IlcInfo("Write2d"); 
 
  if ((fCH2dOn ) && (fWrite[0])) { 
    if (fHisto2d) { 
      fout->WriteTObject(fCH2d); 
    } 
    if (fVector2d) { 
      TString name("Nz"); 
      name += fNz[0]; 
      name += "Nrphi"; 
      name += fNrphi[0]; 
      TTree *treeCH2d = ConvertVectorCTTreeHisto(fVectorCH,fPlaCH,"treeCH2d",(const char *) name); 
      fout->WriteTObject(treeCH2d); 
    } 
  } 
  if ((fPH2dOn ) && (fWrite[1])) { 
    if (fHisto2d) { 
      fout->WriteTObject(fPH2d); 
    } 
    if (fVector2d) { 
      TString name("Nz"); 
      name += fNz[1]; 
      name += "Nrphi"; 
      name += fNrphi[1]; 
      TTree *treePH2d = ConvertVectorPTreeHisto(fVectorPH,fPlaPH,"treePH2d",(const char *) name); 
      fout->WriteTObject(treePH2d); 
    } 
  } 
  if ((fPRF2dOn ) && (fWrite[2])) { 
    if (fHisto2d) { 
      fout->WriteTObject(fPRF2d); 
    } 
    if (fVector2d) { 
      TString name("Nz"); 
      name += fNz[2]; 
      name += "Nrphi"; 
      name += fNrphi[2]; 
      TTree *treePRF2d = ConvertVectorPTreeHisto(fVectorPRF,fPlaPRF,"treePRF2d",(const char *) name); 
      fout->WriteTObject(treePRF2d); 
    } 
  } 
   
  fout->Close(); 
   
  IlcInfo(Form("Execution time Write2d: R:%.2fs C:%.2fs" 
	      ,stopwatch.RealTime(),stopwatch.CpuTime())); 
 
  return kTRUE; 
   
} 
 
//_____________________________________________________________________________ 
IlcDCHCalDet *IlcDCHCalibra::CreateDetObjectTree(TTree *tree, Int_t i) 
{ 
  // 
  // It creates the IlcDCHCalDet object from the tree of the coefficient 
  // for the calibration i (i != 2) 
  // It takes the mean value of the coefficients per detector  
  // This object has to be written in the database 
  // 
   
  // Create the DetObject 
  IlcDCHCalDet *object = 0x0; 
  if (i == 0) { 
    object = new IlcDCHCalDet("ChamberGainFactor","GainFactor (detector value)"); 
  } 
  if (i == 1) { 
    object = new IlcDCHCalDet("ChamberVdrift","DCH drift velocities (detector value)"); 
  } 
  else { 
    object = new IlcDCHCalDet("ChamberT0","T0 (detector value)"); 
  } 
   
  // Read the Tree 
  Int_t   detector = -1; 
  Float_t values[2304]; 
  tree->SetBranchAddress("detector",&detector); 
  if (i == 0) { 
    tree->SetBranchAddress("gainPad",values); 
  } 
  if (i == 1) { 
    tree->SetBranchAddress("vdrift" ,values); 
  } 
  if (i == 3) { 
    tree->SetBranchAddress("t0"     ,values); 
  } 
   
  // For calculating the mean 
  Float_t mean            = 0.0; 
  Int_t   nto             = 0; 
  Int_t   numberofentries = tree->GetEntries(); 
   
  if (numberofentries != 540) { 
    IlcInfo("The tree is not complete"); 
  } 
   
  for (Int_t det = 0; det < numberofentries; ++det) { 
    tree->GetEntry(det); 
    if (GetChamber(detector) == 2) { 
      nto = 1728; 
    } 
    else { 
      nto = 2304; 
    } 
    mean = 0.0; 
    for (Int_t k = 0; k < nto; k++) { 
      mean += TMath::Abs(values[k]) / nto;   
    } 
    object->SetValue(detector,mean); 
  } 
 
  return object; 
 
} 
 
//_____________________________________________________________________________ 
TObject *IlcDCHCalibra::CreatePadObjectTree(TTree *tree, Int_t i 
                                          , IlcDCHCalDet *detobject) 
{ 
  // 
  // It Creates the IlcDCHCalPad object from the tree of the 
  // coefficient for the calibration i (i != 2) 
  // You need first to create the object for the detectors, 
  // where the mean value is put. 
  // This object has to be written in the database 
  // 
   
  // Create the DetObject 
  IlcDCHCalPad *object = 0x0; 
  if (i == 0) { 
    object = new IlcDCHCalPad("GainFactor","GainFactor (local variations)"); 
  } 
  if (i == 1) { 
    object = new IlcDCHCalPad("LocalVdrift","DCH drift velocities (local variations)"); 
  } 
  else { 
    object = new IlcDCHCalPad("LocalT0","T0 (local variations)"); 
  } 
   
  // Read the Tree 
  Int_t   detector = -1; 
  Float_t values[2304]; 
  tree->SetBranchAddress("detector",&detector); 
  if (i == 0) { 
    tree->SetBranchAddress("gainPad",values); 
  } 
  if (i == 1) { 
    tree->SetBranchAddress("vdrift" ,values); 
  } 
  if (i == 3) { 
    tree->SetBranchAddress("t0"     ,values); 
  } 
   
  // Variables 
  Float_t mean            = 0.0; 
  Int_t   numberofentries = tree->GetEntries(); 
   
  if (numberofentries != 540) { 
    IlcInfo("The tree is not complete"); 
  } 
   
  for (Int_t det = 0; det < numberofentries; ++det) { 
    tree->GetEntry(det); 
    IlcDCHCalROC *calROC = object->GetCalROC(detector); 
    mean = detobject->GetValue(detector); 
    if (mean == 0) { 
      continue; 
    } 
    Int_t rowMax = calROC->GetNrows(); 
    Int_t colMax = calROC->GetNcols(); 
    for (Int_t row = 0; row < rowMax; ++row) { 
      for (Int_t col = 0; col < colMax; ++col) { 
	if (i == 0) { 
          calROC->SetValue(col,row,TMath::Abs(values[(Int_t) (col*rowMax+row)])/mean); 
	} 
	if (i == 1) { 
          calROC->SetValue(col,row,TMath::Abs(values[(Int_t) (col*rowMax+row)])/mean); 
	} 
	if (i == 3) { 
	  if (values[(Int_t) (col*rowMax+row)]/mean < 0.0) { 
	    calROC->SetValue(col,row,0.0); 
	  } 
	  if (values[(Int_t) (col*rowMax+row)]/mean >= 0.0) { 
	    calROC->SetValue(col,row,values[(Int_t) (col*rowMax+row)]/mean); 
	  } 
	} 
      } // Col 
    } // Row 
  } 
 
  return object; 
 
} 
 
//_____________________________________________________________________________ 
TObject *IlcDCHCalibra::CreatePadObjectTree(TTree *tree) 
{ 
  // 
  // It Creates the IlcDCHCalPad object from the tree of the 
  // coefficient for the calibration PRF (i = 2) 
  // This object has to be written in the database 
  // 
   
  // Create the DetObject 
  IlcDCHCalPad *object = new IlcDCHCalPad("PRFWidth","PRFWidth"); 
 
  // Read the Tree 
  Int_t   detector = -1; 
  Float_t values[2304]; 
  tree->SetBranchAddress("detector",&detector); 
  tree->SetBranchAddress("width"   ,values); 
    
  // Variables 
  Int_t numberofentries = tree->GetEntries(); 
 
  if (numberofentries != 540) { 
    IlcInfo("The tree is not complete"); 
  } 
 
  for (Int_t det = 0; det < numberofentries; ++det) { 
    tree->GetEntry(det); 
    IlcDCHCalROC *calROC = object->GetCalROC(detector); 
    Int_t rowMax = calROC->GetNrows(); 
    Int_t colMax = calROC->GetNcols(); 
    for (Int_t row = 0; row < rowMax; ++row) { 
      for (Int_t col = 0; col < colMax; ++col) { 
	calROC->SetValue(col,row,TMath::Abs(values[(Int_t) (col*rowMax+row)])); 
      } // Col 
    } // Row 
  } 
 
  return object; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetRelativeScale(Float_t RelativeScale) 
{ 
  // 
  // Set the factor that will divide the deposited charge 
  // to fit in the histo range [0,300] 
  // 
  
  if (RelativeScale > 0.0) { 
    fRelativeScale = RelativeScale; 
  }  
  else { 
    IlcInfo("RelativeScale must be strict positif!"); 
  } 
 
}  
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetNz(Int_t i, Short_t Nz) 
{ 
  // 
  // Set the mode of calibration group in the z direction for the parameter i 
  //  
 
  if ((Nz >= 0) &&  
      (Nz <  5)) { 
    fNz[i] = Nz;  
  } 
  else {  
    IlcInfo("You have to choose between 0 and 4"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetNrphi(Int_t i, Short_t Nrphi) 
{ 
  // 
  // Set the mode of calibration group in the rphi direction for the parameter i 
  // 
  
  if ((Nrphi >= 0) &&  
      (Nrphi <  7)) { 
    fNrphi[i] = Nrphi;  
  } 
  else { 
    IlcInfo("You have to choose between 0 and 6"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetPeriodeFitPH(Int_t periodeFitPH) 
{  
  // 
  // Set FitPH if 1 then each detector will be fitted 
  // 
 
  if (periodeFitPH > 0) { 
    fFitPHPeriode   = periodeFitPH;  
  } 
  else { 
    IlcInfo("periodeFitPH must be higher than 0!"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetBeginFitCharge(Float_t beginFitCharge) 
{  
  // 
  // The fit of the deposited charge distribution begins at 
  // histo->Mean()/beginFitCharge 
  // You can here set beginFitCharge 
  // 
 
  if (beginFitCharge > 0) { 
    fBeginFitCharge = beginFitCharge;  
  } 
  else { 
    IlcInfo("beginFitCharge must be strict positif!"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetT0Shift(Float_t t0Shift)  
{  
  // 
  // The t0 calculated with the maximum positif slope is shift from t0Shift 
  // You can here set t0Shift 
  // 
 
  if (t0Shift > 0) { 
    fT0Shift = t0Shift;  
  }  
  else { 
    IlcInfo("t0Shift must be strict positif!"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetRangeFitPRF(Float_t rangeFitPRF) 
{  
  // 
  // The fit of the PRF is from -rangeFitPRF to rangeFitPRF 
  // You can here set rangeFitPRF 
  // 
 
  if ((rangeFitPRF >    0) &&  
      (rangeFitPRF <= 1.0)) { 
    fRangeFitPRF = rangeFitPRF; 
  }  
  else { 
    IlcInfo("rangeFitPRF must be between 0 and 1.0"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::SetRebin(Short_t rebin) 
{  
  // 
  // Rebin with rebin time less bins the Ch histo 
  // You can set here rebin that should divide the number of bins of CH histo 
  // 
 
  if (rebin > 0) { 
    fRebin = rebin;  
    IlcInfo("You have to be sure that fRebin divides fNumberBinCharge used!"); 
  }  
  else { 
    IlcInfo("You have to choose a positiv value!"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
TTree *IlcDCHCalibra::Sum2Trees(const Char_t *filename1 
                              , const Char_t *filename2 
                              , const Char_t *variablecali) 
{ 
  // 
  // It returns the sum of two trees with the name variablecali 
  // in the files filenam1 and filename2 equivalent of merging two 2D histos 
  // The name of the resulting tree is the same as the two input trees 
  // variablecali can be treeCH2d, treePH2d or treePRF2d  
  // 
 
  // Variables 
  TChain    *treeChain   = new TChain(variablecali); 
  TObjArray *vectorplace = new TObjArray(); 
  TObjArray *where       = new TObjArray(); 
   
  // First tree 
  // Take the tree 
  TFile *file1 = new TFile(filename1,"READ"); 
  TTree *tree1 = (TTree *) file1->Get(variablecali); 
 
  gDirectory = gROOT; 
 
  // Take the places 
  vectorplace = ConvertTreeVector(tree1); 
 
  // Say where it is in tree 1 
  for (Int_t jui = 0; jui < (Int_t) vectorplace->GetEntriesFast(); jui++) { 
    IlcDCHPlace *placejui = new IlcDCHPlace(); 
    placejui->SetPlace(jui); 
    TObjArray *chainplace = new TObjArray(); 
    chainplace->Add((TObject *) placejui); 
    where->Add((TObject *) chainplace); 
  } 
 
  // Add to the chain 
  treeChain->Add(filename1); 
  delete file1; 
 
  // Second tree 
  // Take the tree 
  TFile *file2 = new TFile(filename2,"READ"); 
  TTree *tree2 = (TTree *) file2->Get(variablecali); 
 
  gDirectory = gROOT; 
 
  // Take the places 
  TObjArray *vector2 = ConvertTreeVector(tree2); 
  Int_t j = treeChain->GetEntries(); 
 
  for (Int_t jui = 0; jui < (Int_t) vector2->GetEntriesFast(); jui++) { 
    // Search if already found 
    Int_t place = SearchInTreeVector(vectorplace,((IlcDCHPlace *) vector2->At(jui))->GetPlace()); 
    // Create a new element in the two std vectors 
    if (place == -1) { 
      IlcDCHPlace *placejjui  = new IlcDCHPlace(); 
      placejjui->SetPlace((j+jui)); 
      TObjArray   *chainplace = new TObjArray(); 
      chainplace->Add((TObject *) placejjui); 
      vectorplace->Add((TObject *) (vector2->At(jui))); 
      where->Add((TObject *) chainplace); 
    } 
    // Update the element at the place "place" in the std vector whereinthechain 
    else { 
      IlcDCHPlace *placejjui  = new IlcDCHPlace(); 
      placejjui->SetPlace((j+jui)); 
      TObjArray   *chainplace = ((TObjArray *) where->At(place)); 
      chainplace->Add((TObject *) placejjui); 
      where->AddAt((TObject *) chainplace,place); 
    } 
  } 
 
  // Add to the Chain 
  treeChain->Add(filename2); 
  delete file2;  
 
  // Take care of the profile 
  const Char_t *pattern = "P"; 
  TTree *tree = 0x0; 
 
  if (!strstr(variablecali,pattern)) { 
 
    // Ready to read the chain 
    TH1F *his = 0x0; 
    treeChain->SetBranchAddress("histo",&his); 
 
    // Initialise the final tree 
    Int_t group   = -1; 
    TH1F *histsum = 0x0; 
    
    tree = new TTree(variablecali,variablecali); 
    tree->Branch("groupnumber",&group,"groupnumber/I"); 
    tree->Branch("histo","TH1F",&histsum,32000,0); 
 
    // Init histsum 
    if (treeChain->GetEntries() < 1) { 
      return tree1;  
    } 
     
    for (Int_t h = 0; h < (Int_t) vectorplace->GetEntriesFast(); h++) { 
      group = ((IlcDCHPlace *) vectorplace->At(h))->GetPlace(); 
      TObjArray *chainplace = ((TObjArray *) where->At(h)); 
      treeChain->GetEntry(((IlcDCHPlace *) chainplace->At(0))->GetPlace()); 
      //Init for the first time 
      if (h == 0)  { 
	histsum = new TH1F("","",his->GetXaxis()->GetNbins() 
                                ,his->GetXaxis()->GetBinLowEdge(1) 
                                ,his->GetXaxis()->GetBinUpEdge(his->GetXaxis()->GetNbins())); 
	histsum->Sumw2(); 
      } 
      // Reset for each new group 
      histsum->SetEntries(0.0); 
      for (Int_t l = 0; l <= histsum->GetXaxis()->GetNbins(); l++) { 
	histsum->SetBinContent(l,0.0); 
	histsum->SetBinError(l,0.0); 
      } 
      histsum->Add(his,1); 
      if ((Int_t) chainplace->GetEntriesFast() > 1) { 
	for (Int_t s = 1; s < (Int_t) chainplace->GetEntriesFast(); s++) { 
	  treeChain->GetEntry(((IlcDCHPlace *) chainplace->At(s))->GetPlace()); 
	  histsum->Add(his,1); 
	} 
      } 
      tree->Fill(); 
    } 
 
  } 
  else { 
 
    // Ready to read the chain 
    TGraphErrors *his = 0x0; 
    treeChain->SetBranchAddress("histo",&his); 
     
    // Initialise the final tree 
    Int_t         group   = -1; 
    TGraphErrors *histsum = 0x0; 
    Double_t     *xref    = 0x0; 
   
    tree = new TTree(variablecali,variablecali); 
    tree->Branch("groupnumber",&group,"groupnumber/I"); 
    tree->Branch("histo","TGraphErrors",&histsum,32000,0); 
 
    // Init histsum 
    if (treeChain->GetEntries() < 1) { 
      return tree1;  
    } 
 
    for (Int_t h = 0; h < (Int_t) vectorplace->GetEntriesFast(); h++) { 
 
      group = ((IlcDCHPlace *) vectorplace->At(h))->GetPlace(); 
      TObjArray *chainplace = ((TObjArray *) where->At(h)); 
      treeChain->GetEntry(((IlcDCHPlace *) chainplace->At(0))->GetPlace()); 
      //Init or reset for a new group 
      Int_t nbins = his->GetN(); 
      Double_t *x; 
      x    = new Double_t[nbins]; 
      xref = his->GetX(); 
      Double_t *ex; 
      ex   = new Double_t[nbins]; 
      Double_t *y; 
      y    = new Double_t[nbins]; 
      Double_t *ey; 
      ey   = new Double_t[nbins]; 
      
      for (Int_t lo = 0; lo < nbins; lo++) { 
	x[lo]  = xref[lo]; 
	ex[lo] = 0.0; 
	y[lo]  = 0.0; 
	ey[lo] = 0.0; 
      } 
      delete histsum; 
      histsum = new TGraphErrors(nbins,x,y,ex,ey); 
 
      // Add the first 
      histsum = AddProfiles(his,histsum); 
      if ((Int_t) chainplace->GetEntriesFast() > 1) { 
	for (Int_t s = 1; s < (Int_t) chainplace->GetEntriesFast(); s++) { 
	  treeChain->GetEntry(((IlcDCHPlace *) chainplace->At(s))->GetPlace()); 
	  histsum = AddProfiles(his,histsum); 
	} 
      } 
 
      tree->Fill(); 
 
    } 
 
  } 
     
  return tree; 
 
} 
 
//____________Function fill 2D for the moment out of the code__________________ 
 
//____________Function fill 2D all objects from digits_________________________ 
Bool_t IlcDCHCalibra::Create2DDiSimOnline(Int_t iev1, Int_t iev2) 
{ 
  // 
  // Only for simulations, after the simulation, create the 2D histos 
  // from the digits stored in the file "DCH.Digits.root"  
  // Only for CH and PH 
  // 
   
  const Int_t kNplan = 6; 
  const Int_t kNcham = 5; 
 
  // RunLoader and so on 
  if (gIlc) { 
    delete gIlc->GetRunLoader(); 
    delete gIlc; 
    gIlc = 0; 
  } 
  
  IlcRunLoader *rl = IlcRunLoader::Open("gilc.root"); 
  if (!rl) { 
    return kFALSE; 
  } 
 
  rl->LoadgIlc(); 
  gIlc = rl->GetIlcRun(); 
  if (!gIlc) { 
    return kFALSE; 
  } 
 
  // Import the Trees for the event nEvent in the file 
  rl->LoadKinematics(); 
  rl->GetEvent(0); 
  rl->LoadHeader(); 
   
  IlcLoader *loader = rl->GetLoader("DCHLoader"); 
  if (!loader) { 
    IlcInfo("No DCHLLoader found!"); 
    return kFALSE; 
  } 
 
  // Get the pointer to the DCH detector  
  IlcDCH *trd = (IlcDCH *) gIlc->GetDetector("DCH"); 
  if (!trd) { 
    IlcInfo("No DCH detector found"); 
    return kFALSE; 
  } 
 
  // Get the pointer to the geometry object 
  IlcDCHgeometry *geo; 
  if (trd) { 
    geo = trd->GetGeometry(); 
  } 
  else { 
    IlcInfo("No DCH geometry found"); 
    return kFALSE; 
  } 
 
  // DB Setting 
  IlcCDBManager *man = IlcCDBManager::Instance(); 
  if (!man) { 
    IlcInfo("Could not get CDB Manager"); 
    return kFALSE; 
  } 
  
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam"); 
    return kFALSE; 
  } 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
 
  // Some parameters 
  fTimeMax = cal->GetNumberOfTimeBins(); 
  fSf      = (Float_t) cal->GetSamplingFrequency(); 
  if (fRelativeScaleAuto) { 
    fRelativeScale = 0; 
  } 
  else { 
    if (fRelativeScale <= 0.0) { 
      IlcInfo("You have to set the relativescale factor per hand!"); 
      return kFALSE; 
    } 
  } 
 
  // Create the 2D histos corresponding to the pad group calibration mode 
  if (fCH2dOn) { 
 
    IlcInfo(Form("We will fill the CH2d histo with the pad calibration mode: Nz %d, and Nrphi %d" 
                ,fNz[0] 
                ,fNrphi[0])); 
     
    // Calcul the number of Xbins 
    fNtotal[0]    = 0; 
    ModePadCalibration(2,0); 
    ModePadFragmentation(0,2,0,0); 
    fDetChamb2[0] = fNfragZ[0] * fNfragRphi[0]; 
    fNtotal[0]   += 6 * 18 * fDetChamb2[0]; 
    ModePadCalibration(0,0); 
    ModePadFragmentation(0,0,0,0); 
    fDetChamb0[0] = fNfragZ[0] * fNfragRphi[0]; 
    fNtotal[0]   += 6 * 4 * 18 * fDetChamb0[0]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[0])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreateCH2d(fNtotal[0]); 
    } 
    if (fVector2d) { 
      fVectorCH = new TObjArray(); 
      fPlaCH    = new TObjArray(); 
    } 
     
  } 
   
  if (fPH2dOn) { 
 
    IlcInfo(Form("We will fill the PH2d histo with the pad calibration mode: Nz %d, and Nrphi %d" 
                ,fNz[1] 
                ,fNrphi[1])); 
     
    // Calcul the number of Xbins 
    fNtotal[1]    = 0; 
    ModePadCalibration(2,1); 
    ModePadFragmentation(0,2,0,1); 
    fDetChamb2[1] = fNfragZ[1] * fNfragRphi[1]; 
    fNtotal[1]   += 6 * 18 * fDetChamb2[1]; 
    ModePadCalibration(0,1); 
    ModePadFragmentation(0,0,0,1); 
    fDetChamb0[1] = fNfragZ[1] * fNfragRphi[1]; 
    fNtotal[1]   += 6 * 4 * 18 * fDetChamb0[1]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[1])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreatePH2d(fNtotal[1]); 
    } 
    if (fVector2d) { 
      fVectorPH = new TObjArray(); 
      fPlaPH    = new TObjArray(); 
    } 
 
  } 
 
  loader->LoadDigits(); 
  IlcInfo("LoadDigits "); 
  IlcDCHdigitsManager *digitsManager = new IlcDCHdigitsManager(); 
 
  //iev2 egal to the max if 0 
  if (iev2 == 0) { 
    iev2 = rl->GetNumberOfEvents(); 
    IlcInfo(Form("Total number of events: %d",iev2)); 
  } 
 
  // Loop on event 
  for (Int_t ievent = iev1; ievent < iev2; ievent++) { 
    IlcInfo(Form("Process event %d",ievent)); 
    rl->GetEvent(ievent); 
    if (!loader->TreeD()) { 
      IlcInfo("loader Loading Digits ... "); 
      loader->LoadDigits(); 
    } 
    digitsManager->ReadDigits(loader->TreeD()); 
    IlcInfo("digitsManager Read Digits Done"); 
    // Read the digits from the file 
    if (!(digitsManager->ReadDigits(loader->TreeD()))) { 
      return kFALSE; 
    } 
 
    // Loop on detector 
    for (Int_t iSect = 0; iSect < 18; iSect++) { 
      for (Int_t iChamb = 0; iChamb < kNcham; iChamb++) { 
	for (Int_t iPlane = 0; iPlane < kNplan; iPlane++) { 
	   
	  // A little geometry: 
	  Int_t iDet   = geo->GetDetector(iPlane,iChamb,iSect); 
	  Int_t rowMax = parCom->GetRowMax(iPlane,iChamb,iSect); 
	  Int_t colMax = parCom->GetColMax(iPlane); 
 
	  // Variables for the group 
	  LocalisationDetectorXbins(iDet); 
 
	  // In the cas of charge 
	  Float_t *amptotal; 
	  amptotal = new Float_t[fNfragRphi[0]*fNfragZ[0]]; 
	  if (fCH2dOn) { 
	    for (Int_t k = 0; k < fNfragRphi[0]*fNfragZ[0]; k++) { 
	      amptotal[k] = 0.0; 
	    } 
	  } 
 
	  // Loop through the detector pixel 
	  for (Int_t time = 0; time < fTimeMax; time++) { 
	    for (Int_t  col = 0;  col <  colMax;  col++) { 
	      for (Int_t  row = 0;  row <  rowMax;  row++) { 
 
		// Amplitude and position in pad group  
		IlcDCHdigit *digit   = digitsManager->GetDigit(row,col,time,iDet); 
		Int_t        amp     = digit->GetAmp(); 
		Int_t        posr[2] = {0,0}; 
		Int_t        posc[2] = {0,0}; 
		if ((fCH2dOn) &&  
                    (fNnZ[0]    != 0)) { 
                  posr[0] = (Int_t) row / fNnZ[0]; 
		} 
		if ((fCH2dOn) &&  
                    (fNnRphi[0] != 0)) { 
                  posc[0] = (Int_t) col / fNnRphi[0]; 
		} 
		if ((fPH2dOn) &&  
                    (fNnZ[1]    != 0)) { 
                  posr[1] = (Int_t) row / fNnZ[1]; 
		} 
		if ((fPH2dOn) &&  
                    (fNnRphi[1] != 0)) { 
                  posc[1] = (Int_t) col / fNnRphi[1]; 
		} 
 
		// Total spectrum 
		if (fCH2dOn) { 
		  if (amp < fThresholdDigit) { 
                    amp = 0; 
		  } 
		  amptotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += amp; 
		} 
		if (fPH2dOn) { 
		  if (fHisto2d) { 
		    fPH2d->Fill((fXbins[1]+posc[1]*fNfragZ[1]+posr[1])+0.5,(Float_t) time/fSf,(Double_t) amp); 
		  } 
		  if (fVector2d) { 
		    UpdateVectorPH((fXbins[1]+posc[1]*fNfragZ[1]+posr[1]),time,(Double_t) amp); 
		  } 
		} 
	      
		// Memory stuff 
		delete digit; 
 
	      } // Boucle row 
	    } // Boucle col 
	  } // Boucle time 
 
	  if (fCH2dOn) { 
 
	    // If automatic scale 
	    if ((fCountRelativeScale < 100) && (fRelativeScaleAuto)) { 
	      // Take only the one zone track 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if ((fCountRelativeScale < 100) && (amptotal[k] > 2.0)) { 
		  fRelativeScale += amptotal[k]*0.014*0.01; 
		  fCountRelativeScale++; 
		} 
	      } 
	    } 
	     
	    // We fill the CH2d after having scale with the first 100 
	    if ((fCountRelativeScale >= 100) && (fRelativeScaleAuto)) { 
	      // Case of 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if (fHisto2d &&  
                    (amptotal[k] > 0.0)) { 
		  fCH2d->Fill(fXbins[0]+k+0.5,amptotal[k]/fRelativeScale); 
		} 
		if (fVector2d &&  
                    (amptotal[k] > 0.0)) { 
		  UpdateVectorCH(fXbins[0]+k ,amptotal[k]/fRelativeScale); 
		} 
	      } 
	    } 
 
	    // No relative salce 
	    if (!fRelativeScaleAuto) { 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if (fHisto2d &&  
                    (amptotal[k] > 0.0)) { 
                  fCH2d->Fill(fXbins[0]+k+0.5, amptotal[k]/fRelativeScale);  
		} 
		if (fVector2d &&  
                    (amptotal[k] > 0.0)) { 
                  UpdateVectorCH(fXbins[0]+k, amptotal[k]/fRelativeScale); 
		} 
	      } 
	    } 
 
	  } 
 
	  delete amptotal;	   
	   
	} // Boucle chamber 
      } // Boucle plane 
    } // Boucle sect 
     
    loader->UnloadDigits();   
     
  } // Boucle event 
   
  if (fDebug == 1) { 
    if (fPH2dOn && fHisto2d) { 
      PlotPH2d(); 
    } 
    if (fCH2dOn && fHisto2d) { 
      PlotCH2d(); 
    }   
  } 
   
  if (fWrite[0] || fWrite[1]) { 
 
    TFile *fout = TFile::Open(fWriteName,"RECREATE"); 
    // Check if the file could be opened 
    if (!fout || !fout->IsOpen()) { 
      IlcInfo("<No File found!"); 
      return kFALSE; 
    } 
 
    if (fCH2dOn && fHisto2d && fWrite[0]) { 
      fout->WriteTObject(fCH2d); 
    } 
    if (fPH2dOn && fHisto2d && fWrite[1]) { 
      fout->WriteTObject(fPH2d); 
    } 
 
    if (fVector2d && fCH2dOn && fWrite[0]) { 
      TString name("Nz"); 
      name += fNz[0]; 
      name += "Nrphi"; 
      name += fNrphi[0]; 
      TTree *treeCH2d = ConvertVectorCTTreeHisto(fVectorCH,fPlaCH,"treeCH2d",(const char *) name); 
      fout->WriteTObject(treeCH2d); 
    } 
 
    if (fVector2d && fPH2dOn && fWrite[1]) { 
      TString name("Nz"); 
      name += fNz[1]; 
      name += "Nrphi"; 
      name += fNrphi[1]; 
      TTree *treePH2d = ConvertVectorPTreeHisto(fVectorPH,fPlaPH,"treePH2d",(const char *) name); 
      fout->WriteTObject(treePH2d); 
    } 
 
    fout->Close(); 
 
  } 
  
  return kTRUE; 
 
} 
 
//____________Function fill 2D all objects from Raw Data_______________________ 
Bool_t IlcDCHCalibra::Create2DRaDaOnline(Int_t iev1, Int_t iev2) 
{ 
  // 
  // After having written the RAW DATA in the current directory, create the 
  // 2D histos from these RAW DATA   
  // Only for CH and PH 
  // 
   
  const Int_t kNplan = 6; 
  const Int_t kNcham = 5; 
  TString dirname("."); 
   
  // DB Setting 
  IlcCDBManager *man = IlcCDBManager::Instance(); 
  if (!man) { 
    IlcInfo("Could not get CDB Manager"); 
    return kFALSE; 
  } 
 
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam"); 
    return kFALSE; 
  } 
 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
 
  // Some parameters 
  fTimeMax = cal->GetNumberOfTimeBins(); 
  fSf      = (Float_t) cal->GetSamplingFrequency(); 
  if (fRelativeScaleAuto) { 
    fRelativeScale = 0; 
  } 
  else { 
    if (fRelativeScale <= 0.0) { 
      IlcInfo("You have to set the relativescale factor per hand!"); 
      return kFALSE; 
    } 
  } 
 
  // Create the 2D histo corresponding to the pad group calibration mode 
  if (fCH2dOn) { 
 
    IlcInfo(Form("We will fill the CH2d histo with the pad calibration mode: Nz %d, and Nrphi %d" 
                ,fNz[0] 
                ,fNrphi[0])); 
     
    // Calcul the number of Xbins 
    fNtotal[0]    = 0; 
    ModePadCalibration(2,0); 
    ModePadFragmentation(0,2,0,0); 
    fDetChamb2[0] = fNfragZ[0] * fNfragRphi[0]; 
    fNtotal[0]   += 6 * 18 * fDetChamb2[0]; 
    ModePadCalibration(0,0); 
    ModePadFragmentation(0,0,0,0); 
    fDetChamb0[0] = fNfragZ[0] * fNfragRphi[0]; 
    fNtotal[0]   += 6 * 4 * 18 * fDetChamb0[0]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[0])); 
     
    // Create the 2D histo 
    if (fHisto2d) { 
      CreateCH2d(fNtotal[0]); 
    } 
    if (fVector2d) { 
      fVectorCH = new TObjArray(); 
      fPlaCH    = new TObjArray(); 
    } 
 
  } 
   
  if(fPH2dOn) { 
 
    IlcInfo(Form("We will fill the PH2d histo with the pad calibration mode: Nz %d, and Nrphi %d" 
                ,fNz[1] 
                ,fNrphi[1])); 
     
    // Calcul the number of Xbins 
    fNtotal[1]    = 0; 
    ModePadCalibration(2,1); 
    ModePadFragmentation(0,2,0,1); 
    fDetChamb2[1] = fNfragZ[1] * fNfragRphi[1]; 
    fNtotal[1]   += 6 * 18 * fDetChamb2[1]; 
    ModePadCalibration(0,1); 
    ModePadFragmentation(0,0,0,1); 
    fDetChamb0[1] = fNfragZ[1] * fNfragRphi[1]; 
    fNtotal[1]   += 6 * 4 * 18 * fDetChamb0[1]; 
    IlcInfo(Form("Total number of Xbins: %d",fNtotal[1])); 
 
    // Create the 2D histo 
    if (fHisto2d) { 
      CreatePH2d(fNtotal[1]); 
    } 
    if (fVector2d){ 
      fVectorPH = new TObjArray(); 
      fPlaPH    = new TObjArray(); 
    } 
 
  } 
    
  IlcDCHrawData *rawdata = new IlcDCHrawData(); 
  IlcInfo("IlcDCHrawData object created "); 
   
  // Loop on events 
  for (Int_t ievent = iev1; ievent < iev2; ievent++) { 
    
    // IlcRawReaderFile 
    IlcRawReaderFile *readerfile = new IlcRawReaderFile(dirname,ievent); 
    if (!readerfile) { 
      IlcInfo("No readerfile found!"); 
      return kFALSE; 
    } 
  
    IlcDCHdigitsManager *digitsManager = rawdata->Raw2Digits((IlcRawReader *) readerfile); 
    if (!digitsManager) { 
      IlcInfo("No DigitsManager done!"); 
      return kFALSE; 
    } 
  
    // Loop on detectors 
    for (Int_t iSect = 0; iSect < 18; iSect++) { 
      for (Int_t iPlane = 0; iPlane < kNplan; iPlane++) { 
	for (Int_t iChamb = 0; iChamb < kNcham; iChamb++) { 
	   
	  // A little geometry: 
	  Int_t iDet   = IlcDCHgeometry::GetDetector(iPlane,iChamb,iSect); 
	  Int_t rowMax = parCom->GetRowMax(iPlane,iChamb,iSect); 
	  Int_t colMax = parCom->GetColMax(iPlane); 
 
	  // Variables for the group 
	  LocalisationDetectorXbins(iDet); 
	 
	  // In the cas of charge 
	  Float_t *amptotal; 
	  amptotal = new Float_t[fNfragRphi[0]*fNfragZ[0]]; 
	  if(fCH2dOn) { 
	    for (Int_t k = 0; k < fNfragRphi[0]*fNfragZ[0]; k++) { 
	      amptotal[k] = 0.0; 
	    } 
	  } 
 
	  // Loop through the detector pixel 
	  for (Int_t time = 0; time < fTimeMax; time++) { 
	    for (Int_t  col = 0;  col <  colMax;  col++) { 
	      for (Int_t  row = 0;  row <  rowMax;  row++) { 
 
		// Amplitude and position of the digit 
		IlcDCHdigit *digit   = digitsManager->GetDigit(row,col,time,iDet); 
		Int_t        amp     = digit->GetAmp(); 
		Int_t        posr[2] = { 0, 0 }; 
		Int_t        posc[2] = { 0, 0 }; 
		if ((fCH2dOn) &&  
                    (fNnZ[0]    != 0)) { 
                  posr[0] = (Int_t) row / fNnZ[0]; 
		} 
		if ((fCH2dOn) &&  
                    (fNnRphi[0] != 0)) { 
                  posc[0] = (Int_t) col / fNnRphi[0]; 
		} 
		if ((fPH2dOn) &&  
                    (fNnZ[1]    != 0)) { 
                  posr[1] = (Int_t) row / fNnZ[1]; 
		} 
		if ((fPH2dOn) &&  
                    (fNnRphi[1] != 0)) { 
                  posc[1] = (Int_t) col / fNnRphi[1]; 
		} 
		 
		// Total spectrum 
		if (fCH2dOn) { 
		  if (amp < fThresholdDigit) { 
                    amp = 0; 
		  } 
		  amptotal[(Int_t) (posc[0]*fNfragZ[0]+posr[0])] += amp; 
		} 
 
		if (fPH2dOn ) { 
		  if (fHisto2d) { 
                    fPH2d->Fill((fXbins[1]+posc[1]*fNfragZ[1]+posr[1])+0.5,(Float_t)time/fSf,amp); 
		  } 
		  if (fVector2d) { 
                    UpdateVectorPH(fXbins[1]+posc[1]*fNfragZ[1]+posr[1],time,amp); 
		  } 
		} 
 
		delete digit; 
 
	      } // Boucle row 
	    } // Boucle col 
	  } // Boucle time 
 
	  if (fCH2dOn) { 
 
	    // If automatic scale 
	    if ((fCountRelativeScale < 100) && (fRelativeScaleAuto)) { 
	      // Take only the one zone track 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if ((fCountRelativeScale < 100) && (amptotal[k] > 2.0)) { 
		  fRelativeScale += amptotal[k] * 0.014 * 0.01; 
		  fCountRelativeScale++; 
		} 
	      } 
	    } 
	     
	    // We fill the CH2d after having scale with the first 100 
	    if ((fCountRelativeScale >= 100) && (fRelativeScaleAuto)) { 
	      // Case of 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if (fHisto2d  && (amptotal[k] > 0.0)) { 
		  fCH2d->Fill(fXbins[0]+k+0.5,amptotal[k]/fRelativeScale); 
		} 
		if (fVector2d && (amptotal[k] > 0.0)) { 
		  UpdateVectorCH(fXbins[0]+k, amptotal[k]/fRelativeScale); 
		} 
	      } 
	    } 
 
	    // No relative salce 
	    if (!fRelativeScaleAuto) { 
	      for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
		if (fHisto2d  &&  
                    (amptotal[k] > 0.0)) { 
                  fCH2d->Fill(fXbins[0]+k+0.5,amptotal[k]/fRelativeScale);  
		} 
		if (fVector2d &&  
                    (amptotal[k] > 0.0)) { 
                  UpdateVectorCH(fXbins[0]+k, amptotal[k]/fRelativeScale); 
		} 
	      } 
	    } 
 
	  } 
	  
	  delete amptotal; 
	   
	} // Boucle chamber 
      } // Boucle plane 
    } // Boucle sect 
   
    delete digitsManager; 
    delete readerfile; 
 
  } // Boucle event 
   
  if (fDebug == 1) { 
    if (fPH2dOn && fHisto2d) { 
      PlotPH2d(); 
    } 
    if (fCH2dOn && fHisto2d) { 
      PlotCH2d(); 
    } 
  } 
 
  if (fWrite[0] || fWrite[1]) { 
     
    TFile *fout = TFile::Open(fWriteName,"UPDATE"); 
    // Check if the file could be opened 
    if (!fout || !fout->IsOpen()) { 
      IlcInfo("<No File found!"); 
      return kFALSE; 
    } 
 
    if (fCH2dOn && fHisto2d && fWrite[0]) { 
      fout->WriteTObject(fCH2d); 
    } 
    if (fPH2dOn && fHisto2d && fWrite[1]) { 
      fout->WriteTObject(fPH2d); 
    } 
 
    if (fVector2d && fCH2dOn && fWrite[0]) { 
      TString name("Nz"); 
      name += fNz[0]; 
      name += "Nrphi"; 
      name += fNrphi[0]; 
      TTree *treeCH2d = ConvertVectorCTTreeHisto(fVectorCH,fPlaCH,"treeCH2d",(const Char_t *) name); 
      fout->WriteTObject(treeCH2d); 
    } 
 
    if (fVector2d && fPH2dOn && fWrite[1]) { 
      TString name("Nz"); 
      name += fNz[1]; 
      name += "Nrphi"; 
      name += fNrphi[1]; 
      TTree *treePH2d = ConvertVectorPTreeHisto(fVectorPH,fPlaPH,"treePH2d",(const Char_t *) name); 
      fout->WriteTObject(treePH2d); 
    } 
   
  } 
   
  return kTRUE; 
 
} 
 
//____________Pad Calibration Public___________________________________________ 
 
//____________Define the number of pads per group for one detector and one calibration 
void IlcDCHCalibra::ModePadCalibration(Int_t iChamb, Int_t i) 
{ 
  // 
  // Definition of the calibration mode 
  // from Nz and Nrphi, the number of row and col pads per calibration groups are setted 
  // 
 
 
  fNnZ[i]    = 0; 
  fNnRphi[i] = 0; 
   
  if ((fNz[i] == 0) && (iChamb == 2)) { 
    fNnZ[i] = 12; 
  } 
  if ((fNz[i] == 0) && (iChamb != 2)) { 
    fNnZ[i] = 16; 
  }   
  if ((fNz[i] == 1) && (iChamb == 2)) { 
    fNnZ[i] = 6; 
  } 
  if ((fNz[i] == 1) && (iChamb != 2)) { 
    fNnZ[i] = 8; 
  } 
  if ((fNz[i] == 2) && (iChamb == 2)) { 
    fNnZ[i] = 3; 
  } 
  if ((fNz[i] == 2) && (iChamb != 2)) { 
    fNnZ[i] = 4; 
  } 
  if (fNz[i] == 3) { 
    fNnZ[i] = 2; 
  } 
  if (fNz[i] == 4) { 
    fNnZ[i] = 1; 
  } 
    
  if (fNrphi[i] == 0) { 
    fNnRphi[i] = 144; 
  } 
  if (fNrphi[i] == 1) { 
    fNnRphi[i] = 72; 
  }  
  if (fNrphi[i] == 2) { 
    fNnRphi[i] = 36; 
  }  
  if (fNrphi[i] == 3) { 
    fNnRphi[i] = 18; 
  }  
  if (fNrphi[i] == 4) { 
    fNnRphi[i] = 9; 
  }  
  if (fNrphi[i] == 5) { 
    fNnRphi[i] = 4; 
  }  
  if (fNrphi[i] == 6) { 
    fNnRphi[i] = 1; 
  }  
 
} 
 
//____________Define the number of pad groups in one detector for one calibration 
Bool_t IlcDCHCalibra::ModePadFragmentation(Int_t iPlane,Int_t iChamb, Int_t iSect, Int_t i) 
{ 
  // 
  // Definition of the calibration mode 
  // From the number of row and col pads per calibration groups the 
  // number of calibration groups are setted 
  // 
 
  fNfragZ[i]    = 0; 
  fNfragRphi[i] = 0; 
   
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam Manager"); 
    return kFALSE; 
  } 
 
  // A little geometry: 
  Int_t rowMax = parCom->GetRowMax(iPlane,iChamb,iSect); 
  Int_t colMax = parCom->GetColMax(iPlane); 
   
  // The fragmentation 
  if (fNnZ[i]    != 0) { 
    fNfragZ[i]    = (Int_t) rowMax / fNnZ[i]; 
  } 
 
  if (fNnRphi[i] != 0) { 
    fNfragRphi[i] = (Int_t) colMax / fNnRphi[i]; 
  } 
 
  return kTRUE; 
 
} 
 
//____________Protected Functions______________________________________________ 
//____________Create the 2D histo to be filled online__________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreatePRF2d(Int_t nn) 
{ 
  // 
  // Create the 2D histos 
  // 
 
  TString name("Nz"); 
  name += fNz[2]; 
  name += "Nrphi"; 
  name += fNrphi[2]; 
 
  fPRF2d = new TProfile2D("PRF2d",(const Char_t *) name 
                                 ,nn,0,nn,fNumberBinPRF,-1.0,1.0); 
  fPRF2d->SetXTitle("Det/pad groups"); 
  fPRF2d->SetYTitle("Position x/W [pad width units]"); 
  fPRF2d->SetZTitle("Q_{i}/Q_{total}"); 
  fPRF2d->SetStats(0); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreatePH2d(Int_t nn) 
{ 
  // 
  // Create the 2D histos 
  // 
 
  TString name("Nz"); 
  name += fNz[1]; 
  name += "Nrphi"; 
  name += fNrphi[1]; 
 
  fPH2d = new TProfile2D("PH2d",(const Char_t *) name 
                               ,nn,0,nn,fTimeMax 
                               ,-0.5/fSf,(Float_t) (fTimeMax-0.5)/fSf); 
  fPH2d->SetXTitle("Det/pad groups"); 
  fPH2d->SetYTitle("time [#mus]"); 
  fPH2d->SetZTitle("<PH> [a.u.]"); 
  fPH2d->SetStats(0); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateCH2d(Int_t nn) 
{ 
  // 
  // Create the 2D histos 
  // 
 
  TString name("Nz"); 
  name += fNz[0]; 
  name += "Nrphi"; 
  name += fNrphi[0]; 
 
  fCH2d = new TH2I("CH2d",(const Char_t *) name 
                         ,nn,0,nn,fNumberBinCharge,0,300); 
  fCH2d->SetXTitle("Det/pad groups"); 
  fCH2d->SetYTitle("charge deposit [a.u]"); 
  fCH2d->SetZTitle("counts"); 
  fCH2d->SetStats(0); 
  fCH2d->Sumw2(); 
 
} 
 
//____________Offine tracking in the IlcDCHtracker_____________________________ 
void IlcDCHCalibra::FillTheInfoOfTheTrackCH() 
{ 
  // 
  // For the offline tracking or mcm tracklets 
  // This function will be called in the functions UpdateHistogram...  
  // to fill the info of a track for the relativ gain calibration 
  // 
	 
  Int_t nb =  0; // Nombre de zones traversees 
  Int_t fd = -1; // Premiere zone non nulle 
   
   
  // See if the track goes through different zones 
  for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
    if (fAmpTotal[k] > 0.0) { 
      nb++; 
      if (nb == 1) { 
        fd = k; 
      } 
    } 
  } 
  
  // If automatic scale 
  if ((fCountRelativeScale < 100) && (fRelativeScaleAuto)) { 
    // Take only the one zone track 
    if (nb == 1) { 
      fRelativeScale += fAmpTotal[fd] * 0.014 * 0.01; 
      fCountRelativeScale++; 
    } 
  } 
 
  // We fill the CH2d after having scale with the first 100 
  if ((fCountRelativeScale >= 100) && (fRelativeScaleAuto)) { 
    // Case of track with only one zone 
    if (nb == 1) { 
      if (fHisto2d) { 
        fCH2d->Fill(fXbins[0]+fd+0.5,fAmpTotal[fd]/fRelativeScale); 
      } 
      if (fVector2d) { 
        UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd]/fRelativeScale); 
      } 
    } // Case 1 zone 
    // Case of track with two zones 
    if (nb == 2) { 
      // Two zones voisines sinon rien! 
      if ((fAmpTotal[fd]   > 0.0) &&  
          (fAmpTotal[fd+1] > 0.0)) { 
	// One of the two very big 
	if (fAmpTotal[fd] > fProcent*fAmpTotal[fd+1]) { 
	  if (fHisto2d) { 
            fCH2d->Fill(fXbins[0]+fd+0.5,fAmpTotal[fd]/fRelativeScale); 
	  } 
	  if (fVector2d) { 
            UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd]/fRelativeScale); 
	  } 
	} 
	if (fAmpTotal[fd+1] > fProcent*fAmpTotal[fd])  { 
	  if (fHisto2d) { 
            fCH2d->Fill(fXbins[0]+fd+1.5,fAmpTotal[fd+1]/fRelativeScale); 
	  } 
	  if (fVector2d) { 
            UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd+1]/fRelativeScale); 
	  } 
	} 
      } 
    } // Case 2 zones 
  } 
 
  // Fill with no automatic scale 
  if (!fRelativeScaleAuto) { 
    // Case of track with only one zone 
    if (nb == 1) { 
      fNumberUsedCh[0]++; 
      if (fHisto2d) { 
        fCH2d->Fill(fXbins[0]+fd+0.5,fAmpTotal[fd]/fRelativeScale); 
      } 
      if (fVector2d) { 
        UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd]/fRelativeScale); 
      } 
    } // Case 1 zone 
    // Case of track with two zones 
    if (nb == 2) { 
      // Two zones voisines sinon rien! 
      // Case 1 
      if ((fAmpTotal[fd]   > 0.0) &&  
          (fAmpTotal[fd+1] > 0.0)) { 
	// One of the two very big 
	if (fAmpTotal[fd] > fProcent*fAmpTotal[fd+1]) { 
	  if (fHisto2d) { 
            fCH2d->Fill(fXbins[0]+fd+0.5,fAmpTotal[fd]/fRelativeScale); 
	  } 
	  if (fVector2d) { 
            UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd]/fRelativeScale); 
	  } 
	  fNumberUsedCh[1]++; 
	} 
	if (fAmpTotal[fd+1] > fProcent*fAmpTotal[fd]) { 
	  if (fHisto2d) { 
            fCH2d->Fill(fXbins[0]+fd+1.5,fAmpTotal[fd+1]/fRelativeScale); 
	  } 
	  if (fVector2d) { 
            UpdateVectorCH(fXbins[0]+fd+1,fAmpTotal[fd+1]/fRelativeScale); 
	  } 
	  fNumberUsedCh[1]++; 
	} 
      } 
      // Case 2 
      if (fNfragZ[0] > 1) { 
	if (fAmpTotal[fd] > 0.0) { 
	  if ((fd+fNfragZ[0]) < (fNfragZ[0]*fNfragRphi[0])) { 
	    if (fAmpTotal[fd+fNfragZ[0]] > 0.0) { 
	      // One of the two very big 
	      if (fAmpTotal[fd] > fProcent*fAmpTotal[fd+fNfragZ[0]]) { 
		if (fHisto2d) { 
                  fCH2d->Fill(fXbins[0]+fd+0.5,fAmpTotal[fd]/fRelativeScale); 
		} 
		if (fVector2d) { 
                  UpdateVectorCH(fXbins[0]+fd,fAmpTotal[fd]/fRelativeScale); 
		} 
		fNumberUsedCh[1]++; 
	      } 
	      if (fAmpTotal[fd+fNfragZ[0]] > fProcent*fAmpTotal[fd]) { 
		if (fHisto2d) { 
                  fCH2d->Fill(fXbins[0]+fd+fNfragZ[0]+0.5,fAmpTotal[fd+fNfragZ[0]]/fRelativeScale); 
		} 
		fNumberUsedCh[1]++; 
		if (fVector2d) { 
                  UpdateVectorCH(fXbins[0]+fd+fNfragZ[0],fAmpTotal[fd+fNfragZ[0]]/fRelativeScale); 
		} 
	      } 
	    } 
	  } 
	} 
      } 
    } // Case 2 zones 
 
  } 
 
} 
 
//____________Offine tracking in the IlcDCHtracker_____________________________ 
void IlcDCHCalibra::ResetfVariables() 
{ 
  // 
  // Reset values of fAmpTotal, fPHValue and fPHPlace for 
  // the updateHistogram... functions 
  // 
 
  // Reset the good track 
  fGoodTrack = kTRUE; 
   
  // Reset the fAmpTotal where we put value 
  if (fCH2dOn) { 
    for (Int_t k = 0; k < fNfragZ[0]*fNfragRphi[0]; k++) { 
      fAmpTotal[k] = 0.0; 
    } 
  } 
   
  // Reset the fPHValue 
  if (fPH2dOn) { 
    for (Int_t k = 0; k < fTimeMax; k++) { 
      fPHValue[k] = -1.0; 
      fPHPlace[k] = -1; 
    } 
  } 
 
} 
 
//____________Offine tracking in the IlcDCHtracker_____________________________ 
void IlcDCHCalibra::FillTheInfoOfTheTrackPH() 
{ 
  // 
  // For the offline tracking or mcm tracklets 
  // This function will be called in the functions UpdateHistogram...  
  // to fill the info of a track for the drift velocity  calibration 
  // 
     
  Int_t nb  =  1; // Nombre de zones traversees 1, 2 ou plus de 3 
  Int_t fd1 = -1; // Premiere zone non nulle 
  Int_t fd2 = -1; // Deuxieme zone non nulle 
  Int_t k1  = -1; // Debut de la premiere zone 
  Int_t k2  = -1; // Debut de la seconde zone 
 
  // See if the track goes through different zones 
  for (Int_t k = 0; k < fTimeMax; k++) { 
    if (fPHValue[k] > 0.0) { 
      if (fd1 == -1) { 
	fd1 = fPHPlace[k]; 
	k1  = k;	       
      } 
      if (fPHPlace[k] != fd1) { 
	if (fd2 == -1) { 
	  k2  = k; 
	  fd2 = fPHPlace[k]; 
	  nb  = 2; 
	} 
	if (fPHPlace[k] != fd2) { 
          nb = 3; 
	} 
      } 
    } 
  } 
   
  // Fill  
  // Case of track with only one zone 
  if (nb == 1) { 
    fNumberUsedPh[0]++; 
    for (Int_t i = 0; i < fTimeMax; i++) { 
      if (fPHValue[i] > 0.0) { 
	if (fHisto2d) { 
          fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
	} 
	if (fDebug == 13) { 
	  IlcInfo(Form("WRITE nb %d ,place final: %d, fPHPlace[i]: %d, i: %d, fPHValue[i]: %f" 
                      ,nb,fXbins[1]+fPHPlace[i],fPHPlace[i],i,fPHValue[i])); 
	} 
	if (fVector2d) { 
          UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
	} 
      } 
    } 
  } // Case 1 zone 
  // Case of track with two zones 
  if (nb == 2) { 
    // Two zones voisines sinon rien! 
    // Case 1 
    if ((fd1 == fd2+1) ||  
        (fd2 == fd1+1)) { 
      // One of the two fast all the think 
      if (k2 > (k1+fDifference)) { 
	fNumberUsedPh[1]++; 
	for (Int_t i = k1; i < k2; i++) { 
	  if (fPHValue[i] > 0.0) { 
	    if (fHisto2d) { 
              fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
	    } 
	    if (fVector2d) { 
              UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
	    } 
	  } 
	} 
      } 
      if ((k2+fDifference) < fTimeMax) { 
	fNumberUsedPh[1]++; 
	for (Int_t i = k2; i < fTimeMax; i++) { 
	  if (fPHValue[i] > 0.0) { 
	    if (fHisto2d) { 
              fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
	    } 
	    if (fVector2d) { 
              UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
	    } 
	  } 
	} 
      } 
    } 
    // Two zones voisines sinon rien! 
    if (fNfragZ[1] > 1) { 
      // Case 2 
      if ((fd1+fNfragZ[1]) < (fNfragZ[1]*fNfragRphi[1])) { 
	if (fd2 == (fd1+fNfragZ[1])) { 
	  // One of the two fast all the think 
	  if (k2 > (k1+fDifference)) { 
	    fNumberUsedPh[1]++; 
	    for (Int_t i = k1; i < k2; i++) { 
	      if (fPHValue[i] > 0.0) { 
		if (fHisto2d) { 
                  fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
		} 
		if (fVector2d) { 
                  UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
		} 
	      } 
	    } 
	  } 
	  if ((k2+fDifference) < fTimeMax) { 
	    fNumberUsedPh[1]++; 
	    for (Int_t i = k2; i < fTimeMax; i++) { 
	      if (fPHValue[i] > 0.0) { 
		if (fHisto2d) { 
                  fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
		} 
		if (fVector2d) { 
                  UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
		} 
	      } 
	    } 
	  } 
	} 
      } 
      // Two zones voisines sinon rien! 
      // Case 3 
      if ((fd1 - fNfragZ[1]) >= 0) { 
	if (fd2 == (fd1 - fNfragZ[1])) { 
	  // One of the two fast all the think 
	  if (k2 > (k1 + fDifference)) { 
	    fNumberUsedPh[1]++; 
	    for (Int_t i = k1; i < k2; i++) { 
	      if (fPHValue[i] > 0.0) { 
		if (fHisto2d) { 
                  fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
		} 
		if (fVector2d) { 
                  UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
		} 
	      } 
	    } 
	  } 
	  if ((k2+fDifference) < fTimeMax) { 
	    fNumberUsedPh[1]++; 
	    for (Int_t i = k2; i < fTimeMax; i++) { 
	      if (fPHValue[i] > 0.0) { 
		if (fHisto2d) { 
                  fPH2d->Fill((fXbins[1]+fPHPlace[i])+0.5,(Float_t) i/fSf,(Float_t) fPHValue[i]); 
		} 
		if (fVector2d) { 
                  UpdateVectorPH(fXbins[1]+fPHPlace[i],i,fPHValue[i]); 
		} 
	      } 
	    } 
	  } 
	} 
      } 
    } 
 
  } // case 2 zones 
 
} 
 
//____________Set the pad calibration variables for the detector_______________ 
Bool_t IlcDCHCalibra::LocalisationDetectorXbins(Int_t detector) 
{ 
  // 
  // For the detector calcul the first Xbins and set the number of row 
  // and col pads per calibration groups, the number of calibration 
  // groups in the detector. 
  // 
   
  // first Xbins of the detector 
  if (fCH2dOn) { 
    CalculXBins(detector,0); 
  } 
  if (fPH2dOn) { 
    CalculXBins(detector,1); 
  } 
  if (fPRF2dOn) { 
    CalculXBins(detector,2); 
  } 
 
  // fragmentation of idect 
  for (Int_t i = 0; i < 3; i++) { 
    ModePadCalibration((Int_t) GetChamber(detector),i); 
    ModePadFragmentation((Int_t) GetPlane(detector) 
                       , (Int_t) GetChamber(detector) 
                       , (Int_t) GetSector(detector),i); 
  } 
   
  return kTRUE; 
 
} 
 
//____________Plot the 2D histos filled Online_________________________________ 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPH2d() 
{ 
  // 
  // Plot the 2D histo  
  // 
 
  TCanvas *cph2d = new TCanvas("cph2d","",50,50,600,800); 
  cph2d->cd(); 
  fPH2d->Draw("LEGO"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotCH2d() 
{ 
  // 
  // Plot the 2D histos 
  // 
 
  TCanvas *cch2d = new TCanvas("cch2d","",50,50,600,800); 
  cch2d->cd(); 
  fCH2d->Draw("LEGO"); 
   
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPRF2d() 
{ 
  // 
  // Plot the 2D histos 
  // 
 
  TCanvas *cPRF2d = new TCanvas("cPRF2d","",50,50,600,800); 
  cPRF2d->cd(); 
  fPRF2d->Draw("LEGO"); 
       
} 
 
//____________Fit______________________________________________________________ 
 
//____________Create histos if fDebug == 1 or fDebug >= 3______________________ 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoPH(Int_t nbins, Double_t low, Double_t high) 
{ 
  // 
  // Create the histos for fDebug = 1 and fDebug = 4 (Fit functions) 
  // 
 
  // Histograms to store the coef 
  fCoefVdrift[0]  = new TH1F("coefvdrift0" ,"",nbins,low ,high); 
  fCoefVdrift[1]  = new TH1F("coefvdrift1" ,"",nbins,low ,high); 
  fCoefVdrift[2]  = new TH1F("coefvdrift2" ,"",nbins,low ,high); 
 
  // Histograms for Debug  
  fDeltaVdrift[0] = new TH1F("deltavdrift0","",nbins,low ,high); 
  fDeltaVdrift[1] = new TH1F("deltavdrift1","",nbins,low ,high); 
  fErrorVdrift[0] = new TH1I("errorvdrift0","",300  ,-0.5,0.5); 
  fErrorVdrift[1] = new TH1I("errorvdrift1","",300  ,-0.5,0.5); 
 
  fCoefVdrift[0]->SetXTitle("Det/pad groups"); 
  fCoefVdrift[0]->SetYTitle("Vdrift [cm/#mus]"); 
  fCoefVdrift[1]->SetXTitle("Det/pad groups"); 
  fCoefVdrift[1]->SetYTitle("Vdrift [cm/#mus]"); 
  fCoefVdrift[2]->SetXTitle("Det/pad groups"); 
  fCoefVdrift[2]->SetYTitle("Vdrift [cm/#mus]"); 
 
  fDeltaVdrift[0]->SetXTitle("Det/pad groups"); 
  fDeltaVdrift[0]->SetYTitle("#Deltav/v_{sim}"); 
  fDeltaVdrift[1]->SetXTitle("Det/pad groups"); 
  fDeltaVdrift[1]->SetYTitle("#Deltav/v_{sim}"); 
 
  fErrorVdrift[0]->SetXTitle("#Deltav/v_{sim}"); 
  fErrorVdrift[0]->SetYTitle("counts"); 
  fErrorVdrift[1]->SetXTitle("#Deltav/v_{sim}"); 
  fErrorVdrift[1]->SetYTitle("counts"); 
 
  fCoefVdrift[0]->SetStats(0); 
  fCoefVdrift[1]->SetStats(0); 
  fCoefVdrift[2]->SetStats(0); 
  fDeltaVdrift[0]->SetStats(0); 
  fDeltaVdrift[1]->SetStats(0); 
  fErrorVdrift[0]->SetStats(0); 
  fErrorVdrift[1]->SetStats(0); 
  
  fCoefVdrift[0]->SetMarkerColor(6); 
  fCoefVdrift[0]->SetMarkerStyle(26); 
  fCoefVdrift[0]->SetLineColor(6); 
  fCoefVdrift[1]->SetMarkerColor(2); 
  fCoefVdrift[1]->SetMarkerStyle(24); 
  fCoefVdrift[1]->SetLineColor(2); 
  fCoefVdrift[2]->SetLineColor(4); 
   
  fDeltaVdrift[1]->SetMarkerColor(2); 
  fDeltaVdrift[1]->SetMarkerStyle(24); 
  fDeltaVdrift[1]->SetLineColor(2); 
  fDeltaVdrift[0]->SetMarkerColor(6); 
  fDeltaVdrift[0]->SetMarkerStyle(26); 
  fDeltaVdrift[0]->SetLineColor(6);  
 
  fErrorVdrift[1]->SetLineColor(2); 
  fErrorVdrift[1]->SetLineStyle(2); 
  fErrorVdrift[0]->SetLineColor(6); 
  fErrorVdrift[0]->SetLineStyle(1); 
   
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoT0(Int_t nbins, Double_t low, Double_t high) 
{ 
  // 
  // Create the histos for fDebug = 1 and fDebug = 4 (Fit functions) 
  // 
 
  // Histograms to store the coef 
  fCoefT0[0]  = new TH1F("coefT00" ,"",nbins,low ,high); 
  fCoefT0[1]  = new TH1F("coefT01" ,"",nbins,low ,high); 
  fCoefT0[2]  = new TH1F("coefT02" ,"",nbins,low ,high); 
 
  // Histograms for Debug 
  fDeltaT0[0] = new TH1F("deltaT00","",nbins,low ,high); 
  fDeltaT0[1] = new TH1F("deltaT01","",nbins,low ,high); 
  fErrorT0[0] = new TH1I("errorT00","",300  ,-0.1,0.1); 
  fErrorT0[1] = new TH1I("errorT01","",300  ,-0.1,0.1); 
 
  fCoefT0[0]->SetXTitle("Det/pad groups"); 
  fCoefT0[0]->SetYTitle("t0 [timebin]"); 
  fCoefT0[1]->SetXTitle("Det/pad groups"); 
  fCoefT0[1]->SetYTitle("t0 [timebin]"); 
  fCoefT0[2]->SetXTitle("Det/pad groups"); 
  fCoefT0[2]->SetYTitle("t0 [timebin]"); 
 
  fDeltaT0[0]->SetXTitle("Det/pad groups"); 
  fDeltaT0[0]->SetYTitle("#Deltat0 [timebin]"); 
  fDeltaT0[1]->SetXTitle("Det/pad groups"); 
  fDeltaT0[1]->SetYTitle("#Deltat0 [timebin]"); 
 
  fErrorT0[0]->SetXTitle("#Deltat0 [timebin]"); 
  fErrorT0[0]->SetYTitle("counts"); 
  fErrorT0[1]->SetXTitle("#Deltat0 [timebin]"); 
  fErrorT0[1]->SetYTitle("counts"); 
 
  fCoefT0[0]->SetStats(0); 
  fCoefT0[1]->SetStats(0); 
  fCoefT0[2]->SetStats(0); 
  fDeltaT0[0]->SetStats(0); 
  fDeltaT0[1]->SetStats(0); 
  fErrorT0[0]->SetStats(0); 
  fErrorT0[1]->SetStats(0); 
  
  fCoefT0[0]->SetMarkerColor(6); 
  fCoefT0[0]->SetMarkerStyle(26); 
  fCoefT0[0]->SetLineColor(6); 
  fCoefT0[1]->SetMarkerColor(2); 
  fCoefT0[1]->SetMarkerStyle(24); 
  fCoefT0[1]->SetLineColor(2); 
  fCoefT0[2]->SetLineColor(4); 
   
  fDeltaT0[1]->SetMarkerColor(2); 
  fDeltaT0[1]->SetMarkerStyle(24); 
  fDeltaT0[1]->SetLineColor(2); 
  fDeltaT0[0]->SetMarkerColor(6); 
  fDeltaT0[0]->SetMarkerStyle(26); 
  fDeltaT0[0]->SetLineColor(6);  
 
  fErrorT0[1]->SetLineColor(2); 
  fErrorT0[1]->SetLineStyle(2); 
  fErrorT0[0]->SetLineColor(6); 
  fErrorT0[0]->SetLineStyle(1); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoCH(Int_t nbins, Double_t low, Double_t high) 
{ 
  // 
  // Create the histos for fDebug = 1 and fDebug = 4 (Fit functions) 
  // 
 
  // Histograms to store the coef 
  fCoefCharge[0]  = new TH1F("coefcharge0" ,"",nbins,low ,high); 
  fCoefCharge[1]  = new TH1F("coefcharge1" ,"",nbins,low ,high); 
  fCoefCharge[2]  = new TH1F("coefcharge2" ,"",nbins,low ,high); 
  fCoefCharge[3]  = new TH1F("coefcharge3" ,"",nbins,low ,high); 
 
  // Histograms for Debug  
  fDeltaCharge[0] = new TH1F("deltacharge0","",nbins,low ,high); 
  fDeltaCharge[1] = new TH1F("deltacharge1","",nbins,low ,high); 
  fDeltaCharge[2] = new TH1F("deltacharge2","",nbins,low ,high); 
  
  fErrorCharge[0] = new TH1I("errorcharge0","",100  ,-0.5,0.5); 
  fErrorCharge[1] = new TH1I("errorcharge1","",100  ,-0.5,0.5); 
  fErrorCharge[2] = new TH1I("errorcharge2","",100  ,-0.5,0.5); 
 
  fCoefCharge[0]->SetXTitle("Det/Pad groups"); 
  fCoefCharge[0]->SetYTitle("gain factor"); 
  fCoefCharge[1]->SetXTitle("Det/Pad groups"); 
  fCoefCharge[1]->SetYTitle("gain factor"); 
  fCoefCharge[2]->SetXTitle("Det/Pad groups"); 
  fCoefCharge[2]->SetYTitle("gain factor"); 
  fCoefCharge[3]->SetXTitle("Det/Pad groups"); 
  fCoefCharge[3]->SetYTitle("gain factor"); 
 
  fDeltaCharge[0]->SetXTitle("Det/Pad groups"); 
  fDeltaCharge[0]->SetYTitle("#Deltag/g_{sim}"); 
  fDeltaCharge[1]->SetXTitle("Det/Pad groups"); 
  fDeltaCharge[1]->SetYTitle("#Deltag/g_{sim}"); 
  fDeltaCharge[2]->SetXTitle("Det/Pad groups"); 
  fDeltaCharge[2]->SetYTitle("#Deltag/g_{sim}"); 
  fDeltaCharge[0]->SetAxisRange(-0.5,0.5,"Y"); 
  fDeltaCharge[1]->SetAxisRange(-0.5,0.5,"Y"); 
  fDeltaCharge[2]->SetAxisRange(-0.5,0.5,"Y"); 
 
  fErrorCharge[0]->SetXTitle("#Deltag/g_{sim}"); 
  fErrorCharge[0]->SetYTitle("counts");  
  fErrorCharge[1]->SetXTitle("#Deltag/g_{sim}"); 
  fErrorCharge[1]->SetYTitle("counts");  
  fErrorCharge[2]->SetXTitle("#Deltag/g_{sim}"); 
  fErrorCharge[2]->SetYTitle("counts");  
 
  fDeltaCharge[1]->SetMarkerColor(2); 
  fDeltaCharge[1]->SetMarkerStyle(24); 
  fDeltaCharge[1]->SetLineColor(2); 
  fErrorCharge[1]->SetLineColor(2); 
  fErrorCharge[1]->SetLineStyle(2); 
  fDeltaCharge[2]->SetMarkerColor(8); 
  fDeltaCharge[2]->SetLineColor(8); 
  fDeltaCharge[2]->SetMarkerStyle(9); 
  fErrorCharge[2]->SetLineColor(8); 
  fErrorCharge[2]->SetLineStyle(5); 
  fDeltaCharge[0]->SetMarkerColor(6); 
  fDeltaCharge[0]->SetLineColor(6); 
  fDeltaCharge[0]->SetMarkerStyle(26); 
  fErrorCharge[0]->SetLineColor(6); 
  fErrorCharge[0]->SetLineStyle(1); 
 
  fCoefCharge[3]->SetLineColor(4); 
  fCoefCharge[1]->SetMarkerColor(2); 
  fCoefCharge[1]->SetLineColor(2); 
  fCoefCharge[1]->SetMarkerStyle(24); 
  fCoefCharge[2]->SetMarkerColor(8); 
  fCoefCharge[2]->SetLineColor(8); 
  fCoefCharge[2]->SetMarkerStyle(9); 
  fCoefCharge[0]->SetMarkerColor(6); 
  fCoefCharge[0]->SetLineColor(6); 
  fCoefCharge[0]->SetMarkerStyle(26); 
  
  fErrorCharge[2]->SetLineWidth(3); 
 
  fDeltaCharge[1]->SetStats(0); 
  fDeltaCharge[2]->SetStats(0); 
  fDeltaCharge[0]->SetStats(0); 
  fErrorCharge[1]->SetStats(0); 
  fErrorCharge[2]->SetStats(0); 
  fErrorCharge[0]->SetStats(0); 
  fCoefCharge[1]->SetStats(0); 
  fCoefCharge[0]->SetStats(0); 
  fCoefCharge[3]->SetStats(0);  
  fCoefCharge[2]->SetStats(0); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoPRF(Int_t nbins, Double_t low, Double_t high) 
{ 
  // 
  // Create the histos for fDebug = 1 and fDebug = 4 (Fit functions) 
  // 
 
  // Histograms to store the coef 
  fCoefPRF[0] = new TH1F("coefPRF0","",nbins,low ,high); 
  fCoefPRF[1] = new TH1F("coefPRF1","",nbins,low ,high); 
  
  // Histograms for Debug  
  fDeltaPRF   = new TH1F("deltaPRF","",nbins,low ,high); 
  fErrorPRF   = new TH1I("errorPRF","",300  ,-0.5,0.5); 
 
  fDeltaPRF->SetMarkerColor(6); 
  fDeltaPRF->SetMarkerStyle(26); 
  fDeltaPRF->SetLineColor(6); 
  fErrorPRF->SetLineColor(6); 
  fErrorPRF->SetLineStyle(2); 
 
  fCoefPRF[1]->SetLineColor(4); 
  fCoefPRF[0]->SetMarkerColor(6); 
  fCoefPRF[0]->SetMarkerStyle(26); 
  fCoefPRF[0]->SetLineColor(6); 
 
  fCoefPRF[0]->SetXTitle("Det/Pad groups"); 
  fCoefPRF[0]->SetYTitle("#sigma_{PRF}");  
  fCoefPRF[1]->SetXTitle("Det/Pad groups"); 
  fCoefPRF[1]->SetYTitle("#sigma_{PRF}");  
 
  fDeltaPRF->SetXTitle("Det/Pad groups"); 
  fDeltaPRF->SetYTitle("#Delta#sigma/#sigma_{sim}"); 
 
  fErrorPRF->SetXTitle("#Delta#sigma/#sigma_{sim}"); 
  fErrorPRF->SetYTitle("counts");   
  
  fDeltaPRF->SetStats(0); 
  fErrorPRF->SetStats(0); 
  fCoefPRF[1]->SetStats(0); 
  fCoefPRF[0]->SetStats(0); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoPRFDB(Int_t rowMax, Int_t colMax) 
{ 
  // 
  // Create the histos for fDebug = 3 and fDebug = 4 (Fit functions) 
  // 
 
  fCoefPRFDB = new TH2F("coefPRF","",rowMax,0,rowMax,colMax,0,colMax); 
 
  fCoefPRFDB->SetStats(0); 
  fCoefPRFDB->SetXTitle("row Number"); 
  fCoefPRFDB->SetYTitle("col Number"); 
  fCoefPRFDB->SetZTitle("PRF width [pad width units]"); 
 
  fCoefPRFDB->SetFillColor(6); 
  fCoefPRFDB->SetLineColor(6); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoCHDB(Int_t rowMax, Int_t colMax) 
{ 
  // 
  // Create the histos for fDebug = 3 and fDebug = 4 (Fit functions) 
  // 
 
  fCoefChargeDB[0] = new TH2F("coefchargedb0","",rowMax,0,rowMax,colMax,0,colMax); 
  fCoefChargeDB[1] = new TH2F("coefchargedb1","",rowMax,0,rowMax,colMax,0,colMax); 
  fCoefChargeDB[2] = new TH2F("coefchargedb2","",rowMax,0,rowMax,colMax,0,colMax); 
 
  fCoefChargeDB[0]->SetStats(0); 
  fCoefChargeDB[1]->SetStats(0); 
  fCoefChargeDB[2]->SetStats(0); 
  fCoefChargeDB[0]->SetXTitle("row Number"); 
  fCoefChargeDB[0]->SetYTitle("col Number"); 
  fCoefChargeDB[1]->SetXTitle("row Number"); 
  fCoefChargeDB[1]->SetYTitle("col Number"); 
  fCoefChargeDB[2]->SetXTitle("row Number"); 
  fCoefChargeDB[2]->SetYTitle("col Number"); 
  fCoefChargeDB[0]->SetZTitle("f_{g} Fit method"); 
  fCoefChargeDB[1]->SetZTitle("f_{g} Mean method"); 
  fCoefChargeDB[2]->SetZTitle("f_{g} Fitbis method"); 
 
  fCoefChargeDB[0]->SetFillColor(6); 
  fCoefChargeDB[0]->SetLineColor(6); 
  fCoefChargeDB[0]->SetLineColor(6); 
  fCoefChargeDB[1]->SetFillColor(2); 
  fCoefChargeDB[1]->SetLineColor(2); 
  fCoefChargeDB[1]->SetLineColor(2); 
  fCoefChargeDB[2]->SetFillColor(8); 
  fCoefChargeDB[2]->SetLineColor(8); 
  fCoefChargeDB[2]->SetLineColor(8); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoPHDB(Int_t rowMax, Int_t colMax) 
{ 
  // 
  // Create the histos for fDebug = 3 and fDebug = 4 (Fit functions) 
  // 
 
  fCoefVdriftDB[0] = new TH2F("coefvdriftdb0","",rowMax,0,rowMax,colMax,0,colMax); 
  fCoefVdriftDB[1] = new TH2F("coefvdriftdb1","",rowMax,0,rowMax,colMax,0,colMax); 
   
  fCoefVdriftDB[0]->SetStats(0); 
  fCoefVdriftDB[1]->SetStats(0); 
  fCoefVdriftDB[0]->SetXTitle("row Number"); 
  fCoefVdriftDB[0]->SetYTitle("col Number"); 
  fCoefVdriftDB[1]->SetXTitle("row Number"); 
  fCoefVdriftDB[1]->SetYTitle("col Number"); 
  fCoefVdriftDB[0]->SetZTitle("v_{drift} Fit method"); 
  fCoefVdriftDB[1]->SetZTitle("v_{drift} slope method"); 
   
  fCoefVdriftDB[0]->SetFillColor(6); 
  fCoefVdriftDB[0]->SetLineColor(6); 
  fCoefVdriftDB[0]->SetLineColor(6); 
  fCoefVdriftDB[1]->SetFillColor(2); 
  fCoefVdriftDB[1]->SetLineColor(2); 
  fCoefVdriftDB[1]->SetLineColor(2); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CreateFitHistoT0DB(Int_t rowMax, Int_t colMax) 
{ 
  // 
  // Create the histos for fDebug = 3 and fDebug = 4 (Fit functions) 
  // 
 
  fCoefT0DB[0] = new TH2F("coefT0db0","",rowMax,0,rowMax,colMax,0,colMax); 
  fCoefT0DB[1] = new TH2F("coefT0db1","",rowMax,0,rowMax,colMax,0,colMax); 
   
  fCoefT0DB[0]->SetStats(0); 
  fCoefT0DB[1]->SetStats(0); 
  fCoefT0DB[0]->SetXTitle("row Number"); 
  fCoefT0DB[0]->SetYTitle("col Number"); 
  fCoefT0DB[1]->SetXTitle("row Number"); 
  fCoefT0DB[1]->SetYTitle("col Number"); 
  fCoefT0DB[0]->SetZTitle("t0 Fit method"); 
  fCoefT0DB[1]->SetZTitle("t0 slope method"); 
   
  fCoefT0DB[0]->SetFillColor(6); 
  fCoefT0DB[0]->SetLineColor(6); 
  fCoefT0DB[0]->SetLineColor(6); 
  fCoefT0DB[1]->SetFillColor(2); 
  fCoefT0DB[1]->SetLineColor(2); 
  fCoefT0DB[1]->SetLineColor(2); 
   
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::FillVectorFitCH(Int_t countdet) 
{ 
  // 
  // For the Fit functions fill the vector FitCH special for the gain calibration 
  // 
 
  IlcDCHFitCHInfo *fitCHInfo = new IlcDCHFitCHInfo(); 
 
  Int_t ntotal = 1; 
  if (GetChamber(countdet) == 2) { 
    ntotal = 1728; 
  } 
  else { 
    ntotal = 2304; 
  } 
 
  Float_t *coef = new Float_t[ntotal]; 
  for (Int_t i = 0; i < ntotal; i++) { 
    coef[i] = fCoefCH[i]; 
  } 
 
  Int_t detector = countdet; 
  // Set 
  fitCHInfo->SetCoef(coef); 
  fitCHInfo->SetDetector(detector); 
  fVectorFitCH->Add((TObject *) fitCHInfo); 
 
  return kTRUE; 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
Bool_t IlcDCHCalibra::InitFit(Int_t nbins, Double_t lowedge 
                            , Double_t upedge, Int_t i) 
{ 
  // 
  // Init the calibration mode (Nz, Nrphi), the histograms for 
  // debugging the fit methods if fDebug > 0,  
  // 
 
  gStyle->SetPalette(1); 
  gStyle->SetOptStat(1111); 
  gStyle->SetPadBorderMode(0); 
  gStyle->SetCanvasColor(10); 
  gStyle->SetPadLeftMargin(0.13); 
  gStyle->SetPadRightMargin(0.01); 
 
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam"); 
    return kFALSE; 
  } 
 
  // Mode groups of pads: the total number of bins! 
  Int_t numberofbinsexpected = 0; 
  ModePadCalibration(2,i); 
  ModePadFragmentation(0,2,0,i); 
  fDetChamb2[i] = fNfragZ[i] * fNfragRphi[i]; 
  if (fDebug == 1) { 
    IlcInfo(Form("For the chamber 2: %d",fDetChamb2[i])); 
  } 
  numberofbinsexpected += 6 * 18 * fDetChamb2[i]; 
  ModePadCalibration(0,i); 
  ModePadFragmentation(0,0,0,i); 
  fDetChamb0[i] = fNfragZ[i] * fNfragRphi[i]; 
  if (fDebug == 1) { 
    IlcInfo(Form("For the other chamber 0: %d",fDetChamb0[i])); 
  } 
  numberofbinsexpected += 6 * 4 * 18 * fDetChamb0[i]; 
   
  // Quick verification that we have the good pad calibration mode if 2D histos! 
  if (nbins != 0) { 
    if (numberofbinsexpected != nbins) { 
      IlcInfo("It doesn't correspond to the mode of pad group calibration!"); 
      return kFALSE; 
    } 
  } 
 
  // Security for fDebug 3 and 4 
  if ((fDebug >= 3) &&  
      ((fDet[0] >  5) ||  
       (fDet[1] >  4) ||  
       (fDet[2] > 17))) { 
    IlcInfo("This detector doesn't exit!"); 
    return kFALSE; 
  } 
 
  // Determine fDet1 and fDet2 
  fDect1[i] = -1; 
  fDect2[i] = -1; 
  if (fDebug == 2) { 
    fDect1[i] = fFitVoir; 
    fDect2[i] = fDect1[i] +1; 
  } 
  if (fDebug <= 1) { 
    fDect1[i] = 0; 
    fDect2[i] = numberofbinsexpected; 
  } 
  if (fDebug >= 3) { 
    CalculXBins(IlcDCHgeometry::GetDetector(fDet[0],fDet[1],fDet[2]),i); 
    fDect1[i] = fXbins[i]; 
    CalculXBins((IlcDCHgeometry::GetDetector(fDet[0],fDet[1],fDet[2])+1),i); 
    fDect2[i] = fXbins[i]; 
  } 
 
  // Create the histos for debugging 
  // CH 
  if (i == 0) { 
     
    gDirectory = gROOT; 
    // Init the VectorFitCH 
    fVectorFitCH = new TObjArray(); 
    fCoefCH      = new Float_t[2304]; 
    for (Int_t k = 0; k < 2304; k++) { 
      fCoefCH[k] = 0.0;     
    } 
    fScaleFitFactor = 0.0; 
 
    // Number of Xbins(detectors or groups of pads) if Vector2d 
    // Quick verification that we are not out of range! 
    if (fVectorCH && fPlaCH) { 
      if ((nbins                            == 0) &&  
          (fVectorCH->GetEntriesFast()      >  0) &&  
          ((Int_t) fPlaCH->GetEntriesFast() >  0)) { 
	if ((Int_t) fVectorCH->GetEntriesFast() > numberofbinsexpected) { 
	  IlcInfo("ch doesn't correspond to the mode of pad group calibration!"); 
	  return kFALSE; 
	} 
	if ((Int_t) fVectorCH->GetEntriesFast() != (Int_t) fPlaCH->GetEntriesFast()) { 
	  IlcInfo("VectorCH doesn't correspond to PlaCH!"); 
	  return kFALSE; 
	} 
      } 
    } 
 
    // 
    // Debugging: Create the histos 
    // 
 
    // fDebug == 0 nothing 
     
    // fDebug == 1  
    if (fDebug == 1) { 
      if (nbins != 0) { 
	// Create the histos replique de ch if histos2D 
	CreateFitHistoCH(nbins,lowedge,upedge); 
      } 
      else { 
	// Ccreate the histos replique de ch vector2d 
	CreateFitHistoCH(numberofbinsexpected,0,numberofbinsexpected); 
      } 
    } 
 
    // fDebug == 2 and fFitVoir no histo 
    if (fDebug == 2) { 
      if (fFitVoir < numberofbinsexpected) { 
	IlcInfo(Form("We will see the fit of the object %d",fFitVoir)); 
      } 
      else { 
	IlcInfo("fFitVoir is out of range of the histo!"); 
	return kFALSE; 
      } 
    } 
 
    // fDebug == 3  or 4 and fDet 
    if (fDebug >= 3) { 
      if ((fNz[0] == 0) && (fNrphi[0] == 0)) { 
	IlcInfo("Do you really want to see one detector without pad groups?"); 
	return kFALSE; 
      } 
      else { 
	IlcInfo(Form("You will see the detector: iPlane %d, iChamb %d, iSect %d" 
                    ,fDet[0],fDet[1],fDet[2])); 
	// A little geometry: 
	Int_t rowMax = parCom->GetRowMax(fDet[0],fDet[1],fDet[2]); 
	Int_t colMax = parCom->GetColMax(fDet[0]); 
	// Create the histos to visuilcse 
	CreateFitHistoCHDB(rowMax,colMax); 
	if (fDebug == 4) { 
          CreateFitHistoCH((Int_t) (fDect2[0]-fDect1[0]),fDect1[0],fDect2[0]); 
	} 
      } 
    } 
 
  } 
     
  // PH and T0 
  if (i == 1) { 
     
    // Number of Xbins (detectors or groups of pads) if vector2d 
    // Quick verification that we are not out of range! 
    if (fVectorPH && fPlaPH) { 
      if ((nbins == 0) &&  
          (fVectorPH->GetEntriesFast()      > 0) &&  
          ((Int_t) fPlaPH->GetEntriesFast() > 0)) { 
	if ((Int_t) fVectorPH->GetEntriesFast() > numberofbinsexpected) { 
	  IlcInfo("ph doesn't correspond to the mode of pad group calibration!"); 
	  return kFALSE; 
	} 
	if ((Int_t) fVectorPH->GetEntriesFast() != (Int_t) fPlaPH->GetEntriesFast()) { 
	  IlcInfo("VectorPH doesn't correspond to PlaPH!"); 
	  return kFALSE; 
	} 
      } 
    } 
         
    // Init tree 
    InitTreePH(); 
    InitTreeT0();     
 
    // 
    // Debugging: Create the histos 
    // 
 
    // fDebug == 0 nothing 
     
    // fDebug == 1  
    if (fDebug == 1) { 
      if (nbins != 0) { 
	// Create the histos replique de ch 
	CreateFitHistoPH(nbins,lowedge,upedge); 
	CreateFitHistoT0(nbins,lowedge,upedge); 
      } 
      else { 
	// Create the histos replique de ch if vector2d 
	CreateFitHistoPH(numberofbinsexpected,0,numberofbinsexpected); 
	CreateFitHistoT0(numberofbinsexpected,0,numberofbinsexpected); 
      } 
    } 
 
    // fDebug == 2 and fFitVoir no histo 
    if (fDebug == 2) { 
      if (fFitVoir < numberofbinsexpected) { 
	IlcInfo(Form("We will see the fit of the object %d",fFitVoir)); 
      } 
      else { 
	IlcInfo("fFitVoir is out of range of the histo!"); 
	return kFALSE; 
      } 
    } 
 
    // fDebug == 3  or 4 and fDet 
    if (fDebug >= 3) { 
      if ((fNz[1]    == 0) &&  
          (fNrphi[1] == 0)) { 
	IlcInfo("Do you really want to see one detector without pad groups?"); 
	return kFALSE; 
      } 
      else  { 
	IlcInfo(Form("You will see the detector: iPlane %d, iChamb %d, iSect %d" 
                    ,fDet[0],fDet[1],fDet[2])); 
	// A little geometry: 
	Int_t rowMax = parCom->GetRowMax(fDet[0],fDet[1],fDet[2]); 
	Int_t colMax = parCom->GetColMax(fDet[0]); 
	// Create the histos to visuilcse 
	CreateFitHistoPHDB(rowMax,colMax); 
	CreateFitHistoT0DB(rowMax,colMax); 
	if (fDebug == 4) { 
	  CreateFitHistoPH((Int_t) (fDect2[1]-fDect1[1]),fDect1[1],fDect2[1]); 
	  CreateFitHistoT0((Int_t) (fDect2[1]-fDect1[1]),fDect1[1],fDect2[1]); 
	} 
      } 
    } 
 
  } 
 
  // PRF 
  if (i == 2) { 
     
    // Number of Xbins(detectors or groups of pads) if vector2d 
    if (fVectorPRF && fPlaPRF){ 
      if ((nbins == 0) &&  
          (fVectorPRF->GetEntriesFast() > 0) &&  
          (fPlaPRF->GetEntriesFast()    > 0)) { 
	// Quick verification that we are not out of range! 
	if ((Int_t) fVectorPRF->GetEntriesFast() > numberofbinsexpected) { 
	  IlcInfo("ch doesn't correspond to the mode of pad group calibration!"); 
	  return kFALSE; 
	} 
	if ((Int_t) fVectorPRF->GetEntriesFast() != (Int_t) fPlaPRF->GetEntriesFast()) { 
	  IlcInfo("VectorPRF doesn't correspond to PlaCH!"); 
	  return kFALSE; 
	} 
      } 
    } 
     
    // Init tree 
    InitTreePRF(); 
 
    // 
    // Debugging: Create the histos 
    // 
 
    // fDebug == 0 nothing 
 
    // fDebug == 1  
    if (fDebug == 1) { 
      if (nbins != 0) { 
	// Create the histos replique de ch 
	CreateFitHistoPRF(nbins,lowedge,upedge); 
      } 
      else { 
	// Create the histos replique de ch 
	CreateFitHistoPRF(numberofbinsexpected,0,numberofbinsexpected); 
      } 
    } 
     
    // fDebug == 2 and fFitVoir no histo 
    if (fDebug == 2) { 
      if (fFitVoir < numberofbinsexpected) { 
	IlcInfo(Form("We will see the fit of the object %d",fFitVoir)); 
      } 
      else { 
	IlcInfo("fFitVoir is out of range of the histo!"); 
	return kFALSE; 
      } 
    } 
 
    // fDebug == 3  or 4 and fDet 
    if (fDebug >= 3) { 
      if ((fNz[2]    == 0) &&  
          (fNrphi[2] == 0)) { 
	IlcInfo("Do you really want to see one detector without pad groups?"); 
	return kFALSE; 
      } 
      else { 
	IlcInfo(Form("You will see the detector: iPlane %d, iChamb %d, iSect %d" 
                    ,fDet[0],fDet[1],fDet[2])); 
	// A little geometry: 
	Int_t rowMax = parCom->GetRowMax(fDet[0],fDet[1],fDet[2]); 
	Int_t colMax = parCom->GetColMax(fDet[0]); 
	// Create the histos to visuilcse 
	CreateFitHistoPRFDB(rowMax,colMax); 
	if (fDebug == 4) { 
          CreateFitHistoPRF((Int_t) (fDect2[2]-fDect1[2]),fDect1[2],fDect2[2]); 
	} 
      } 
    } 
 
  } 
 
  return kTRUE; 
   
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
void IlcDCHCalibra::InitfCountDetAndfCount(Int_t i) 
{ 
  // 
  // Init the current detector where we are fCountDet and the 
  // next fCount for the functions Fit...  
  // 
   
  // Loop on the Xbins of ch!! 
  fCountDet[i] = -1; // Current detector 
  fCount[i]    =  0; // To find the next detector 
   
  // If fDebug >= 3 
  if (fDebug >= 3) { 
 
    // Set countdet to the detector 
    fCountDet[i] = IlcDCHgeometry::GetDetector(fDet[0],fDet[1],fDet[2]); 
         
    // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
    ModePadCalibration(fDet[1],i); 
    ModePadFragmentation(fDet[0],fDet[1],fDet[2],i); 
     
    // Set counter to write at the end of the detector 
    fCount[i] = fDect1[i] + fNfragZ[i]*fNfragRphi[i]; 
 
  } 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
void IlcDCHCalibra::UpdatefCountDetAndfCount(Int_t idect, Int_t i) 
{ 
  // 
  // See if we are in a new detector and update the 
  // variables fNfragZ and fNfragRphi if yes  
  // 
 
  // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
  // If fDebug == 1 or 0 
  if ((fDebug == 0) ||  
      (fDebug == 1)) { 
 
    if (fCount[i] == idect) { 
       
      // On en est au detector 
      fCountDet[i] += 1; 
       
      // Determination of fNnZ, fNnRphi, fNfragZ and fNfragRphi 
      ModePadCalibration((Int_t) GetChamber(fCountDet[i]),i); 
      ModePadFragmentation((Int_t) GetPlane(fCountDet[i]) 
                          ,(Int_t) GetChamber(fCountDet[i]) 
                          ,(Int_t) GetSector(fCountDet[i]),i); 
 
      // Set for the next detector 
      fCount[i] += fNfragZ[i]*fNfragRphi[i]; 
 
    } 
 
  } 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
void IlcDCHCalibra::ReconstructFitRowMinRowMax(Int_t idect, Int_t i) 
{ 
  // 
  // Reconstruct the min pad row, max pad row, min pad col and 
  // max pad col of the calibration group for the Fit functions 
  // 
 
  if (fDebug <  2) { 
    ReconstructionRowPadGroup((Int_t) (idect-(fCount[i]-(fNfragZ[i]*fNfragRphi[i]))),i); 
  } 
  if (fDebug >= 3) { 
    ReconstructionRowPadGroup((Int_t) (idect-fDect1[i]),i); 
  } 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
Bool_t IlcDCHCalibra::NotEnoughStatistic(Int_t idect, Int_t i) 
{ 
  // 
  // For the case where there are not enough entries in the histograms 
  // of the calibration group, the value present in the choosen database 
  // will be put. A negativ sign enables to know that a fit was not possible. 
  // 
   
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam Manager"); 
    return kFALSE; 
  } 
   
  // Get cal 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
 
  if (fDebug != 2) { 
    IlcInfo(Form("The element %d in this detector %d has not enough statistic to be fitted" 
                ,idect-(fCount[i]-(fNfragZ[i]*fNfragRphi[i])),fCountDet[i])); 
  } 
  if (fDebug == 2) { 
    IlcInfo("The element has not enough statistic to be fitted"); 
  } 
 
  if ((i == 0) && (fDebug != 2)) { 
     
    // Calcul the coef from the database choosen 
    CalculChargeCoefMean(fCountDet[0],(Int_t) (idect-fDect1[0]),kFALSE); 
     
    // Fill the coefCH[2304] with negative value to say: not fitted 
    for (Int_t k = fRowMin[0]; k < fRowMax[0]; k++) { 
      for (Int_t j = fColMin[0]; j < fColMax[0]; j++) { 
	if (GetChamber(fCountDet[0]) == 2) { 
          fCoefCH[(Int_t)(j*12+k)] = -TMath::Abs(fChargeCoef[3]); 
	} 
	if (GetChamber(fCountDet[0]) != 2) { 
          fCoefCH[(Int_t)(j*16+k)] = -TMath::Abs(fChargeCoef[3]); 
	} 
      } 
    } 
       
    // End of one detector 
    if ((idect == (fCount[0]-1))) { 
      FillVectorFitCH((Int_t) fCountDet[0]); 
      // Reset 
      for (Int_t k = 0; k < 2304; k++) { 
	fCoefCH[k] = 0.0; 
      } 
    } 
 
  } 
   
  if ((i == 1) && (fDebug != 2)) { 
 
    CalculVdriftCoefMean(fCountDet[1],(Int_t) (idect-fDect1[1])); 
    CalculT0CoefMean(fCountDet[1],(Int_t) (idect-fDect1[1])); 
 
    // Put the default value  
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
 
      if (fFitPHOn) { 
	fCoefVdrift[0]->SetBinContent(idect-fDect1[1]+1,fVdriftCoef[2]); 
	fCoefT0[0]->SetBinContent(idect-fDect1[1]+1,fT0Coef[2]); 
      } 
 
      fCoefVdrift[1]->SetBinContent(idect-fDect1[1]+1,fVdriftCoef[2]); 
      fCoefT0[1]->SetBinContent(idect-fDect1[1]+1,fT0Coef[2]); 
 
    } 
     
    // Put the default value 
    if (fDebug >= 3) { 
      fVdriftCoef[0] = fVdriftCoef[2]; 
      fVdriftCoef[1] = fVdriftCoef[2]; 
      FillCoefVdriftDB(); 
      fT0Coef[0]     = fT0Coef[2]; 
      fT0Coef[1]     = fT0Coef[2]; 
      FillCoefT0DB(); 
    } 
 
    // Fill the tree if end of a detector. 
    // The pointer to the branch stays with the default value 1.5!!! 
    // PH 
    // Pointer to the branch 
    for (Int_t k = fRowMin[1]; k < fRowMax[1]; k++) { 
      for (Int_t j = fColMin[1]; j < fColMax[1]; j++) { 
	if (GetChamber(fCountDet[1]) == 2) { 
          fVdriftPad[(Int_t)(j*12+k)] = -TMath::Abs(fVdriftCoef[2]); 
	} 
	if (GetChamber(fCountDet[1]) != 2) { 
          fVdriftPad[(Int_t)(j*16+k)] = -TMath::Abs(fVdriftCoef[2]); 
	} 
      } 
    } 
 
    // End of one detector 
    if ((idect == (fCount[1]-1)) && (fDebug != 2)) { 
      FillTreeVdrift((Int_t) fCountDet[1]); 
    } 
 
    // T0 
    // Fill the tree if end of a detector. 
    // The pointer to the branch stays with the default value 1.5!!! 
    // Pointer to the branch 
    for (Int_t k = fRowMin[1]; k < fRowMax[1]; k++) { 
      for (Int_t j = fColMin[1]; j < fColMax[1]; j++) { 
	if (GetChamber(fCountDet[1]) == 2) { 
          fT0Pad[(Int_t)(j*12+k)] = -TMath::Abs(fT0Coef[2]); 
	} 
	if (GetChamber(fCountDet[1]) != 2) { 
          fT0Pad[(Int_t)(j*16+k)] = -TMath::Abs(fT0Coef[2]); 
	} 
      } 
    } 
 
    // End of one detector 
    if ((idect == (fCount[1]-1)) && (fDebug != 2)) { 
      FillTreeT0((Int_t) fCountDet[1]); 
    } 
 
  } 
 
  if ((i == 2) && (fDebug != 2)) { 
 
    CalculPRFCoefMean(fCountDet[2],(Int_t) (idect-fDect1[2])); 
       
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      fCoefPRF[0]->SetBinContent(idect-fDect1[2]+1,fPRFCoef[1]); 
    } 
 
    if (fDebug >= 3){ 
      fPRFCoef[0] = fPRFCoef[1]; 
      FillCoefPRFDB(); 
    } 
 
    // Fill the tree if end of a detector. 
    // The pointer to the branch stays with the default value 1.5!!! 
    // Pointer to the branch 
    for (Int_t k = fRowMin[2]; k < fRowMax[2]; k++) { 
      for (Int_t j = fColMin[2]; j < fColMax[2]; j++) { 
	if((parCom->GetColMax(GetPlane(fCountDet[2])) != (j+1)) && (j != 0)){ 
	  if (GetChamber(fCountDet[2]) == 2) { 
            fPRFPad[(Int_t)(j*12+k)] = -fPRFCoef[1]; 
	  } 
	  if (GetChamber(fCountDet[2]) != 2) { 
            fPRFPad[(Int_t)(j*16+k)] = -fPRFCoef[1]; 
	  } 
	} 
	else { 
	  if (fAccCDB) { 
	    if (GetChamber(fCountDet[2]) == 2) { 
              fPRFPad[(Int_t)(j*12+k)] = -((Float_t) cal->GetPRFWidth(fCountDet[2],j,k)); 
	    } 
	    if (GetChamber(fCountDet[2]) != 2) { 
              fPRFPad[(Int_t)(j*16+k)] = -((Float_t) cal->GetPRFWidth(fCountDet[2],j,k)); 
	    } 
	  } 
	  if (!fAccCDB) { 
	    if (GetChamber(fCountDet[2]) == 2) { 
              fPRFPad[(Int_t)(j*12+k)] = -((Float_t) GetPRFDefault(GetPlane(fCountDet[2]))); 
	     } 
	    if (GetChamber(fCountDet[2]) != 2) { 
              fPRFPad[(Int_t)(j*16+k)] = -((Float_t) GetPRFDefault(GetPlane(fCountDet[2]))); 
	    } 
	  } 
	} 
      } 
    } 
 
    // End of one detector 
    if ((idect == (fCount[2]-1)) && (fDebug != 2)) { 
      FillTreePRF((Int_t) fCountDet[2]); 
    } 
 
  } 
   
  return kTRUE; 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
Bool_t IlcDCHCalibra::FillInfosFit(Int_t idect, Int_t i) 
{ 
  // 
  // Fill the coefficients found with the fits or other 
  // methods from the Fit functions 
  // 
 
  // Get the parameter object 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam Manager"); 
    return kFALSE; 
  } 
 
  // Get cal 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB"); 
    return kFALSE; 
  } 
 
  if ((i == 0) && (fDebug != 2)) { 
    // Fill the coefCH[2304] with fChargeCoef[0] 
    // that would be negativ only if the fit failed totally 
    for (Int_t k = fRowMin[0]; k < fRowMax[0]; k++) { 
      for (Int_t j = fColMin[0]; j < fColMax[0]; j++) { 
	if (GetChamber(fCountDet[0]) == 2) { 
          fCoefCH[(Int_t)(j*12+k)] = fChargeCoef[0]; 
	} 
	if (GetChamber(fCountDet[0]) != 2) { 
          fCoefCH[(Int_t)(j*16+k)] = fChargeCoef[0]; 
	} 
      } 
    }                 
    // End of one detector 
    if ((idect == (fCount[0]-1))) { 
      FillVectorFitCH((Int_t) fCountDet[0]); 
      // Reset 
      for (Int_t k = 0; k < 2304; k++) { 
	fCoefCH[k] = 0.0; 
      } 
    } 
  } 
 
  if ((i == 1) && (fDebug != 2)) { 
 
    // PH 
    // Pointer to the branch: fVdriftCoef[1] will ne negativ only if the fit failed totally  
    for (Int_t k = fRowMin[1]; k < fRowMax[1]; k++) { 
      for (Int_t j = fColMin[1]; j < fColMax[1]; j++) { 
	if (GetChamber(fCountDet[1]) == 2) { 
          fVdriftPad[(Int_t)(j*12+k)]=fVdriftCoef[1]; 
	} 
	if (GetChamber(fCountDet[1]) != 2) { 
          fVdriftPad[(Int_t)(j*16+k)]=fVdriftCoef[1]; 
	} 
      } 
    }                 
    // End of one detector 
    if ((idect == (fCount[1]-1)) && (fDebug != 2)) { 
      FillTreeVdrift((Int_t) fCountDet[1]); 
    } 
 
    // T0 
    // Pointer to the branch: fT0Coef[1] will ne negativ only if the fit failed totally  
    for (Int_t k = fRowMin[1]; k < fRowMax[1]; k++) { 
      for (Int_t j = fColMin[1]; j < fColMax[1]; j++) { 
	if (GetChamber(fCountDet[1]) == 2) { 
          fT0Pad[(Int_t)(j*12+k)]=fT0Coef[1]; 
	} 
	if (GetChamber(fCountDet[1]) != 2) { 
          fT0Pad[(Int_t)(j*16+k)]=fT0Coef[1]; 
	} 
      } 
    }                 
    // End of one detector 
    if ((idect == (fCount[1]-1)) && (fDebug != 2)) { 
      FillTreeT0((Int_t) fCountDet[1]); 
    } 
 
  } 
 
  if ((i == 2) && (fDebug != 2)) { 
    // Pointer to the branch 
    for (Int_t k = fRowMin[2]; k < fRowMax[2]; k++) { 
      for (Int_t j = fColMin[2]; j < fColMax[2]; j++) { 
	if ((parCom->GetColMax(GetPlane(fCountDet[2])) != (j+1)) && (j != 0)) { 
	  if (GetChamber(fCountDet[2]) == 2) { 
            fPRFPad[(Int_t)(j*12+k)] = fPRFCoef[0]; 
	  } 
	  if (GetChamber(fCountDet[2]) != 2) { 
            fPRFPad[(Int_t)(j*16+k)] = fPRFCoef[0]; 
	  } 
	} 
	else { 
	  if (fAccCDB) { 
	    if (GetChamber(fCountDet[2]) == 2) { 
              fPRFPad[(Int_t)(j*12+k)] = (Float_t) cal->GetPRFWidth(fCountDet[2],j,k); 
	    } 
	    if (GetChamber(fCountDet[2]) != 2) { 
              fPRFPad[(Int_t)(j*16+k)] = (Float_t) cal->GetPRFWidth(fCountDet[2],j,k); 
	    } 
	  } 
	  if (!fAccCDB) { 
	    if (GetChamber(fCountDet[2]) == 2) { 
              fPRFPad[(Int_t)(j*12+k)] = (Float_t) GetPRFDefault(GetPlane(fCountDet[2])); 
	    } 
	    if (GetChamber(fCountDet[2]) != 2) { 
              fPRFPad[(Int_t)(j*16+k)] = (Float_t) GetPRFDefault(GetPlane(fCountDet[2]));  
	    } 
	  } 
	} 
      } 
    } 
    // End of one detector 
    if ((idect == (fCount[2]-1)) && (fDebug != 2)) { 
      FillTreePRF((Int_t) fCountDet[2]); 
    } 
  } 
 
  return kTRUE; 
 
} 
 
//____________Functions for initialising the IlcDCHCalibra in the code_________ 
Bool_t IlcDCHCalibra::WriteFitInfos(Int_t i) 
{ 
  // 
  // In the case the user wants to write a file with a tree of the found 
  // coefficients for the calibration before putting them in the database 
  // 
 
  TFile *fout = TFile::Open(fWriteNameCoef,"UPDATE"); 
  // Check if the file could be opened 
  if (!fout || !fout->IsOpen()) { 
    IlcInfo("No File found!"); 
    return kFALSE; 
  } 
 
  if ((i == 0) && (fDebug != 2)) { 
    // The error stuff   
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      WriteCH(fout); 
    } 
    // The DB stuff 
    if ((fDebug == 4) ||  
        (fDebug == 3)) { 
      WriteCHDB(fout); 
    } 
    // The tree 
    fout->WriteTObject(fGain,fGain->GetName(),(Option_t *) "writedelete"); 
  } 
 
  if ((i == 1) && (fDebug != 2)) { 
    // PH 
    // The error stuff   
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      WritePH(fout); 
    } 
    // The DB stuff 
    if ((fDebug == 4) ||  
        (fDebug == 3)) { 
      WritePHDB(fout); 
    } 
    // The tree 
    fout->WriteTObject(fVdrift,fVdrift->GetName(),(Option_t *) "writedelete"); 
    // T0 
    // The error stuff   
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      WriteT0(fout); 
    } 
    // The DB stuff 
    if ((fDebug == 4) ||  
        (fDebug == 3)) { 
      WriteT0DB(fout); 
    } 
    // The tree 
    fout->WriteTObject(fT0,fT0->GetName(),(Option_t *) "writedelete"); 
  } 
 
  if ((i == 2) && (fDebug != 2)) { 
    // The error stuff   
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      WritePRF(fout); 
    } 
    // The DB stuff 
    if ((fDebug == 4) ||  
        (fDebug == 3)) { 
      WritePRFDB(fout); 
    } 
    // The tree 
    fout->WriteTObject(fPRF,fPRF->GetName(),(Option_t *) "writedelete"); 
  } 
 
  fout->Close(); 
 
  return kTRUE; 
 
} 
 
// 
//____________Fill the Error histos in case of fDebug == 1_____________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ErrorPRF() 
{ 
  // 
  // Fill the error histos for fDebug = 1 and fDebug = 4 from the delta histos 
  // 
 
  for (Int_t k= 0; k < fDeltaPRF->GetNbinsX(); k++) { 
    if (fDeltaPRF->GetBinContent(k+1) != 0.0) { 
      fErrorPRF->Fill(fDeltaPRF->GetBinContent(k+1)); 
    } 
  } 
  
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ErrorCH() 
{ 
  // 
  // Fill the error histos for fDebug = 1 and fDebug = 4 from the delta histos 
  // 
 
  for (Int_t k= 0; k < fDeltaCharge[0]->GetNbinsX(); k++) { 
    if (fDeltaCharge[0]->GetBinContent(k+1) != 0.0) { 
      fErrorCharge[0]->Fill(fDeltaCharge[0]->GetBinContent(k+1)); 
    } 
  } 
  if (fMeanChargeOn) { 
    for (Int_t k= 0; k < fDeltaCharge[1]->GetNbinsX(); k++) { 
      if (fDeltaCharge[1]->GetBinContent(k+1) != 0.0) { 
        fErrorCharge[1]->Fill(fDeltaCharge[1]->GetBinContent(k+1)); 
      } 
    } 
  } 
  if (fFitChargeBisOn ) { 
    for (Int_t k= 0; k < fDeltaCharge[2]->GetNbinsX(); k++) { 
      if (fDeltaCharge[2]->GetBinContent(k+1) != 0.0) { 
        fErrorCharge[2]->Fill(fDeltaCharge[2]->GetBinContent(k+1)); 
      } 
    } 
  } 
  
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ErrorPH() 
{ 
  // 
  // Fill the error histos for fDebug = 1 and fDebug = 4 from the delta histos 
  // 
 
  if (fFitPHOn ) { 
    for (Int_t k = 0; k < fDeltaVdrift[0]->GetNbinsX(); k++) {      
      if (fDeltaVdrift[0]->GetBinContent(k+1) != 0.0) { 
        fErrorVdrift[0]->Fill(fDeltaVdrift[0]->GetBinContent(k+1)); 
      } 
    } 
  } 
  for (Int_t k = 0; k < fDeltaVdrift[1]->GetNbinsX(); k++) { 
    if (fDeltaVdrift[1]->GetBinContent(k+1) != 0.0) { 
      fErrorVdrift[1]->Fill(fDeltaVdrift[1]->GetBinContent(k+1)); 
    } 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ErrorT0() 
{ 
  // 
  // Fill the error histos for fDebug = 1 and fDebug = 4 from the delta histos 
  // 
 
  if (fFitPHOn ) { 
    for (Int_t k = 0; k < fDeltaT0[0]->GetNbinsX(); k++) {      
      if (fDeltaT0[0]->GetBinContent(k+1) != 0.0) { 
        fErrorT0[0]->Fill(fDeltaT0[0]->GetBinContent(k+1)); 
      } 
    } 
  } 
  for (Int_t k = 0; k < fDeltaT0[1]->GetNbinsX(); k++) { 
    if (fDeltaT0[1]->GetBinContent(k+1) != 0.0) { 
      fErrorT0[1]->Fill(fDeltaT0[1]->GetBinContent(k+1)); 
    } 
  } 
 
} 
 
// 
//____________Fill Coef DB in case of visuilcsation of one detector____________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillCoefVdriftDB() 
{ 
  // 
  // Fill the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
  
  for (Int_t row = fRowMin[1]; row < fRowMax[1]; row++) { 
    for (Int_t col = fColMin[1]; col < fColMax[1]; col++) { 
      fCoefVdriftDB[1]->SetBinContent(row+1,col+1,TMath::Abs(fVdriftCoef[1])); 
      if (fFitPHOn ) { 
        fCoefVdriftDB[0]->SetBinContent(row+1,col+1,TMath::Abs(fVdriftCoef[0])); 
      } 
    } 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillCoefT0DB() 
{ 
  // 
  // Fill the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
  
  for (Int_t row = fRowMin[1]; row < fRowMax[1]; row++) { 
    for (Int_t col = fColMin[1]; col < fColMax[1]; col++) { 
      fCoefT0DB[1]->SetBinContent(row+1,col+1,TMath::Abs(fT0Coef[1])); 
      if (fFitPHOn) { 
        fCoefT0DB[0]->SetBinContent(row+1,col+1,TMath::Abs(fT0Coef[0])); 
      } 
    } 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillCoefChargeDB() 
{ 
  // 
  // Fill the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  for (Int_t row = fRowMin[0]; row < fRowMax[0]; row++) { 
    for (Int_t col = fColMin[0]; col < fColMax[0]; col++) { 
      if (fMeanChargeOn) { 
        fCoefChargeDB[1]->SetBinContent(row+1,col+1,TMath::Abs(fChargeCoef[1])); 
      } 
      if (fFitChargeBisOn) { 
        fCoefChargeDB[2]->SetBinContent(row+1,col+1,TMath::Abs(fChargeCoef[2])); 
      } 
      fCoefChargeDB[0]->SetBinContent(row+1,col+1,TMath::Abs(fChargeCoef[0])); 
    } 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillCoefPRFDB() 
{ 
  // 
  // Fill the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  for (Int_t row = fRowMin[2]; row < fRowMax[2]; row++) { 
    for (Int_t col = fColMin[2]; col < fColMax[2]; col++) { 
      fCoefPRFDB->SetBinContent(row+1,col+1,fPRFCoef[0]); 
    } 
  } 
 
} 
 
// 
//____________Plot histos CoefPRF....__________________________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotCH() 
{ 
  // 
  // Plot the histos for fDebug = 1 and fDebug = 4 for the errors 
  // 
 
  TCanvas *cch1 = new TCanvas("cch1","",50,50,600,800); 
  cch1->cd(); 
  TLegend *legch1 = new TLegend(0.4,0.6,0.89,0.89); 
  legch1->AddEntry(fCoefCharge[3],"f_{g} simulated","l"); 
  if (fMeanChargeOn) { 
    legch1->AddEntry(fCoefCharge[1],"f_{g} mean","p"); 
  } 
  legch1->AddEntry(fCoefCharge[0],"f_{g} fit","p"); 
  if (fFitChargeBisOn ) { 
    legch1->AddEntry(fCoefCharge[2],"f_{g} fitbis","p"); 
  }  
 
  fCoefCharge[0]->Draw("E2"); 
  if (fMeanChargeOn) { 
    fCoefCharge[1]->Draw("E2 same"); 
  } 
  if (fFitChargeBisOn ) { 
    fCoefCharge[2]->Draw("E2 same"); 
  } 
  fCoefCharge[3]->Draw("same"); 
  legch1->Draw("same"); 
 
  TCanvas *cch2 = new TCanvas("cch2","",50,50,600,800); 
  cch2->Divide(2,1); 
  cch2->cd(1); 
  TLegend *legch2 = new TLegend(0.4,0.6,0.89,0.89); 
  if (fMeanChargeOn) { 
    legch2->AddEntry(fErrorCharge[1],"f_{g} mean","l"); 
  } 
  legch2->AddEntry(fErrorCharge[0],"f_{g} fit","l"); 
  if (fFitChargeBisOn) { 
    legch2->AddEntry(fErrorCharge[2],"f_{g} fitbis","l"); 
  } 
  fErrorCharge[0]->Draw(); 
  if (fMeanChargeOn) { 
    fErrorCharge[1]->Draw("same"); 
  } 
  if (fFitChargeBisOn) { 
    fErrorCharge[2]->Draw("same"); 
  } 
  legch2->Draw("same"); 
  cch2->cd(2); 
  TLegend *legch3 = new TLegend(0.4,0.6,0.89,0.89); 
  if (fMeanChargeOn) { 
    legch3->AddEntry(fDeltaCharge[1],"mean","p"); 
  } 
  legch3->AddEntry(fDeltaCharge[0],"fit","p"); 
  if (fFitChargeBisOn) { 
    legch3->AddEntry(fDeltaCharge[2],"fit","p"); 
  } 
  fDeltaCharge[0]->Draw("E2"); 
  if (fMeanChargeOn) { 
    fDeltaCharge[1]->Draw("E2 same"); 
  } 
  if (fFitChargeBisOn) { 
    fDeltaCharge[2]->Draw("E2 same"); 
  } 
  legch3->Draw("same"); 
   
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPH() 
{ 
  // 
  // Plot the histos for fDebug = 1 and fDebug = 4 for the errors 
  // 
 
  TCanvas *cph1 = new TCanvas("cph1","",50,50,600,800); 
  cph1->cd(); 
  TLegend *legph1 = new TLegend(0.4,0.6,0.89,0.89); 
  legph1->AddEntry(fCoefVdrift[2],"v_{real} simulated","l"); 
  legph1->AddEntry(fCoefVdrift[1],"v_{sm} slope 1 method","p"); 
  
  if (fFitPHOn) { 
    legph1->AddEntry(fCoefVdrift[0],"v_{fit} fit","p"); 
  } 
  fCoefVdrift[1]->Draw("E2"); 
  fCoefVdrift[2]->Draw("same"); 
  if (fFitPHOn){ 
    fCoefVdrift[0]->Draw("E2 same"); 
  } 
  legph1->Draw("same"); 
 
  TCanvas *cph2 = new TCanvas("cph2","",50,50,600,800); 
  cph2->Divide(2,1); 
  cph2->cd(1); 
  TLegend *legph2 = new TLegend(0.4,0.6,0.89,0.89); 
  legph2->AddEntry(fErrorVdrift[1],"v_{sm} slope 1 method","l"); 
  if (fFitPHOn) { 
    legph2->AddEntry(fErrorVdrift[0],"v_{fit} fit","l"); 
  } 
  fErrorVdrift[1]->Draw(); 
  if (fFitPHOn) { 
    fErrorVdrift[0]->Draw("l,same"); 
  } 
  legph2->Draw("same"); 
  cph2->cd(2); 
  TLegend *legph3 = new TLegend(0.4,0.6,0.89,0.89); 
  legph3->AddEntry(fDeltaVdrift[1],"v_{sm} slope 1 method","p"); 
  if (fFitPHOn) { 
    legph3->AddEntry(fDeltaVdrift[0],"v_{fit} fit","p"); 
  } 
  fDeltaVdrift[1]->Draw("E2"); 
  if (fFitPHOn) { 
    fDeltaVdrift[0]->Draw("E2 same"); 
  } 
  legph3->Draw("same"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotT0() 
{ 
  // 
  // Plot the histos for fDebug = 1 and fDebug = 4 for the errors 
  // 
 
  TCanvas *ct01 = new TCanvas("ct01","",50,50,600,800); 
  ct01->cd(); 
  TLegend *legt01 = new TLegend(0.4,0.6,0.89,0.89); 
  legt01->AddEntry(fCoefT0[2],"t0 simulated","l"); 
  legt01->AddEntry(fCoefT0[1],"t0 slope 1 method","p"); 
  
  if (fFitPHOn) { 
    legt01->AddEntry(fCoefT0[0],"t0 fit","p"); 
  } 
  fCoefT0[1]->Draw("E2"); 
  fCoefT0[2]->Draw("same"); 
  if (fFitPHOn){ 
    fCoefT0[0]->Draw("E2 same"); 
  } 
  legt01->Draw("same"); 
 
  TCanvas *ct02 = new TCanvas("ct02","",50,50,600,800); 
  ct02->Divide(2,1); 
  ct02->cd(1); 
  TLegend *legt02 = new TLegend(0.4,0.6,0.89,0.89); 
  legt02->AddEntry(fErrorT0[1],"t0 slope 1 method","l"); 
  if (fFitPHOn) { 
    legt02->AddEntry(fErrorT0[0],"t0 fit","l"); 
  } 
  fErrorT0[1]->Draw(); 
  if (fFitPHOn) { 
    fErrorT0[0]->Draw("l,same"); 
  } 
  legt02->Draw("same"); 
  ct02->cd(2); 
  TLegend *legt03 = new TLegend(0.4,0.6,0.89,0.89); 
  legt03->AddEntry(fDeltaT0[1],"t0 slope 1 method","p"); 
  if (fFitPHOn) { 
    legt03->AddEntry(fDeltaT0[0],"t0 fit","p"); 
  } 
  fDeltaT0[1]->Draw("E2"); 
  if (fFitPHOn) { 
    fDeltaT0[0]->Draw("E2 same"); 
  } 
  legt03->Draw("same"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPRF() 
{ 
  // 
  // Plot the histos for fDebug = 1 and fDebug = 4 for the errors 
  // 
 
  TCanvas *cprf1 = new TCanvas("cprf1","",50,50,600,800); 
  cprf1->cd(); 
  TLegend *legprf1 = new TLegend(0.4,0.6,0.89,0.89); 
  legprf1->AddEntry(fCoefPRF[1],"#sigma_{real} simulated","l"); 
  legprf1->AddEntry(fCoefPRF[0],"#sigma_{fit} reconstructed","p"); 
 
  fCoefPRF[0]->Draw("E2"); 
  fCoefPRF[1]->Draw("same"); 
  legprf1->Draw("same"); 
   
  TCanvas *cprf2 = new TCanvas("cprf2","",50,50,600,800); 
  cprf2->Divide(2,1); 
  cprf2->cd(1); 
  TLegend *legprf2 = new TLegend(0.4,0.6,0.89,0.89); 
  legprf2->AddEntry(fErrorPRF,"#sigma_{fit} reconstructed","l"); 
  fErrorPRF->Draw(""); 
  legprf2->Draw("same"); 
  cprf2->cd(2); 
  TLegend *legprf3 = new TLegend(0.4,0.6,0.89,0.89); 
  legprf3->AddEntry(fDeltaPRF,"#sigma_{fit} reconstructed","p"); 
  fDeltaPRF->Draw("E2"); 
  legprf3->Draw("same"); 
 
} 
 
// 
//____________Plot histos DB___________________________________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotCHDB() 
{ 
  // 
  // Plot the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  TCanvas *cchdb = new TCanvas("cchdb","",50,50,600,800); 
  if ((fFitChargeBisOn) && (fMeanChargeOn)) { 
    cchdb->Divide(3,1); 
    cchdb->cd(1); 
    fCoefChargeDB[0]->Draw("LEGO"); 
    cchdb->cd(2); 
    fCoefChargeDB[1]->Draw("LEGO"); 
    cchdb->cd(3); 
    fCoefChargeDB[2]->Draw("LEGO"); 
  } 
  if ((!fFitChargeBisOn) && (fMeanChargeOn)) { 
    cchdb->Divide(2,1); 
    cchdb->cd(1); 
    fCoefChargeDB[0]->Draw("LEGO"); 
    cchdb->cd(2); 
    fCoefChargeDB[1]->Draw("LEGO"); 
  } 
  else { 
    cchdb->cd(); 
    fCoefChargeDB[0]->Draw("LEGO"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPHDB() 
{ 
  // 
  // Plot the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  TCanvas *cphdb = new TCanvas("cphdb","",50,50,600,800); 
  if (fFitPHOn) { 
    cphdb->Divide(2,1); 
    cphdb->cd(1); 
    fCoefVdriftDB[0]->Draw("LEGO"); 
    cphdb->cd(2); 
    fCoefVdriftDB[1]->Draw("LEGO"); 
  } 
  else { 
    cphdb->cd(); 
    fCoefVdriftDB[1]->Draw("LEGO"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotT0DB() 
{ 
  // 
  // Plot the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  TCanvas *ct0db = new TCanvas("ct0db","",50,50,600,800); 
  if (fFitPHOn ) { 
    ct0db->Divide(2,1); 
    ct0db->cd(1); 
    fCoefT0DB[0]->Draw("LEGO"); 
    ct0db->cd(2); 
    fCoefT0DB[1]->Draw("LEGO"); 
  } 
  else { 
    ct0db->cd(); 
    fCoefT0DB[1]->Draw("LEGO"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::PlotPRFDB() 
{ 
  // 
  // Plot the histos for fDebug = 3 and fDebug = 4 to visuilcse the detector 
  // 
 
  TCanvas *cprfdb = new TCanvas("cprfdb","",50,50,600,800); 
  cprfdb->cd(); 
  fCoefPRFDB->Draw("LEGO"); 
 
} 
 
// 
//____________Write histos Coef________________________________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WriteCH(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 1 and fDebug = 4 
  // 
 
  fout->WriteTObject(fCoefCharge[0],fCoefCharge[0]->GetName(),(Option_t *) "OverWrite"); 
  if (fMeanChargeOn) { 
    fout->WriteTObject(fCoefCharge[1],fCoefCharge[1]->GetName(),(Option_t *) "OverWrite"); 
  } 
  if (fFitChargeBisOn) { 
    fout->WriteTObject(fCoefCharge[2],fCoefCharge[2]->GetName(),(Option_t *) "OverWrite"); 
  }   
 
  fout->WriteTObject(fCoefCharge[3],fCoefCharge[3]->GetName(),(Option_t *) "OverWrite"); 
   
  fout->WriteTObject(fDeltaCharge[0],fDeltaCharge[0]->GetName(),(Option_t *) "OverWrite"); 
  if (fMeanChargeOn) { 
    fout->WriteTObject(fDeltaCharge[1],fDeltaCharge[1]->GetName(),(Option_t *) "OverWrite"); 
  } 
  if (fFitChargeBisOn) { 
    fout->WriteTObject(fDeltaCharge[2],fDeltaCharge[2]->GetName(),(Option_t *) "OverWrite"); 
  } 
   
  fout->WriteTObject(fErrorCharge[0],fErrorCharge[0]->GetName(),(Option_t *) "OverWrite"); 
  if (fMeanChargeOn) { 
    fout->WriteTObject(fErrorCharge[1],fErrorCharge[1]->GetName(),(Option_t *) "OverWrite"); 
  } 
  if (fFitChargeBisOn) { 
    fout->WriteTObject(fErrorCharge[2],fErrorCharge[2]->GetName(),(Option_t *) "OverWrite"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WritePH(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 1 and fDebug = 4 
  // 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fCoefVdrift[0],fCoefVdrift[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fCoefVdrift[1],fCoefVdrift[1]->GetName(),(Option_t *) "OverWrite"); 
  fout->WriteTObject(fCoefVdrift[2],fCoefVdrift[2]->GetName(),(Option_t *) "OverWrite"); 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fDeltaVdrift[0],fDeltaVdrift[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fDeltaVdrift[1],fDeltaVdrift[1]->GetName(),(Option_t *) "OverWrite"); 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fErrorVdrift[0],fErrorVdrift[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fErrorVdrift[1],fErrorVdrift[1]->GetName(),(Option_t *) "OverWrite"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WriteT0(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 1 and fDebug = 4 
  // 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fCoefT0[0],fCoefT0[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fCoefT0[1],fCoefT0[1]->GetName(),(Option_t *) "OverWrite"); 
  fout->WriteTObject(fCoefT0[2],fCoefT0[2]->GetName(),(Option_t *) "OverWrite"); 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fDeltaT0[0],fDeltaT0[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fDeltaT0[1],fDeltaT0[1]->GetName(),(Option_t *) "OverWrite"); 
     
  if (fFitPHOn) { 
    fout->WriteTObject(fErrorT0[0],fErrorT0[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fErrorT0[1],fErrorT0[1]->GetName(),(Option_t *) "OverWrite"); 
 
} 
 
//________________________________________________________________________________ 
void IlcDCHCalibra::WritePRF(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 1 and fDebug = 4 
  // 
   
  fout->WriteTObject(fCoefPRF[0],fCoefPRF[0]->GetName(),(Option_t *) "OverWrite"); 
  fout->WriteTObject(fCoefPRF[1],fCoefPRF[1]->GetName(),(Option_t *) "OverWrite"); 
 
  fout->WriteTObject(fDeltaPRF,fDeltaPRF->GetName(), (Option_t *)"OverWrite"); 
  fout->WriteTObject(fErrorPRF,fErrorPRF->GetName(), (Option_t *)"OverWrite"); 
 
} 
 
// 
//____________Write DB Histos__________________________________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WriteCHDB(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 3 and fDebug = 4 
  // 
 
  fout->WriteTObject(fCoefChargeDB[0],fCoefChargeDB[0]->GetName(),(Option_t *) "OverWrite"); 
  if (fMeanChargeOn) { 
    fout->WriteTObject(fCoefChargeDB[1],fCoefChargeDB[1]->GetName(),(Option_t *) "OverWrite"); 
  } 
  if (fFitChargeBisOn ) { 
    fout->WriteTObject(fCoefChargeDB[2],fCoefChargeDB[2]->GetName(),(Option_t *) "OverWrite"); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WritePHDB(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 3 and fDebug = 4 
  // 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fCoefVdriftDB[0],fCoefVdriftDB[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fCoefVdriftDB[1],fCoefVdriftDB[1]->GetName(),(Option_t *) "OverWrite"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WriteT0DB(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 3 and fDebug = 4 
  // 
 
  if (fFitPHOn) { 
    fout->WriteTObject(fCoefT0DB[0],fCoefT0DB[0]->GetName(),(Option_t *) "OverWrite"); 
  } 
  fout->WriteTObject(fCoefT0DB[1],fCoefT0DB[1]->GetName(),(Option_t *) "OverWrite"); 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::WritePRFDB(TFile *fout) 
{ 
  // 
  // If wanted, write the debug histos for fDebug = 3 and fDebug = 4 
  // 
 
  fout->WriteTObject(fCoefPRFDB,fCoefPRFDB->GetName(),(Option_t *) "OverWrite"); 
 
} 
 
// 
//____________Calcul Coef Mean_________________________________________________ 
// 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::CalculT0CoefMean(Int_t dect, Int_t idect) 
{ 
  // 
  // For the detector Dect calcul the mean time 0 
  // for the calibration group idect from the choosen database 
  // 
 
  IlcDCHcalibDB *cal = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB  Manager"); 
    return kFALSE; 
  } 
 
  fT0Coef[2] = 0.0; 
 
  if ((fDebug != 2) && fAccCDB) { 
 
    for (Int_t row = fRowMin[1]; row < fRowMax[1]; row++) { 
      for (Int_t col = fColMin[1]; col < fColMax[1]; col++) { 
	// Groups of pads 
	if ((fNz[1]    > 0) &&  
            (fNrphi[1] > 0)) { 
	  fT0Coef[2] += (Float_t) cal->GetT0(dect,col,row); 
	} 
	// Per detectors 
	else { 
          fT0Coef[2] += (Float_t) cal->GetT0Average(dect); 
	} 
      } 
    } 
 
    fT0Coef[2] = fT0Coef[2] / ((fColMax[1]-fColMin[1])*(fRowMax[1]-fRowMin[1])); 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      fCoefT0[2]->SetBinContent(idect+1,fT0Coef[2]); 
    } 
 
  } 
 
  return kTRUE; 
   
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::CalculChargeCoefMean(Int_t dect, Int_t idect, Bool_t vrai) 
{ 
  // 
  // For the detector Dect calcul the mean gain factor 
  // for the calibration group idect from the choosen database 
  // 
 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB  Manager"); 
    return kFALSE; 
  } 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam  Manager"); 
    return kFALSE; 
  } 
 
  fChargeCoef[3] = 0.0; 
 
  if (fDebug != 2) { 
 
    for (Int_t row = fRowMin[0]; row < fRowMax[0]; row++) { 
      for (Int_t col = fColMin[0]; col < fColMax[0]; col++) { 
	// Groups of pads 
	if ((fNz[0]    > 0) ||  
            (fNrphi[0] > 0)) { 
	  if (fAccCDB) { 
            fChargeCoef[3] += (Float_t) cal->GetGainFactor(dect,col,row); 
	  } 
	  if (vrai && fAccCDB) { 
            fScaleFitFactor += (Float_t) cal->GetGainFactor(dect,col,row); 
	  } 
	  if (!fAccCDB) { 
            fChargeCoef[3] += 1.0; 
	  } 
	  if (vrai && (!fAccCDB)) { 
            fScaleFitFactor += 1.0; 
	  } 
	} 
	// Per detectors 
	else { 
	  if (fAccCDB) { 
            fChargeCoef[3] += (Float_t) cal->GetGainFactorAverage(dect); 
	  } 
	  if (vrai && fAccCDB) { 
            fScaleFitFactor += ((Float_t) cal->GetGainFactorAverage(dect)); 
	  } 
	  if (!fAccCDB) { 
            fChargeCoef[3] += 1.0; 
	  } 
	  if (vrai && (!fAccCDB)) { 
            fScaleFitFactor += 1.0; 
	  } 
	} 
      } 
    } 
 
    fChargeCoef[3] = fChargeCoef[3] / ((fColMax[0]-fColMin[0])*(fRowMax[0]-fRowMin[0])); 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      fCoefCharge[3]->SetBinContent(idect+1,fChargeCoef[3]); 
    } 
 
  } 
 
  return kTRUE; 
 
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::CalculPRFCoefMean(Int_t dect, Int_t idect) 
{ 
  // 
  // For the detector Dect calcul the mean sigma of pad response 
  // function for the calibration group idect from the choosen database 
  // 
 
  IlcDCHcalibDB     *cal    = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB  Manager"); 
    return kFALSE; 
  } 
 
  IlcDCHCommonParam *parCom = IlcDCHCommonParam::Instance(); 
  if (!parCom) { 
    IlcInfo("Could not get CommonParam  Manager"); 
    return kFALSE; 
  } 
 
  fPRFCoef[1] = 0.0; 
  Int_t cot = 0; 
 
  if (fDebug != 2) { 
     
    for (Int_t row = fRowMin[2]; row < fRowMax[2]; row++) { 
      for (Int_t col = fColMin[2]; col < fColMax[2]; col++) { 
	if ((parCom->GetColMax(GetPlane(dect)) != (col+1)) && (col != 0)) { 
	  cot++; 
	  if (fAccCDB) { 
            fPRFCoef[1] += (Float_t) cal->GetPRFWidth(dect,col,row); 
	  } 
	  if (!fAccCDB) { 
            fPRFCoef[1] += GetPRFDefault(GetPlane(dect)); 
	  } 
	} 
      } 
    } 
 
    if (cot > 0) { 
      fPRFCoef[1] = fPRFCoef[1]/cot; 
      if ((fDebug == 1) || 
          (fDebug == 4)) { 
        fCoefPRF[1]->SetBinContent(idect+1,fPRFCoef[1]); 
      } 
    } 
    if (cot <= 0) { 
      if ((fDebug == 1) || 
          (fDebug == 4)) { 
	if (fAccCDB) { 
          fCoefPRF[1]->SetBinContent(idect+1,cal->GetPRFWidth(dect,fColMin[2],fRowMin[2])); 
	} 
	if (!fAccCDB) { 
          fCoefPRF[1]->SetBinContent(idect+1,GetPRFDefault(GetPlane(dect))); 
	} 
      } 
    } 
 
  } 
 
  return kTRUE; 
   
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::CalculVdriftCoefMean(Int_t dect, Int_t idect) 
{ 
  // 
  // For the detector dect calcul the mean drift velocity for the 
  // calibration group idect from the choosen database 
  // 
 
  IlcDCHcalibDB *cal = IlcDCHcalibDB::Instance(); 
  if (!cal) { 
    IlcInfo("Could not get calibDB  Manager"); 
    return kFALSE; 
  } 
 
  fVdriftCoef[2] = 0.0; 
 
  if (fDebug != 2) { 
    for (Int_t row = fRowMin[1]; row < fRowMax[1]; row++) { 
      for (Int_t col = fColMin[1]; col < fColMax[1]; col++) { 
	// Groups of pads 
	if ((fNz[1]    > 0) ||  
            (fNrphi[1] > 0)) { 
	  if (fAccCDB) { 
            fVdriftCoef[2] += (Float_t) cal->GetVdrift(dect,col,row); 
	  } 
	  if (!fAccCDB) { 
            fVdriftCoef[2] += 1.5; 
	  } 
	} 
	// Per detectors 
	else { 
	  if (fAccCDB) { 
            fVdriftCoef[2] += (Float_t) cal->GetVdriftAverage(dect); 
	  } 
	  if (!fAccCDB) { 
            fVdriftCoef[2] += 1.5; 
	  } 
	} 
      } 
    } 
    fVdriftCoef[2] = fVdriftCoef[2] / ((fColMax[1]-fColMin[1])*(fRowMax[1]-fRowMin[1])); 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      fCoefVdrift[2]->SetBinContent(idect+1,fVdriftCoef[2]); 
    } 
  } 
 
  return kTRUE; 
   
} 
 
//_____________________________________________________________________________ 
Float_t IlcDCHCalibra::GetPRFDefault(Int_t plane) const 
{ 
  // 
  // Default width of the PRF if there is no database as reference 
  // 
 
  if (plane == 0) { 
    return 0.515; 
  } 
  if (plane == 1) { 
    return 0.502; 
  } 
  if (plane == 2) { 
    return 0.491; 
  } 
  if (plane == 3) { 
    return 0.481; 
  } 
  if (plane == 4) { 
    return 0.471; 
  } 
  if (plane == 5) { 
    return 0.463; 
  } 
  else { 
    return 0.0; 
  } 
   
} 
 
// 
//____________Pad group calibration mode_______________________________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ReconstructionRowPadGroup(Int_t idect, Int_t i) 
{ 
  // 
  // For the calibration group idect in a detector calculate the 
  // first and last row pad and col pad. 
  // The pads in the interval will have the same calibrated coefficients 
  // 
 
  Int_t posc = -1; 
  Int_t posr = -1; 
  fRowMin[i] = -1; 
  fRowMax[i] = -1; 
  fColMin[i] = -1; 
  fColMax[i] = -1; 
   
  if (fNfragZ[i]    != 0) { 
    posc = (Int_t) idect / fNfragZ[i]; 
  } 
  if (fNfragRphi[i] != 0) { 
    posr = (Int_t) idect % fNfragZ[i]; 
  } 
  fRowMin[i] = posr     * fNnZ[i]; 
  fRowMax[i] = (posr+1) * fNnZ[i]; 
  fColMin[i] = posc     * fNnRphi[i]; 
  fColMax[i] = (posc+1) * fNnRphi[i]; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::CalculXBins(Int_t idect, Int_t i) 
{ 
  // 
  // For the detector idect calcul the first Xbins 
  // 
 
  fXbins[i] = 0; 
  if (fDebug == 4) { 
    IlcInfo(Form("detector: %d", idect)); 
  } 
 
  // In which sector? 
  Int_t sector = GetSector(idect); 
  fXbins[i] += sector*(6*fDetChamb2[i]+6*4*fDetChamb0[i]); 
  
  // In which chamber? 
  Int_t chamber = GetChamber(idect); 
  Int_t kc      = 0; 
  while (kc < chamber) { 
    if (kc == 2) { 
      fXbins[i] += 6 * fDetChamb2[i]; 
    } 
    else { 
      fXbins[i] += 6 * fDetChamb0[i]; 
    } 
    kc ++; 
  } 
   
  // In which plane? 
  Int_t plane = GetPlane(idect); 
  if (chamber == 2) { 
    fXbins[i] += plane*fDetChamb2[i]; 
  } 
  else { 
    fXbins[i] += plane*fDetChamb0[i]; 
  } 
  
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::SearchInVector(Int_t group, Int_t i) const 
{ 
  // 
  // Search if the calibration group "group" has already been 
  // initialised by a previous track in the vector 
  // 
 
  if (i == 0) { 
    for (Int_t k = 0; k < (Int_t) fPlaCH->GetEntriesFast(); k++) { 
      if (((IlcDCHPlace *) fPlaCH->At(k))->GetPlace() == group) { 
        return k; 
      } 
    } 
    return -1; 
  } 
 
  if (i == 1) { 
    for (Int_t k = 0; k < (Int_t) fPlaPH->GetEntriesFast(); k++) { 
      if (((IlcDCHPlace *) fPlaPH->At(k))->GetPlace() == group) { 
        return k; 
      } 
    } 
    return -1; 
  } 
 
  if (i == 2) { 
    for (Int_t k = 0; k < (Int_t) fPlaPRF->GetEntriesFast(); k++) { 
      if (((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace() == group) { 
        return k; 
      } 
    } 
    return -1; 
  } 
 
  return -1; 
 
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::SearchInTreeVector(TObjArray *vectorplace, Int_t group) const 
{ 
  // 
  // Search if the calibration group "group" is present in the tree 
  // 
 
  for (Int_t k = 0; k < (Int_t) vectorplace->GetEntriesFast(); k++) { 
    if (((IlcDCHPlace *) vectorplace->At(k))->GetPlace() == group) { 
      return k; 
    } 
  } 
 
  return -1; 
 
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::SearchBin(Float_t value, Int_t i) const 
{ 
  // 
  // Search the bin 
  // 
 
  Int_t reponse      = 0; 
  Int_t fbinmin      = 0; 
  Int_t fbinmax      = (Int_t) value; 
  Int_t fNumberOfBin = -1; 
 
  // Charge 
  if (i == 0) { 
    fbinmax      = 300; 
    fbinmin      = 0; 
    fNumberOfBin = fNumberBinCharge; 
  } 
 
  // PRF 
  if (i == 2) { 
    fbinmax      = 1; 
    fbinmin      = -1; 
    fNumberOfBin = fNumberBinPRF; 
  } 
 
  // Return -1 if out 
  if ((value >= fbinmax) ||  
      (value <  fbinmin)) { 
    return -1; 
  } 
  // Sinon 
  else { 
    reponse = (Int_t) ((fNumberOfBin*(value-fbinmin)) / (fbinmax-fbinmin)); 
  } 
 
  return reponse; 
 
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::UpdateVectorCH(Int_t group, Float_t value) 
{ 
  // 
  // Fill the vector if a new calibration group "group" or update the 
  // values of the calibration group "group" if already here   
  // 
 
  // Search bin 
  Int_t bin = SearchBin(value,0); 
  // Out 
  if ((bin < 0) || (bin >= fNumberBinCharge)) { 
    return kFALSE;  
  } 
 
  // Search place 
  Int_t place = SearchInVector(group,0); 
 
  // New group 
  if (place == -1) { 
    IlcDCHPlace *placegroup = new IlcDCHPlace(); 
    placegroup->SetPlace(group); 
    fPlaCH->Add((TObject *) placegroup); 
    // Variable 
    IlcDCHCTInfo *fCHInfo = new IlcDCHCTInfo(); 
    UShort_t *entries = new UShort_t[fNumberBinCharge]; 
    // Initialise first 
    for(Int_t k = 0; k < fNumberBinCharge; k++) { 
      entries[k] = 0; 
    } 
    // Add the value 
    entries[bin]= 1; 
    // Set 
    fCHInfo->SetEntries(entries); 
    // Set in the vector 
    fVectorCH->Add((TObject *) fCHInfo); 
  } 
  // Group already exits 
  else { 
    // Variable 
    IlcDCHCTInfo *fCHInfo = new IlcDCHCTInfo(); 
    // Retrieve 
    fCHInfo = ((IlcDCHCTInfo *) fVectorCH->At(place)); 
    UShort_t *entries = fCHInfo->GetEntries(); 
    // Add 
    entries[bin]++; 
    // Set 
    fCHInfo->SetEntries(entries); 
    // Update the vector 
    fVectorCH->AddAt((TObject *) fCHInfo,place); 
  } 
 
  return kTRUE; 
 
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::UpdateVectorPRF(Int_t group, Float_t x, Float_t y) 
{ 
  // 
  // Fill the vector if a new calibration group "group" or update the 
  // values of the calibration group "group" if already here   
  // 
 
  // Search bin 
  Int_t bin = SearchBin(x,2); 
  // Out 
  if ((bin < 0) || (bin >= fNumberBinPRF)) { 
    return kFALSE;  
  } 
 
  // Search place 
  Int_t place = SearchInVector(group,2); 
 
  // New group 
  if (place == -1) { 
 
    IlcDCHPlace *placegroup = new IlcDCHPlace(); 
    placegroup->SetPlace(group); 
    fPlaPRF->Add((TObject *) placegroup); 
    IlcDCHPInfo *fPRFInfo = new IlcDCHPInfo(); 
 
    Float_t  *sum       = new Float_t[fNumberBinPRF]; 
    Float_t  *sumsquare = new Float_t[fNumberBinPRF]; 
    UShort_t *entries   = new UShort_t[fNumberBinPRF]; 
 
    // Initialise first 
    for (Int_t k = 0; k < fNumberBinPRF; k++) { 
      sum[k]       = 0.0; 
      sumsquare[k] = 0.0; 
      entries[k]   = 0; 
    } 
 
    // Add the value 
    sum[bin]       += y; 
    sumsquare[bin] += y*y; 
    entries[bin]++; 
 
    // Set 
    fPRFInfo->SetSum(sum); 
    fPRFInfo->SetSumSquare(sumsquare); 
    fPRFInfo->SetEntries(entries); 
 
    // Set in the vector 
    fVectorPRF->Add((TObject *) fPRFInfo); 
         
  } 
  // Group already exits 
  else { 
 
    IlcDCHPInfo *fPRFInfo = new IlcDCHPInfo(); 
    // Retrieve 
    fPRFInfo = (IlcDCHPInfo *) fVectorPRF->At(place); 
 
    Float_t  *sum       = fPRFInfo->GetSum(); 
    Float_t  *sumsquare = fPRFInfo->GetSumSquare(); 
    UShort_t *entries   = fPRFInfo->GetEntries(); 
 
    // Add 
    Double_t calcul       = (((Double_t) fPRFInfo->GetEntries()[bin]) 
                           * ((Double_t) fPRFInfo->GetSum()[bin]) + (Double_t) y) 
                          / (((Double_t) fPRFInfo->GetEntries()[bin]) + 1); 
    sum[bin]       = (Float_t) calcul; 
    Double_t calculsquare = (((Double_t) fPRFInfo->GetSumSquare()[bin]) 
                           * ((Double_t) fPRFInfo->GetEntries()[bin]) + ((Double_t) y)*((Double_t) y)) 
                          / (((Double_t) fPRFInfo->GetEntries()[bin]) + 1); 
    sumsquare[bin] = (Float_t) calculsquare; 
    entries[bin]++; 
 
    // Set 
    fPRFInfo->SetSum(sum); 
    fPRFInfo->SetSumSquare(sumsquare); 
    fPRFInfo->SetEntries(entries); 
  
    // Update the vector 
    fVectorPRF->AddAt((TObject *) fPRFInfo,place); 
 
  } 
 
  return kTRUE; 
 
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::UpdateVectorPH(Int_t group, Int_t time, Float_t value) 
{ 
  // 
  // Fill the vector if a new calibration group "group" or update 
  // the values of the calibration group "group" if already here   
  // 
 
  // Search bin 
  Int_t bin = time; 
  // Out 
  if ((bin <         0) ||  
      (bin >= fTimeMax)) { 
    return kFALSE;  
  } 
 
  // Search place 
  Int_t place = SearchInVector(group,1); 
 
  // New group 
  if(place == -1){ 
 
    IlcDCHPlace *placegroup = new IlcDCHPlace(); 
    placegroup->SetPlace(group); 
    fPlaPH->Add((TObject *) placegroup); 
    IlcDCHPInfo *fPHInfo = new IlcDCHPInfo(); 
 
    Float_t  *sum       = new Float_t[fTimeMax]; 
    Float_t  *sumsquare = new Float_t[fTimeMax]; 
    UShort_t *entries   = new UShort_t[fTimeMax]; 
 
    // Initialise first 
    for (Int_t k = 0; k < fTimeMax; k++) { 
      sum[k]       = 0.0; 
      sumsquare[k] = 0.0; 
      entries[k]   = 0; 
    } 
 
    // Add the value 
    sum[bin]       += value; 
    sumsquare[bin] += value*value; 
    entries[bin]++; 
 
    // Set 
    fPHInfo->SetSum(sum); 
    fPHInfo->SetSumSquare(sumsquare); 
    fPHInfo->SetEntries(entries); 
 
    // Set in the vector 
    fVectorPH->Add((TObject *) fPHInfo); 
 
  } 
  // Group already exits 
  else { 
 
    IlcDCHPInfo *fPHInfo = new IlcDCHPInfo(); 
    // Retrieve 
    fPHInfo = (IlcDCHPInfo *) fVectorPH->At(place); 
 
    Float_t  *sum       = fPHInfo->GetSum(); 
    Float_t  *sumsquare = fPHInfo->GetSumSquare(); 
    UShort_t *entries   = fPHInfo->GetEntries(); 
 
    // Add 
    Double_t calcul       = (((Double_t) fPHInfo->GetEntries()[bin]) 
                           * ((Double_t) fPHInfo->GetSum()[bin]) + (Double_t) value) 
                          / (((Double_t) fPHInfo->GetEntries()[bin]) + 1); 
    sum[bin]       = (Float_t) calcul; 
    Double_t calculsquare = ((((Double_t) fPHInfo->GetSumSquare()[bin]) 
                            * ((Double_t) fPHInfo->GetEntries()[bin]))  
                          + (((Double_t) value) * ((Double_t)value)))  
                          / (((Double_t) fPHInfo->GetEntries()[bin]) + 1); 
    sumsquare[bin] = (Float_t) calculsquare; 
    entries[bin]++; 
 
    // Set 
    fPHInfo->SetSum(sum); 
    fPHInfo->SetSumSquare(sumsquare); 
    fPHInfo->SetEntries(entries); 
 
    // Update the vector 
    fVectorPH->AddAt((TObject *) fPHInfo,place); 
 
  } 
 
  return kTRUE; 
 
} 
   
//_____________________________________________________________________________ 
TGraphErrors *IlcDCHCalibra::ConvertVectorPHisto(IlcDCHPInfo *pInfo 
                                               , const Char_t *name) const 
{ 
  // 
  // Convert the PInfo in a 1D grapherror, name must contains "PRF" 
  // if PRF calibration and not "PRF" for Vdrift calibration 
  // 
 
  TGraphErrors *histo; 
  const Char_t *pattern1 = "PRF"; 
 
  // Axis 
  Double_t *x; 
  Double_t *y; 
  Double_t *ex; 
  Double_t *ey; 
  Double_t step = 0.0; 
  Double_t min  = 0.0; 
 
  // Ntimes 
  Int_t ntimes = 0; 
  if (strstr(name,pattern1)) { 
    ntimes = fNumberBinPRF; 
  } 
  else { 
    ntimes = fTimeMax; 
  } 
  x  = new Double_t[ntimes]; // Xaxis 
  y  = new Double_t[ntimes]; // Mean 
  ex = new Double_t[ntimes]; // Nentries 
  ey = new Double_t[ntimes]; // Sum of square/nentries 
 
  // Init histo 
  if (!strstr(name,pattern1)) { 
    step = 1.0 / fSf; 
    min  = 0.0; 
  } 
  else { 
    step = (1.0 - (-1.0)) / fNumberBinPRF; 
    min  = -1.0 + step / 2.0; 
  } 
 
  // Fill histo 
  for (Int_t k = 0; k < ntimes; k++) { 
    x[k]  = min + k*step; 
    y[k]  = 0.0; 
    ex[k] = 0.0; 
    ey[k] = 0.0; 
    // Fill only if there is more than 0 something 
    if (pInfo->GetEntries()[k] > 0) { 
      ex[k] = pInfo->GetEntries()[k]; 
      y[k]  = pInfo->GetSum()[k]; 
      ey[k] =  pInfo->GetSumSquare()[k]; 
    } 
  } 
 
  // Define the TGraphErrors 
  histo = new TGraphErrors(ntimes,x,y,ex,ey); 
  histo->SetTitle(name);  
  return histo; 
 
} 
  
//_____________________________________________________________________________ 
TH1F *IlcDCHCalibra::ConvertVectorCTHisto(IlcDCHCTInfo *cTInfo 
                                        , const Char_t * name) const 
{ 
  // 
  // Convert the CTInfo in a 1D histo 
  // 
 
  TH1F *histo; 
   
  Int_t     ntimes  = fNumberBinCharge; 
  UShort_t *entries = cTInfo->GetEntries(); 
   
  // Init histo 
  histo = new TH1F(name,name,fNumberBinCharge,0,300); 
  histo->Sumw2(); 
  // Fill histo 
  for (Int_t k = 0; k < ntimes; k++) { 
    histo->SetBinContent(k+1,entries[k]); 
    histo->SetBinError(k+1,TMath::Sqrt(TMath::Abs(entries[k]))); 
  } 
   
  return histo; 
 
} 
 
//_____________________________________________________________________________ 
TTree *IlcDCHCalibra::ConvertVectorCTTreeHisto(TObjArray *vVectorCT 
                                             , TObjArray *pPlaCT 
                                             , const Char_t *name 
                                             , const Char_t *nametitle) const 
{ 
  // 
  // Convert the vector in a tree with two branchs: the group number 
  // and the TH1F histo reconstructed from the vector 
  // 
 
  // Size of the things 
  Int_t ntotal = (Int_t) pPlaCT->GetEntriesFast(); 
  if (ntotal == 0) { 
    IlcInfo("nothing to write!"); 
    TTree *treeCT = new TTree(name,nametitle); 
    return treeCT; 
  } 
   
  // Variable of the tree 
  Int_t groupnumber = -1; // Group calibration 
  TH1F      *histo = 0x0; 
  TObjArray  vectorCT = *vVectorCT; 
  TObjArray  plaCT    = *pPlaCT; 
 
  // Init the tree 
  TTree *treeCT = new TTree(name,nametitle); 
  treeCT->Branch("groupnumber",&groupnumber,"groupnumber/I"); 
  treeCT->Branch("histo","TH1F",&histo,32000,0); 
 
  // Fill 
  Int_t k = 0; 
  while (k < ntotal) { 
    TString nome(name); 
    groupnumber  = ((IlcDCHPlace *) plaCT.At(0))->GetPlace(); 
    nome        += groupnumber; 
    histo        = ConvertVectorCTHisto(((IlcDCHCTInfo *) vectorCT.At(0)),nome); 
    treeCT->Fill(); 
    vectorCT.RemoveAt(0); 
    vectorCT.Compress(); 
    plaCT.RemoveAt(0); 
    plaCT.Compress(); 
    k++; 
  }  
 
  return treeCT; 
 
} 
 
//_____________________________________________________________________________ 
TTree *IlcDCHCalibra::ConvertVectorPTreeHisto(TObjArray *vVectorP 
                                            , TObjArray *pPlaP 
                                            , const Char_t *name 
                                            , const Char_t *nametitle) const 
{ 
  // 
  // Convert the vector in a tree with two branchs: the group number 
  // and the TGraphErrors histo reconstructed from the vector. 
  // The name must contain "PRF" for PRF calibration and not "PRF" 
  // for Vdrift calibration 
  // 
 
  // Size of the things 
  Int_t ntotal = (Int_t) pPlaP->GetEntriesFast(); 
  if (ntotal == 0) { 
    IlcInfo("nothing to write!"); 
    TTree *treeP = new TTree(name,nametitle); 
    return treeP; 
  } 
 
  // Variable of the tree 
  Int_t groupnumber = -1; // Group calibration 
  TGraphErrors *histo   = 0x0; 
  TObjArray     vectorP = *vVectorP; 
  TObjArray     plaP    = *pPlaP; 
 
  // Init the tree 
  TTree *treeP = new TTree(name,nametitle); 
  treeP->Branch("groupnumber",&groupnumber,"groupnumber/I"); 
  treeP->Branch("histo","TGraphErrors",&histo,32000,0); 
 
  // Fill 
  Int_t k = 0; 
  while (k < ntotal) { 
    TString nome(name); 
    groupnumber = ((IlcDCHPlace *) plaP.At(0))->GetPlace(); 
    nome       += groupnumber; 
    histo       = ConvertVectorPHisto((IlcDCHPInfo *) vectorP.At(0),nome); 
    treeP->Fill(); 
    vectorP.RemoveAt(0); 
    vectorP.Compress(); 
    plaP.RemoveAt(0); 
    plaP.Compress(); 
    k++; 
  }  
 
  return treeP; 
 
} 
 
//_____________________________________________________________________________ 
TObjArray *IlcDCHCalibra::ConvertTreeVector(TTree *tree) const 
{ 
  // 
  // Convert the branch groupnumber of the tree taken from 
  // DCH.calibration.root in case of vector method in a std::vector  
  // to be faster 
  // 
 
  // Initialise 
  TObjArray *vectorplace = new TObjArray(); 
   
  // Variable of the tree 
  Int_t groupnumber = -1; // Group calibration 
 
  // Set the branch 
  tree->SetBranchAddress("groupnumber",&groupnumber); 
     
  // Fill 
  Int_t ntotal = tree->GetEntries(); 
  for (Int_t k = 0; k < ntotal; k++) { 
    tree->GetEntry(k); 
    IlcDCHPlace *placegroupnumber = new IlcDCHPlace(); 
    placegroupnumber->SetPlace(groupnumber); 
    vectorplace->Add((TObject *) placegroupnumber); 
  } 
   
  return vectorplace; 
 
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::MergeVectorCT(TObjArray *vVectorCT2, TObjArray *pPlaCT2) 
{ 
  // 
  // Add the two vectors and place the result in the first 
  // 
 
  if (((Int_t) pPlaCT2->GetEntriesFast()) != ((Int_t) vVectorCT2->GetEntriesFast())){ 
    IlcInfo("VectorCT2 doesn't correspond to PlaCT2!"); 
    return kFALSE; 
  } 
  
  // CH case 
  for (Int_t k = 0; k < (Int_t) fPlaCH->GetEntriesFast(); k++) { 
     
    // Look if PlaCT1[k] it is also in the second vector 
    Int_t place = -1; 
    for (Int_t j = 0; j < (Int_t) pPlaCT2->GetEntriesFast(); j++) { 
      if (((IlcDCHPlace *) pPlaCT2->At(j))->GetPlace() ==  
            ((IlcDCHPlace *) fPlaCH->At(k))->GetPlace()) { 
	place = j; 
	break; 
      } 
    } 
     
    // If not in the second vector nothing to do 
 
    // If in the second vector 
    if (place != -1) { 
       
      IlcDCHCTInfo *fCTInfo = new IlcDCHCTInfo(); 
      UShort_t *entries = new UShort_t[fNumberBinCharge]; 
       
      for (Int_t nu = 0; nu < fNumberBinCharge; nu++) { 
	entries[nu] = ((IlcDCHCTInfo *)  fVectorCH->At(((IlcDCHPlace *) fPlaCH->At(k))->GetPlace()))->GetEntries()[nu] 
                    + ((IlcDCHCTInfo *) vVectorCT2->At(((IlcDCHPlace *) fPlaCH->At(k))->GetPlace()))->GetEntries()[nu]; 
      } 
       
      // Set 
      fCTInfo->SetEntries(entries); 
 
      // Nothing to do on PlaCT1 
       
      // Update the vector  
      fVectorCH->AddAt((TObject *) fCTInfo,((IlcDCHPlace *) fPlaCH->At(k))->GetPlace()); 
 
    } 
     
  }  
  
  // And at the end the vector in CT2 but not in CH1 
  for (Int_t k = 0; k < (Int_t) pPlaCT2->GetEntriesFast(); k++) { 
     
    // Look if pPlaCT2[k] it is also in the second vector 
    Int_t place = -1; 
    for (Int_t j = 0; j < (Int_t) fPlaCH->GetEntriesFast(); j++) { 
      if (((IlcDCHPlace *) fPlaCH->At(j))->GetPlace() == ((IlcDCHPlace *) pPlaCT2->At(k))->GetPlace()) { 
	place = j; 
	break; 
      } 
    } 
 
    // If not in the first vector 
    if (place == -1) { 
       
      IlcDCHCTInfo *fCTInfo = new IlcDCHCTInfo();      
      fCTInfo = ((IlcDCHCTInfo *) vVectorCT2->At(((IlcDCHPlace *) pPlaCT2->At(k))->GetPlace())); 
       
      // Add at the end  
      fPlaCH->Add((TObject *) (pPlaCT2->At(k))); 
      fVectorCH->Add((TObject *) fCTInfo); 
 
    } 
     
  } 
   
  return kTRUE; 
   
} 
 
//_____________________________________________________________________________ 
Bool_t IlcDCHCalibra::MergeVectorP(TObjArray *vVectorP2 
                                 , TObjArray *pPlaP2 
                                 , Int_t i) 
{ 
  // 
  // Add the two vectors and place the result in the first 
  // 
 
  if (((Int_t) pPlaP2->GetEntriesFast()) != ((Int_t) vVectorP2->GetEntriesFast())) { 
    IlcInfo("VectorP2 doesn't correspond to PlaP2!"); 
    return kFALSE; 
  } 
 
  // PH case 
  if (i == 1) { 
 
     for (Int_t k = 0; k < (Int_t) fPlaPH->GetEntriesFast(); k++) { 
        
       // Look if fPlaPH[k] it is also in the second vector 
       Int_t place = -1; 
       for (Int_t j = 0; j < (Int_t) pPlaP2->GetEntriesFast(); j++) { 
	 if (((IlcDCHPlace *) pPlaP2->At(j))->GetPlace() == ((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()) { 
	   place = j; 
	   break; 
	 } 
       } 
        
       // If not in the second vector nothing to do 
 
       // If in the second vector 
       if (place != -1) { 
 
	 IlcDCHPInfo *fPInfo = new IlcDCHPInfo(); 
	 UShort_t *entries   = new UShort_t[fTimeMax]; 
	 Float_t  *sum       = new Float_t[fTimeMax]; 
	 Float_t  *sumsquare = new Float_t[fTimeMax]; 
 
	 for (Int_t nu = 0; nu < fTimeMax; nu++) { 
	    
	   entries[nu]   = ((IlcDCHPInfo *) fVectorPH->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu] 
                         + ((IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu]; 
	    
	   Double_t calcul       = ((((Double_t) ((IlcDCHPInfo *) fVectorPH->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetSum()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) fVectorPH->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu])) 
                                  + (((Double_t) ((IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetSum()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu]))) 
                                 / ((Double_t) fPInfo->GetEntries()[nu]); 
 
	   sum[nu]       = (Float_t) calcul; 
	    
	   Double_t calculsquare = ((((Double_t) ((IlcDCHPInfo *) fVectorPH->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetSumSquare()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) fVectorPH->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu])) 
                                  + (((Double_t) ((IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetSumSquare()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()))->GetEntries()[nu]))) 
                                 / ((Double_t) fPInfo->GetEntries()[nu]); 
	    
	    
	   sumsquare[nu] = calculsquare; 
 
	 } 
 
	 // Set 
	 fPInfo->SetSum(sum); 
	 fPInfo->SetSumSquare(sumsquare); 
	 fPInfo->SetEntries(entries); 
	  
	 // Nothing to do on PlaCT1 
	  
	 // Update the vector VectorCT1 
	 fVectorPH->AddAt((TObject *) fPInfo,((IlcDCHPlace *) fPlaPH->At(k))->GetPlace()); 
	  
       } 
 
     } 
 
     // And at the end the vector in P2 but not in CH1 
     for (Int_t k = 0; k < (Int_t) pPlaP2->GetEntriesFast(); k++) { 
        
       // Look if PlaCT2[k] it is also in the second vector 
       Int_t place = -1; 
       for (Int_t j = 0; j < (Int_t) fPlaPH->GetEntriesFast(); j++) { 
	 if (((IlcDCHPlace *) fPlaPH->At(j))->GetPlace() == ((IlcDCHPlace *) pPlaP2->At(k))->GetPlace()) { 
	   place = j; 
	   break; 
	 } 
       } 
        
       // If not in the first vector 
       if (place == -1) { 
	 	  
	 IlcDCHPInfo *fPInfo = new IlcDCHPInfo(); 
	 fPInfo = (IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) pPlaP2->At(k))->GetPlace()); 
	  
	 // Add at the end of CH1 
	 fPlaPH->Add(((TObject *) pPlaP2->At(k))); 
	 fVectorPH->Add((TObject *) fPInfo); 
 
       } 
 
     } 
 
   } 
    
 
   // PRF case 
   if (i == 1) { 
 
     for (Int_t k = 0; k < (Int_t) fPlaPRF->GetEntriesFast(); k++) { 
 
       // Look if fPlaPRF[k] it is also in the second vector 
       Int_t place = -1; 
       for (Int_t j = 0; j < (Int_t) pPlaP2->GetEntriesFast(); j++) { 
	 if (((IlcDCHPlace *) pPlaP2->At(j))->GetPlace() == ((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()) { 
	   place = j; 
	   break; 
	 } 
       } 
 
       // If not in the second vector nothing to do 
 
       // If in the second vector 
       if (place != -1) { 
	 
	 IlcDCHPInfo *fPInfo = new IlcDCHPInfo(); 
	 UShort_t *entries   = new UShort_t[fNumberBinPRF]; 
	 Float_t  *sum       = new Float_t[fNumberBinPRF]; 
	 Float_t  *sumsquare = new Float_t[fNumberBinPRF]; 
 
	 for (Int_t nu = 0; nu < fNumberBinPRF; nu++) { 
	    
	   entries[nu]           = ((IlcDCHPInfo *) fVectorPRF->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu] 
                                 + ((IlcDCHPInfo *)  vVectorP2->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu]; 
	    
	   Double_t calcul       = ((((Double_t) ((IlcDCHPInfo *) fVectorPRF->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetSum()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) fVectorPRF->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu])) 
                                  + (((Double_t) ((IlcDCHPInfo *)  vVectorP2->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetSum()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *)  vVectorP2->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu]))) 
                                 / ((Double_t) fPInfo->GetEntries()[nu]); 
 
	   sum[nu]               = (Float_t) calcul; 
	    
	   Double_t calculsquare = ((((Double_t) ((IlcDCHPInfo *) fVectorPRF->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetSumSquare()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *) fVectorPRF->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu])) 
                                  + (((Double_t) ((IlcDCHPInfo *)  vVectorP2->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetSumSquare()[nu]) 
                                   * ((Double_t) ((IlcDCHPInfo *)  vVectorP2->At(((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()))->GetEntries()[nu]))) 
                                 / ((Double_t) fPInfo->GetEntries()[nu]); 
 
	   sumsquare[nu]         = calculsquare; 
 
	 } 
 
	 // Set 
	 fPInfo->SetSum(sum); 
	 fPInfo->SetSumSquare(sumsquare); 
	 fPInfo->SetEntries(entries); 
 
	 // Nothing to do on PlaCT1 
	  
	 // Update the vector VectorCT1 
	 fVectorPRF->AddAt((TObject *) fPInfo,((IlcDCHPlace *) fPlaPRF->At(k))->GetPlace()); 
	  
       } 
 
     } 
 
     // And at the end the vector in P2 but not in CH1 
     for (Int_t k = 0; k < (Int_t) pPlaP2->GetEntriesFast(); k++) { 
        
       // Look if PlaCT2[k] it is also in the second vector 
       Int_t place = -1; 
       for (Int_t j = 0; j < (Int_t) fPlaPRF->GetEntriesFast(); j++) { 
	 if (((IlcDCHPlace *) fPlaPRF->At(j))->GetPlace() == ((IlcDCHPlace *) pPlaP2->At(k))->GetPlace()) { 
	   place = j; 
	   break; 
	 } 
       } 
 
       // If not in the first vector 
       if (place == -1) { 
 
	 IlcDCHPInfo *fPInfo = new IlcDCHPInfo(); 
	 fPInfo = (IlcDCHPInfo *) vVectorP2->At(((IlcDCHPlace *) pPlaP2->At(k))->GetPlace()); 
 
	 // Add at the end of CH1 
	 fPlaPRF->Add(((TObject *) pPlaP2->At(k))); 
	 fVectorPRF->Add((TObject *) fPInfo); 
 
       } 
        
     } 
 
   }  
 
   return kTRUE; 
 
} 
 
//____________Fit Methods______________________________________________________ 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FitPente(TH1* projPH, Int_t idect) 
{ 
  // 
  // Slope methode for the drift velocity 
  // 
   
  // Constants 
  const Float_t kDrWidth = IlcDCHgeometry::DrThick(); 
  Int_t binmax   = 0; 
  Int_t binmin   = 0; 
  fPhd[0]        = 0.0; 
  fPhd[1]        = 0.0; 
  fPhd[2]        = 0.0; 
  Int_t ju       = 0; 
  fVdriftCoef[1] = 0.0; 
  fT0Coef[1]     = 0.0; 
  TLine *line = new TLine(); 
 
  // Some variables 
  TAxis   *xpph    = projPH->GetXaxis(); 
  Int_t    nbins   = xpph->GetNbins(); 
  Double_t lowedge = xpph->GetBinLowEdge(1); 
  Double_t upedge  = xpph->GetBinUpEdge(xpph->GetNbins()); 
  Double_t widbins = (upedge - lowedge) / nbins; 
  Double_t limit   = upedge + 0.5 * widbins;  
 
  // Beginning of the signal 
  TH1D *pentea = new TH1D("pentea","pentea",projPH->GetNbinsX(),0,(Float_t) limit); 
  for (Int_t k = 1; k <  projPH->GetNbinsX(); k++) { 
    pentea->SetBinContent(k,(Double_t) (projPH->GetBinContent(k+1) - projPH->GetBinContent(k))); 
  } 
 
  binmax = (Int_t) pentea->GetMaximumBin(); 
  if (binmax == 1) { 
    binmax = 2; 
    IlcInfo("Put the binmax from 1 to 2 to enable the fit"); 
  } 
  pentea->Fit("pol2","0MR","",TMath::Max(pentea->GetBinCenter(binmax-1),0.0),pentea->GetBinCenter(binmax+1)); 
  Float_t l3P1am = pentea->GetFunction("pol2")->GetParameter(1); 
  Float_t l3P2am = pentea->GetFunction("pol2")->GetParameter(2); 
  if (l3P2am != 0) { 
    fPhd[0] = -(l3P1am / (2 * l3P2am)); 
  } 
   
  // Amplification region 
  binmax = 0; 
  ju     = 0; 
  for (Int_t kbin = 1; kbin < projPH->GetNbinsX(); kbin ++) { 
    if (((projPH->GetBinContent(kbin+1) - projPH->GetBinContent(kbin)) <= 0.0) && (ju == 0)) { 
      binmax = kbin; 
      ju     = 1; 
    } 
  } 
  if (binmax == 1) { 
    binmax = 2; 
    IlcInfo("Put the binmax from 1 to 2 to enable the fit"); 
  } 
  projPH->Fit("pol2","0MR","",TMath::Max(projPH->GetBinCenter(binmax-1),0.0),projPH->GetBinCenter(binmax+1)); 
  Float_t l3P1amf = projPH->GetFunction("pol2")->GetParameter(1); 
  Float_t l3P2amf = projPH->GetFunction("pol2")->GetParameter(2); 
   
  if (l3P2amf != 0) { 
    fPhd[1] = -(l3P1amf / (2 * l3P2amf)); 
  } 
 
  // Drift region 
  TH1D *pente = new TH1D("pente","pente",projPH->GetNbinsX(),0,(Float_t) limit); 
  for (Int_t k = binmax+4; k <  projPH->GetNbinsX(); k++) { 
    pente->SetBinContent(k,(Double_t) (projPH->GetBinContent(k+1) - projPH->GetBinContent(k))); 
  } 
  binmin = (Int_t) pente->GetMinimumBin(); 
  if (binmin == 1) { 
    binmin = 2; 
    IlcInfo("Put the binmax from 1 to 2 to enable the fit"); 
  } 
  pente->Fit("pol2" 
            ,"0MR" 
            ,"" 
            ,TMath::Max(pente->GetBinCenter(binmin-1),             0.0) 
            ,TMath::Min(pente->GetBinCenter(binmin+2),(Double_t) limit)); 
  Float_t l3P1dr = pente->GetFunction("pol2")->GetParameter(1); 
  Float_t l3P2dr = pente->GetFunction("pol2")->GetParameter(2); 
  if (l3P2dr != 0) { 
    fPhd[2] = -(l3P1dr / (2 * l3P2dr)); 
  } 
 
  if ((fPhd[2] > fPhd[0]) &&  
      (fPhd[2] > fPhd[1]) &&  
      (fPhd[1] > fPhd[0])) { 
    fVdriftCoef[1] = (kDrWidth) / (fPhd[2]-fPhd[1]); 
    if (fPhd[0] >= 0.0) { 
      fT0Coef[1] = (fPhd[0] - fT0Shift) / widbins; 
      if (fT0Coef[1] <= -1.0) { 
        fT0Coef[1] = - TMath::Abs(fT0Coef[2]); 
      } 
    } 
    else { 
      fT0Coef[1] = -TMath::Abs(fT0Coef[2]); 
    } 
  } 
  else { 
    fVdriftCoef[1] = -TMath::Abs(fVdriftCoef[2]); 
    fT0Coef[1]     = -TMath::Abs(fT0Coef[2]); 
  } 
 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    fCoefVdrift[1]->SetBinContent(idect+1,TMath::Abs(fVdriftCoef[1])); 
    fCoefT0[1]->SetBinContent(idect+1,fT0Coef[1]); 
    if (fVdriftCoef[1] > 0.0) { 
      if (fVdriftCoef[2] != 0.0) { 
        fDeltaVdrift[1]->SetBinContent(idect+1,(fVdriftCoef[1] - fVdriftCoef[2]) / fVdriftCoef[2]); 
      } 
      fDeltaT0[1]->SetBinContent(idect+1,(fT0Coef[1] - fT0Coef[2])); 
    } 
  } 
   
  if (fDebug == 2) { 
    TCanvas *cpentei = new TCanvas("cpentei","cpentei",50,50,600,800); 
    cpentei->cd(); 
    projPH->Draw(); 
    line->SetLineColor(2); 
    line->DrawLine(fPhd[0],0,fPhd[0],projPH->GetMaximum()); 
    line->DrawLine(fPhd[1],0,fPhd[1],projPH->GetMaximum()); 
    line->DrawLine(fPhd[2],0,fPhd[2],projPH->GetMaximum()); 
    IlcInfo(Form("fPhd[0] (beginning of the signal): %f"                  ,(Float_t) fPhd[0])); 
    IlcInfo(Form("fPhd[1] (end of the amplification region): %f"          ,(Float_t) fPhd[1])); 
    IlcInfo(Form("fPhd[2] (end of the drift region): %f"                  ,(Float_t) fPhd[2])); 
    IlcInfo(Form("fVriftCoef[1] (with only the drift region(default)): %f",(Float_t) fVdriftCoef[1])); 
  } 
 
  if (fDebug != 2) { 
    delete pentea; 
  } 
  if (fDebug != 2) { 
    delete pente; 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FitPH(TH1* projPH, Int_t idect) 
{ 
  // 
  // Fit methode for the drift velocity 
  // 
   
  // Constants 
  const Float_t kDrWidth = IlcDCHgeometry::DrThick();   
 
  // Some variables 
  TAxis   *xpph   = projPH->GetXaxis(); 
  Double_t upedge = xpph->GetBinUpEdge(xpph->GetNbins()); 
 
  TF1 *fPH = new TF1("fPH",IlcDCHCalibra::PH,-0.05,3.2,6); 
  fPH->SetParameter(0,0.469);     // Scaling 
  fPH->SetParameter(1,0.18);      // Start  
  fPH->SetParameter(2,0.0857325); // AR 
  fPH->SetParameter(3,1.89);      // DR 
  fPH->SetParameter(4,0.08);      // QA/QD 
  fPH->SetParameter(5,0.0);       // Baseline 
 
  TLine *line = new TLine(); 
 
  fVdriftCoef[0] = 0.0; 
  fT0Coef[0]     = 0.0; 
  
  if (idect%fFitPHPeriode == 0) { 
 
    IlcInfo(Form("<IlcDCHCalibra::FitPH> The detector %d will be fitted",idect)); 
    fPH->SetParameter(0,(projPH->Integral()-(projPH->GetBinContent(1)*projPH->GetNbinsX())) * 0.00028); // Scaling 
    fPH->SetParameter(1,fPhd[0] - 0.1);                                                                 // Start  
    fPH->SetParameter(2,fPhd[1] - fPhd[0]);                                                             // AR 
    fPH->SetParameter(3,fPhd[2] - fPhd[1]);                                                             // DR 
    fPH->SetParameter(4,0.225);                                                                         // QA/QD 
    fPH->SetParameter(5,(Float_t) projPH->GetBinContent(1)); 
     
    if (fDebug != 2) { 
      projPH->Fit(fPH,"0M","",0.0,upedge); 
    } 
 
    if (fDebug == 2) { 
      TCanvas *cpente = new TCanvas("cpente","cpente",50,50,600,800); 
      cpente->cd(); 
      projPH->Fit(fPH,"M+","",0.0,upedge); 
      projPH->Draw("E0"); 
      line->SetLineColor(4); 
      line->DrawLine(fPH->GetParameter(1) 
                    ,0 
                    ,fPH->GetParameter(1) 
                    ,projPH->GetMaximum()); 
      line->DrawLine(fPH->GetParameter(1)+fPH->GetParameter(2) 
                    ,0 
                    ,fPH->GetParameter(1)+fPH->GetParameter(2) 
                    ,projPH->GetMaximum()); 
      line->DrawLine(fPH->GetParameter(1)+fPH->GetParameter(2)+fPH->GetParameter(3) 
                    ,0 
                    ,fPH->GetParameter(1)+fPH->GetParameter(2)+fPH->GetParameter(3) 
                    ,projPH->GetMaximum()); 
    } 
 
    if (fPH->GetParameter(3) != 0) { 
      fVdriftCoef[0] = kDrWidth / (fPH->GetParameter(3)); 
      fT0Coef[0]     = fPH->GetParameter(1); 
    }  
    else { 
      fVdriftCoef[0] = -TMath::Abs(fVdriftCoef[2]); 
      fT0Coef[0]     = -TMath::Abs(fT0Coef[2]); 
    } 
 
    if ((fDebug == 1) ||  
        (fDebug == 4)) { 
      fCoefVdrift[0]->SetBinContent(idect+1,TMath::Abs(fVdriftCoef[0])); 
      fCoefT0[0]->SetBinContent(idect+1,TMath::Abs(fT0Coef[0])); 
    if (fVdriftCoef[0] > 0.0){ 
	if (fVdriftCoef[2] != 0.0) { 
          fDeltaVdrift[0]->SetBinContent(idect+1,(fVdriftCoef[0]-fVdriftCoef[2])/fVdriftCoef[2]); 
	} 
	fDeltaT0[0]->SetBinContent(idect+1,(fT0Coef[0]-fT0Coef[2])); 
      } 
    } 
    if (fDebug == 2) { 
      IlcInfo(Form("fVdriftCoef[0]: %f",(Float_t) fVdriftCoef[0])); 
    } 
 
  } 
 
  else { 
 
    // Put the default value  
    if ((fDebug <= 1) ||  
        (fDebug  = 4)) { 
      fCoefVdrift[0]->SetBinContent(idect+1,fVdriftCoef[2]); 
      fCoefT0[0]->SetBinContent(idect+1,fT0Coef[2]); 
    } 
 
  } 
 
  if (fDebug != 2) { 
    delete fPH; 
  } 
   
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FitPRF(TH1 *projPRF, Int_t idect) 
{ 
  // 
  // Fit methode for the sigma of the pad response function 
  // 
   
  fPRFCoef[0] = 0.0; 
 
  if (fDebug != 2) { 
    projPRF->Fit("gaus","0M","",-fRangeFitPRF,fRangeFitPRF); 
  } 
   
  if (fDebug == 2) { 
    TCanvas *cfit = new TCanvas("cfit","cfit",50,50,600,800); 
    cfit->cd(); 
    projPRF->Fit("gaus","M+","",-fRangeFitPRF,fRangeFitPRF); 
    projPRF->Draw(); 
  } 
 
  fPRFCoef[0] = projPRF->GetFunction("gaus")->GetParameter(2); 
 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    fCoefPRF[0]->SetBinContent(idect+1,fPRFCoef[0]); 
    if (fPRFCoef[1] != 0.0) { 
      fDeltaPRF->SetBinContent(idect+1,(fPRFCoef[0]-fPRFCoef[1])/fPRFCoef[1]); 
    } 
  } 
  if (fDebug == 2) { 
    IlcInfo(Form("fPRFCoef[0]: %f",(Float_t) fPRFCoef[0])); 
  } 
   
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FitCH(TH1 *projch, Int_t idect) 
{ 
  // 
  // Fit methode for the gain factor 
  // 
  
  fChargeCoef[0] = 0.0; 
  fChargeCoef[1] = 0.0; 
  TF1 *fLandauGaus = new TF1("fLandauGaus",FuncLandauGaus,0,300,5); 
 
  fChargeCoef[1] = projch->GetMean(); 
  projch->Fit("landau","0","" 
             ,(Float_t) fChargeCoef[1]/fBeginFitCharge 
             ,projch->GetBinCenter(projch->GetNbinsX())); 
  fL3P0         = projch->GetFunction("landau")->GetParameter(0); 
  Double_t l3P1 = projch->GetFunction("landau")->GetParameter(1); 
  fL3P2         = projch->GetFunction("landau")->GetParameter(2); 
     
  projch->Fit("gaus","0","" 
             ,(Float_t) fChargeCoef[1]/fBeginFitCharge 
             ,projch->GetBinCenter(projch->GetNbinsX())); 
  Double_t g3P0 = projch->GetFunction("gaus")->GetParameter(0); 
  fG3P2         = projch->GetFunction("gaus")->GetParameter(2); 
         
  fLandauGaus->SetParameters(fL3P0,l3P1,fL3P2,g3P0,fG3P2); 
  if ((fDebug <= 1) ||  
      (fDebug >= 3)) { 
    projch->Fit("fLandauGaus","0","" 
               ,(Float_t) fChargeCoef[1]/fBeginFitCharge 
               ,projch->GetBinCenter(projch->GetNbinsX())); 
  } 
 
  if (fDebug == 2) { 
    TCanvas *cp = new TCanvas("cp","cp",50,50,600,800); 
    cp->cd(); 
    projch->Fit("fLandauGaus","+","" 
               ,(Float_t) fChargeCoef[1]/fBeginFitCharge 
               ,projch->GetBinCenter(projch->GetNbinsX())); 
    projch->Draw(); 
    fLandauGaus->Draw("same"); 
  } 
   
  if (projch->GetFunction("fLandauGaus")->GetParameter(1) > 0) { 
    // Calcul of "real" coef 
    CalculChargeCoefMean(fCountDet[0],(Int_t) idect,kTRUE); 
    fChargeCoef[0] = projch->GetFunction("fLandauGaus")->GetParameter(1); 
  } 
  else { 
    // Calcul of "real" coef 
    CalculChargeCoefMean(fCountDet[0],(Int_t) idect,kFALSE); 
    fChargeCoef[0] = -TMath::Abs(fChargeCoef[3]); 
  } 
 
  if (fDebug == 2) { 
    IlcInfo(Form("fChargeCoef[0]: %f",(Float_t) fChargeCoef[0])); 
    IlcInfo(Form("fChargeCoef[1]: %f",(Float_t) fChargeCoef[1])); 
  } 
   
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    if (fChargeCoef[0] > 0.0) { 
      fCoefCharge[0]->SetBinContent(idect+1,fChargeCoef[0]); 
      fCoefCharge[1]->SetBinContent(idect+1,fChargeCoef[1]); 
      fDeltaCharge[0]->SetBinContent(idect+1,fChargeCoef[0]); 
      fDeltaCharge[1]->SetBinContent(idect+1,fChargeCoef[1]); 
    } 
  } 
  fL3P0 = fLandauGaus->Integral(0.3*projch->GetMean(),3*projch->GetMean()); 
  fG3P2 = fLandauGaus->GetParameter(2); 
  fL3P2 = fLandauGaus->GetParameter(4); 
    
  if (fDebug != 2) { 
    delete fLandauGaus; 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FitBisCH(TH1* projch, Int_t idect) 
{ 
  // 
  // Fit methode for the gain factor more time consuming 
  // 
 
  // Setting fit range and start values 
  Double_t fr[2]; 
  //Double_t sv[4] = { l3P2, fChargeCoef[1], projch->Integral("width"), fG3P2 }; 
  Double_t sv[4]   = { fL3P2, fChargeCoef[1], fL3P0, fG3P2 }; 
  Double_t pllo[4] = { 0.001, 0.001, 0.001, 0.001 }; 
  Double_t plhi[4] = { 300.0, 300.0, 100000000.0, 300.0 }; 
  Double_t fp[4]   = { 1.0, 1.0, 1.0, 1.0 }; 
  Double_t fpe[4]  = { 1.0, 1.0, 1.0, 1.0 }; 
  fr[0]            = 0.3 * projch->GetMean(); 
  fr[1]            = 3.0 * projch->GetMean(); 
  fChargeCoef[2]   = 0.0; 
 
  Double_t chisqr; 
  Int_t    ndf; 
  TF1 *fitsnr = LanGauFit(projch,&fr[0],&sv[0] 
                                ,&pllo[0],&plhi[0] 
                                ,&fp[0],&fpe[0] 
                                ,&chisqr,&ndf); 
     
  Double_t projchPeak; 
  Double_t projchFWHM; 
  LanGauPro(fp,projchPeak,projchFWHM); 
 
  if (fp[1] > 0) { 
    fChargeCoef[2] = fp[1]; 
  }  
  else { 
    fChargeCoef[2] = -TMath::Abs(fChargeCoef[3]); 
  } 
   
  if (fDebug == 2) { 
    IlcInfo(Form("fChargeCoef[2]: %f",(Float_t) fChargeCoef[2])); 
    TCanvas *cpy = new TCanvas("cpy","cpy",50,50,600,800); 
    cpy->cd(); 
    projch->Draw(); 
    fitsnr->Draw("same"); 
  } 
 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    if (fChargeCoef[2] > 0.0) { 
      fCoefCharge[2]->SetBinContent(idect+1,fChargeCoef[2]); 
      fDeltaCharge[2]->SetBinContent(idect+1,fChargeCoef[2]); 
    } 
  } 
 
  if (fDebug != 2) { 
    delete fitsnr; 
  } 
 
}  
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::NormierungCharge() 
{ 
  // 
  // Normilcsation of the gain factor resulting for the fits 
  // 
   
  // Calcul of the mean of the fit 
  Double_t sum         = 0.0; 
  Float_t  scalefactor = 1.0; 
  for (Int_t k = 0; k < (Int_t) fVectorFitCH->GetEntriesFast(); k++) { 
    Int_t    total    = 0; 
    Int_t    detector = ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetDetector(); 
    Float_t *coef     = ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetCoef(); 
    if (GetChamber(detector) == 2) { 
      total = 1728; 
    } 
    if (GetChamber(detector) != 2) { 
      total = 2304; 
    } 
    for (Int_t j = 0; j < total; j++) { 
      if (coef[j] >= 0) { 
	sum += coef[j]; 
      } 
    } 
  } 
 
  if (sum > 0) { 
    fScaleFitFactor = fScaleFitFactor / sum; 
  } 
  else { 
    fScaleFitFactor = 1.0; 
  } 
 
  // Scale the histo 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
    if ((fCoefCharge[0]->GetEntries()      > 0.0) &&  
        (fCoefCharge[0]->GetSumOfWeights() > 0.0)) { 
      scalefactor = fCoefCharge[0]->GetEntries() / fCoefCharge[0]->GetSumOfWeights(); 
      fCoefCharge[0]->Scale(scalefactor); 
      fDeltaCharge[0]->Scale(scalefactor); 
    } 
    if ((fMeanChargeOn) &&  
        (fCoefCharge[1]->GetEntries()      > 0.0) &&  
        (fCoefCharge[1]->GetSumOfWeights() > 0.0)) { 
      fCoefCharge[1]->Scale(fCoefCharge[1]->GetEntries() / fCoefCharge[1]->GetSumOfWeights()); 
    } 
    if ((fFitChargeBisOn) &&  
        (fCoefCharge[2]->GetEntries()      > 0.0) &&  
        (fCoefCharge[2]->GetSumOfWeights() > 0.0)) { 
      fCoefCharge[2]->Scale(fCoefCharge[2]->GetEntries() / fCoefCharge[2]->GetSumOfWeights()); 
    } 
    if ((fMeanChargeOn) &&  
        (fDeltaCharge[1]->GetEntries()      > 0.0) &&  
        (fDeltaCharge[1]->GetSumOfWeights() > 0.0)) { 
      fDeltaCharge[1]->Scale(fDeltaCharge[1]->GetEntries() / fDeltaCharge[1]->GetSumOfWeights()); 
    } 
    if ((fFitChargeBisOn) &&  
        (fDeltaCharge[2]->GetEntries()      > 0.0) &&  
        (fDeltaCharge[2]->GetSumOfWeights() > 0.0)) { 
      fDeltaCharge[2]->Scale(fDeltaCharge[2]->GetEntries() / fDeltaCharge[2]->GetSumOfWeights()); 
    } 
  } 
 
  if ((fDebug == 3) ||  
      (fDebug == 4)) { 
    fCoefChargeDB[0]->Scale(scalefactor); 
    if ((fMeanChargeOn)  &&  
        (fCoefChargeDB[1]->GetEntries()      > 0.0) &&  
        (fCoefChargeDB[1]->GetSumOfWeights() > 0.0)) { 
      fCoefChargeDB[1]->Scale(fCoefChargeDB[1]->GetEntries() / fCoefChargeDB[1]->GetSumOfWeights()); 
    } 
    if ((fFitChargeBisOn) &&  
        (fCoefChargeDB[2]->GetEntries()      > 0.0) &&  
        (fCoefChargeDB[2]->GetSumOfWeights() > 0.0)) { 
      fCoefChargeDB[2]->Scale(fCoefChargeDB[2]->GetEntries() / fCoefChargeDB[2]->GetSumOfWeights()); 
    } 
  } 
 
  if ((fDebug == 1) ||  
      (fDebug == 4)) { 
   
    fDeltaCharge[0]->Add(fCoefCharge[3],-1); 
    fDeltaCharge[0]->Divide(fCoefCharge[3]); 
     
    if (fMeanChargeOn) { 
      fDeltaCharge[1]->Add(fCoefCharge[3],-1); 
    } 
    if (fMeanChargeOn) { 
      fDeltaCharge[1]->Divide(fCoefCharge[3]); 
    } 
     
    if (fFitChargeBisOn) { 
      fDeltaCharge[2]->Add(fCoefCharge[3],-1); 
    } 
    if (fFitChargeBisOn) { 
      fDeltaCharge[2]->Divide(fCoefCharge[3]); 
    } 
 
  } 
   
} 
 
//_____________________________________________________________________________ 
TH1I *IlcDCHCalibra::ReBin(TH1I *hist) const 
{ 
  // 
  // Rebin of the 1D histo for the gain calibration if needed. 
  // you have to choose fRebin, divider of fNumberBinCharge 
  // 
 
 TAxis *xhist  = hist->GetXaxis(); 
 TH1I  *rehist = new TH1I("projrebin","",(Int_t) xhist->GetNbins()/fRebin 
                                        ,xhist->GetBinLowEdge(1) 
                                        ,xhist->GetBinUpEdge(xhist->GetNbins())); 
 
 IlcInfo(Form("fRebin: %d",fRebin)); 
 Int_t i = 1; 
 for (Int_t k = 1; k <= (Int_t) xhist->GetNbins()/fRebin; k++) { 
   Double_t sum = 0.0; 
   for (Int_t ji = i; ji < i+fRebin; ji++) { 
     sum += hist->GetBinContent(ji); 
   } 
   sum = sum / fRebin; 
   rehist->SetBinContent(k,sum); 
   i += fRebin; 
 } 
 
 if (fDebug == 2) { 
   TCanvas *crebin = new TCanvas("crebin","",50,50,600,800); 
   crebin->cd(); 
   rehist->Draw(); 
 } 
 
 return rehist; 
 
} 
 
//_____________________________________________________________________________ 
TH1F *IlcDCHCalibra::ReBin(TH1F *hist) const 
{ 
  // 
  // Rebin of the 1D histo for the gain calibration if needed 
  // you have to choose fRebin divider of fNumberBinCharge 
  // 
 
  TAxis *xhist  = hist->GetXaxis(); 
  TH1F  *rehist = new TH1F("projrebin","",(Int_t) xhist->GetNbins()/fRebin 
                                         ,xhist->GetBinLowEdge(1) 
                                         ,xhist->GetBinUpEdge(xhist->GetNbins())); 
 
  IlcInfo(Form("fRebin: %d",fRebin)); 
  Int_t i = 1; 
  for (Int_t k = 1; k <= (Int_t) xhist->GetNbins()/fRebin; k++) { 
    Double_t sum = 0.0; 
    for (Int_t ji = i; ji < i+fRebin; ji++) { 
      sum += hist->GetBinContent(ji); 
    } 
    sum = sum/fRebin; 
    rehist->SetBinContent(k,sum); 
    i += fRebin; 
  } 
 
  if (fDebug == 2) { 
    TCanvas *crebin = new TCanvas("crebin","",50,50,600,800); 
    crebin->cd(); 
    rehist->Draw(); 
  } 
   
  return rehist; 
   
} 
 
//_____________________________________________________________________________ 
TH1F *IlcDCHCalibra::CorrectTheError(TGraphErrors *hist) 
{ 
  // 
  // In the case of the vectors method the trees contains TGraphErrors for PH and PRF 
  // to be able to add them after 
  // We convert it to a TH1F to be able to applied the same fit function method 
  // After having called this function you can not add the statistics anymore 
  // 
 
  TH1F *rehist = 0x0; 
 
  Int_t     nbins   = hist->GetN(); 
  Double_t *x       = hist->GetX(); 
  Double_t *entries = hist->GetEX(); 
  Double_t *mean    = hist->GetY(); 
  Double_t *square  = hist->GetEY(); 
  fEntriesCurrent   = 0; 
 
  if (nbins < 2) { 
    return rehist;  
  } 
 
  Double_t step     = x[1] - x[0];  
  Double_t minvalue = x[0] - step/2; 
  Double_t maxvalue = x[(nbins-1)] + step/2; 
 
  rehist = new TH1F("projcorrecterror","",nbins,minvalue,maxvalue); 
 
  for (Int_t k = 0; k < nbins; k++) { 
    rehist->SetBinContent(k+1,mean[k]); 
    if (entries[k] > 0.0) { 
      fEntriesCurrent += (Int_t) entries[k]; 
      Double_t d = TMath::Abs(square[k] - (mean[k]*mean[k])); 
      rehist->SetBinError(k+1,TMath::Sqrt(d/entries[k])); 
    } 
    else { 
      rehist->SetBinError(k+1,0.0); 
    } 
  } 
 
  return rehist; 
  
} 
 
//_____________________________________________________________________________ 
TGraphErrors *IlcDCHCalibra::AddProfiles(TGraphErrors *hist1 
                                       , TGraphErrors *hist2) const 
{ 
  // 
  // In the case of the vectors method we use TGraphErrors for PH and PRF 
  // to be able to add the them after 
  // Here we add the TGraphErrors   
  // 
 
  // First TGraphErrors 
  Int_t     nbins1 = hist1->GetN(); 
  Double_t *x1     = hist1->GetX(); 
  Double_t *ex1    = hist1->GetEX(); 
  Double_t *y1     = hist1->GetY(); 
  Double_t *ey1    = hist1->GetEY(); 
 
  TGraphErrors *rehist = new TGraphErrors(nbins1); 
 
  // Second TGraphErrors 
  Double_t *ex2    = hist2->GetEX(); 
  Double_t *y2     = hist2->GetY(); 
  Double_t *ey2    = hist2->GetEY(); 
 
  // Define the Variables for the new TGraphErrors 
  Double_t x; 
  Double_t ex; 
  Double_t y; 
  Double_t ey; 
   
  for (Int_t k = 0; k < nbins1; k++) { 
    Double_t nentries = 0.0; 
    x  = x1[k]; 
    y  = 0.0; 
    ey = 0.0; 
    ex = 0.0; 
    if ((ex2[k] == 0.0) &&  
        (ex1[k] == 0.0)) { 
      nentries = 0.0; 
    } 
    if ((ex2[k] == 0.0) &&  
        (ex1[k]  > 0.0)) { 
      nentries = ex1[k]; 
      y  = y1[k]; 
      ey = ey1[k]; 
      ex = ex1[k]; 
    } 
    if ((ex2[k]  > 0.0) &&  
        (ex1[k] == 0.0)) { 
      nentries = ex2[k]; 
      y  = y2[k]; 
      ey = ey2[k]; 
      ex = ex2[k]; 
    } 
    if ((ex2[k] > 0.0) &&  
        (ex1[k] > 0.0)) {  
     nentries = ex1[k] + ex2[k]; 
     y  = ( y1[k]*ex1[k]+ y2[k]*ex2[k]) / nentries; 
     ey = (ey1[k]*ex1[k]+ey2[k]*ex2[k]) / nentries; 
     ex = nentries; 
   } 
   rehist->SetPoint(k,x,y); 
   rehist->SetPointError(k,ex,ey); 
 } 
 
 return rehist; 
 
} 
 
// 
//____________Some basic geometry function_____________________________________ 
// 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::GetPlane(Int_t d) const 
{ 
  // 
  // Reconstruct the plane number from the detector number 
  // 
 
  return ((Int_t) (d % 6)); 
 
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::GetChamber(Int_t d) const 
{ 
  // 
  // Reconstruct the chamber number from the detector number 
  // 
  Int_t fgkNplan = 6; 
 
  return ((Int_t) (d % 30) / fgkNplan); 
 
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::GetSector(Int_t d) const 
{ 
  // 
  // Reconstruct the sector number from the detector number 
  // 
  Int_t fg = 30; 
 
  return ((Int_t) (d / fg)); 
 
} 
 
// 
//____________Fill and Init tree Gain, PRF, Vdrift and T0______________________ 
// 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::InitTreePRF() 
{ 
  // 
  // Init the tree where the coefficients from the fit methods can be stored 
  // 
 
  gDirectory = gROOT;  
  fPRFPad    = new Float_t[2304]; 
  fPRF       = new TTree("PRF","PRF"); 
  fPRF->Branch("detector",&fPRFDetector,"detector/I"); 
  fPRF->Branch("width"   ,fPRFPad      ,"width[2304]/F"); 
 
  // Set to default value for the plane 0 supposed to be the first one 
  for (Int_t k = 0; k < 2304; k++) { 
    fPRFPad[k] = 0.515; 
  } 
  fPRFDetector = -1; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillTreePRF(Int_t countdet) 
{ 
  // 
  // Fill the tree with the sigma of the pad response function for the detector countdet 
  // 
   
  Int_t numberofgroup = 0;  
  fPRFDetector = countdet; 
  fPRF->Fill(); 
 
  if (GetChamber((Int_t)(countdet+1)) == 2) { 
    numberofgroup = 1728; 
  } 
  else { 
    numberofgroup = 2304; 
  } 
 
  // Reset to default value for the next 
  for (Int_t k = 0; k < numberofgroup; k++) { 
    if (GetPlane((Int_t) (countdet+1)) == 0) { 
      fPRFPad[k] = 0.515; 
    } 
    if (GetPlane((Int_t) (countdet+1)) == 1) { 
      fPRFPad[k] = 0.502; 
    } 
    if (GetPlane((Int_t) (countdet+1)) == 2) { 
      fPRFPad[k] = 0.491; 
    } 
    if (GetPlane((Int_t) (countdet+1)) == 3) { 
      fPRFPad[k] = 0.481; 
    } 
    if (GetPlane((Int_t) (countdet+1)) == 4) { 
      fPRFPad[k] = 0.471; 
    } 
    if (GetPlane((Int_t) (countdet+1)) == 5) { 
      fPRFPad[k] = 0.463; 
    } 
  } 
 
  fPRFDetector = -1; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::ConvertVectorFitCHTree() 
{ 
  // 
  // Convert the vector stuff to a tree of 1D histos if the user 
  // want to write it after the fill functions 
  // 
 
  Int_t detector      = -1; 
  Int_t numberofgroup =  1; 
  Float_t gainPad[2304]; 
 
  fGain = new TTree("Gain","Gain"); 
  fGain->Branch("detector",&detector,"detector/I"); 
  fGain->Branch("gainPad" ,gainPad  ,"gainPad[2304]/F"); 
 
  Int_t loop = (Int_t) fVectorFitCH->GetEntriesFast(); 
  for (Int_t k = 0; k < loop; k++) { 
    detector = ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetDetector(); 
    if (GetChamber((Int_t) ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetDetector()) == 2) { 
      numberofgroup = 1728; 
    } 
    else { 
      numberofgroup = 2304; 
    } 
    for (Int_t i = 0; i < numberofgroup; i++) { 
      if (((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetCoef()[i] >= 0) { 
        gainPad[i] = ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetCoef()[i] * fScaleFitFactor; 
      } 
      else { 
	gainPad[i] = (Float_t) ((IlcDCHFitCHInfo *) fVectorFitCH->At(k))->GetCoef()[i]; 
      } 
    } 
    fGain->Fill(); 
  } 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillTreeVdrift(Int_t countdet) 
{ 
  // 
  // Fill the tree with the drift velocities for the detector countdet 
  // 
 
  Int_t numberofgroup = 0; 
  fVdriftDetector = countdet; 
 
  for (Int_t k = 0; k < 2304; k++) { 
    // ???? 
  } 
  fVdrift->Fill(); 
  if (GetChamber((Int_t)(countdet+1)) == 2) { 
    numberofgroup = 1728; 
  } 
  else { 
    numberofgroup = 2304; 
  } 
  // Reset to default value the gain coef 
  for (Int_t k = 0; k < numberofgroup; k++) { 
    fVdriftPad[k] = -1.5; 
  } 
  fVdriftDetector = -1; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::InitTreePH() 
{ 
  // 
  // Init the tree where the coefficients from the fit methods can be stored 
  // 
   
  gDirectory = gROOT; 
  fVdriftPad = new Float_t[2304]; 
  fVdrift    = new TTree("Vdrift","Vdrift"); 
  fVdrift->Branch("detector",&fVdriftDetector,"detector/I"); 
  fVdrift->Branch("vdrift"  ,fVdriftPad      ,"vdrift[2304]/F"); 
  // Set to default value for the plane 0 supposed to be the first one 
  for (Int_t k = 0; k < 2304; k++) { 
    fVdriftPad[k] = -1.5; 
  } 
  fVdriftDetector = -1; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::FillTreeT0(Int_t countdet) 
{ 
  // 
  // Fill the tree with the t0 value for the detector countdet 
  // 
 
  Int_t numberofgroup = 0; 
 
  fT0Detector = countdet; 
  for (Int_t k = 0; k < 2304; k++) { 
    // ???? 
  } 
  fT0->Fill(); 
  if (GetChamber((Int_t) (countdet+1)) == 2) { 
    numberofgroup = 1728; 
  } 
  else { 
    numberofgroup = 2304; 
  } 
  // Reset to default value the gain coef 
  for (Int_t k = 0; k < numberofgroup; k++) { 
    fT0Pad[k] = 0.0; 
  } 
  fT0Detector = -1; 
 
} 
 
//_____________________________________________________________________________ 
void IlcDCHCalibra::InitTreeT0() 
{ 
  // 
  // Init the tree where the coefficients from the fit methods can be stored 
  // 
   
  gDirectory = gROOT; 
  fT0Pad = new Float_t[2304]; 
  fT0 = new TTree("T0","T0"); 
  fT0->Branch("detector",&fT0Detector,"detector/I"); 
  fT0->Branch("t0",fT0Pad,"t0[2304]/F"); 
  //Set to default value for the plane 0 supposed to be the first one 
  for(Int_t k = 0; k < 2304; k++){ 
    fT0Pad[k] = 0.0; 
  } 
  fT0Detector = -1; 
 
} 
 
// 
//____________Private Functions________________________________________________ 
// 
 
//_____________________________________________________________________________ 
Double_t IlcDCHCalibra::PH(Double_t *x, Double_t *par)  
{ 
  // 
  // Function for the fit 
  // 
 
  //TF1 *fAsymmGauss = new TF1("fAsymmGauss",AsymmGauss,0,4,6); 
 
  //PARAMETERS FOR FIT PH 
  // PASAv.4 
  //fAsymmGauss->SetParameter(0,0.113755); 
  //fAsymmGauss->SetParameter(1,0.350706); 
  //fAsymmGauss->SetParameter(2,0.0604244); 
  //fAsymmGauss->SetParameter(3,7.65596); 
  //fAsymmGauss->SetParameter(4,1.00124); 
  //fAsymmGauss->SetParameter(5,0.870597);  // No tail cancelation 
 
  Double_t xx = x[0]; 
   
  if (xx < par[1]) { 
    return par[5]; 
  } 
 
  Double_t dx       = 0.005; 
  Double_t xs       = par[1]; 
  Double_t ss       = 0.0; 
  Double_t paras[2] = { 0.0, 0.0 }; 
 
  while (xs < xx) { 
    if ((xs >= par[1]) && 
        (xs < (par[1]+par[2]))) { 
      //fAsymmGauss->SetParameter(0,par[0]); 
      //fAsymmGauss->SetParameter(1,xs); 
      //ss += fAsymmGauss->Eval(xx); 
      paras[0] = par[0]; 
      paras[1] = xs; 
      ss += AsymmGauss(&xx,paras); 
    } 
    if ((xs >= (par[1]+par[2])) &&  
        (xs <  (par[1]+par[2]+par[3]))) { 
      //fAsymmGauss->SetParameter(0,par[0]*par[4]); 
      //fAsymmGauss->SetParameter(1,xs); 
      //ss += fAsymmGauss->Eval(xx); 
      paras[0] = par[0]*par[4]; 
      paras[1] = xs; 
      ss += AsymmGauss(&xx,paras); 
    } 
    xs += dx; 
  } 
   
  return ss + par[5]; 
 
} 
 
//_____________________________________________________________________________ 
Double_t IlcDCHCalibra::AsymmGauss(Double_t *x, Double_t *par)  
{ 
  // 
  // Function for the fit 
  // 
 
  //par[0] = normilczation 
  //par[1] = mean 
  //par[2] = sigma 
  //norm0  = 1 
  //par[3] = lambda0 
  //par[4] = norm1 
  //par[5] = lambda1 
   
  Double_t par1save = par[1];     
  //Double_t par2save = par[2]; 
  Double_t par2save = 0.0604244; 
  //Double_t par3save = par[3]; 
  Double_t par3save = 7.65596; 
  //Double_t par5save = par[5]; 
  Double_t par5save = 0.870597; 
  Double_t dx       = x[0] - par1save; 
 
  Double_t  sigma2  = par2save*par2save; 
  Double_t  sqrt2   = TMath::Sqrt(2.0); 
  Double_t  exp1    = par3save * TMath::Exp(-par3save * (dx - 0.5 * par3save * sigma2)) 
                               * (1.0 - TMath::Erf((par3save * sigma2 - dx) / (sqrt2 * par2save))); 
  Double_t  exp2    = par5save * TMath::Exp(-par5save * (dx - 0.5 * par5save * sigma2)) 
                               * (1.0 - TMath::Erf((par5save * sigma2 - dx) / (sqrt2 * par2save))); 
 
  //return par[0]*(exp1+par[4]*exp2); 
  return par[0] * (exp1 + 1.00124 * exp2); 
 
} 
 
//_____________________________________________________________________________ 
Double_t IlcDCHCalibra::FuncLandauGaus(Double_t *x, Double_t *par) 
{ 
  // 
  // Sum Landau + Gaus with identical mean 
  // 
 
  Double_t valLandau = par[0] * TMath::Landau(x[0],par[1],par[2]); 
  //Double_t valGaus   = par[3] * TMath::Gaus(x[0],par[4],par[5]); 
  Double_t valGaus   = par[3] * TMath::Gaus(x[0],par[1],par[4]); 
  Double_t val       = valLandau + valGaus; 
 
  return val; 
 
} 
 
//_____________________________________________________________________________ 
Double_t IlcDCHCalibra::LanGauFun(Double_t *x, Double_t *par)  
{ 
  // 
  // Function for the fit 
  // 
  // Fit parameters: 
  // par[0]=Width (scale) parameter of Landau density 
  // par[1]=Most Probable (MP, location) parameter of Landau density 
  // par[2]=Total area (integral -inf to inf, normilczation constant) 
  // par[3]=Width (sigma) of convoluted Gaussian function 
  // 
  // In the Landau distribution (represented by the CERNLIB approximation),  
  // the maximum is located at x=-0.22278298 with the location parameter=0. 
  // This shift is corrected within this function, so that the actual 
  // maximum is identical to the MP parameter. 
  //   
 
  // Numeric constants 
  Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2) 
  Double_t mpshift  = -0.22278298;       // Landau maximum location 
   
  // Control constants 
  Double_t np       = 100.0;             // Number of convolution steps 
  Double_t sc       =   5.0;             // Convolution extends to +-sc Gaussian sigmas 
   
  // Variables 
  Double_t xx; 
  Double_t mpc; 
  Double_t fland; 
  Double_t sum = 0.0; 
  Double_t xlow; 
  Double_t xupp; 
  Double_t step; 
  Double_t i; 
   
  // MP shift correction 
  mpc = par[1] - mpshift * par[0];  
 
  // Range of convolution integral 
  xlow = x[0] - sc * par[3]; 
  xupp = x[0] + sc * par[3]; 
   
  step = (xupp - xlow) / np; 
 
  // Convolution integral of Landau and Gaussian by sum 
  for (i = 1.0; i <= np/2; i++) { 
 
    xx    = xlow + (i-.5) * step; 
    fland = TMath::Landau(xx,mpc,par[0]) / par[0]; 
    sum  += fland * TMath::Gaus(x[0],xx,par[3]); 
     
    xx    = xupp - (i-.5) * step; 
    fland = TMath::Landau(xx,mpc,par[0]) / par[0]; 
    sum  += fland * TMath::Gaus(x[0],xx,par[3]); 
 
  } 
 
  return (par[2] * step * sum * invsq2pi / par[3]); 
 
} 
 
//_____________________________________________________________________________ 
TF1 *IlcDCHCalibra::LanGauFit(TH1 *his, Double_t *fitrange, Double_t *startvalues 
                                      , Double_t *parlimitslo, Double_t *parlimitshi 
                                      , Double_t *fitparams, Double_t *fiterrors 
                                      , Double_t *chiSqr, Int_t *ndf) 
{ 
  // 
  // Function for the fit 
  // 
   
  Int_t i; 
  Char_t funname[100]; 
   
  IlcInfo(Form(funname,"Fitfcn_%s",his->GetName())); 
   
  TF1 *ffitold = (TF1 *) gROOT->GetListOfFunctions()->FindObject(funname); 
  if (ffitold) { 
    delete ffitold; 
  }   
 
  TF1 *ffit    = new TF1(funname,LanGauFun,fitrange[0],fitrange[1],4); 
  ffit->SetParameters(startvalues); 
  ffit->SetParNames("Width","MP","Area","GSigma"); 
   
  for (i = 0; i < 4; i++) { 
    ffit->SetParLimits(i,parlimitslo[i],parlimitshi[i]); 
  } 
   
  his->Fit(funname,"RB0");                   // Fit within specified range, use ParLimits, do not plot 
   
  ffit->GetParameters(fitparams);            // Obtain fit parameters 
  for (i = 0; i < 4; i++) { 
    fiterrors[i] = ffit->GetParError(i);     // Obtain fit parameter errors 
  } 
  chiSqr[0] = ffit->GetChisquare();          // Obtain chi^2 
  ndf[0]    = ffit->GetNDF();                // Obtain ndf 
 
  return (ffit);                             // Return fit function 
    
} 
 
//_____________________________________________________________________________ 
Int_t IlcDCHCalibra::LanGauPro(Double_t *params, Double_t &maxx, Double_t &fwhm)  
{ 
  // 
  // Function for the fit 
  // 
 
  Double_t p; 
  Double_t x; 
  Double_t fy; 
  Double_t fxr; 
  Double_t fxl; 
  Double_t step; 
  Double_t l; 
  Double_t lold; 
 
  Int_t    i        = 0; 
  Int_t    maxcalls = 10000; 
   
  // Search for maximum 
  p    = params[1] - 0.1 * params[0]; 
  step = 0.05 * params[0]; 
  lold = -2.0; 
  l    = -1.0; 
   
  while ((l != lold) && (i < maxcalls)) { 
    i++; 
    lold = l; 
    x    = p + step; 
    l    = LanGauFun(&x,params); 
    if (l < lold) { 
      step = -step / 10.0; 
    } 
    p += step; 
  } 
   
  if (i == maxcalls) { 
    return (-1); 
  } 
  maxx = x; 
  fy = l / 2.0; 
 
  // Search for right x location of fy   
  p    = maxx + params[0]; 
  step = params[0]; 
  lold = -2.0; 
  l    = -1e300; 
  i    = 0; 
   
  while ( (l != lold) && (i < maxcalls) ) { 
    i++; 
     
    lold = l; 
    x = p + step; 
    l = TMath::Abs(LanGauFun(&x,params) - fy); 
     
    if (l > lold) 
      step = -step/10; 
  
    p += step; 
  } 
   
  if (i == maxcalls) 
    return (-2); 
   
  fxr = x; 
   
   
  // Search for left x location of fy 
   
  p = maxx - 0.5 * params[0]; 
  step = -params[0]; 
  lold = -2.0; 
  l    = -1.0e300; 
  i    = 0; 
   
  while ((l != lold) && (i < maxcalls)) { 
    i++; 
    lold = l; 
    x    = p + step; 
    l    = TMath::Abs(LanGauFun(&x,params) - fy); 
    if (l > lold) { 
      step = -step / 10.0; 
    } 
    p += step; 
  } 
   
  if (i == maxcalls) { 
    return (-3); 
  } 
 
  fxl  = x; 
  fwhm = fxr - fxl; 
 
  return (0); 
 
} 
