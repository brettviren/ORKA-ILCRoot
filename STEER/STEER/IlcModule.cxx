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

/* $Id: IlcModule.cxx 53942 2012-01-16 16:47:12Z fca $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Base class for ILC modules. Both sensitive modules (Modules) and      //
// non-sensitive ones are described by this base class. This class           //
// supports the hit and digit trees produced by the simulation and also      //
// the objects produced by the reconstruction.                               //
//                                                                           //
// This class is also responsible for building the geometry of the           //
// Modules.                                                                //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcModuleClass.gif">
*/
//End_Html
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TClonesArray.h>
#include <TTree.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>
#include <TString.h>

#include "IlcLog.h"
#include "IlcConfig.h"
#include "IlcLoader.h"
#include "IlcMagF.h"
#include "IlcModule.h"
#include "IlcRun.h"
#include "IlcTrackReference.h"
#include "IlcMC.h"
#include "IlcSimulation.h"
#include "IlcRawDataHeader.h"
#include "IlcDigitizationInput.h"

#include "IlcDAQ.h"

ClassImp(IlcModule)
 
Float_t IlcModule::fgDensityFactor = 1.0;
 
//_______________________________________________________________________
IlcModule::IlcModule():
  fIdtmed(0),
  fIdmate(0),
  fLoMedium(0),
  fHiMedium(0),
  fActive(0),
  fEnable(1),
  fMaxIterTrackRef(0),
  fCurrentIterTrackRef(0),
  fRunLoader(0),
  fDigInput(0)
{
  //
  // Default constructor for the IlcModule class
  //
}
 
//_______________________________________________________________________
IlcModule::IlcModule(const char* name,const char *title):
  TNamed(name,title),
  fIdtmed(new TArrayI(100)),
  fIdmate(new TArrayI(100)),
  fLoMedium(65536),
  fHiMedium(0),
  fActive(0),
  fEnable(1),
  fMaxIterTrackRef(0),
  fCurrentIterTrackRef(0),
  fRunLoader(0),
  fDigInput(0)
{
  //
  // Normal constructor invoked by all Modules.
  // Create the list for Module specific histograms
  // Add this Module to the global list of Modules in Run.
  //
  // Get the Module numeric ID

  Int_t id = gIlc->GetModuleID(name);
  if (id>=0) {
    // Module already added !
     IlcWarning(Form("Module: %s already present at %d",name,id));
     return;
  }
  //
  // Add this Module to the list of Modules

  gIlc->AddModule(this);

  //PH  SetMarkerColor(3);
  //
  // Clear space for tracking media and material indexes

  for(Int_t i=0;i<100;i++) (*fIdmate)[i]=(*fIdtmed)[i]=0;
}
 
//_______________________________________________________________________
IlcModule::~IlcModule()
{
  //
  // Destructor
  //

  // Remove this Module from the list of Modules
  if (gIlc) {
    TObjArray * modules = gIlc->Modules();
    if (modules) modules->Remove(this);
  }

  // Delete TArray objects
  delete fIdtmed;
  delete fIdmate;

} 

//_______________________________________________________________________
void IlcModule::IlcMaterial(Int_t imat, const char* name, Float_t a, 
                            Float_t z, Float_t dens, Float_t radl,
                            Float_t absl, Float_t *buf, Int_t nwbuf) const
{
  //
  // Store the parameters for a material
  //
  // imat        the material index will be stored in (*fIdmate)[imat]
  // name        material name
  // a           atomic mass
  // z           atomic number
  // dens        density
  // radl        radiation length
  // absl        absorbtion length
  // buf         adress of an array user words
  // nwbuf       number of user words
  //
  Int_t kmat;
  //Build the string uniquename as "DET_materialname"
  TString uniquename = GetName();
  uniquename.Append("_");
  uniquename.Append(name);
  //if geometry loaded from file only fill fIdmate, else create material too
  if(IlcSimulation::Instance()->IsGeometryFromFile()){
    TGeoMaterial *mat = gGeoManager->GetMaterial(uniquename.Data());
    kmat = mat->GetUniqueID();
    (*fIdmate)[imat]=kmat;
  }else{
    if (fgDensityFactor != 1.0)
      IlcWarning(Form("Material density multiplied by %.2f!", fgDensityFactor));
    gMC->Material(kmat, uniquename.Data(), a, z, dens * fgDensityFactor, radl, absl, buf, nwbuf);
    (*fIdmate)[imat]=kmat;
  }
}

