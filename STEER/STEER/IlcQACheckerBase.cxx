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


/* $Id: IlcQACheckerBase.cxx 54863 2012-02-28 12:10:29Z hristov $ */

//
//  Base class for detectors quality assurance checkers 
//  Compares Data made by QADataMakers with reference data
//  Y. Schutz CERN August 2007
//

// --- ROOT system ---
#include <TCanvas.h>
#include <TClass.h>
#include <TH1F.h> 
#include <TH1I.h> 
#include <TIterator.h> 
#include <TKey.h> 
#include <TFile.h> 
#include <TList.h>
#include <TNtupleD.h>
#include <TParameter.h>
#include <TPaveText.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcCDBEntry.h"
#include "IlcLog.h"
#include "IlcQAv1.h"
#include "IlcQAChecker.h"
#include "IlcQACheckerBase.h"
#include "IlcQADataMaker.h"
#include "IlcQAManager.h"
#include "IlcDetectorRecoParam.h"

ClassImp(IlcQACheckerBase)

           
//____________________________________________________________________________ 
IlcQACheckerBase::IlcQACheckerBase(const char * name, const char * title) : 
  TNamed(name, title), 
  fDataSubDir(0x0),
  fRefSubDir(0x0), 
  fRefOCDBSubDir(new TObjArray*[IlcRecoParam::kNSpecies]), 
  fLowTestValue(new Float_t[IlcQAv1::kNBIT]),
  fUpTestValue(new Float_t[IlcQAv1::kNBIT]),
  fImage(new TCanvas*[IlcRecoParam::kNSpecies]), 
  fPrintImage(kTRUE), 
  fExternParamList(new TList())
{
  // ctor
  fLowTestValue[IlcQAv1::kINFO]    =  0.5   ; 
  fUpTestValue[IlcQAv1::kINFO]     = 1.0 ; 
  fLowTestValue[IlcQAv1::kWARNING] =  0.002 ; 
  fUpTestValue[IlcQAv1::kWARNING]  = 0.5 ; 
  fLowTestValue[IlcQAv1::kERROR]   =  0.0   ; 
  fUpTestValue[IlcQAv1::kERROR]    = 0.002 ; 
  fLowTestValue[IlcQAv1::kFATAL]   = -1.0   ; 
  fUpTestValue[IlcQAv1::kFATAL]    = 0.0 ; 
  
  IlcDebug(IlcQAv1::GetQADebugLevel(), "Default setting is:") ;
  if ( IlcDebugLevel()  == IlcQAv1::GetQADebugLevel() ) {
    const Char_t * text= Form(" INFO    -> %1.5f <  value <  %1.5f  WARNING -> %1.5f <  value <= %1.5f \n ERROR   -> %1.5f <  value <= %1.5f \n FATAL   -> %1.5f <= value <  %1.5f \n", 
                              fLowTestValue[IlcQAv1::kINFO], fUpTestValue[IlcQAv1::kINFO], 
                              fLowTestValue[IlcQAv1::kWARNING], fUpTestValue[IlcQAv1::kWARNING], 
                              fLowTestValue[IlcQAv1::kERROR], fUpTestValue[IlcQAv1::kERROR], 
                              fLowTestValue[IlcQAv1::kFATAL], fUpTestValue[IlcQAv1::kFATAL]) ; 
    IlcInfo(Form("%s", text)) ; 
  }
  
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    fImage[specie] = NULL ; 
    fRefOCDBSubDir[specie] = NULL ;
  }
}

//____________________________________________________________________________ 
IlcQACheckerBase::~IlcQACheckerBase()
{
  delete [] fLowTestValue ; 
  delete [] fUpTestValue ; 
  DeleteImages();  
  delete[] fImage ; 
  delete[] fRefOCDBSubDir ; 
  IlcQAv1::GetQAResultFile()->Close() ; 
  if (fExternParamList) {
    fExternParamList->Clear() ; 
    delete fExternParamList ; 
  }
}

