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
 *                                                                        *
 **************************************************************************/

/********************************************************************************
*                                                                               *
*   IlcLHCData: summary of the LHC related information from LHC DIP.            *
*   Created from the TMap provided by the IlcLHCReader with optional beginning  *
*                                                                               *
*   The data are (wrapped in the IlcLHCDipValT):                                *
*   made of TimeStamp (double) and array of values                              *
*                                                                               *
*   Multiple entries for each type of data are possible. To obtaine number of   *
*   records (with distinct timestamp) for give type od records use:             *
*   int GetNBunchConfigMeasured(int beam) (with beam=0,1) etc.                  *
*                                                                               *
*   To get i-th entry, use brec= IlcLHCDipValI* GetBunchConfigMeasured(bm,i);   *
*   Note: exact type of templated IlcLHCDipValT pointer depends on the record   *
*   type, concult getters to know it.                                           *
*                                                                               *
*   Then, once the pointer is obtained, details can be accessed:                *
*   int nBunches = brec->GetSize();                                             *
*   for (int i=0;i<nBunches;i++) printf("Bunch#%d: %d\n",i,(*brec)[i]);         *
*                                                                               *
*                                                                               *
*   Author: ruben.shahoyan@cern.ch                                              *
*                                                                               *
********************************************************************************/

#include "IlcLHCData.h"
#include "TMap.h"
#include "IlcDCSArray.h"
#include "IlcLHCReader.h"
#include "IlcTriggerBCMask.h"
#include <TString.h>
#include <TObjArray.h>

ClassImp(IlcLHCData)

const Char_t* IlcLHCData::fgkDCSNames[] = {
  "LHC_IntensityBeam%1d_totalIntensity",
  "LHC_BeamIntensityPerBunchBeam%1d_averageBeamIntensity",
  "LHC_BeamIntensityPerBunchBeam%1d_Average_BunchIntensities",
  //
  "LHC_LumAverageBRANB_4%c2_acqMode",
  "LHC_LumAverageBRANB_4%c2_meanLuminosity",
  "LHC_LumAverageBRANB_4%c2_meanLuminosityError",
  "LHC_BeamLuminosityPerBunchBRANB_4%c2_Average_BunchLuminosity",
  "LHC_BeamLuminosityPerBunchBRANB_4%c2_BunchLuminosityError",
  "LHC_LumAverageBRANB_4%c2_meanCrossingAngle",
  "LHC_LumAverageBRANB_4%c2_meanCrossingAngleError",
  "LHC_CirculatingBunchConfig_Beam%d",
  "LHC_FillNumber",
  //
  "LHC_BunchLengthBeam%1d_nBunches",
  "LHC_BunchLengthBeam%1d_bunchesLenghts",
  "LHC_BunchLengthBeam%1d_filledBuckets",
  //
  "LHC_RunControl_ActiveInjectionScheme",
  "LHC_RunControl_BetaStar",
  "LHC_RunControl_IP2_Xing_Murad",
  "LHC_RunControl_IP2_ILC_Murad",

  "LHC_BeamSizeBeam%1d_acqMode",
  "LHC_BeamSizeBeam%1d_sigmaH",
  "LHC_BeamSizeBeam%1d_sigmaV",
  "LHC_BeamSizeBeam%1d_emittanceH",
  "LHC_BeamSizeBeam%1d_emittanceV",
  "LHC_BeamSizeBeam%1d_errorSigmaH",
  "LHC_BeamSizeBeam%1d_errorSigmaV",
  //
  "LHC_CollimatorPos_%s_lvdt_%s"
};

const Char_t* IlcLHCData::fgkDCSColNames[] = {
  "TCTVB_4L2",
  "TCTVB_4R2",
  "TCLIA_4R2"
};

const Char_t* IlcLHCData::fgkDCSColJaws[] = {
  "gap_downstream","gap_upstream","left_downstream",
  "left_upstream","right_downstream","right_upstream"};

//___________________________________________________________________
IlcLHCData::IlcLHCData(const TMap* dcsMap, double tmin, double tmax)
  : fTMin(0),fTMax(0),fFillNumber(0),fData(0),fkFile2Process(0),fkMap2Process(0)
{
  FillData(dcsMap,tmin,tmax);
}

//___________________________________________________________________
IlcLHCData::IlcLHCData(const Char_t* dcsFile, double tmin, double tmax)
  : fTMin(0),fTMax(0),fFillNumber(0),fData(0),fkFile2Process(dcsFile),fkMap2Process(0)
{
  FillData(dcsFile,tmin,tmax);
}

//___________________________________________________________________
Bool_t IlcLHCData::FillData(const TMap*   dcsMap,double tmin, double tmax)
{
  // process DCS map and fill all fields. 
  Clear();
  fkMap2Process = dcsMap;
  FillData(tmin,tmax);
  return kTRUE;
}

//___________________________________________________________________
Bool_t IlcLHCData::FillData(const Char_t* dcsFile,double tmin, double tmax)
{
  // process DCS file and fill all fields. 
  Clear();
  fkFile2Process = dcsFile;
  FillData(tmin,tmax);
  return kTRUE;
}

