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


/* $Id: IlcCorrQADataMakerRec.cxx 27570 2008-07-24 21:49:27Z cvetan $ */

/*
  Produces the data needed to calculate the quality assurance. 
  All data must be mergeable objects.
  Y. Schutz CERN July 2007
*/

// --- ROOT system ---
#include <TClonesArray.h>
#include <TFile.h> 
#include <TH1F.h> 
#include <TH1I.h> 
#include <TH2F.h> 
#include <TNtupleD.h>
#include <TParameter.h>
#include <TMath.h> 

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcCorrQADataMakerRec.h"
#include "IlcQAChecker.h"

ClassImp(IlcCorrQADataMakerRec)
           
//____________________________________________________________________________ 
IlcCorrQADataMakerRec::IlcCorrQADataMakerRec(IlcQADataMaker ** qadm ) : 
IlcQADataMakerRec(IlcQAv1::GetDetName(IlcQAv1::kCORR), "Corr Quality Assurance Data Maker"),
  fMaxRawVar(0),  
  fqadm(qadm),
  fVarvalue(NULL)
{
  // ctor
  fCorrNt = new TNtupleD *[IlcRecoParam::kNSpecies] ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
    fCorrNt[specie] = NULL ; 
}

//____________________________________________________________________________ 
IlcCorrQADataMakerRec::IlcCorrQADataMakerRec(const IlcCorrQADataMakerRec& qadm) :
  IlcQADataMakerRec(qadm),
  fMaxRawVar(qadm.fMaxRawVar), 
  fqadm(qadm.fqadm),
  fVarvalue(NULL)
{
  //copy ctor 
  if ( fMaxRawVar > 0 ) {
    fVarvalue = new Double_t[fMaxRawVar] ;
    memcpy(fVarvalue, qadm.fVarvalue, fMaxRawVar*sizeof(Double_t));
  }

  // Replace shallow copy done by IlcQADataMakerRec by a semi-deep
  // copy where the pointer container is recreated but the Ntuples pointed 
  // to are simply copied
  fCorrNt = new TNtupleD *[IlcRecoParam::kNSpecies] ; 
  for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
    fCorrNt[specie] = qadm.fCorrNt[specie] ; 

}

//__________________________________________________________________
IlcCorrQADataMakerRec& IlcCorrQADataMakerRec::operator = (const IlcCorrQADataMakerRec& qadm )
{
  // assign operator.
  if(this != &qadm) {
    IlcQADataMakerRec::operator=(qadm);
    fMaxRawVar = qadm.fMaxRawVar;
    fqadm = qadm.fqadm;
    delete [] fVarvalue;
    if ( fMaxRawVar > 0 ) {
      fVarvalue = new Double_t[fMaxRawVar] ;
      memcpy(fVarvalue, qadm.fVarvalue, fMaxRawVar*sizeof(Double_t));
    } else fVarvalue = 0;

    // Replace shallow copy done by IlcQADataMakerRec by a semi-deep
    // copy where the pointer container is recreated but the Ntuples pointed 
    // to are simply copied
    fCorrNt = new TNtupleD *[IlcRecoParam::kNSpecies] ; 
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
      fCorrNt[specie] = qadm.fCorrNt[specie] ; 
  }
  return *this;
}
   
//____________________________________________________________________________ 
IlcCorrQADataMakerRec::~IlcCorrQADataMakerRec()  
{
  //
  // dtor only destroy the ntuple otherwise it would violate ownership...
  // however when the last IlcCorrQADataMakerRec is deleted there is
  // a leak
  //
  //  if ( fCorrNt ) 
  //    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; ++specie) 
  //        delete fCorrNt[specie] ; 
  //  
  delete [] fCorrNt ; 
  delete [] fVarvalue ;
}
  
//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::EndOfDetectorCycle(IlcQAv1::TASKINDEX_t task, TObjArray ** /*list*/)
{
  //Detector specific actions at end of cycle
  // do the QA checking
  if (task == IlcQAv1::kRAWS) {
     IlcQAChecker::Instance()->Run(IlcQAv1::kCORR, task, fCorrNt) ; 
  }
}

//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::InitESDs()
{
  //Create histograms to controll ESD

  IlcInfo("TO BE IMPLEMENTED") ; 
  //
  ClonePerTrigClass(IlcQAv1::kESDS); // this should be the last line
}


