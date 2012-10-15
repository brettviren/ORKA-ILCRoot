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


/* $Id: IlcQADataMaker.cxx 53002 2011-11-18 11:52:54Z shahoian $ */

//
//  Base Class
//  Produces the data needed to calculate the quality assurance. 
//  All data must be mergeable objects.
//  Y. Schutz CERN July 2007
//

// --- ROOT system ---
#include <TROOT.h> 
#include <TSystem.h> 
#include <TFile.h>
#include <TList.h> 
#include <TTree.h>
#include <TClonesArray.h>
#include <TParameter.h>
#include <TH1K.h>
#include <TH2C.h>
#include <TH2F.h>
#include <TH2F.h>
#include <TH2I.h>
#include <TH3C.h>
#include <TH3F.h>
#include <TH3F.h>
#include <TH3I.h>
#include <TH3S.h>
#include <TArrayI.h>
#include <TPRegexp.h>
// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcQADataMaker.h"
#include "IlcQAChecker.h"
#include "IlcESDEvent.h"
#include "IlcRawReader.h"
#include "IlcDetectorRecoParam.h"


ClassImp(IlcQADataMaker)

const Char_t* IlcQADataMaker::fgkTriggerPrefix = "_$TR$_";
TObjArray* IlcQADataMaker::fgCloningRequest = 0;
TObjArray* IlcQADataMaker::fgTrigClasses = 0;
TObjArray  IlcQADataMaker::fgEventTrigClasses;

//____________________________________________________________________________ 
IlcQADataMaker::IlcQADataMaker(const Char_t * name, const Char_t * title) : 
  TNamed(name, title), 
  fOutput(0x0),
  fDetectorDir(0x0),
  fDetectorDirName(""), 
  fCurrentCycle(0), 
  fCycle(9999999), 
  fCycleCounter(0), 
  fWriteExpert(kFALSE),
  fParameterList(new TList*[IlcRecoParam::kNSpecies]), 
  fRun(0), 
  fEventSpecie(IlcRecoParam::kDefault), 
  fDigitsArray(NULL) 
{
  // ctor
  fDetectorDirName = GetName() ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    fParameterList[specie] = NULL ; 
    fEvCountCycle[specie].Set(IlcQAv1::kNTASKINDEX);
    fEvCountTotal[specie].Set(IlcQAv1::kNTASKINDEX);
  }
  
}

//____________________________________________________________________________ 
IlcQADataMaker::IlcQADataMaker(const IlcQADataMaker& qadm) :
  TNamed(qadm.GetName(), qadm.GetTitle()),
  fOutput(qadm.fOutput),
  fDetectorDir(qadm.fDetectorDir),
  fDetectorDirName(qadm.fDetectorDirName),
  fCurrentCycle(qadm.fCurrentCycle), 
  fCycle(qadm.fCycle), 
  fCycleCounter(qadm.fCycleCounter), 
  fWriteExpert(qadm.fWriteExpert),
  fParameterList(qadm.fParameterList),  
  fRun(qadm.fRun), 
  fEventSpecie(qadm.fEventSpecie),
  fDigitsArray(NULL) 
{
  //copy ctor
  fDetectorDirName = GetName() ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) {
    fParameterList[specie] = qadm.fParameterList[specie] ; 
    fEvCountCycle[specie] = qadm.fEvCountCycle[specie];
    fEvCountTotal[specie] = qadm.fEvCountTotal[specie];
    //  fImage[specie] = qadm.fImage[specie] ; 
  }
}

//____________________________________________________________________________ 
IlcQADataMaker::~IlcQADataMaker()
{
  for (Int_t esIndex = 0 ; esIndex < IlcRecoParam::kNSpecies ; esIndex++) {
    if (fParameterList[esIndex] )
      delete fParameterList[esIndex] ; 
  }
  delete[] fParameterList ; 
  if (fDigitsArray) {
    fDigitsArray->Clear() ; 
    delete fDigitsArray ;
  }
}

