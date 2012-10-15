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

#include "Riostream.h"
#include "TFile.h"
#include "TSystem.h"
#include "TClonesArray.h"

#include "IlcSurveyToAlignObjs.h"
#include "IlcSurveyPoint.h"
#include "IlcAlignObjParams.h"

#include "IlcLog.h"

#include "IlcCDBManager.h"
#include "IlcCDBEntry.h"
#include "IlcCDBStorage.h"
#include "IlcCDBMetaData.h"

ClassImp(IlcSurveyToAlignObjs)

//________________________________________________________________________
IlcSurveyToAlignObjs::IlcSurveyToAlignObjs() :
  TObject(),
  fSurveyObj(new IlcSurveyObj()),
  fSurveyPoints(NULL),
  fIlcgnObjArray(new TClonesArray("IlcAlignObjParams",10)),
  fIlcgnObj(0)
{
  //
  //  default constructor
  //
}   

//_________________________________________________________________________
IlcSurveyToAlignObjs::IlcSurveyToAlignObjs(const IlcSurveyToAlignObjs &s2aObj) :
  TObject(s2aObj),
  fSurveyObj(s2aObj.fSurveyObj),
  fSurveyPoints(NULL),
  fIlcgnObjArray(NULL),
  fIlcgnObj(s2aObj.fIlcgnObj)
{
  // copy constructor
	fSurveyPoints = (TObjArray*)(s2aObj.fSurveyObj->Clone());
	fIlcgnObjArray = (TClonesArray*) (s2aObj.fIlcgnObjArray->Clone());
}

//__________________________________________________________________________
IlcSurveyToAlignObjs & IlcSurveyToAlignObjs::operator= (const IlcSurveyToAlignObjs &s2aObj) {
  //
  // assignment operator
  //
    if(this != &s2aObj) {
	//if(s2aObj.fSurveyObj){
	    //delete fSurveyObj;
	    this->fSurveyObj = s2aObj.fSurveyObj;
	//}
	//if(s2aObj.fSurveyPoints){
	    //fSurveyPoints->Delete();
	    //delete fSurveyPoints;
	    fSurveyPoints = (TObjArray*)(s2aObj.fSurveyObj->Clone());
	//}
	//if(s2aObj.fIlcgnObjArray){
	    //fIlcgnObjArray->Delete();
	    //delete fIlcgnObjArray;
	    fIlcgnObjArray = (TClonesArray*) (s2aObj.fIlcgnObjArray->Clone());
	//}
	//if(s2aObj.fIlcgnObj){
	    //delete fIlcgnObj;
	    this->fIlcgnObj = s2aObj.fIlcgnObj;
	//}
    }
    return *this;
}

//__________________________________________________________________________
IlcSurveyToAlignObjs::~IlcSurveyToAlignObjs() 
{
  //
  // destructor
  //
  delete fSurveyObj;
  delete fSurveyPoints;
  delete fIlcgnObjArray;
  delete fIlcgnObj;
}

//__________________________________________________________________________
Bool_t IlcSurveyToAlignObjs::LoadSurveyFromLocalFile(const char* filename) {
  // Load survey data from a formatted text file
  // residing locally
  //
  
  //Load survey data from the local file
  if(fSurveyObj->FillFromLocalFile(filename))
    fSurveyPoints = fSurveyObj->GetData();
  else 
    return kFALSE;
  
  IlcInfo(Form("%d survey points read",fSurveyPoints->GetEntries()));  
  
  return kTRUE;
}

//__________________________________________________________________________
Bool_t IlcSurveyToAlignObjs::LoadSurveyFromAlienFile(const char* det, Int_t repNum, Int_t repVersion) {
  // Load survey data from the formatted text file
  // residing in the default location in alien
  //
  
  const char* alienUser=gSystem->Getenv("alien_API_USER");
  if(fSurveyObj->Fill(det, repNum, repVersion, alienUser))
  {
    fSurveyPoints = fSurveyObj->GetData();
  }else{
    IlcError("Error reading survey file from alien!");
    return kFALSE;
  }
  
  IlcInfo(Form("%d survey points read",fSurveyPoints->GetEntries()));  
  
  return kTRUE;
}

//_________________________________________________________________________
Bool_t IlcSurveyToAlignObjs::StoreAlignObjToFile(const char* filename, const char* det){
  // Stores the TClonesArray of alignment objects into the
  // file specified as argument
  //
  TFile *f = TFile::Open(filename,"RECREATE");
  if(!f){
    IlcError(Form("cannot open file %s\n",filename));
    return kFALSE;
  }
  IlcInfo(Form("Saving alignment objects into the file %s",filename));
  TString arrayname(det);
  arrayname+="AlignObjs";
      
  f->cd();
  f->WriteObject(fIlcgnObjArray,arrayname,"kSingleKey");
  f->Close();

  return kTRUE;
}

//_________________________________________________________________________
Bool_t IlcSurveyToAlignObjs::StoreAlignObjToCDB(const char* cdbFolder, const char* det){
  // Stores the TClonesArray of alignment objects into a
  // CDB entry in the CDB folder specified by the argument
  //

  IlcCDBManager* cdb = IlcCDBManager::Instance();
  cdb->SetDefaultStorage(cdbFolder);
  cdb->SetRun(0);

  IlcCDBMetaData* md = new IlcCDBMetaData();
  md->SetComment(Form("Misalignment for subdetector %s from survey",det));
  TString path(det);
  path+="/Align/Data";
  IlcCDBId id(path.Data(),0,IlcCDBRunRange::Infinity());
  cdb->Put(fIlcgnObjArray,id,md);

  return kTRUE;
}