//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::InitRaws()
{
  // createa ntuple taking all the parameters declared by detectors
  if (fCorrNt && fCorrNt[IlcRecoParam::AConvert(fEventSpecie)]) 
    return ; 

  if (!fCorrNt) {
    fCorrNt = new TNtupleD *[IlcRecoParam::kNSpecies] ; ;
    for (Int_t specie = 0 ; specie < IlcRecoParam::kNSpecies ; specie++) 
      fCorrNt[specie] = NULL ;
  } 

  if ( fRawsQAList ) 
  {
    delete[] fRawsQAList ; // not needed for the time being 
    fRawsQAList = NULL ; 
  }
  TString varlist("") ;
  for ( Int_t detIndex = 0 ; detIndex < IlcQAv1::kNDET ; detIndex++ ) {
    IlcQADataMaker * qadm = fqadm[detIndex] ; 
    if ( ! qadm ) 
      continue ;
    TList * list = qadm->GetParameterList() ; 
    if (list) {
      TIter next(list) ; 
      TParameter<double> * p ; 
      while ( (p = static_cast<TParameter<double>*>(next()) ) ) {
        varlist.Append(p->GetName()) ; 
        varlist.Append(":") ; 
        fMaxRawVar++ ; 
      }
    }
  }
  varlist = varlist.Strip(TString::kTrailing, ':') ; 
  if (fMaxRawVar == 0) { 
    IlcWarning("NTUPLE not created") ; 
  } else {
    char * name = Form("%s_%s", IlcQAv1::GetQACorrName(), IlcRecoParam::GetEventSpecieName(fEventSpecie)) ; 
    fCorrNt[IlcRecoParam::AConvert(fEventSpecie)] = new TNtupleD(name, "Raws data correlation among detectors", varlist.Data()) ;  
    fVarvalue = new Double_t[fMaxRawVar] ;
  }  
  //
  ClonePerTrigClass(IlcQAv1::kRAWS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::InitRecPoints()
{
    // create Reconstructed Points histograms in RecPoints subdir
  
  IlcInfo("TO BE IMPLEMENTED") ; 
  ClonePerTrigClass(IlcQAv1::kRECPOINTS); // this should be the last line
}

//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::InitRecoParams()
{
    // Get the recoparam form the OCDB for every detector involved in CORR
  
  IlcInfo("TO BE IMPLEMENTED") ; 
}

//____________________________________________________________________________
void IlcCorrQADataMakerRec::MakeESDs(IlcESDEvent * /*esd*/)
{
  // make QA data from ESDs

  IlcInfo("TO BE IMPLEMENTED") ; 
  IncEvCountCycleESDs();
  IncEvCountTotalESDs();
  //
}

//____________________________________________________________________________
void IlcCorrQADataMakerRec::MakeRaws(IlcRawReader *)
{
  //Fill prepared histograms with Raw digit properties
  
  if ( !fCorrNt || ! fCorrNt[IlcRecoParam::AConvert(fEventSpecie)])
      InitRaws() ; 
  
  if ( fMaxRawVar > 0 ) {
    Int_t index = 0 ;
    for ( Int_t detIndex = 0 ; detIndex < IlcQAv1::kNDET ; detIndex++ ) {
      IlcQADataMaker * qadm = fqadm[detIndex] ; 
      if ( ! qadm ) 
        continue ;
      TList * list = qadm->GetParameterList() ; 
      if (list) {
	TIter next(list) ; 
	TParameter<double> * p ; 
	while ( (p = static_cast<TParameter<double>*>(next()) ) ) {
	  if (index >= fMaxRawVar) {
	    IlcError(Form("Variables list size exceeded (%d) !",index));
	    break;
	  }
	  fVarvalue[index] = p->GetVal() ; 
	  index++ ; 
	}
      }
    }
    static_cast<TNtupleD*>(fCorrNt[IlcRecoParam::AConvert(fEventSpecie)])->Fill(fVarvalue);
  }
  //
  IncEvCountCycleRaws();
  IncEvCountTotalRaws();
  //
}

//____________________________________________________________________________
void IlcCorrQADataMakerRec::MakeRecPoints(TTree * /*clustersTree*/)
{
  IlcInfo("TO BE IMPLEMENTED") ; 
  IncEvCountCycleRecPoints();
  IncEvCountTotalRecPoints();
  //
}

//____________________________________________________________________________ 
void IlcCorrQADataMakerRec::StartOfDetectorCycle()
{
  //Detector specific actions at start of cycle  

}
