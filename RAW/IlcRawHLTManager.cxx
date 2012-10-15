// $Id: IlcRawHLTManager.cxx 23039 2007-12-13 20:53:02Z richterm $

//**************************************************************************
//* This file is property of and copyright by the ILC HLT Project        * 
//* 2012-2013 Infrastructure for Large Collider Experiment.       *
//* All rights reserved.                                                   *
//*                                                                        *
//* Primary Authors: Matthias Richter <Matthias.Richter@ift.uib.no>        *
//*                  for The ILC HLT Project.                            *
//*                                                                        *
//* Permission to use, copy, modify and distribute this software and its   *
//* documentation strictly for non-commercial purposes is hereby granted   *
//* without fee, provided that the above copyright notice appears in all   *
//* copies and that both the copyright notice and this permission notice   *
//* appear in the supporting documentation. The authors make no claims     *
//* about the suitability of this software for any purpose. It is          *
//* provided "as is" without express or implied warranty.                  *
//**************************************************************************

/** @file   IlcRawHLTManager.cxx
    @author Matthias Richter
    @date   
    @brief  dynamic generation of HLT RAW readers and streams
*/

#include "IlcRawHLTManager.h"
#include "IlcLog.h"
#include "TSystem.h"
#include "TClass.h"

ClassImp(IlcRawHLTManager)

IlcRawHLTManager::IlcRawHLTManager()
{
  // The class gives dynamic access to creater methods for HLT RAW readers and
  // streams without any library dependencies to HLT libraries.
  //
  // The IlcRawReaderHLT allows the redirection of input from the HLT DDL links
  // to the detector equipment ids. To access the data, the IlcRawReaderHLT
  // needs a valid RAW reader (parent).
  // usage:
  // IlcRawReader* pHLTReader=IlcRawHLTManager::CreateRawReaderHLT(pParent, "TPC");
}

IlcRawHLTManager::~IlcRawHLTManager()
{
  // destructor
}

int IlcRawHLTManager::fLibraryStatus=kUnloaded;
IlcRawReaderHLTCreateInstance_t IlcRawHLTManager::fFctCreateRawReaderHLT=NULL;
void* IlcRawHLTManager::fFctCreateRawStream=NULL;

IlcRawReader* IlcRawHLTManager::CreateRawReaderHLT(IlcRawReader* pParent, const char* detectors)
{
  // see header file for class documentation
  if (fLibraryStatus==kUnloaded) fLibraryStatus=LoadLibrary();
  if (fLibraryStatus==kUnavailable) return NULL;

  if (!fFctCreateRawReaderHLT) {
    IlcErrorClass("internal error, library loaded but function entry not known");
    return NULL;
  }
  return ((IlcRawReaderHLTCreateInstance_t)fFctCreateRawReaderHLT)(pParent, detectors);
}

TObject* IlcRawHLTManager::CreateRawStream(const char* /*className*/)
{
  // see header file for class documentation

  // not yet implemented
  return NULL;
}

int IlcRawHLTManager::LoadLibrary()
{
  // see header file for class documentation
  int iResult=kUnavailable;
  if (fLibraryStatus!=kUnloaded) return fLibraryStatus;

  // strictly speaken we do not need a trial counter as gSystem->Load only returns 1 if the
  // library has been loaded. If it was already loaded we get 0 
  int iTrials=0;
  do {
    fFctCreateRawReaderHLT=(IlcRawReaderHLTCreateInstance_t)gSystem->DynFindSymbol(ILCHLTREC_LIBRARY, ILCRAWREADERHLT_CREATE_INSTANCE);
  } while (fFctCreateRawReaderHLT==NULL && gSystem->Load(ILCHLTREC_LIBRARY)==0 && iTrials++<1);
  if (fFctCreateRawReaderHLT) {
    iResult=kLoaded;
  } else {
    IlcErrorClass(Form("can not find library/entry %s/%s", ILCHLTREC_LIBRARY, ILCRAWREADERHLT_CREATE_INSTANCE));
    iResult=kUnavailable;
  }
  return iResult;
}