//____________________________________________________________________________
void IlcQACheckerBase::PrivateCheck(Double_t * test, IlcQAv1::ILCTASK_t index, const IlcDetectorRecoParam * recoParam) 
{
  // Performs a basic checking
  // Compares all the histograms stored in the directory
  // With reference histograms either in a file of in OCDB  

  TObjArray ** list = new TObjArray *[IlcRecoParam::kNSpecies] ; 
  Int_t specie ;
  for (specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    list[specie] =  new TObjArray(IlcQAv1::GetMaxQAObj()) ; 
    if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie) ) 
      continue ; 
    if (fDataSubDir) {
      TList * keyList = fDataSubDir->GetListOfKeys() ; 
      TIter next(keyList) ; 
      TKey * key ;
      while ( (key = static_cast<TKey *>(next())) ) {
        TDirectory * specieDir = fDataSubDir->GetDirectory(key->GetName()) ; 
        TList * keykeyList = specieDir->GetListOfKeys() ; 
        TIter next2(keykeyList) ; 
        TKey * keykey ;
        while ( (keykey = static_cast<TKey *>(next2())) ) {
          TObject * odata = specieDir->Get(keykey->GetName()) ; 
          if ( odata->IsA()->InheritsFrom("TH1") ) {
            TH1 * hdata = static_cast<TH1*>(odata) ;
            list[specie]->Add(hdata) ; 
          } else if (!odata->IsA()->InheritsFrom("TDirectory")) // skip the expert directory
            IlcError(Form("%s Is a Classname that cannot be processed", key->GetClassName())) ;
        }
      }
    }
  }
 
  Check(test, index, list, recoParam) ;
  
  delete[] list ; 
    
}  

//____________________________________________________________________________
void IlcQACheckerBase::Check(Double_t * test, IlcQAv1::ILCTASK_t task, TObjArray ** list, const IlcDetectorRecoParam * /* recoParam */) 
{
  // Performs a basic checking
  // Compares all the histograms in the list

  Int_t count[IlcRecoParam::kNSpecies]   = { 0 }; 

  GetRefSubDir(GetName(), IlcQAv1::GetTaskName(task), fRefSubDir, fRefOCDBSubDir) ;
 // SetRefandData(refDir, refOCDBDir) ; 
  
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    test[specie] = 1.0 ; 
    if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie)) continue ; 
    if (list[specie]->GetEntries() == 0)  
      test[specie] = 0. ; // nothing to check
    else {
      if (!fRefSubDir && !fRefOCDBSubDir)
        test[specie] = -1 ; // no reference data
      else {
        TIter next(list[specie]) ; 
        TH1 * hdata ;
        count[specie] = 0 ; 
        while ( (hdata = static_cast<TH1 *>(next())) ) {
          if ( hdata->IsA()->InheritsFrom("TH1") ) {
            if ( hdata->TestBit(IlcQAv1::GetExpertBit()) )  // does not perform the test for expert data
              continue ; 
            // 
	    // First try to find the ref histo with exact name (with possible trigger clon ending)
	    TString hname = hdata->GetName();
	    TH1 * href = NULL ; 
            if (fRefSubDir)                  href  = static_cast<TH1*>(fRefSubDir->Get(hname.Data())) ;
            else if (fRefOCDBSubDir[specie]) href  = static_cast<TH1*>(fRefOCDBSubDir[specie]->FindObject(hname.Data()));
	    //
	    if (!href && hdata->TestBit(IlcQAv1::GetClonedBit())) { // try to find the histo for the base name (w/o trigger ending
	      int ind = hname.Index(IlcQADataMaker::GetTriggerPrefix());
	      if (ind>0) {
		hname.Resize(ind);
		if (fRefSubDir)                  href  = static_cast<TH1*>(fRefSubDir->Get(hname.Data())) ;
		else if (fRefOCDBSubDir[specie]) href  = static_cast<TH1*>(fRefOCDBSubDir[specie]->FindObject(hname.Data()));
	      }		    
	    }
	    //
            if (!href) 
              test[specie] = -1 ; // no reference data ; 
            else {
              Double_t rv =  DiffK(hdata, href) ;
              IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%s ->Test = %f", hdata->GetName(), rv)) ; 
              test[specie] += rv ; 
              count[specie]++ ;
            }
          } else
            IlcError("Data type cannot be processed") ;
          if (count[specie] != 0) 
            test[specie] /= count[specie] ;
        }
      }
    }
  }
}  