//___________________________________________________________________
Bool_t IlcLHCData::FillData(double tmin, double tmax)
{
  // process DCS map and fill all fields. 
  // Accept only entries with timestamp between tmin and tmax
  //
  char buff[100],buff1[100];
  //
  SetTMin(tmin);
  SetTMax(tmax);
  //
  // -------------------------- extract Fill Number
  int iFirst=0,iLast=0;
  TObjArray* arr = GetDCSEntry(fgkDCSNames[kFillNum],iFirst,iLast,fTMin,fTMax);
  if (arr) SetFillNumber( ExtractInt( (IlcDCSArray*)arr->At(iFirst), 0) );
  if (fkFile2Process) delete arr; // array was created on demand
  //
  for (int ibm=0;ibm<2;ibm++) {
    //
    snprintf(buff,99,fgkDCSNames[kBunchConf],ibm+1);         // ----- declared bunch configuration
    FillBunchConfig(fBunchConfDecl[ibm], buff);
    //
    snprintf(buff,99,fgkDCSNames[kBunchLgtFillB],ibm+1);     // ----- measured bunch configuration
    FillBunchConfig(fBunchConfMeas[ibm], buff);
    //
    snprintf(buff,99,fgkDCSNames[kBunchLgt],ibm+1);          // ----- measured bunch lenghts
    FillBunchInfo(fBunchLengths[ibm],buff,ibm,kFALSE);  
    //
    snprintf(buff,99,fgkDCSNames[kIntBunchAv],ibm+1);        // ----- B-by-B intensities
    FillBunchInfo(fIntensPerBunch[ibm],buff,ibm,kTRUE);
    //
    //
    snprintf(buff,99,fgkDCSNames[kIntTot],ibm+1);            // ----- total intensities for beam 1 and 2
    FillScalarRecord(fIntensTotal[ibm], buff);
    //
    snprintf(buff,99,fgkDCSNames[kIntTotAv],ibm+1);          // ----- total intensities for beam 1 and 2 from B-by-B average
    FillScalarRecord(fIntensTotalAv[ibm], buff);
    //
    snprintf(buff,99,fgkDCSNames[kBeamSzEmittH],ibm+1);      // ----- H emittance for beam 1 and 2 
    FillScalarRecord(fEmittanceH[ibm], buff);
    //
    snprintf(buff,99,fgkDCSNames[kBeamSzEmittV],ibm+1);      // ----- V emittance for beam 1 and 2 
    FillScalarRecord(fEmittanceV[ibm], buff);
    //
    snprintf(buff,99 ,fgkDCSNames[kBeamSzSigH],   ibm+1);    // ----- H sigmas and errors for beam 1 and 2 
    snprintf(buff1,99,fgkDCSNames[kBeamSzSigHErr],ibm+1);
    FillScalarRecord(fBeamSigmaH[ibm], buff, buff1);
    //
    snprintf(buff,99 ,fgkDCSNames[kBeamSzSigV],   ibm+1);    // ----- V sigmas and errors for beam 1 and 2 
    snprintf(buff1,99,fgkDCSNames[kBeamSzSigVErr],ibm+1);
    FillScalarRecord(fBeamSigmaV[ibm], buff, buff1);
    //
  }
  //
  FlagInteractingBunches(fBunchConfMeas[0],fBunchConfMeas[1]);
  FlagInteractingBunches(fBunchConfDecl[0],fBunchConfDecl[1]);
  //
  for (int ilr=0;ilr<2;ilr++) {
    //
    snprintf(buff,99 ,fgkDCSNames[kLumBunch], ilr ? 'R':'L');       // ---- BC-by-BC luminosity at IP2 and its error
    snprintf(buff1,99,fgkDCSNames[kLumBunchErr], ilr ? 'R':'L');
    FillBCLuminosities(fLuminPerBC[ilr], buff, buff1, 0); // BRAN L uses beam2 as a reference, BRAN R - beam1
    //
    snprintf(buff,99 ,fgkDCSNames[kLumTot]   , ilr ? 'R':'L');       // ---- total luminosity at IP2 and its error
    snprintf(buff1,99,fgkDCSNames[kLumTotErr], ilr ? 'R':'L');
    FillScalarRecord(fLuminTotal[ilr], buff, buff1);
    //
    snprintf(buff,99 ,fgkDCSNames[kLumAcqMode], ilr ? 'R':'L');      // ---- luminosity acquisition mode
    FillAcqMode(fLuminAcqMode[ilr], buff);
    //
    snprintf(buff,99, fgkDCSNames[kLumCrossAng]   , ilr ? 'R':'L');  //----- crossing angle at IP2 and its error
    snprintf(buff1,99,fgkDCSNames[kLumCrossAngErr], ilr ? 'R':'L');
    FillScalarRecord(fCrossAngle[ilr], buff, buff1);
    //    
  }
  //
  for (int icl=0;icl<kNCollimators;icl++) {             // ----- collimators positions
    for (int jaw=0;jaw<kNJaws;jaw++) {
      snprintf(buff,99,fgkDCSNames[kCollPos], fgkDCSColNames[icl],fgkDCSColJaws[jaw]);        
      FillScalarRecord(fCollimators[icl][jaw], buff);
    } // jaws
  } // collimators
  //
  //
  // RunControl info
  FillStringRecord(fRCInjScheme, fgkDCSNames[kRCInjSch]);   // ---- active injection scheme
  FillScalarRecord(fRCBeta, fgkDCSNames[kRCBeta]);          // ---- target beta 
  FillScalarRecord(fRCAngH, fgkDCSNames[kRCCrossAng]);      // ---- horisontal angle
  FillScalarRecord(fRCAngV,fgkDCSNames[kRCVang] );          // ---- vertical angle
  //
  return kTRUE;
}