//____________________________________________________________________________
Int_t IlcQADataMaker::Add2List(TH1 * hist, const Int_t index, TObjArray ** list, const Bool_t expert, const Bool_t image, const Bool_t saveForCorr) 
{ 
  // Set histograms memory resident and add to the list
  // Maximm allowed is 10000
  
  Int_t rv = -1 ; 
  TClass * classType = hist->Class() ;
  TString className(classType->GetName()) ; 
  if( ! className.BeginsWith("T") && ! classType->InheritsFrom("TH1") ) {
    IlcError(Form("QA data Object must be a generic ROOT object and derive fom TH1 and not %s", className.Data())) ; 
  } else if ( index > IlcQAv1::GetMaxQAObj() ) {
    IlcError(Form("Max number of authorized QA objects is %d", IlcQAv1::GetMaxQAObj())) ; 
  } else {
    hist->SetDirectory(0); 
    if (expert) 
      hist->SetBit(IlcQAv1::GetExpertBit()) ;
    if (image) 
      hist->SetBit(IlcQAv1::GetImageBit()) ;  
    const Char_t * name = Form("%s_%s", IlcRecoParam::GetEventSpecieName(fEventSpecie), hist->GetName()) ;
    hist->SetName(name) ; 
    if(saveForCorr) {  
      const Char_t * cname = Form("%s_%s", list[IlcRecoParam::AConvert(IlcRecoParam::kDefault)]->GetName(), hist->GetName()) ;  
      TParameter<double> * p = new TParameter<double>(cname, 9999.9999) ;
      if ( fParameterList[IlcRecoParam::AConvert(fEventSpecie)] == NULL )
      {
        fParameterList[IlcRecoParam::AConvert(fEventSpecie)] = new TList() ; 
	//        fParameterList[IlcRecoParam::AConvert(fEventSpecie)]->SetOwner(kTRUE);
      }
      fParameterList[IlcRecoParam::AConvert(fEventSpecie)]->Add(p) ;
    }
    TObject* old = list[IlcRecoParam::AConvert(fEventSpecie)]->At(index);
    if (old) {
      IlcError(Form("%s - OUPS ! Already got an object (%p,%s) for index=%d => will most probably get a memory leak by replacing it with (%p,%s) !",
                    GetName(),old,old->GetName(),index,hist,hist->GetName()));
    }
    list[IlcRecoParam::AConvert(fEventSpecie)]->AddAtAndExpand(hist, index) ; 
    rv = list[IlcRecoParam::AConvert(fEventSpecie)]->GetLast() ;
  }
  return rv ; 
}

//____________________________________________________________________________
void IlcQADataMaker::ClonePerTrigClassL(TObjArray ** list, IlcQAv1::TASKINDEX_t task)
{
  // scan the histos of the list, match them to requested histo names to be 
  // cloned and create the clones per trigger class
  int spec = IlcRecoParam::AConvert(fEventSpecie);
  if (!fgCloningRequest || !GetNTrigClasses()) return;
  if (!list || !list[spec]) {
    IlcDebug(1,Form("Array for %s specie is not created",IlcRecoParam::GetEventSpecieName(fEventSpecie)));
    return;
  }
  //
  // if the counters were not expanded, do this now
  if (fEvCountCycle[spec].GetSize()==IlcQAv1::kNTASKINDEX) {
    fEvCountCycle[spec].Set(IlcQAv1::kNTASKINDEX*(GetNTrigClasses()+1)); // +1 for trigger-independent count
    fEvCountTotal[spec].Set(IlcQAv1::kNTASKINDEX*(GetNTrigClasses()+1)); // +1 for trigger-independent count
  }
  //

  TString prefix = Form("%s/%s/",GetName(),IlcQAv1::GetTaskName(task).Data()); // take care of requests with this prefix
  int prefLgt = prefix.Length();
  //
  for (int ir=fgCloningRequest->GetEntriesFast();ir--;) {
    TNamed* rec = dynamic_cast<TNamed*>(fgCloningRequest->At(ir));
    if (!rec) continue;
    TString hfname = rec->GetName();     // histogram names
    if (!hfname.BeginsWith(prefix)) continue;
    Bool_t keepOrig = rec->TestBit(IlcQAv1::GetOrigHistoKeptBit()); // is original to be kept?
    ClonePerTrigClassA(list[spec], hfname.Data()+prefLgt, rec->GetTitle(), keepOrig);
  }
  //
}

