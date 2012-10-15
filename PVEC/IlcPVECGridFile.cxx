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
/* $Id: IlcPVECGridFile.cxx 47780 2011-02-24 15:37:59Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.5  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// To navigate in the Grid catalogue (very elementary)
// check here : /afs/cern.ch/user/p/peters/public/README.ALIEN
//-- Author: Yves Schutz (CERN)

// --- ROOT system ---
#include "TObjString.h"
#include "TGrid.h"
#include "TGridResult.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVECGridFile.h"

ClassImp(IlcPVECGridFile)

//____________________________________________________________________________
IlcPVECGridFile::IlcPVECGridFile(TString grid):
  fGrid(0),
  fRoot("/ilc/production/ilcprod"),
  fYear(""),
  fProd(""),
  fVers(""),
  fType(""),
  fRun(""),
  fEvt(""),
  fPath("")
{
  // default ctor; Doing initialisation ;
  if (grid == "alien")
    fGrid = TGrid::Connect("alien://aliendb1.cern.ch:15000/?direct") ;
  else
    Error("IlcPVECGridFile", " %s is an unknown grid system", grid.Data()) ;
  if ( !fGrid )
    Error("ctor", "Cannot connect to alien://aliendb1.cern.ch:15000/?direct") ;

#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(fRoot) )
    Error("ctor", "Cannot find directory %s ", fRoot.Data() ) ;
#else
  Error("IlcPVECGridFile", "needs to be ported to new TGrid");
#endif

  fPath += fRoot ;
}

//____________________________________________________________________________
IlcPVECGridFile::IlcPVECGridFile(const IlcPVECGridFile &rhs) :
  TObject(rhs),
  fGrid(0),
  fRoot(""),
  fYear(""),
  fProd(""),
  fVers(""),
  fType(""),
  fRun(""),
  fEvt(""),
  fPath("")
{
  //copy ctor
  rhs.Copy(*this);
}

//____________________________________________________________________________
IlcPVECGridFile::~IlcPVECGridFile()
{
}

//____________________________________________________________________________
TString IlcPVECGridFile::GetLFN() const
{
  TString fileName(Pwd()) ;
  fileName += "gilc.root" ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->GetAccessPath(fileName) ) {
    IlcWarning(Form("file %s does not exist", fileName.Data())) ;
    fileName = "" ;
  }
  else
    fileName.Prepend("alien://") ;
#else
  Error("GetLFN", "needs to be ported to new TGrid");
#endif
  return fileName ;
}

//____________________________________________________________________________
void IlcPVECGridFile::Copy(TObject & obj)const
{
  //Copy method used by the Copy ctor
  IlcPVECGridFile &lfn = static_cast<IlcPVECGridFile &>(obj);
  /* incorrect Copy, destination is OBJ, not this.
  fRoot = lfn.fRoot ;
  fYear = lfn.fYear ;
  fProd = lfn.fProd ;
  fVers = lfn.fVers ;
  fType = lfn.fType ;
  fRun  = lfn.fRun ;
  fEvt  = lfn.fEvt ;
  TObject::Copy(lfn) ;
  */
  lfn.fRoot = fRoot ;
  lfn.fYear = fYear ;
  lfn.fProd = fProd ;
  lfn.fVers = fVers ;
  lfn.fType = fType ;
  lfn.fRun  = fRun ;
  lfn.fEvt  = fEvt ;
  TObject::Copy(lfn) ;
}

//____________________________________________________________________________
void IlcPVECGridFile::Help()
{
  // Prints information on available lfn's

  IlcInfo(Form(" ")) ;

}

//____________________________________________________________________________
void IlcPVECGridFile::ListEvents() const
{
  // list the available events for the current path and run selected

  char path[80] ;
  snprintf(path,80,"%s/%s-%s/%s/%s/%s", fRoot.Data(), fYear.Data(), fProd.Data(), fVers.Data(), fType.Data(), fRun.Data()) ;
  IlcInfo(Form("Searching %s", path)) ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  Grid_ResultHandle_t gr = fGrid->Find(path, "gilc.root") ;
  TGridResult ar(gr) ;
  ar.Print() ;
#else
  Error("ListEvents", "needs to be ported to new TGrid");
#endif
}