//___________________________________________________________________
TObjArray* IlcLHCData::GetDCSEntry(const char* key,int &entry,int &last,double tmin,double tmax) const
{
  // extract array from the DCS map or file and find the first entry within the time limits
  entry = -1;
  last = -2;
  TObjArray* arr;
  if (fkMap2Process) arr = (TObjArray*)fkMap2Process->GetValue(key);
  else if (fkFile2Process) {
    IlcLHCReader rd;
    arr = rd.ReadSingleLHCDP(fkFile2Process,key);
  }
  else {
    IlcError("Neither DCS map nor DCS filename are set");
    return 0;  
  }
  //
  if (!arr || !arr->GetEntriesFast()) { 
    IlcWarning(Form("No data for %s",key)); 
    if (fkMap2Process) delete arr; // created on demand
    return 0;
  }
  int ntot = arr->GetEntriesFast();
  //
  // search 1st entry before or at tmin
  IlcDCSArray* ent = 0;
  Bool_t found = kFALSE;
  for (entry=0;entry<ntot;entry++) {
    ent = (IlcDCSArray*)arr->At(entry);
    if (ent->GetTimeStamp()>=tmin-kMarginSOR && ent->GetTimeStamp()<=tmax+kMarginEOR) {
      found = kTRUE;
      if (ent->GetTimeStamp()>tmin) break;
    }
  }
  if (!found) {
    entry = -1;
    TString str;
    str += IlcLHCDipValF::TimeAsString(tmin);
    str += " : ";
    str += IlcLHCDipValF::TimeAsString(tmax);
    IlcWarning(Form("All entries for %s are outside the requested range:\n%s",key,str.Data()));
    if (fkMap2Process) delete arr; // created on demand
    return 0;
  }
  if (entry>0) entry--;
  //
  // search last entry at or after tmin
  ent = 0;
  for (last=entry;last<ntot;last++) {
    ent = (IlcDCSArray*)arr->At(last);
    if (ent->GetTimeStamp()>tmax) break;
  }
  if (last == ntot) last--;
  else if (ent->GetTimeStamp()>tmax+kMarginEOR) last--;
  //
  return arr;
}

//___________________________________________________________________
Int_t IlcLHCData::TimeDifference(double v1,double v2,double tol) const
{
  // return 0 if the times are the same within the tolerance
  //        1 if v1>v2
  //       -1 if v1<v2
  v1-=v2;
  if (v1>tol)  return  1;
  if (v1<-tol) return -1;
  return 0;
}

//___________________________________________________________________
Bool_t IlcLHCData::GoodPairID(int beam) const
{
  // check for correct beam identifier 
  if (beam>kBeam2||beam<0) {IlcError(Form("BeamID can be 0 or 1, %d requested",beam)); return kFALSE;}
  return kTRUE;
}

//___________________________________________________________________
IlcLHCDipValI* IlcLHCData::GetBunchConfigMeasured(int beam,double tstamp) const
{
  // find measured bunch configuration valid for given tstamp
  if (!GoodPairID(beam)) return 0;
  return (IlcLHCDipValI*)FindRecValidFor(fBunchConfMeas[beam][kStart],fBunchConfMeas[beam][kNStor],tstamp);
}

//___________________________________________________________________
IlcLHCDipValI* IlcLHCData::GetBunchConfigDeclared(int beam,double tstamp) const
{
  // find declared bunch configuration valid for given tstamp
  if (!GoodPairID(beam)) return 0;
  return (IlcLHCDipValI*)FindRecValidFor(fBunchConfDecl[beam][kStart],fBunchConfDecl[beam][kNStor],tstamp);
}

//___________________________________________________________________
TObject* IlcLHCData::FindRecValidFor(int start,int nrec, double tstamp) const
{
  // find record within this limits valid for given tstamp (i.e. the last one before or equal to tstamp)
  IlcLHCDipValI *prevObj = 0;
  for (int i=0;i<nrec;i++) {
    IlcLHCDipValI* curObj = (IlcLHCDipValI*)fData[start+i];
    if (TimeDifference(tstamp,curObj->GetTimeStamp())<0) break;
    prevObj = curObj;
  }
  if (!prevObj && nrec>0) prevObj = (IlcLHCDipValI*)fData[start]; // if no exact match, return the 1st one
  return prevObj;
}