//____________________________________________________________________________
Int_t IlcQADataMaker::ClonePerTrigClassA(TObjArray * list, const char* hnm, const char* triggers, Bool_t keepOriginal)
{
  // substitute the matching histogram by the TObjArray containing clones of 
  // original histogram for each trigger from the space-separated tiggers string
  // When keepOriginal is requested, the original histo is not deleted and filled for each trigger
  if (!list) return 0;
  int nh = list->GetEntriesFast();
  if (!nh) return 0;
  //
  IlcDebug(IlcQAv1::GetQADebugLevel(),Form("Cloning histogram %s for triggers %s in array %s",hnm,triggers,list->GetName()));
  //
  TString trigStr = triggers;
  TObjArray* trigArr = trigStr.Tokenize(" ");
  int nTrigs = trigArr->GetEntriesFast();
  Bool_t cloneAll=kFALSE, cloneMany=kFALSE;
  if (!nTrigs) {
    IlcError(Form("Triggers string supplied for histo %s is empty", hnm));
    delete trigArr;
    return 0;
  }
  // match the trigger class id to selected aliases
  for (int i=nTrigs;i--;) {
    TObject* alias = fgTrigClasses->FindObject( trigArr->At(i)->GetName() );
    if (!alias) {
      IlcError(Form("Cloning for undefined trigger class %s was requested for histogram %s",trigArr->At(i)->GetName(),hnm));
      delete trigArr->RemoveAt(i); 
      trigArr->Compress();
      continue;
    }
    trigArr->At(i)->SetUniqueID( alias->GetUniqueID() );
  }
  //
  TString histPatt = hnm;
  if (histPatt == "*") {
    cloneAll = kTRUE;
    cloneMany = kTRUE;
  }
  else {
    histPatt.Prepend(Form("%s_", IlcRecoParam::GetEventSpecieName(fEventSpecie)));
    // 
    // The histogram name may contain %d formats for histos created in loop. 
    // Then we expect more than 1 histo matching to name
    TPRegexp rgx("%\\d*d");
    if (histPatt.Contains(rgx)) { // need to convert to perl expression, make sure there are no perl special characters
      const char *specials = "^$.{}[]*+?\\";
      for (int i=strlen(specials);i--;) histPatt.ReplaceAll(specials+i,1,Form("\\%c",specials[i]),2);
      rgx.Substitute(histPatt,"\\d+","g");
      histPatt.Prepend('^'); histPatt += "$";
      cloneMany = kTRUE;
    }
    else if ( histPatt.Contains("*") ) {
      histPatt.ReplaceAll("*",".*");
      histPatt.Prepend('^'); histPatt += "$";
      cloneMany = kTRUE;
    }
  }
  //
  int nCloned = 0;
  for (int ih=0;ih<nh;ih++) {
    TH1* hist = (TH1*)list->At(ih);
    if (!hist) continue;
    if (!cloneAll) {
      TString hname = hist->GetName();
      if (cloneMany) {
	TPRegexp patPR(histPatt);
	if ( !hname.Contains(patPR) ) continue;
      }
      else {
	if ( hname.CompareTo(histPatt) ) continue;
      }
    }
    //
    // histo name matches, clone it
    nCloned += ClonePerTrigClassH(ih, list, trigArr, keepOriginal);
    if (!cloneMany) break; // no wildcards, just one histo is expected
  }
  //
  delete trigArr;
  return nCloned;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::ClonePerTrigClassH(Int_t index, TObjArray* list, TObjArray* trigArr, Bool_t keepOriginal)
{
  // substitute the histogram at position index by the TObjArray containing clones of 
  // original histogram for each trigger from the trigArr
  // When keepOriginal is requested, the original histo is not deleted and filled for each trigger
  TH1* hist = (TH1*) list->At(index);
  if (!hist) return 0; // should never happen, checked in calling method

  if (IsCloningForbidden(hist)) {
    IlcError(Form("Cloning per trigger is forbidden for histo %s at slot %d",hist->GetName(),index));
    return 0;
  }
  //
  if (hist->TestBit(IlcQAv1::GetClonedBit())) {
    IlcError(Form("histo %s at slot %d was already cloned!",((TObjArray*)hist)->GetName(),index));
    return 0;
  }
  //
  int nTrigs = trigArr->GetEntriesFast();
  TObjArray* clonesArr = new TObjArray(GetNTrigClasses()+keepOriginal);
  // transfer the user bits of the histo to be cloned to array
  for (int ib=14;ib<=23;ib++) if (hist->TestBit(1<<ib)) clonesArr->SetBit(1<<ib);
  //
  if (keepOriginal) {
    clonesArr->AddAt(hist,GetNTrigClasses());
    clonesArr->SetBit(IlcQAv1::GetOrigHistoKeptBit());
  }
  clonesArr->SetBit(IlcQAv1::GetClonedBit());
  clonesArr->SetName(hist->GetName());
  for (int itr=0;itr<nTrigs;itr++) {
    TObject* trigClass =  trigArr->At(itr);
    TH1* hclone = (TH1*)hist->Clone(Form("%s%s%s",hist->GetName(), fgkTriggerPrefix, trigClass->GetName()));
    hclone->SetTitle(Form("%s %s",hist->GetTitle(), trigClass->GetName()));
    hclone->SetDirectory(hist->GetDirectory());
    //
    // NOTE: the clone is added at position corresponding to ID of the trigger alias
    clonesArr->AddAt(hclone, trigClass->GetUniqueID());
    hclone->SetBit(IlcQAv1::GetClonedBit());
    IlcDebug(IlcQAv1::GetQADebugLevel(),Form("Cloned histogram %s for trigger class %s",hist->GetName(),trigClass->GetName()));
  }
  //
  list->RemoveAt(index);    // remove original histogram
  list->AddAt(clonesArr, index);
  if (!keepOriginal) delete hist;
  //
  clonesArr->SetOwner(kTRUE);
  return clonesArr->GetEntries();
}

//____________________________________________________________________________
Char_t* IlcQADataMaker::GetHistoTrigger(const TH1* h)  const
{
  // extract the name of the trigger from the histogram name
  if (!h || !h->TestBit(IlcQAv1::GetClonedBit())) return 0;
  TString hnm = h->GetName();
  int idx = hnm.Index(fgkTriggerPrefix);
  if (idx<0) return 0;  // not cloned
  return (char*) h->GetName() + idx + sizeof(fgkTriggerPrefix) - 1; 
}

//____________________________________________________________________________
Int_t IlcQADataMaker::SetEventTrigClasses(const char* triggers)
{
  // sets the array of triggers fired in given event, return the number of triggers
  //
  ResetEventTrigClasses();
  fgEventTrigClasses.SetUniqueID(1); // flag that this is a triggered event
  if (!GetNTrigClasses()) return 0; // meaningless if the trigger aliases are not set
  TString trigStr = triggers;
  TObjArray* tarr = trigStr.Tokenize(" ");
  int nTrigs = tarr->GetEntriesFast();
  if (!nTrigs) {
    IlcWarningClass("The string for event triggers is empty");
    delete tarr;
    return 0;
  }
  int nTrClasses = 0;
  for (int ial=fgTrigClasses->GetEntriesFast();ial--;) {
    TNamed *pair = (TNamed*)fgTrigClasses->At(ial);
    TString triggSet = pair->GetTitle();      // list of trigger names
    for (int itr=nTrigs;itr--;) {
      if ( !triggSet.Contains(tarr->At(itr)->GetName()) ) continue;
      fgEventTrigClasses.Add(pair);
      nTrClasses++;
      break; 
    }
  }
  //
  fgEventTrigClasses.SetOwner(kFALSE);
  delete tarr;
  //
  return nTrClasses;
}

//____________________________________________________________________________
TH1* IlcQADataMaker::GetMatchingHisto(TObjArray ** list, Int_t index, Int_t trigId)
{
  // get histo with given index matching to trigId-th current trigger class
  // trigId < 0 means default hist (only if cloning was done) 
  TObject* data = GetData(list,index);
  if (!data) return 0;
  if (trigId<0 || trigId==GetNEventTrigClasses()) { // original histo is requested
    if (!data->TestBit(IlcQAv1::GetClonedBit())) return (TH1*)data; // was not cloned per trigger
    // check if during the cloning the original histo was kept (always at slot GetNTrigClasses())
    else if (data->TestBit(IlcQAv1::GetOrigHistoKeptBit())) return (TH1*)((TObjArray*)data)->At(GetNTrigClasses());
    return 0;
  }
  // 
  if (!data->TestBit(IlcQAv1::GetClonedBit())) return 0; // no cloning was done
  if (trigId>GetNEventTrigClasses()) {
    IlcError(Form("Current event has %d triggers only, %d-th is requested",GetNEventTrigClasses(),trigId));
    return 0; // invalid trigger
  }
  int slot = fgEventTrigClasses.At(trigId)->GetUniqueID(); // the histos are at slots corresponding to trig.class ID
  return (TH1*)((TObjArray*)data)->At(slot);
  //
}

//____________________________________________________________________________
TObjArray* IlcQADataMaker::GetMatchingHistosSet(TObjArray ** list, const Int_t* indexList, Int_t nHist, Int_t trigId)
{
  // get set of nHist histos with indices from the indexList matching to trigId-th current trigger
  // Note: a static array is returned, it will be overwritten at the next call  
  static TObjArray garr;
  garr.Clear();
  for (int ih=0;ih<nHist;ih++) {
    TH1* histo = GetMatchingHisto(list, indexList[ih],trigId);
    if (!histo) return 0;    // if at least 1 histo is not found, return nothing
    garr.Add(histo);
  }
  return &garr;
}

//____________________________________________________________________________
Bool_t IlcQADataMaker::CheckCloningConsistency(TObjArray ** list, const Int_t* indexList, Int_t nHist, Bool_t checkTriggers) 
{
  // Make sure that histograms at slots mentioned in the indexList are either
  // not cloned per trigger or
  // cloned for the same amount of triggers and if original histo was kept, then it is done for all histos.
  // If checkTriggers is requested, apart from clones count also the trigger names are checked 
  //
  // This consistency check is needed to ensure validity of the operations over the group of histograms in the EndOfDetectorCycle's
  //
  TObjArray* refArr = 0;
  int refSlot = -1;
  for (int isl=0;isl<nHist;isl++) {
    int slot = indexList[isl];
    TObject* data = GetData(list,slot);
    if (!data) {IlcError(Form("Slot %d is not booked",slot)); return kFALSE;}
    //
    if (refSlot<0) { // just take the 1st mentioned slot as a reference;
      if (data->TestBit(IlcQAv1::GetClonedBit())) refArr = (TObjArray*) data; // data was cloned
      refSlot = slot;
      continue;
    }
    //
    if (!data->TestBit(IlcQAv1::GetClonedBit())) { // this slot is not cloned, compare with reference
      if (refArr) {IlcError(Form("Slot %d is not cloned, slot %d is cloned",slot, refSlot)); return kFALSE;}
      else continue;
    }
    // 
    TObjArray* currArr = (TObjArray*) data;  // current slot is cloned
    if (!refArr) {IlcError(Form("Slot %d is not cloned, slot %d is cloned",refSlot,slot)); return kFALSE;}
    // 
    // both are cloned
    if (refArr->GetEntriesFast()!=currArr->GetEntriesFast()) {
      IlcError(Form("Different number of clones: slot %d: %d vs slot %d: %d",
		    refSlot,refArr->GetEntriesFast(), slot,currArr->GetEntriesFast()));
      return kFALSE;
    }
    //
    if (refArr->TestBit(IlcQAv1::GetOrigHistoKeptBit())!=currArr->TestBit(IlcQAv1::GetOrigHistoKeptBit())) {
      IlcError(Form("Slot %d kept original histo, slot %d did not",
		    refArr->TestBit(IlcQAv1::GetOrigHistoKeptBit()) ? refSlot:slot,refArr->TestBit(IlcQAv1::GetOrigHistoKeptBit()) ? slot:refSlot));
      return kFALSE;
    }
    //
    if (checkTriggers) {
      for (int ih=refArr->GetEntriesFast();ih--;) {
	TH1* hRef = (TH1*)refArr->At(ih);
	if (!hRef->TestBit(IlcQAv1::GetClonedBit())) continue; // original histo was kept, no trigger name is expected
	TString hnm = ((TH1*)currArr->At(ih))->GetName();
	if (!hnm.EndsWith( GetHistoTrigger(hRef) )) {
	  IlcError(Form("Slots %d and %d have different trigger names for clone %d: %s vs %s",
			refSlot,slot,ih,hRef->GetName(),hnm.Data()));
	  return kFALSE;
	}
      }
    }
  }
  //
  return kTRUE;
}

//____________________________________________________________________________
TObjArray* IlcQADataMaker::GetMatchingHistos(TObjArray ** list, Int_t index, TObjArray* optDest)
{
  // return all histograms of given index matching to any trigger in the current event trigger's list
  // If the destination optDest is provided - use it, otherwise fill/return own static array
  // In the latter case one should be careful: the content is overwritten at each call
  // If the original histo was cloned but kept, it is also returned
  //
  // Note: if there are no triggers set, all eventual clones are returned 
  // (used for EndOfDetectorCycle loop over all histos)
  static TObjArray garr;
  TObjArray *dest = optDest ? optDest : &garr;
  dest->SetOwner(kFALSE);
  dest->Clear();
  TObject* data = GetData(list,index);
  if (!data) return 0;
  if (!data->TestBit(IlcQAv1::GetClonedBit())) dest->AddLast(data); // not cloned per trigger
  else {
    TObjArray* arr = dynamic_cast<TObjArray*>(data);
    if (!arr) return 0;
    if (IsEventTriggerSet()) {
      int ncl = GetNEventTrigClasses();
      for (int icl=0;icl<ncl;icl++) {
	int indCl = GetEventTrigClass(icl)->GetUniqueID();
	//	IlcInfo(Form("Ind %d at %d of %p of %p",index,indCl, arr, list[IlcRecoParam::AConvert(fEventSpecie)]));
	TH1* histo = dynamic_cast<TH1*>(arr->At(indCl));
	if (histo) dest->AddLast(histo);
	//	IlcInfo(Form("EvCl%d IndTr%d add to %d IndH %d %p %s",icl,indCl,dest->LastIndex(),index,histo,histo ? histo->GetName():"XXX"));
      }
      // check if original histo was kept
      if (arr->TestBit(IlcQAv1::GetOrigHistoKeptBit())) {
	TH1* histo = dynamic_cast<TH1*>(arr->At(GetNTrigClasses()));
	if (histo) dest->AddLast(histo);
	//	IlcInfo(Form("Def add to %d inex: %d %p %s",dest->LastIndex(),index,histo,histo ? histo->GetName():"XXX"));
      }
    }
    else { // trigger is not set, return all clones
      for (int ih=arr->GetEntriesFast();ih--;) if (arr->At(ih)) dest->AddLast(arr->At(ih));
    }
  }
  return dest;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::FillData(TObjArray ** list, Int_t index, double x)
{
  // fills single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;) if ((TH1*)arr->At(ih)) ((TH1*)arr->At(ih))->Fill(x);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::FillData(TObjArray ** list, Int_t index, double x, double y)
{
  // fills single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;) if ((TH1*)arr->At(ih)) ((TH1*)arr->At(ih))->Fill(x,y);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::FillData(TObjArray ** list, Int_t index, double x, double y, double z)
{
  // fills single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;) if ((TH2*)arr->At(ih)) ((TH2*)arr->At(ih))->Fill(x,y,z);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::SetDataBinContent(TObjArray ** list, Int_t index, int bin, double w)
{
  // set bin content of single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;)  if ((TH2*)arr->At(ih)) ((TH2*)arr->At(ih))->SetBinContent(bin,w);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::SetDataBinContent(TObjArray ** list, Int_t index, int binX, int binY, double w)
{
  // set bin content of single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;)  if ((TH2*)arr->At(ih)) ((TH2*)arr->At(ih))->SetBinContent(binX,binY,w);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::SetDataBinError(TObjArray ** list, Int_t index, int bin, double err)
{
  // set bin content of single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;)  if ((TH2*)arr->At(ih)) ((TH2*)arr->At(ih))->SetBinError(bin,err);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::SetDataBinError(TObjArray ** list, Int_t index, int binX, int binY, double err)
{
  // set bin content of single histo or its trigger-dependent clones, return number of histos filled
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;)  if ((TH2*)arr->At(ih)) ((TH2*)arr->At(ih))->SetBinError(binX,binY,err);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::ResetData(TObjArray ** list, Int_t index, Option_t* option)
{
  // reset content of single histo or its trigger-dependent clones, return number of histos processed
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;)  if ((TH1*)arr->At(ih)) ((TH1*)arr->At(ih))->Reset(option);
  }
  return count;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::ResetStatsData(TObjArray ** list, Int_t index)
{
  // reset stats of single histo or its trigger-dependent clones, return number of histos processed
  TObjArray* arr = GetMatchingHistos(list,index);
  int count = 0;
  if (arr) {
    count = arr->GetEntriesFast();
    for (int ih=count;ih--;) if ((TH1*)arr->At(ih))  ((TH1*)arr->At(ih))->ResetStats();
  }
  return count;
}

//____________________________________________________________________________
TH1 *  IlcQADataMaker::CloneMe(TH1 * hist, Int_t specie) const  
{
  // clones a histogram 
  const Char_t * name = Form("%s_%s", IlcRecoParam::GetEventSpecieName(specie), hist->GetName()) ;
  TH1 * hClone = static_cast<TH1 *>(hist->Clone(name)) ; 
  if ( hist->TestBit(IlcQAv1::GetExpertBit()) )
    hClone->SetBit(IlcQAv1::GetExpertBit()) ; 
  if ( hist->TestBit(IlcQAv1::GetImageBit()) )
    hClone->SetBit(IlcQAv1::GetImageBit()) ; 
  return hClone ; 
}

//____________________________________________________________________________
void IlcQADataMaker::DefaultEndOfDetectorCycle(IlcQAv1::TASKINDEX_t task) 
{
  // this method must be oveloaded by detectors
  // sets the QA result to Fatal
  IlcQAv1::Instance(IlcQAv1::GetDetIndex(GetName())) ;
  IlcQAv1 * qa = IlcQAv1::Instance(task) ;
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
    qa->Set(IlcQAv1::kFATAL, specie) ; 
  IlcQAv1::GetQAResultFile()->cd() ; 
  qa->Write(IlcQAv1::GetQAName(), kWriteDelete) ;   
  IlcQAv1::GetQAResultFile()->Close() ; 
}

//____________________________________________________________________________ 
void IlcQADataMaker::Finish() const 
{ 
  // write to the output File
  if (fOutput) 
    fOutput->Close() ; 
} 

//____________________________________________________________________________ 
TH1* IlcQADataMaker::GetData(TObjArray ** list, const Int_t index, int cloneID)  
{ 
  // get the cloneID-th clone of the histo, -1 for original (if kept)
  TObject* data = GetData(list,index);
  if (!data) return 0;
  Bool_t orig = cloneID<0 || cloneID==GetNTrigClasses();
  if (data->TestBit(IlcQAv1::GetClonedBit())) {
    if (orig) return data->TestBit(IlcQAv1::GetOrigHistoKeptBit()) ? (TH1*)((TObjArray*)data)->At(GetNTrigClasses()) : 0;
    else      return (TH1*)((TObjArray*)data)->At(cloneID); // there was a cloning
  }
  //
  // not cloned, is the original histo requested?
  if (cloneID<0 || cloneID==GetNTrigClasses()) return (TH1*)data;
  return 0;
  //
}

//____________________________________________________________________________ 
TObjArray* IlcQADataMaker::GetDataOfTrigClass(TObjArray ** list, Int_t specieIndex, int cloneID, TObjArray *dest)  
{ 
  // get all histos for cloneID-th trigger class (-1 is for original "all-triggers" histos) in 
  // a single array (if dest is not provided, use its own static array). 
  // Existing histos are attached at their original positions
  if (!list) return 0x0;
  TObjArray* arr = list[specieIndex] ;
  return GetDataOfTrigClass(arr, cloneID, dest);
}

//____________________________________________________________________________ 
TObjArray* IlcQADataMaker::GetDataOfTrigClass(TObjArray *arr, int cloneID, TObjArray *dest)  
{ 
  // get all histos for cloneID-th trigger class (-1 is for original "all-triggers" histos) in 
  // a single array (if dest is not provided, use its own static array). 
  // Existing histos are attached at their original positions
  static TObjArray arrTransf;
  TObjArray* destS = dest ? dest : &arrTransf;
  destS->SetOwner(kFALSE);
  destS->Clear();
  if (arr) {
    Bool_t orig = cloneID<0 || cloneID==GetNTrigClasses(); // is trigger-blind histo requested?
    int nh = arr->GetEntriesFast();
    if (nh>destS->GetSize()) destS->Expand(nh);
    for (int ih=nh;ih--;) {
      TObject* ob = arr->At(ih);
      if (!ob) continue;
      //
      if (ob->TestBit(IlcQAv1::GetClonedBit())) { // this is array of cloned histos
	if (orig) ob = ob->TestBit(IlcQAv1::GetOrigHistoKeptBit()) ? ((TObjArray*)ob)->At(GetNTrigClasses()) : 0;
	else      ob = ((TObjArray*)ob)->At(cloneID);
      }
      else if (!orig) ob = 0;   // histogram was not cloned but the clone is requested
      if (!ob) continue;
      (*destS)[ih] = ob;
    }
    destS->SetUniqueID(arr->GetUniqueID());
  } 
  return destS;
  //
}

//____________________________________________________________________________ 
TObjArray* IlcQADataMaker::GetDataOfTrigClass(TObjArray ** list, int cloneID, TObjArray *dest)  
{ 
  // get all histos for cloneID-th trigger class (-1 is for original "all-triggers" histos) in 
  // a single array (if dest is not provided, use its own static array). 
  // Existing histos are attached at their original positions
  //
  GetData(list,0);  //just to initialize, if needed
  Int_t esindex = IlcRecoParam::AConvert(fEventSpecie);
  return GetDataOfTrigClass(list, esindex, cloneID, dest);
  //
}

//____________________________________________________________________________ 
TObject * IlcQADataMaker::GetData(TObjArray ** list, const Int_t index)  
{ 
  // Returns the QA object at index. Limit is IlcQAv1::GetMaxQAObj() 
  if ( ! list ) {
    IlcError("Data list is NULL !!") ; 
    return NULL ; 		
  }
  Int_t esindex = IlcRecoParam::AConvert(fEventSpecie) ; 
  TObject* histClone = NULL ; 
  TObjArray * arr = list[esindex] ; 
  if (arr) {
    if ( ! arr->GetEntriesFast() ) {
      // Initializes the histograms 
      TString arrName(arr->GetName()) ; 
      if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kRAWS)))
        InitRaws() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kHITS)))
        InitHits() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kSDIGITS)))
        InitSDigits() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kDIGITS)))
        InitDigits() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kDIGITSR)))
        InitDigits() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kRECPOINTS)))
        InitRecPoints() ; 
      else if (arrName.Contains(IlcQAv1::GetTaskName(IlcQAv1::kESDS)))
        InitESDs() ; 
    }
    if ( index > IlcQAv1::GetMaxQAObj() ) {
      IlcError(Form("Max number of authorized QA objects is %d", IlcQAv1::GetMaxQAObj())) ; 
    } else {
      if ( arr->At(index) )  {
        histClone = arr->At(index); 
      } 	
    }
  }
  return histClone ; 		
}