//____________________________________________________________________________ 
void IlcQACheckerBase::DeleteImages()
{
  // clean images
  for (Int_t esIndex = 0 ; esIndex < IlcRecoParam::kNSpecies ; esIndex++) {
    if ( fImage[esIndex] )          {delete fImage[esIndex];          fImage[esIndex] = 0;}
    if ( fRefOCDBSubDir[esIndex] )  {delete fRefOCDBSubDir[esIndex];  fRefOCDBSubDir[esIndex] = 0;}
  }
}

//____________________________________________________________________________ 
Double_t IlcQACheckerBase::DiffC(const TH1 * href, const TH1 * hin) const
{
  // compares two histograms using the Chi2 test
  if ( hin->Integral() == 0 ) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Spectrum %s is empty", hin->GetName())) ; 
    return 0. ;
  }
    
  return hin->Chi2Test(href) ;  
}

//____________________________________________________________________________ 
Double_t IlcQACheckerBase::DiffK(const TH1 * href, const TH1 * hin) const
{
  // compares two histograms using the Kolmogorov test
  if ( hin->Integral() == 0 || href->Integral() == 0) {
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Spectrum %s or its reference is empty", hin->GetName())) ; 
    return 0. ;
  }
    
  return hin->KolmogorovTest(href) ;  
}

  //_____________________________________________________________________________
void IlcQACheckerBase::GetRefSubDir(const char * det, const char * task, TDirectory *& dirFile, TObjArray **& dirOCDB)     
{ 
    // Opens and returns the file with the reference data 
  dirFile = NULL ; 
  TString refStorage(IlcQAv1::GetQARefStorage()) ;
  if (!refStorage.Contains(IlcQAv1::GetLabLocalOCDB()) && !refStorage.Contains(IlcQAv1::GetLabAliEnOCDB())) {
    IlcError(Form("%s is not a valid location for reference data", refStorage.Data())) ; 
    return ; 
  } else {
    IlcQAManager* manQA = IlcQAManager::QAManager(IlcQAv1::GetTaskIndex(task)) ;
      //    dirOCDB = new TObjArray*[IlcRecoParam::kNSpecies] ;	
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
      dirOCDB[specie] = NULL ; 
      if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie) ) 
        continue ; 
      IlcQAv1::SetQARefDataDirName(specie) ;
      if ( ! manQA->GetLock() ) { 
        manQA->SetDefaultStorage(IlcQAv1::GetQARefStorage()) ; 
        manQA->SetSpecificStorage("*", IlcQAv1::GetQARefStorage()) ;
        manQA->SetRun(IlcCDBManager::Instance()->GetRun()) ; 
        manQA->SetLock() ; 
      }
      char * detOCDBDir = Form("%s/%s/%s", det, IlcQAv1::GetRefOCDBDirName(), IlcQAv1::GetRefDataDirName()) ; 
      IlcCDBEntry * entry = manQA->Get(detOCDBDir, manQA->GetRun()) ;
      if (entry) {
        TList * listDetQAD =static_cast<TList *>(entry->GetObject()) ;
        if ( listDetQAD && strcmp(listDetQAD->ClassName(), "TList") != 0 ) {
          IlcError(Form("Expected a Tlist and found a %s for detector %s", listDetQAD->ClassName(), det)) ; 
          listDetQAD = NULL ; 
          continue ; 
        } 
        if ( listDetQAD ) {
          TIter next(listDetQAD) ;
          while ( (TObjArray*)next() ) 
            dirOCDB[specie] = static_cast<TObjArray *>(listDetQAD->FindObject(Form("%s/%s", task, IlcRecoParam::GetEventSpecieName(specie)))) ;             
        }
      }
    }
  }
}