//_______________________________________________________________________
void IlcModule::IlcGetMaterial(Int_t imat, char* name, Float_t &a, 
                               Float_t &z, Float_t &dens, Float_t &radl,
                               Float_t &absl) const
{
  //
  // Store the parameters for a material
  //
  // imat        the material index will be stored in (*fIdmate)[imat]
  // name        material name
  // a           atomic mass
  // z           atomic number
  // dens        density
  // radl        radiation length
  // absl        absorbtion length
  // buf         adress of an array user words
  // nwbuf       number of user words
  //

  Int_t kmat=(*fIdmate)[imat];
  TString sname;
  TArrayD par;
  Double_t da, dz, ddens, dradl, dabsl;
  gMC->GetMaterial(kmat, sname, da, dz, ddens, dradl, dabsl, par);

  const char* chname = sname.Data();
  strncpy(name, chname, strlen(chname)+1);
  a = da;
  z = dz;
  dens = ddens;
  radl = dradl;
  absl = dabsl;
}

//_______________________________________________________________________
void IlcModule::IlcMixture(Int_t imat, const char *name, Float_t *a,
                           Float_t *z, Float_t dens, Int_t nlmat,
                           Float_t *wmat) const
{ 
  //
  // Defines mixture or compound imat as composed by 
  // nlmat materials defined by arrays a, z and wmat
  // 
  // If nlmat > 0 wmat contains the proportion by
  // weights of each basic material in the mixture  
  // 
  // If nlmat < 0 wmat contains the number of atoms 
  // of eack kind in the molecule of the compound
  // In this case, wmat is changed on output to the relative weigths.
  //
  // imat        the material index will be stored in (*fIdmate)[imat]
  // name        material name
  // a           array of atomic masses
  // z           array of atomic numbers
  // dens        density
  // nlmat       number of components
  // wmat        array of concentrations
  //
  Int_t kmat;
  //Build the string uniquename as "DET_mixturename"
  TString uniquename = GetName();
  uniquename.Append("_");
  uniquename.Append(name);
  //if geometry loaded from file only fill fIdmate, else create mixture too
  if(IlcSimulation::Instance()->IsGeometryFromFile()){
    TGeoMaterial *mat = gGeoManager->GetMaterial(uniquename.Data());
    kmat = mat->GetUniqueID();
    (*fIdmate)[imat]=kmat;
  }else{
    if (fgDensityFactor != 1.0)
      IlcWarning(Form("Material density multiplied by %.2f!", fgDensityFactor));
    gMC->Mixture(kmat, uniquename.Data(), a, z, dens * fgDensityFactor, nlmat, wmat);
    (*fIdmate)[imat]=kmat;
  }
} 
 
//_______________________________________________________________________
void IlcModule::IlcMedium(Int_t numed, const char *name, Int_t nmat,
                          Int_t isvol, Int_t ifield, Float_t fieldm,
                          Float_t tmaxfd, Float_t stemax, Float_t deemax,
                          Float_t epsil, Float_t stmin, Float_t *ubuf,
                          Int_t nbuf) const
{ 
  //
  // Store the parameters of a tracking medium
  //
  // numed       the medium number is stored into (*fIdtmed)[numed]
  // name        medium name
  // nmat        the material number is stored into (*fIdmate)[nmat]
  // isvol       sensitive volume if isvol!=0
  // ifield      magnetic field flag (see below)
  // fieldm      maximum magnetic field
  // tmaxfd      maximum deflection angle due to magnetic field
  // stemax      maximum step allowed
  // deemax      maximum fractional energy loss in one step
  // epsil       tracking precision in cm
  // stmin       minimum step due to continuous processes
  //
  // ifield =  0       no magnetic field
  //        = -1       user decision in guswim
  //        =  1       tracking performed with Runge Kutta
  //        =  2       tracking performed with helix
  //        =  3       constant magnetic field along z
  //  
  Int_t kmed;
  //Build the string uniquename as "DET_mediumname"
  TString uniquename = GetName();
  uniquename.Append("_");
  uniquename.Append(name);
  //if geometry loaded from file only fill fIdtmed, else create medium too
  if(IlcSimulation::Instance()->IsGeometryFromFile()){
    TGeoMedium *med = gGeoManager->GetMedium(uniquename.Data());
    kmed = med->GetId();
    (*fIdtmed)[numed]=kmed;
  }else{
    gMC->Medium(kmed, uniquename.Data(), (*fIdmate)[nmat], isvol, ifield,
                fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, nbuf);
    (*fIdtmed)[numed]=kmed;
  }
} 
 