//____________________________________________________________________________ 
TObjArray*  IlcQADataMaker::Init(IlcQAv1::TASKINDEX_t task, IlcRecoParam::EventSpecie_t es, Int_t cycles)
{
  // Initializes and  returns the QAData list for a given event specie
  TObjArray ** ar = Init(task, cycles) ; 
  return ar[IlcRecoParam::AConvert(es)] ;  
}

//____________________________________________________________________________ 
Bool_t IlcQADataMaker::IsValidEventSpecie(Int_t eventSpecieIndex, TObjArray ** list)
{
  // check if event specie was present in current run or 
  // if histograms of this event specie have been created
  if (! IlcQAv1::Instance(IlcQAv1::GetDetIndex(GetName()))->IsEventSpecieSet(IlcRecoParam::ConvertIndex(eventSpecieIndex)) || ! list[eventSpecieIndex]->GetEntriesFast() )
    return kFALSE ;
  else
    return kTRUE ;
}

//____________________________________________________________________________ 
void IlcQADataMaker::SetCloningRequest(TObjArray* aliases, TObjArray* histos)
{
  // set associations {trigger names} <-> {trigger class alias}
  //
  const UInt_t kDummyID = 0xffff;
  //
  if (fgTrigClasses || fgCloningRequest) {
    IlcWarningClass("Cloning request is already set");
    return;
  }
  ResetEventTrigClasses(); 
  if (!aliases || !histos) {
    IlcWarningClass("Cloning request is incomplete");
    return;
  }
  fgTrigClasses = aliases;
  fgCloningRequest = histos;
  //
  // flag those aliases which are actually used
  fgCloningRequest->Compress();
  fgTrigClasses->Compress();
  int nalias = fgTrigClasses->GetEntriesFast();
  int nhisto = fgCloningRequest->GetEntriesFast();
  //
  int naliasUsed = 0;
  for (int ial=0;ial<nalias;ial++) fgTrigClasses->At(ial)->SetUniqueID(kDummyID); // reset usage flag
  //
  for (int ih=nhisto;ih--;) {
    TNamed* histo = (TNamed*)fgCloningRequest->At(ih);
    TString histoReq = histo->GetTitle();    // list of aliases for which the histo must be cloned
    TObjArray* alList = histoReq.Tokenize(" ");
    // 
    if (!alList) {
      IlcErrorClass(Form("Invalid cloning request is found for histo %s :|%s|",histo->GetName(),histo->GetTitle()));
      fgCloningRequest->RemoveAt(ih);
      delete histo;
      continue;
    }
    TString confirmedAliases = "";
    for (int iha=alList->GetEntriesFast();iha--;) {
      // check if corresponding alias is defined
      Bool_t aliasOK = kFALSE;
      for (int ial=0;ial<nalias;ial++) {
	TNamed* alias = (TNamed*)fgTrigClasses->At(ial);
	TString aliasName = alias->GetName();
	if (aliasName == alList->At(iha)->GetName()) { // found
	  aliasOK = kTRUE;
	  if (alias->GetUniqueID() == kDummyID) alias->SetUniqueID(naliasUsed++);      // acknowledge used alias
	}
      }
      if (!aliasOK) {
	IlcErrorClass(Form("Cloning for undefined trigger alias %s is requested for histo %s, SUPPRESSING",
		      alList->At(iha)->GetName(),histo->GetName()));
      }
      else confirmedAliases += Form("%s ",alList->At(iha)->GetName());
    }
    if (confirmedAliases.IsNull()) {
      IlcErrorClass(Form("All requested trigger aliases for histo %s are undefined, SUPPRESSING",histo->GetName()));
      fgCloningRequest->RemoveAt(ih);
      delete histo;
    }
    else histo->SetTitle(confirmedAliases.Data());
    //
    delete alList;
  }
  fgCloningRequest->Compress();
  //
  // check if there are unused aliases
  naliasUsed = 0;
  for (int ial=0;ial<nalias;ial++) {
    TNamed* alias = (TNamed*)fgTrigClasses->At(ial);
    if (alias->GetUniqueID() != kDummyID) alias->SetUniqueID(naliasUsed++); // count as defined
    else {
      IlcWarningClass(Form("Suppressing unused trigger alias %s\t->\t%s",alias->GetName(),alias->GetTitle()));
      delete fgTrigClasses->RemoveAt(ial);
    }
  }
  fgTrigClasses->Compress();
  //
  IlcInfoClass("Confirmed aliases for trigger classes:");
  for (int i=0;i<fgTrigClasses->GetEntriesFast();i++) {
    TNamed* item = (TNamed*)fgTrigClasses->At(i);
    IlcInfoClass(Form("%s -> %s",item->GetName(),item->GetTitle()));
  }
  //
  IlcInfoClass("Confirmed histograms to clone:");
  for (int i=0;i<fgCloningRequest->GetEntriesFast();i++) {
    TNamed* item = (TNamed*)fgCloningRequest->At(i);
    IlcInfoClass(Form("%s -> %s %s",item->GetName(),item->GetTitle(),
		      item->TestBit(IlcQAv1::GetOrigHistoKeptBit()) ? " + TriggerBlind":""));
  }
  // 
}