//____________________________________________________________________________
void IlcQACheckerBase::PrintExternParam() 
{
    // Print the list of external parameter list
  TIter next(fExternParamList) ; 
  TParameter<double> *pp ; 
  TString printit("\n") ;
  while( (pp = (TParameter<double>*)next()) )
    printit += Form("%s = %f\n", pp->GetName(), pp->GetVal());  
  IlcInfo(Form("%s", printit.Data())) ;
}
  
//____________________________________________________________________________
void IlcQACheckerBase::Run(IlcQAv1::ILCTASK_t index, const IlcDetectorRecoParam * recoParam) 
{ 
    //Run the checker for all kind of species
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Processing %s", IlcQAv1::GetIlcTaskName(index))) ; 
  
  Double_t * rv = new Double_t[IlcRecoParam::kNSpecies] ;
  for (int i=IlcRecoParam::kNSpecies;i--;) rv[i] = 0.0;
  PrivateCheck(rv, index, recoParam) ;
  SetQA(index, rv) ; 	
  
  IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Test result of %s", IlcQAv1::GetIlcTaskName(index))) ;
  
  delete [] rv ; 
  Finish() ; 
}

//____________________________________________________________________________
void IlcQACheckerBase::Run(IlcQAv1::ILCTASK_t index, TObjArray ** list, const IlcDetectorRecoParam * recoParam) 
{ 
  // RS: perform check for all trigger classes in loop
  Double_t * rv = new Double_t[IlcRecoParam::kNSpecies] ;
  //
  TObjArray ** listTrig = new TObjArray *[IlcRecoParam::kNSpecies];
  //
  for (int itc=-1;itc<IlcQADataMaker::GetNTrigClasses();itc++) {
    //
    // RS: fetch the histograms for each specie and for given trigger
    //IlcInfo(Form("Processing %s for trigger: %s", IlcQAv1::GetIlcTaskName(index),IlcQADataMaker::GetTrigClassName(itc))); 
    
    for (int specie=0;specie<IlcRecoParam::kNSpecies;specie++) {
      listTrig[specie] = 0;
      if ( !IlcQAv1::Instance()->IsEventSpecieSet(specie) || !list[specie]) continue;
      listTrig[specie] = new TObjArray( list[specie]->GetSize() ); // destination for clones of this trigger
      IlcQADataMaker::GetDataOfTrigClass(list[specie],itc, listTrig[specie]);
    }
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Processing %s for trigger: %s", IlcQAv1::GetIlcTaskName(index),IlcQADataMaker::GetTrigClassName(itc))); 
    Check(rv, index, listTrig, recoParam) ;
    SetQA(index, rv) ; 	
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("Test result of %s for trigger: %s", IlcQAv1::GetIlcTaskName(index),IlcQADataMaker::GetTrigClassName(itc)));
    //
    for (int specie=0;specie<IlcRecoParam::kNSpecies;specie++) if (listTrig[specie]) delete listTrig[specie]; // clean temporary container
  }
  delete [] rv ; 
  delete [] listTrig;
  Finish() ; 
}

//____________________________________________________________________________
void IlcQACheckerBase::Finish() const 
{
  // wrap up and save QA in proper file
  IlcQAv1::GetQAResultFile() ; 
  IlcQAv1 * qa = IlcQAv1::Instance() ; 
  qa->Write(IlcQAv1::GetQAName(), kWriteDelete) ;   
}