//___________________________________________________________________
Int_t IlcLHCData::FillScalarRecord(int refs[2], const char* rec, const char* recErr, Double_t maxAbsVal)
{
  // fill record for scalar value, optionally accompanied by measurement error 
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  //
  TObjArray *arr=0,*arrE=0;
  Int_t iLast=0,iLastE=0,iFirst=0,iFirstE=0;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  int dim = 1;
  if (recErr) {
    arrE = GetDCSEntry(recErr,iFirstE,iLastE,fTMin,fTMax);
    dim += 1;
  }
  //
  // Bool_t last = kFALSE;
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    IlcLHCDipValF* curValD = new IlcLHCDipValF(dim,tstamp);  // start new period
    double vcheck = ExtractDouble(dcsVal,0);     // value
    if (TMath::Abs(vcheck) > maxAbsVal) {
      IlcError(Form("ANOMALOUS VALUE %e for slot %d of %s: exceeds %e",vcheck, 0, rec, maxAbsVal));
      vcheck = 0.;
    }    
    (*curValD)[0] = vcheck;
    //
    if (recErr) {
      double errVal = -1;
      while (iFirstE<=iLastE) {       // try to find corresponding error
	IlcDCSArray *dcsValE = (IlcDCSArray*) arrE->At(iFirstE);
        double tstampE = dcsValE->GetTimeStamp();
        int tdif = TimeDifference(tstamp,tstampE);
        if (!tdif) { // error matches to value
	  errVal = ExtractDouble(dcsVal,0);     // value
	  if (TMath::Abs(errVal) > maxAbsVal) {
	    IlcError(Form("ANOMALOUS VALUE %e for slot %d of %s: exceeds %e",errVal, 0, recErr, maxAbsVal));
	    errVal = 0.;
	  }
	  iFirstE++; 
	  break;
	}
        else if (tdif>0) iFirstE++; // error time lags behind, read the next one
        else break;                 // error time is ahead of value, no error associated
      }
      (*curValD)[dim-1] = errVal;   // error
      curValD->SetLastSpecial();    // lable the last entry as an error
    }
    //
    fData.Add(curValD);
    refs[kNStor]++;
    // if (last) break;
  }
  //
  if (fkFile2Process) {
    delete arr;
    delete arrE;
  }
  return refs[kNStor];
}

//___________________________________________________________________
Int_t IlcLHCData::FillBunchConfig(int refs[2],const char* rec)
{
  // fill record for bunch configuration
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  TObjArray *arr;
  Int_t iLast,iFirst;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  IlcLHCDipValI* prevRecI=0;
  //  
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    int bucket=0, nbunch=0, ndiff=0;
    int nSlots = dcsVal->GetNEntries();     // count number of actual bunches (non-zeros)
    int* dcsArr = dcsVal->GetInt();
    while(nbunch<nSlots && (bucket=dcsArr[nbunch])) {
      if (prevRecI && prevRecI->GetSize()>nbunch && bucket!=prevRecI->GetValue(nbunch)) ndiff++;
      nbunch++;
    }
    if (!nbunch) IlcWarning(Form("%s record is present but empty: no beam?",rec));
    if (prevRecI && !ndiff && nbunch==prevRecI->GetSize()) continue; // record similar to previous one
    IlcLHCDipValI* curValI = new IlcLHCDipValI(nbunch,tstamp);      
    for (int i=nbunch;i--;) (*curValI)[i] = dcsArr[i];
    fData.Add(curValI);
    refs[kNStor]++;
    prevRecI = curValI;
  }
  //
  if (fkFile2Process) delete arr;
  return refs[kNStor];
}
 
//___________________________________________________________________
Int_t IlcLHCData::FillAcqMode(int refs[2],const char* rec)
{
  // fill acquisition mode
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  TObjArray *arr;
  Int_t iLast,iFirst;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  IlcLHCDipValI* prevRecI=0;
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    int nSlots = dcsVal->GetNEntries();
    if (nSlots<1) continue;
    int acqMode = dcsVal->GetInt()[0];
    if (prevRecI && (*prevRecI)[0] == acqMode) continue; // record similar to previous one
    IlcLHCDipValI* curValI = new IlcLHCDipValI(1,tstamp);      
    (*curValI)[0] = acqMode;
    fData.Add(curValI);
    refs[kNStor]++;
    prevRecI = curValI;
  }
  //
  if (fkFile2Process) delete arr;
  return refs[kNStor];
}
 
//___________________________________________________________________
Int_t IlcLHCData::FillStringRecord(int refs[2],const char* rec)
{
  // fill record with string value
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  TString prevRec;
  TObjArray *arr;
  Int_t iLast,iFirst;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    TString &str = ExtractString(dcsVal);
    if (!prevRec.IsNull()) {if (str == prevRec) continue;} // skip similar record
    else prevRec = str;
    //
    IlcLHCDipValC* curValS = new IlcLHCDipValC(1,tstamp);      
    curValS->SetValues(str.Data(),str.Length()+1);
    //
    fData.Add(curValS);
    refs[kNStor]++;
  }
  if (fkFile2Process) delete arr;
  return refs[kNStor];
}

