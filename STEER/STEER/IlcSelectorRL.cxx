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

/* $Id: IlcSelectorRL.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcSelectorRL.h"

#include <IlcLog.h>
#include "IlcRunLoader.h"

#include <TTree.h>
#include <TFile.h>

// Selector base class for analysis based on whole IlcRoot
// Please derive your selector-based analysis from this class, if you need to access the
// RunLoader or anything provided by it.
//
// This selector depends on the RunLoader, therefore to use it you have to have the whole IlcRoot available
// The benefit is that you can use the RunLoader to access everything in the data structure
//
// The class has functions to return the stack (GetStack()), the RunLoader (GetRunLoader()), and
// the event header (GetHeader()).
//
// Author: Jan.Fiete.Grosse-Oetringhaus@cern.ch

ClassImp(IlcSelectorRL)

IlcSelectorRL::IlcSelectorRL() :
  IlcSelector(),
  fRunLoader(0),
  fKinematicsLoaded(kFALSE),
  fHeaderLoaded(kFALSE)
{
  //
  // Constructor. Initialization of pointers
  //
}

IlcSelectorRL::~IlcSelectorRL()
{
  //
  // Destructor
  //

  // histograms are in the output list and deleted when the output
  // list is deleted by the TSelector dtor
}

Bool_t IlcSelectorRL::Notify()
{
  // Calls base class Notify
  // On top of that run loader is closed, because we change the input file

  if (IlcSelector::Notify() == kFALSE)
    return kFALSE;

  DeleteRunLoader();

  return kTRUE;
}

Bool_t IlcSelectorRL::Process(Long64_t entry)
{
  // Call the baseclass Process and set event number in runLoader (if loaded)

  if (IlcSelector::Process(entry) == kFALSE)
    return kFALSE;

  if (fRunLoader)
  {
    if (fRunLoader->GetEvent((Int_t)entry) != 0)
      return kFALSE;
  }

  return kTRUE;
}

void IlcSelectorRL::SlaveTerminate()
{
  // removes runloader

  IlcSelector::SlaveTerminate();

  DeleteRunLoader();
}

IlcRunLoader* IlcSelectorRL::GetRunLoader()
{
  // Returns IlcRun instance corresponding to current ESD active in fTree
  // Loads gilc.root, the file is identified by replacing "IlcESDs" to
  // "gilc" in the file path of the ESD file. 

  if (!fRunLoader)
  {
    if (!fTree->GetCurrentFile())
      return 0;

    TString fileName(fTree->GetCurrentFile()->GetName());
    fileName.ReplaceAll("IlcESDs", "gilc");

    // temporary workaround for PROOF bug #18505
    fileName.ReplaceAll("#gilc.root#gilc.root", "#gilc.root");

    fRunLoader = IlcRunLoader::Open(fileName);
    if (!fRunLoader)
      return 0;

    fRunLoader->GetEvent((Int_t)(fTree->GetTree()->GetReadEntry()));
  }

  return fRunLoader;
}

void IlcSelectorRL::DeleteRunLoader()
{
  //
  // deletes the runloader
  //

  if (fRunLoader)
  {
    fRunLoader->Delete();
    fRunLoader = 0;
  }

  fKinematicsLoaded = kFALSE;
  fHeaderLoaded = kFALSE;
}

IlcHeader* IlcSelectorRL::GetHeader()
{
  // Returns header retrieved from RunLoader

  IlcRunLoader* runLoader = GetRunLoader();
  if (!runLoader)
    return 0;

  if (fHeaderLoaded == kFALSE)
    if (runLoader->LoadHeader() != 0)
      return 0;

  fHeaderLoaded = kTRUE;

  return runLoader->GetHeader();
}

IlcStack* IlcSelectorRL::GetStack()
{
  // Returns stack retrieved from RunLoader

  IlcRunLoader* runLoader = GetRunLoader();
  if (!runLoader)
    return 0;

  if (fKinematicsLoaded == kFALSE)
    if (runLoader->LoadKinematics() != 0)
      return 0;

  fKinematicsLoaded = kTRUE;

  return runLoader->Stack();
}