//____________________________________________________________________________ 
void IlcQACheckerBase::MakeImage( TObjArray ** list, IlcQAv1::TASKINDEX_t task, IlcQAv1::MODE_t mode) 
{
  // makes the QA image for sim and rec
  TObjArray tmpArr;  // array to store flat version of original array (which may contain clones)
  //
  for (Int_t esIndex = 0; esIndex < IlcRecoParam::kNSpecies; esIndex++) {
    if (! IlcQAv1::Instance(IlcQAv1::GetDetIndex(GetName()))->IsEventSpecieSet(IlcRecoParam::ConvertIndex(esIndex)) || list[esIndex]->GetEntries() == 0) continue;
    Int_t nImages = 0;
    TIter next(list[esIndex]);
    TObject* hdata = NULL;
    tmpArr.Clear();
    while ( (hdata=(next())) ) { // count histos and transfere to flat array
      if (hdata->InheritsFrom(TH1::Class()) && hdata->TestBit(IlcQAv1::GetImageBit()) ) {  // histo, not cloned
	nImages++; 
	tmpArr.AddLast(hdata); 
	continue;
      }
      if (!hdata->TestBit(IlcQAv1::GetClonedBit())) continue;  // not an array of clones, unknown object
      TIter nextCl((TObjArray*)hdata);   // array of histo clones
      TObject* hcl = 0;
      while ((hcl=nextCl())) if (hcl->InheritsFrom(TH1::Class()) && hcl->TestBit(IlcQAv1::GetImageBit())) {tmpArr.AddLast(hcl); nImages++;}
    }
    //
    if ( nImages == 0 ) {
      IlcDebug(IlcQAv1::GetQADebugLevel(), Form("No histogram will be plotted for %s %s %s\n", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(esIndex)));  
      continue;
    }
    IlcDebug(IlcQAv1::GetQADebugLevel(), Form("%d histograms will be plotted for %s %s %s\n", nImages, GetName(), IlcQAv1::GetTaskName(task).Data(),IlcRecoParam::GetEventSpecieName(esIndex)));  
    //        
    const Char_t * title = Form("QA_%s_%s_%s", GetName(), IlcQAv1::GetTaskName(task).Data(), IlcRecoParam::GetEventSpecieName(esIndex)); 
    //
    if ( !fImage[esIndex] ) fImage[esIndex] = new TCanvas(title, title);
    //
    fImage[esIndex]->Clear(); 
    fImage[esIndex]->SetTitle(title); 
    fImage[esIndex]->cd(); 
    TPaveText someText(0.015, 0.015, 0.98, 0.98);
    someText.AddText(title);
    someText.Draw(); 
    fImage[esIndex]->Print(Form("%s%s%d.%s", IlcQAv1::GetImageFileName(), IlcQAv1::GetModeName(mode), IlcQAChecker::Instance()->GetRunNumber(), IlcQAv1::GetImageFileFormat())); 
    fImage[esIndex]->Clear(); 
    Int_t nx = TMath::Nint(TMath::Sqrt(nImages));
    Int_t ny = nx; 
    if (nx < TMath::Sqrt(nImages)) ny++; 
    //
    fImage[esIndex]->Divide(nx, ny); 
    TIter nexthist(&tmpArr);
    Int_t npad = 1; 
    fImage[esIndex]->cd(npad); 
    TH1* histo = 0;
    while ( (histo=(TH1*)nexthist()) ) { // tmpArr is guaranteed to contain only plottable histos, no checks needed
      TString opts = histo->GetDrawOption();
      if (opts.Contains("logy",TString::kIgnoreCase)) gPad->SetLogy();
      if (opts.Contains("logx",TString::kIgnoreCase)) gPad->SetLogx();
      histo->DrawCopy(); 
      fImage[esIndex]->cd(++npad); 
    }
    fImage[esIndex]->Print(Form("%s%s%d.%s", IlcQAv1::GetImageFileName(), IlcQAv1::GetModeName(mode), IlcQAChecker::Instance()->GetRunNumber(), IlcQAv1::GetImageFileFormat()), "ps"); 
  }
}