//___________________________________________________________________
Int_t IlcLHCData::FillBunchInfo(int refs[2],const char* rec, int ibm, Bool_t inRealSlots, Double_t maxAbsVal)
{
  // fill bunch properties for beam ibm
  // if inRealSlots = true, then the value is taken from bunchRFbucket/10, otherwise, the value 
  // for the i-th bunch is taken from the i-th element
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  TObjArray *arr;
  Int_t iLast,iFirst;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    IlcLHCDipValI *bconf = GetBunchConfigMeasured(ibm,tstamp);
    if (!bconf) {
      IlcWarning(Form("Mearured bunch configuration for beam %d at t=%.1f is not available, trying declared one",ibm+1,tstamp));
      bconf = GetBunchConfigDeclared(ibm,tstamp);
    }
    if (!bconf) {
      IlcWarning(Form("Declared bunch configuration for beam %d at t=%.1f is not available, skip this record",ibm+1,tstamp));
      return -1;
    }
    int nSlots = dcsVal->GetNEntries();     // count number of actual bunches (non-zeros)
    int nbunch = bconf->GetSize();
    if (nbunch>nSlots) {
      IlcWarning(Form("More N bunches than slots in %s at time %.1f",rec,tstamp));
      continue;
    }
    double* dcsArr = dcsVal->GetDouble();
    IlcLHCDipValF* curValD = new IlcLHCDipValF(nbunch,tstamp);
    for (int i=nbunch;i--;) {
      int ind = inRealSlots ? (*bconf)[i]/10 : i;
      if (ind>nSlots) {
	IlcError(Form("Bunch %d refers to wrong slot %d, set to -1",i,(*bconf)[i]));
	(*curValD)[i] = -1;
      }
      else {
	double vcheck = dcsArr[ind];
	if (TMath::Abs(vcheck) > maxAbsVal) {
	  IlcError(Form("ANOMALOUS VALUE %e for slot %d of %s: exceeds %e",vcheck, ind, rec, maxAbsVal));
	  vcheck = 0.;
	}
	(*curValD)[i] = vcheck;
      }
    }
    fData.Add(curValD);
    refs[kNStor]++;
  }
  if (fkFile2Process) delete arr;
  return refs[kNStor];
  //
}
 
//___________________________________________________________________
Int_t IlcLHCData::FillBCLuminosities(int refs[2],const char* rec, const char* recErr, int useBeam, Double_t maxAbsVal)
{
  // fill luminosities per bunch crossing
  //
  IlcInfo(Form("Acquiring record: %s",rec));
  TObjArray *arr,*arrE=0;
  Int_t iLast=0,iLastE=0,iFirst=0,iFirstE=0;
  //
  refs[kStart] = fData.GetEntriesFast();
  refs[kNStor] = 0;
  //
  if ( !(arr=GetDCSEntry(rec,iFirst,iLast,fTMin,fTMax)) ) return -1;
  //
  while (iFirst<=iLast) {
    IlcDCSArray *dcsVal = (IlcDCSArray*) arr->At(iFirst++);
    double tstamp = dcsVal->GetTimeStamp();
    //
    IlcLHCDipValI *bconf;
    bconf = GetBunchConfigMeasured(useBeam,tstamp);  // luminosities are stored according to beam bunches
    if (!bconf) {
      IlcWarning(Form("Mearured bunch configuration for beam%d at t=%.1f is not available, trying declared one",useBeam,tstamp));
      bconf = GetBunchConfigDeclared(useBeam,tstamp);
    }
    if (!bconf) {
      IlcWarning(Form("Declared bunch configuration for beam%i at t=%.1f is not available, skip this record",useBeam,tstamp));
      return -1;
    }
    int nSlots = dcsVal->GetNEntries();     // count number of actual bunches (non-zeros)
    int nbunch = bconf->GetSize();
    double* dcsArr = dcsVal->GetDouble();
    //
    if (nbunch>nSlots) {
      IlcWarning(Form("More N bunches than slots in %s at time %.1f",rec,tstamp));
      continue;
    }
    int dim = 0;
    if (!bconf->IsProcessed1()) {
      IlcWarning(Form("Bunch conf. for beam%d has no marked interacting bunches, store all luminosity for all filled bunches",useBeam));
      dim = nbunch;
    }
    else { // count number of interacting bunches
      for (int i=nbunch;i--;) if ((*bconf)[i]<0) dim++;
    }
    //
    if (recErr) {
      arrE=GetDCSEntry(recErr,iFirstE,iLastE,fTMin,fTMax);
      dim += 1;
    }
    IlcLHCDipValF* curValD = new IlcLHCDipValF(dim,tstamp);
    int cnt = 0;
    for (int i=0;i<nbunch;i++) {
      int slot = (*bconf)[i];
      if (bconf->IsProcessed1() && slot>0) continue;
      //
      int ind = TMath::Abs(slot)/10;
      if (ind>nSlots) {
	IlcError(Form("Bunch %d refers to wrong slot %d, set to -1",cnt,slot));
	(*curValD)[cnt] = -1;
      }
      else {
	double vcheck = dcsArr[ind];
	if (TMath::Abs(vcheck) > maxAbsVal) {
	  IlcError(Form("ANOMALOUS VALUE %e for slot %d of %s: exceeds %e",vcheck, ind, rec, maxAbsVal));
	  vcheck = 0.;
	}
	(*curValD)[i] = vcheck;
      }
      cnt++;
    }
    //
    if (recErr) {
      double errVal = -1;
      while (iFirstE<=iLastE) {       // try to find corresponding error
	IlcDCSArray *dcsValE = (IlcDCSArray*) arrE->At(iFirstE);
	double tstamp1 = dcsValE->GetTimeStamp();
	int tdif = TimeDifference(tstamp,tstamp1);
	if (!tdif) { // error matches to value
	  errVal = dcsValE->GetDouble()[0];
	  if (TMath::Abs(errVal) > maxAbsVal) {
	    IlcError(Form("ANOMALOUS VALUE %e for slot %d of %s: exceeds %e",errVal,0, rec, maxAbsVal));
	    errVal = 0.;
	  }	  
	  iFirstE++; 
	  break;
	}
	else if (tdif>0) iFirstE++; // error time lags behind, read the next one
	else break;                 // error time is ahead of value, no error associated
      }
      (*curValD)[dim-1] = errVal;   // error
      curValD->SetLastSpecial();    // lable the last entry as an error
    }
    //
    fData.Add(curValD);
    refs[kNStor]++;
  }
  if (fkFile2Process) {
    delete arr;
    delete arrE;
  }
  return refs[kNStor];
  //
}