//____________________________________________________________________________
void IlcPVECGridFile::ListRuns() const
{
  // list the available runs for the current path selected

  char path[80] ;
  snprintf(path,80,"%s/%s-%s/%s/%s", fRoot.Data(), fYear.Data(), fProd.Data(), fVers.Data(), fType.Data()) ;
  IlcInfo(Form("Searching %s", path)) ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  Grid_ResultHandle_t gr = fGrid->OpenDir(path) ;
  TGridResult ar(gr) ;
  ar.Print() ;
#else
  Error("ListRuns", "needs to be ported to new TGrid");
#endif
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetYearProd(TString year, TString prod)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;
  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s", year.Data(), prod.Data()) ;

  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fYear = year ;
    fProd = prod ;
    fPath = path ;
  }
#else
  Error("SetYearProd", "needs to be ported to new TGrid");
#endif
  return rv ;
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetVers(TString vers)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;
  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s/%s", fYear.Data(), fProd.Data(), vers.Data()) ;
  fVers = tempo ;

  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s ", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fVers = vers ;
    fPath = path ;
  }
#else
  Error("SetVers", "needs to be ported to new TGrid");
#endif
  return rv ;
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetType(TString type)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;
  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s/%s/%s", fYear.Data(), fProd.Data(), fVers.Data(), type.Data()) ;

  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s ", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fType = type ;
    fPath = path ;
  }
#else
  Error("SetType", "needs to be ported to new TGrid");
#endif
  return rv ;
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetPath(TString year, TString prod, TString vers, TString type)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;
  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s/%s/%s", year.Data(), prod.Data(), vers.Data(), type.Data()) ;

  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s ", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fPath = path ;
    fYear += year ;
    fProd += prod ;
    fVers += vers ;
    fType += type ;
  }
#else
  Error("SetPath", "needs to be ported to new TGrid");
#endif
  return rv ;
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetRun(Int_t run)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;

  TString zero("00000") ;
  TString srun ;
  srun += run ;
  Int_t nzero = zero.Length() - srun.Length() ;
  Int_t index ;
  for (index = 0 ; index < nzero ; index++)
    srun.Prepend("0") ;

  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s/%s/%s/%s", fYear.Data(), fProd.Data(), fVers.Data(), fType.Data(), srun.Data()) ;

  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s ", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fRun = srun ;
    fPath = path ;
  }
#else
  Error("SetRun", "needs to be ported to new TGrid");
#endif
  return rv ;
}

//____________________________________________________________________________
Bool_t IlcPVECGridFile::SetEvt(Int_t evt)
{
  // set the year and verifies if the directory exists
  Bool_t rv = kFALSE ;

  TString zero("00000") ;
  TString sevt ;
  sevt += evt ;
  Int_t nzero = zero.Length() - sevt.Length() ;
  Int_t index ;
  for (index = 0 ; index < nzero ; index++)
    sevt.Prepend("0") ;

  char tempo[80] ;
  snprintf(tempo,80,"/%s-%s/%s/%s/%s/%s/", fYear.Data(), fProd.Data(), fVers.Data(), fType.Data(), fRun.Data(), sevt.Data()) ;
  TString path(fRoot) ;
  path += tempo ;
#if ROOT_VERSION_CODE < ROOT_VERSION(5,0,0)
  if ( !fGrid->OpenDir(path) ) {
    IlcError(Form("Cannot find directory %s ", path.Data() )) ;
  } else {
    rv = kTRUE ;
    fEvt = sevt ;
    fPath = path ;
  }
#else
  Error("SetEvt", "needs to be ported to new TGrid");
#endif
  return rv ;
}