//____________________________________________________________________________ 
void IlcQADataMaker::IncEvCountCycle(IlcQAv1::TASKINDEX_t task, Int_t diff)
{
  // incerment by "diff" the cycle counters for the current event trigger classes (+ the global one)
  int ncl = GetNTrigClasses();
  int isp = IlcRecoParam::AConvert(fEventSpecie);
  //
  if (fEvCountCycle[isp].GetSize()==IlcQAv1::kNTASKINDEX && ncl>0) fEvCountCycle[isp].Set(IlcQAv1::kNTASKINDEX*(GetNTrigClasses()+1)); // +1 for trigger-independent count
  //
  fEvCountCycle[isp][task*(1+ncl)+ncl] += diff; // trigger independent counter
  for (int icl=GetNEventTrigClasses();icl--;) fEvCountCycle[isp][task*(1+ncl)+ GetEventTrigClass(icl)->GetUniqueID()] += diff;
}

//____________________________________________________________________________ 
void IlcQADataMaker::IncEvCountTotal(IlcQAv1::TASKINDEX_t task, Int_t diff)
{
  // incerment by "diff" the cycle counters for the current event trigger classes (+ the global one)
  int ncl = GetNTrigClasses();
  int isp = IlcRecoParam::AConvert(fEventSpecie);
  //
  if (fEvCountTotal[isp].GetSize()==IlcQAv1::kNTASKINDEX && ncl>0) fEvCountTotal[isp].Set(IlcQAv1::kNTASKINDEX*(GetNTrigClasses()+1)); // +1 for trigger-independent count
  //
  fEvCountTotal[isp][task*(1+ncl)+ncl] += diff; // trigger independent counter
  for (int icl=GetNEventTrigClasses();icl--;) fEvCountTotal[isp][task*(1+ncl)+ GetEventTrigClass(icl)->GetUniqueID()] += diff;
}