//___________________________________________________________________
Int_t IlcLHCData::ExtractInt(IlcDCSArray* dcsArray,Int_t el) const
{
  // extract integer from the dcsArray
  int val = -1;
  //
  int sz = dcsArray->GetNEntries();
  if (sz<=el) return val;
  //
  if (dcsArray->GetType()==IlcDCSArray::kInt)  val = dcsArray->GetInt(el);
  else if (dcsArray->GetType()==IlcDCSArray::kString) {
    TObjString *stro = dcsArray->GetStringArray(el);
    if (stro) val = stro->GetString().Atoi();
    else IlcError(Form("DCSArray TObjString for element %d is missing",el));
  }
  else if (dcsArray->GetType()==IlcDCSArray::kUInt) val = dcsArray->GetUInt(el);
  else IlcError(Form("Integer requested from DCSArray of type %d",dcsArray->GetType()));
  return val;
}

//___________________________________________________________________
Double_t IlcLHCData::ExtractDouble(IlcDCSArray* dcsArray,Int_t el) const
{
  // extract double from the dcsArray
  double val = 0;
  //
  int sz = dcsArray->GetNEntries();
  if (sz<=el) return val;
  //
  if      (dcsArray->GetType()==IlcDCSArray::kDouble) val = dcsArray->GetDouble(el);
  else if (dcsArray->GetType()==IlcDCSArray::kFloat)  val = dcsArray->GetFloat(el);
  else if (dcsArray->GetType()==IlcDCSArray::kString) {
    TObjString *stro = dcsArray->GetStringArray(el);
    if (stro) val = stro->GetString().Atof();
    else IlcError(Form("DCSArray has TObjString for element %d is missing",el));
  }
  else if (dcsArray->GetType()==IlcDCSArray::kChar)   val = dcsArray->GetChar(el);
  else if (dcsArray->GetType()==IlcDCSArray::kInt)    val = dcsArray->GetInt(el);
  else if (dcsArray->GetType()==IlcDCSArray::kUInt)   val = dcsArray->GetUInt(el);
  else     IlcError(Form("Double requested from DCSArray of type %d",dcsArray->GetType()));
  return val;
}

//___________________________________________________________________
TString& IlcLHCData::ExtractString(IlcDCSArray* dcsArray) const
{
  // extract string from the dcsArray
  static TString str;
  str = "";
  //
  int sz = dcsArray->GetNEntries();
  if (dcsArray->GetType()!=IlcDCSArray::kString)  {
    IlcError(Form("String requested from DCSArray of type %d",dcsArray->GetType()));
    return str;
  }
  //
  for (int i=0;i<sz;i++) {
    str += dcsArray->GetStringArray(i)->GetString();
    if (i<sz-1) str += " ";
  }
  return str;
}

