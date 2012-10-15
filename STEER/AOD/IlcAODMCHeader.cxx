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

//-------------------------------------------------------------------------
//                      Implementation of   Class IlcAODMCHeader
//   Header data
//   for the ESD   
//   Origin: Christian Klein-Boesing, CERN, Christian.Klein-Boesing@cern.ch 
//-------------------------------------------------------------------------

#include "TList.h"
#include "IlcAODMCHeader.h"
#include "IlcGenEventHeader.h"
#include "IlcGenCocktailEventHeader.h"




ClassImp(IlcAODMCHeader)

// Without a trailing dot root does not support
// direct drawing of some variables if the name is not unique on top label
// bnrach e.g. fEventType is found here and in IlcAODHeader....
TString IlcAODMCHeader::fgkStdBranchName("mcHeader");

//______________________________________________________________________________

IlcAODMCHeader::IlcAODMCHeader() :
  IlcVHeader()
  ,fGenerator("")
  ,fImpactPar(0)
  ,fPtHard(0)
  ,fXsection(0)
  ,fTrials(0)
  ,fEventType(0)
  ,fReactionPlaneAngle(0)  
  ,fHeaders(0)
{
  // default constructor
  fVertex[0] = fVertex[1] = fVertex[2] = 0;  
  SetName(fgkStdBranchName.Data());
}


IlcAODMCHeader::~IlcAODMCHeader() 
{

  Reset();
  delete fHeaders;
  // destructor
}


IlcAODMCHeader::IlcAODMCHeader(const IlcAODMCHeader &header) :
  IlcVHeader(header)
  ,fGenerator(header.fGenerator)
  ,fImpactPar(header.fImpactPar)
  ,fPtHard(header.fPtHard)
  ,fXsection(0)
  ,fTrials(0)
  ,fEventType(header.fEventType)
  ,fReactionPlaneAngle(header.fReactionPlaneAngle)  
  ,fHeaders(0)
{
  // copy constructor
  for(int i = 0;i<3;++i)fVertex[i] = header.fVertex[i];
  SetName(header.fName);
  SetTitle(header.fTitle);
}

IlcAODMCHeader& IlcAODMCHeader::operator=(const IlcAODMCHeader &header)
{ 
  // assigment operator

  if(this!=&header) {
    Reset();
    IlcVHeader::operator=(header);
    fGenerator = header.fGenerator;
    for(int i = 0;i<3;++i)fVertex[i] = header.fVertex[i];
    fImpactPar = header.fImpactPar;
    fPtHard = header.fPtHard;
    fXsection = header.fXsection;
    fTrials = header.fTrials;
    fEventType = header.fEventType;
    fReactionPlaneAngle = header.fReactionPlaneAngle;

    if(header.fHeaders){
      for(int i = 0;i < header.fHeaders->GetEntries();i++){
	AddCocktailHeader(dynamic_cast<IlcGenEventHeader*>(header.fHeaders->At(i)));
      }
    }
  } 
  return *this;
}

void IlcAODMCHeader::AddCocktailHeader(const IlcGenEventHeader* header)
{
// Add a header to the list
  if(!header)return;
  if (!fHeaders){ 
    fHeaders = new TList();
    fHeaders->SetOwner(kTRUE);
  }
  fHeaders->Add(header->Clone());
}

void IlcAODMCHeader::Copy(TObject &obj) const {
  
  // this overwrites the virtual TOBject::Copy()
  // to allow run time copying without casting
  // in IlcESDEvent

  if(this==&obj)return;
  IlcAODMCHeader *robj = dynamic_cast<IlcAODMCHeader*>(&obj);
  if(!robj)return; // not an IlcAODMCHeader
  *robj = *this;

}



//______________________________________________________________________________
void IlcAODMCHeader::Reset()
{
  // reset all data members
  fGenerator = "";
  fImpactPar = 0;
  fEventType = 0;
  fPtHard = 0;
  fXsection = 0;
  fTrials = 0;
  fVertex[0] = fVertex[1] = fVertex[2] = 0;  
  fReactionPlaneAngle = 0;
  if(fHeaders)fHeaders->Delete();
}

//______________________________________________________________________________
void IlcAODMCHeader::Print(const Option_t *) const
{
  // Print some data members
  Printf("MC EventHeader Generators: %s # EventType %d  Vtx = (%3.3f,%3.3f,%3.3f) ptHard = %3.3f GeV Impact parameter %3.3f  \n",
	 GetGeneratorName(),
	 GetEventType(),
	 GetVtxX(),GetVtxY(),GetVtxZ(),GetPtHard(),
	 GetImpactParameter());
  if(fHeaders){
    fHeaders->Print();
    for(int i = 0;i<fHeaders->GetEntries();++i){
      TObject *obj = fHeaders->At(i);
      if(obj){
	Printf(">> %d: %s %s",i,obj->GetName(),obj->GetTitle());
      }
    }
  }
}

IlcGenEventHeader* IlcAODMCHeader::GetCocktailHeader(Int_t i){
  if(i<0)return 0;
  return (IlcGenEventHeader*)(fHeaders->At(i));
}

void  IlcAODMCHeader::AddCocktailHeaders(IlcGenEventHeader* header){
  IlcGenCocktailEventHeader *cHeader = dynamic_cast<IlcGenCocktailEventHeader*>(header);
  if(cHeader){
      TList *genHeaders = cHeader->GetHeaders();
      IlcGenEventHeader* gH = 0;
      for (int i=0; i<genHeaders->GetEntries(); i++) {
	gH = (IlcGenEventHeader*)genHeaders->At(i);
	if(gH){
	  AddGeneratorName(gH->GetName());
	  AddCocktailHeader(dynamic_cast<IlcGenEventHeader*>(genHeaders->At(i)));	
	}
      }
  }
  else{
    // no cocktail header just addd the global header
    AddCocktailHeader(header);	
  }
}

void   IlcAODMCHeader::AddGeneratorName(const char* c){
  if(fGenerator.Length()==0)fGenerator += c;
  else {
    fGenerator += " ";
    fGenerator += c;
  }
}
