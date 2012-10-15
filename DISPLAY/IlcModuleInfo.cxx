/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ALICE Off-line Project.         *
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
/////////////////////////////////////////////////////////////////////////
// ILC MODULE INFORMATION CLASS                                      //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "IlcModuleInfo.h"

ClassImp(IlcModuleInfo)

//_____________________________________________________________
IlcModuleInfo::IlcModuleInfo(int n)
{
  // Constructor
  fName = new char*[n];
  fId = new Int_t[n];
  fEnabled = new Bool_t[n];
  fNb = 0;
}

//_____________________________________________________________
IlcModuleInfo::IlcModuleInfo(const IlcModuleInfo& rh)
{
  // Copy constructor
  fName = new char*[rh.fNb];
  fId = new Int_t[rh.fNb];
  fEnabled = new Bool_t[rh.fNb];
  fNb = rh.fNb;

  for (Int_t i=0; i<fNb; i++) {
    strcpy(fName[i],rh.fName[i]);
    fId[i] = rh.fId[i];
    fEnabled[i] = rh.fEnabled[i];
  }
}

//_____________________________________________________________
IlcModuleInfo& IlcModuleInfo::operator = (const IlcModuleInfo& rh)
{
  // Assignment operator
  fName = new char*[rh.fNb];
  fId = new Int_t[rh.fNb];
  fEnabled = new Bool_t[rh.fNb];
  fNb = rh.fNb;

  for (Int_t i=0; i<fNb; i++) {
    strcpy(fName[i],rh.fName[i]);
    fId[i] = rh.fId[i];
    fEnabled[i] = rh.fEnabled[i];
  }
  return *this;
}

//_____________________________________________________________
IlcModuleInfo::~IlcModuleInfo(){
  // Destructor
  delete [] fName;
  delete [] fId;
  delete [] fEnabled;
}

//_____________________________________________________________
void IlcModuleInfo::Add(const char *name,Int_t i)
{
  // Adds new module to the list
  fName[fNb]=new char[strlen(name)];
  strcpy(fName[fNb],name);
  fId[fNb]=i;
  fEnabled[fNb]=kTRUE;
  fNb++;
}

//_____________________________________________________________
void IlcModuleInfo::SetId(const char *name,Int_t id)
{
  // Sets the Id of the module "name"
  Int_t i=0;
  while((strcmp(name,fName[i])!=0)&&(i!=fNb)) i++;
  if(strcmp(name,fName[i])==0) fId[i]=id;
}

//_____________________________________________________________
const char* IlcModuleInfo::Name(Int_t id) const
{
  // Sets the name of module id
  Int_t i=0;
  while((fId[i]!=id)&&(i!=fNb)) i++;
  if(fId[i]==id) return fName[i];
  return 0;
}
  
//_____________________________________________________________
Int_t IlcModuleInfo::Id(const char *name) const
{
  // Return the id of module "name"
  Int_t i=0;
  while((strcmp(name,fName[i])!=0)&&(i!=fNb)) i++;
  if(strcmp(name,fName[i])==0) return fId[i];
  return -1;
}

//_____________________________________________________________
Bool_t IlcModuleInfo::IsEnabled(Int_t id) const
{
  //return the current status of the detector
  Int_t i=0;
  while((fId[i]!=id)&&(i!=fNb)) i++;
  if(fId[i]==id) return fEnabled[i];
  return kFALSE;
}

//_____________________________________________________________
void IlcModuleInfo::Disable(Int_t id)
{
  //Disable the detector 
  Int_t i=0;
  while((fId[i]!=id)&&(i!=fNb)) i++;
  if(fId[i]==id) fEnabled[i]=kFALSE;
}

//_____________________________________________________________
void IlcModuleInfo::Enable(Int_t id)
{
  //Enable the detector 
  Int_t i=0;
  while((fId[i]!=id)&&(i!=fNb)) i++;
  if(fId[i]==id) fEnabled[i]=kTRUE;
}

//_____________________________________________________________
void IlcModuleInfo::Print() const
{
  // Prints the content of all arrays
  printf("\n***Modules***");
  printf("\nName\tId\tEnabled"); 
  for(Int_t i=0;i<fNb;i++){
    printf("\n%s",fName[i]);
    printf("\t%d",fId[i]);
    printf("\t%d",fEnabled[i]);
  }
}