//___________________________________________________________________
void IlcLHCData::Print(const Option_t* opt) const
{
  // print full info
  TString opts = opt;
  opts.ToLower();
  Bool_t utcTime = opts.Contains("loc") ? kFALSE:kTRUE;
  Bool_t full = kTRUE;
  if (!opts.Contains("f")) {
    printf("Use Print(\"f\") to print full info\n");
    printf("Printing short info:\n<RecordType>(number of records): <TimeStamp, value> for 1st record only\n");
    full = kFALSE;
  }
  TString sdtmn = IlcLHCDipValI::TimeAsString(fTMin,utcTime);
  TString sdtmx = IlcLHCDipValI::TimeAsString(fTMax,utcTime);
  printf("Fill#%6d Validity: %s - %s (%s)\n",fFillNumber,sdtmn.Data(),sdtmx.Data(),utcTime ? "UTC":"LOC");
  //
  printf("********** SETTINGS FROM RUN CONTROL **********\n");
  //
  printf("* %-38s","Injection Scheme");
  PrintAux(full,fRCInjScheme,opts);
  //
  printf("* %-38s","Beta Star");
  PrintAux(full,fRCBeta,opts);
  //
  printf("* %-38s","Horisontal Crossing Angle");
  PrintAux(full,fRCAngH,opts);
  //
  printf("* %-38s","Vertical   Crossing Angle");
  PrintAux(full,fRCAngV,opts);
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d filling  [- interacts at IR2!]  ",ib+1);
    PrintAux(full,fBunchConfDecl[ib],opts);
  }
  //
  printf("\n**********       MEASURED DATA       **********\n");
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d filling  [- interacts at IR2!]  ",ib+1);
    PrintAux(full,fBunchConfMeas[ib],opts);
  } 
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d total intensity                 ",ib+1);
    PrintAux(full,fIntensTotal[ib],opts);
  } 
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d total intensity (bunch average) ",ib+1);
    PrintAux(full,fIntensTotalAv[ib],opts);
  } 
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d intensity per bunch             ",ib+1);
    PrintAux(full,fIntensPerBunch[ib],opts);
  }
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d bunch lengths                   ",ib+1);
    PrintAux(full,fBunchLengths[ib],opts);
  } 
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d Horisontal emittance            ",ib+1);
    PrintAux(full,fEmittanceH[ib],opts);
  }
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d Vertical emittance              ",ib+1);
    PrintAux(full,fEmittanceV[ib],opts);
  }
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d Horisontal sigma                ",ib+1);
    PrintAux(full,fBeamSigmaH[ib],opts);
  }
  //
  for (int ib=0;ib<2;ib++) {
    printf("* Beam%d Vertical sigma                  ",ib+1);
    PrintAux(full,fBeamSigmaV[ib],opts);
  }
  //
  for (int lr=0;lr<2;lr++) {
    printf("* Total luminosity from BRANB_4%c2       ",lr ? 'R':'L');
    PrintAux(full,fLuminTotal[lr],opts);
  } 
  //
  for (int lr=0;lr<2;lr++) {
    printf("* Luminosity acq.mode, BRANB_4%c2        ",lr ? 'R':'L');
    PrintAux(full,fLuminAcqMode[lr],opts+"bit");
  } 
  //
  for (int lr=0;lr<2;lr++) {
    printf("* Luminosity per BC from BRANB_4%c2      ",lr ? 'R':'L');
    PrintAux(full,fLuminPerBC[lr],opts);
  }
  //
  for (int lr=0;lr<2;lr++) {
    printf("* Crossing angle, side %c                ",lr ? 'R':'L');
    PrintAux(full,fCrossAngle[lr],opts);
  }
  //
  for (int coll=0;coll<kNCollimators;coll++)
    for (int jaw=0;jaw<kNJaws;jaw++) {
      printf("* Collimator %10s:%16s",fgkDCSColNames[coll],fgkDCSColJaws[jaw]);
      PrintAux(full,fCollimators[coll][jaw],opts);
    }
  //
}

//___________________________________________________________________
void IlcLHCData::PrintAux(Bool_t full, const Int_t refs[2], const Option_t *opt) const
{
  // aux method to print the reocrds of the same type
  int nrec = refs[kNStor];
  if (nrec<1) {
    printf(": N/A\n"); 
    return;
  }
  printf(": (%3d):\t",nrec); // number of records
  if (!full) nrec = 1;
  int sz = ((IlcLHCDipValI*)fData[refs[kStart]])->GetSizeTotal(); // dimension of the record
  Bool_t isStr = ((IlcLHCDipValI*)fData[refs[kStart]])->IsTypeC();
  if ((!isStr && sz>2) || nrec>1) printf("\n"); // long record, open new line
  for (int i=0;i<nrec;i++) fData[refs[kStart]+i]->Print(opt);
  //
}

//___________________________________________________________________
void IlcLHCData::Clear(const Option_t *)
{
  // clear all info
  fData.Delete();
  fFillNumber = 0;
  fTMin = 0;
  fTMax = 1e10;
  fkFile2Process = 0;
  fkMap2Process  = 0;
  //
  for (int i=2;i--;) {
    fRCInjScheme[i] = 0;
    fRCBeta[i] = 0;
    fRCAngH[i] = 0;
    fRCAngV[i] = 0;
    //
    for (int icl=kNCollimators;icl--;) for (int jaw=kNJaws;jaw--;) fCollimators[icl][jaw][i]=0;
    //
    for (int j=2;j--;) {
      fBunchConfDecl[j][i] = 0;
      fBunchConfMeas[j][i] = 0;
      fBunchLengths[j][i] = 0;
      fIntensTotal[j][i] = 0;
      fIntensTotalAv[j][i] = 0;
      fIntensPerBunch[j][i] = 0;      
      fCrossAngle[j][i] = 0;
      fEmittanceH[j][i] = 0;
      fEmittanceV[j][i] = 0;
      fBeamSigmaH[j][i] = 0;
      fBeamSigmaV[j][i] = 0;
      fLuminTotal[j][i] = 0;
      fLuminPerBC[j][i] = 0;
      fLuminAcqMode[j][i] = 0;
    }
  }
}

//___________________________________________________________________
Int_t IlcLHCData::GetNInteractingBunchesMeasured(int i) const
{
  // get number of interacting bunches at IR2
  IlcLHCDipValI* rec = GetBunchConfigMeasured(kBeam1,i);
  if (!rec) {IlcInfo(Form("No record %d found",i)); return -1;}
  if (!rec->IsProcessed1()) { IlcInfo("Interacting bunches were not marked"); return -1;}
  int n = 0;
  for (int j=rec->GetSize();j--;) if ( (*rec)[j]<0 ) n++;
  return n;
}