//____________________________________________________________________________
void IlcQADataMaker::ResetEvCountCycle(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task) 
{
  // reset counter for specific trigger class for specific specie (within the cycle)
  int ncl = GetNTrigClasses();
  // check if it was already expanded
  if (fEvCountCycle[isp].GetSize()==IlcQAv1::kNTASKINDEX) fEvCountCycle[isp].Set(IlcQAv1::kNTASKINDEX*(ncl+1));
  for (int icl=ncl+1;icl--;) fEvCountCycle[IlcRecoParam::AConvert(isp)][task*(1+ncl) + icl] = 0;
}

//____________________________________________________________________________
void IlcQADataMaker::ResetEvCountTotal(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task) 
{
  // reset counter for specific trigger class for specific specie (global count)
  int ncl = GetNTrigClasses();
  // check if it was already expanded
  if (fEvCountTotal[isp].GetSize()==IlcQAv1::kNTASKINDEX) fEvCountTotal[isp].Set(IlcQAv1::kNTASKINDEX*(ncl+1));
  for (int icl=ncl+1;icl--;) fEvCountTotal[IlcRecoParam::AConvert(isp)][task*(1+ncl) + icl] = 0;
}

//____________________________________________________________________________
Int_t IlcQADataMaker::GetEvCountCycle(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task, Int_t trCl) const
{
  // get counter for specific trigger class for specific specie (within the cycle)
  int ncl = GetNTrigClasses();
  return fEvCountCycle[IlcRecoParam::AConvert(isp)][task*(1+ncl) + (trCl<0||trCl>=ncl ? ncl:trCl)];
}
//____________________________________________________________________________
Int_t IlcQADataMaker::GetEvCountTotal(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task, Int_t trCl) const
{
  // get counter for specific trigger class for specific specie (global count)
  int ncl = GetNTrigClasses();
  return fEvCountTotal[IlcRecoParam::AConvert(isp)][task*(1+ncl) + (trCl<0||trCl>=ncl ? ncl:trCl)];
}