//_______________________________________________________________________
void IlcModule::IlcMatrix(Int_t &nmat, Float_t theta1, Float_t phi1,
                          Float_t theta2, Float_t phi2, Float_t theta3,
                          Float_t phi3) const
{
  // 
  // Define a rotation matrix. Angles are in degrees.
  //
  // nmat        on output contains the number assigned to the rotation matrix
  // theta1      polar angle for axis I
  // phi1        azimuthal angle for axis I
  // theta2      polar angle for axis II
  // phi2        azimuthal angle for axis II
  // theta3      polar angle for axis III
  // phi3        azimuthal angle for axis III
  //
  gMC->Matrix(nmat, theta1, phi1, theta2, phi2, theta3, phi3); 
} 

//_______________________________________________________________________
Float_t IlcModule::ZMin() const
{
  return -500;
}

//_______________________________________________________________________
Float_t IlcModule::ZMax() const
{
  return 500;
}

//_______________________________________________________________________
void IlcModule::AddAlignableVolumes() const
{
  // 
  if (IsActive())
    IlcWarning(Form(" %s still has to implement the AddAlignableVolumes method!",GetName()));
}

//_______________________________________________________________________

IlcLoader*  IlcModule::MakeLoader(const char* /*topfoldername*/)
{
  return 0x0;
}
 

//_____________________________________________________________________________
IlcTrackReference*  IlcModule::AddTrackReference(Int_t label, Int_t id){
  //
  // add a trackrefernce to the list
    return (gIlc->GetMCApp()->AddTrackReference(label, id));
}

//_____________________________________________________________________________
TTree* IlcModule::TreeTR()
{
  //
  // Return TR tree pointer
  //
  if ( fRunLoader == 0x0)
   {
     IlcError("Can not get the run loader");
     return 0x0;
   }

  TTree* tree = fRunLoader->TreeTR();
  return tree;
}


//_____________________________________________________________________________
void IlcModule::Digits2Raw()
{
// This is a dummy version that just copies the digits file contents
// to a raw data file.

  IlcWarning(Form("Dummy version called for %s", GetName()));

  Int_t nDDLs = IlcDAQ::NumberOfDdls(GetName());

  if (!GetLoader()) return;
  fstream digitsFile(GetLoader()->GetDigitsFileName(), ios::in);
  if (!digitsFile) return;

  digitsFile.seekg(0, ios::end);
  UInt_t size = digitsFile.tellg();
  UInt_t ddlSize = 4 * (size / (4*nDDLs));
  Char_t* buffer = new Char_t[ddlSize+1];

  for (Int_t iDDL = 0; iDDL < nDDLs; iDDL++) {
    char fileName[256]="";
    strncpy(fileName,IlcDAQ::DdlFileName(GetName(),iDDL),255);
    fstream rawFile(fileName, ios::out);
    if (!rawFile) break;

    IlcRawDataHeader header;
    header.fSize = ddlSize + sizeof(header);
    rawFile.write((char*) &header, sizeof(header));

    digitsFile.read(buffer, ddlSize);
    rawFile.write(buffer, ddlSize);
    rawFile.close();
  }

  digitsFile.close();
  delete[] buffer;
}