//___________________________________________________________________
Int_t IlcLHCData::GetNInteractingBunchesDeclared(int i) const
{
  // get number of interacting bunches at IR2
  IlcLHCDipValI* rec = GetBunchConfigMeasured(kBeam1,i);
  if (!rec) {IlcInfo(Form("No record %d found",i)); return -1;}
  if (!rec->IsProcessed1()) { IlcInfo("Interacting bunches were not marked"); return -1; }
  int n = 0;
  for (int j=rec->GetSize();j--;) if ( (*rec)[j]<0 ) n++;
  return n;
}

//___________________________________________________________________
Int_t IlcLHCData::IsPilotPresent(int i) const
{
  // check in the filling scheme is the pilot bunch is present
  IlcLHCDipValC* rec = GetInjectionScheme();
  if (!rec) {IlcInfo(Form("No record %d found",i)); return -1;}
  TString scheme = rec->GetValues();
  return scheme.Contains("wp",TString::kIgnoreCase);
}

//___________________________________________________________________
void IlcLHCData::FlagInteractingBunches(const Int_t beam1[2],const Int_t beam2[2])
{
  // assign - sign to interacting bunches
  //
  for (int ib1=0;ib1<beam1[kNStor];ib1++) {
    IlcLHCDipValI *bm1 = (IlcLHCDipValI*)fData[ beam1[kStart] + ib1];
    if (!bm1) continue;
    IlcLHCDipValI *bm2 = (IlcLHCDipValI*)FindRecValidFor(beam2[kStart],beam2[kNStor], bm1->GetTimeStamp());
    if (!bm2) continue;
    //
    int nb1 = bm1->GetSize();
    int nb2 = bm2->GetSize();
    int i1,i2;
    for (i1=0;i1<nb1;i1++) {
      int bunch2=-1, bunch1 = TMath::Abs((*bm1)[i1]);
      int slot2 =-1, slot1  = GetBCId(bunch1,0);
      for (i2=0;i2<nb2;i2++) {
	bunch2 = TMath::Abs((*bm2)[i2]);
	slot2 = GetBCId(bunch2,1);
	if (slot1==slot2) break;
      }
      if (slot1!=slot2) continue;
      (*bm1)[i1] = -bunch1;
      (*bm2)[i2] = -bunch2;
      bm1->SetProcessed1();
      bm2->SetProcessed1();
    }
  }
}

//___________________________________________________________________
Int_t IlcLHCData::GetMeanIntensity(int beamID, Double_t &colliding, Double_t &noncolliding, const TObjArray* bcmasks) const
{
  // get average intensity for all, colliding and non-colliding bunches
  // on success returns number of intensity records used (1 per ~10 min)
  // If triggered BC masks are provided, calculation is done for Triggered BC only
  colliding = noncolliding = -1.;
  if (beamID<0||beamID>1) {
    IlcError(Form("BeamID must be either 0 or 1, %d requested",beamID));
    return -10;
  }
  //
  IlcTriggerBCMask *bcMaskBoth=0,*bcMaskSingle=0;
  int nbcm = 0;
  if (bcmasks && (nbcm=bcmasks->GetEntries())) {
    if (nbcm>1) bcMaskBoth = (IlcTriggerBCMask*)bcmasks->At(1);
    if      (nbcm>0 && beamID==kBeam1) bcMaskSingle = (IlcTriggerBCMask*)bcmasks->At(0);
    else if (nbcm>2 && beamID==kBeam2) bcMaskSingle = (IlcTriggerBCMask*)bcmasks->At(2);
    //
    if (!bcMaskSingle) IlcError(Form("Only triggered BSs are requested but %c mask is not provided",beamID ? 'C':'A'));
    if (!bcMaskBoth)   IlcError("Only triggered BSs are requested but B mask is not provided");
  }
  else {
    IlcWarning("No BC masks are provided");
  }
  //
  int nrec = GetNIntensityPerBunch(beamID);
  if (nrec<1) return -1;
  IlcLHCDipValI *conf = GetBunchConfigMeasured(beamID);
  if (!conf) conf = GetBunchConfigDeclared(beamID);
  if (!conf) return -2;
  int nb = conf->GetSize();
  //
  for (int irec=0;irec<nrec;irec++) {
    //
    IlcLHCDipValD* rIntD = 0;
    IlcLHCDipValF* rIntF = GetIntensityPerBunch(beamID,irec);
    // for BWD compatibility of some periods
    if (rIntF->IsA() == IlcLHCDipValD::Class()) {rIntD=(IlcLHCDipValD*)rIntF; rIntF=0;}
    if (!rIntF && !rIntD) {
      IlcError(Form("Failed to get GetIntensityPerBunch(%d,%d)",beamID,irec));
      continue;
    }
    for (int ib=0;ib<nb;ib++) {
      double val = 0;
      if (rIntF) val = rIntF->GetValue(ib);
      else if (rIntD) val = rIntD->GetValue(ib);
      if (val<0) continue;
      int bID = conf->GetValue(ib);
      // check if this is a triggered bunch
      int bcID = GetBCId(bID, beamID);
      if (bID<0) { // interacting
	if (bcMaskBoth && bcMaskBoth->GetMask(bcID)) continue; // masked
	colliding += val;
      }
      else {
	if (bcMaskSingle && bcMaskSingle->GetMask(bcID)) continue; // masked	
	noncolliding += val;
      }
    }
  }
  colliding /= nrec;
  noncolliding /= nrec;
  return nrec;
}