//____________________________________________________________________________
void IlcQACheckerBase::SetHiLo(Float_t * hiValue, Float_t * lowValue) 
{
  IlcDebug(IlcQAv1::GetQADebugLevel(), "Previous setting was:") ;
  if ( IlcDebugLevel() == IlcQAv1::GetQADebugLevel() ) {
    const Char_t * text= Form(" INFO    -> %1.5f <  value <  %1.5f  WARNING -> %1.5f <  value <= %1.5f \n ERROR   -> %1.5f <  value <= %1.5f \n FATAL   -> %1.5f <= value <  %1.5f \n", 
                              fLowTestValue[IlcQAv1::kINFO], fUpTestValue[IlcQAv1::kINFO], 
                              fLowTestValue[IlcQAv1::kWARNING], fUpTestValue[IlcQAv1::kWARNING], 
                              fLowTestValue[IlcQAv1::kERROR], fUpTestValue[IlcQAv1::kERROR], 
                              fLowTestValue[IlcQAv1::kFATAL], fUpTestValue[IlcQAv1::kFATAL]) ; 
    IlcInfo(Form("%s", text)) ; 
  }
  
  for (Int_t index = 0 ; index < IlcQAv1::kNBIT ; index++) {
    fLowTestValue[index]  = lowValue[index] ; 
    fUpTestValue[index]   = hiValue[index] ; 
  }
  IlcDebug(IlcQAv1::GetQADebugLevel(), "Current setting is:") ;
  if ( IlcDebugLevel()  == IlcQAv1::GetQADebugLevel() ) {
    const Char_t * text= Form(" INFO    -> %1.5f <  value <  %1.5f  WARNING -> %1.5f <  value <= %1.5f \n ERROR   -> %1.5f <  value <= %1.5f \n FATAL   -> %1.5f <= value <  %1.5f \n", 
                              fLowTestValue[IlcQAv1::kINFO], fUpTestValue[IlcQAv1::kINFO], 
                              fLowTestValue[IlcQAv1::kWARNING], fUpTestValue[IlcQAv1::kWARNING], 
                              fLowTestValue[IlcQAv1::kERROR], fUpTestValue[IlcQAv1::kERROR], 
                              fLowTestValue[IlcQAv1::kFATAL], fUpTestValue[IlcQAv1::kFATAL]) ;     IlcInfo(Form("%s", text)) ; 
  }
}

//____________________________________________________________________________
void IlcQACheckerBase::SetQA(IlcQAv1::ILCTASK_t index, Double_t * value) const
{
	// sets the QA according the return value of the Check

  IlcQAv1 * qa = IlcQAv1::Instance(index) ;

  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    if (! qa->IsEventSpecieSet(IlcRecoParam::ConvertIndex(specie)))
      continue ;
    if (  value == NULL ) { // No checker is implemented, set all QA to Fatal
      qa->Set(IlcQAv1::kFATAL, specie) ; 
    } else {
      if ( value[specie] >= fLowTestValue[IlcQAv1::kFATAL] && value[specie] < fUpTestValue[IlcQAv1::kFATAL] ) 
        qa->Set(IlcQAv1::kFATAL, IlcRecoParam::ConvertIndex(specie)) ; 
      else if ( value[specie] > fLowTestValue[IlcQAv1::kERROR] && value[specie] <= fUpTestValue[IlcQAv1::kERROR]  )
        qa->Set(IlcQAv1::kERROR, IlcRecoParam::ConvertIndex(specie)) ; 
      else if ( value[specie] > fLowTestValue[IlcQAv1::kWARNING] && value[specie] <= fUpTestValue[IlcQAv1::kWARNING]  )
        qa->Set(IlcQAv1::kWARNING, IlcRecoParam::ConvertIndex(specie)) ;
      else if ( value[specie] > fLowTestValue[IlcQAv1::kINFO] && value[specie] <= fUpTestValue[IlcQAv1::kINFO] ) 
        qa->Set(IlcQAv1::kINFO, IlcRecoParam::ConvertIndex(specie)) ; 	
    }
  }
}
