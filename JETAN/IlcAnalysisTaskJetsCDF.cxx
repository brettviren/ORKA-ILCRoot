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

/* $Id: IlcAnalysisTaskJets.cxx 24457 2008-03-13 10:02:22Z morsch $ */

#include <TROOT.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include "IlcAnalysisManager.h"
#include "IlcCdfJetFinder.h"
#include "IlcESDEvent.h"
#include "IlcESD.h"
#include "IlcAODEvent.h"
#include "IlcAODHandler.h"
#include "IlcMCEventHandler.h"
#include "IlcESDInputHandler.h"
#include "IlcMCEvent.h"
#include "IlcStack.h"
#include "IlcJetReader.h"

#include "IlcAnalysisTaskJetsCDF.h"

ClassImp(IlcAnalysisTaskJetsCDF)

//______________________________________________________________________________
IlcAnalysisTaskJetsCDF::IlcAnalysisTaskJetsCDF():
    IlcAnalysisTaskSE(),
    fJetFinder(0x0),
    fListOfHistos(0x0)
{
  // Default constructor
}

//______________________________________________________________________________
IlcAnalysisTaskJetsCDF::IlcAnalysisTaskJetsCDF(const char* name):
    IlcAnalysisTaskSE(name),
    fJetFinder(0x0),
    fListOfHistos(0x0)
{
  // Default constructor
    DefineOutput(1, TList::Class());
}

//______________________________________________________________________________
void IlcAnalysisTaskJetsCDF::UserCreateOutputObjects()
{
// Create the output container
//
    if (fDebug > 1) printf("AnalysisTaskJetsCDF::CreateOutPutData() \n");
//  Connect default AOD to jet finder

    fJetFinder->ConnectAOD(AODEvent());
//
//  Histograms
    OpenFile(1);
    fListOfHistos = new TList();
    if (!fJetFinder) {
       Error("UserCreateOutputObjects", "No jet finder connected.");
       return;
    }
    fJetFinder->CreateOutputObjects(fListOfHistos);
}

//______________________________________________________________________________
void IlcAnalysisTaskJetsCDF::LocalInit()
{
    // Initialization
    if (fDebug > 1) printf("AnalysisTaskJets::Init() \n");

    // Call configuration file
    gROOT->LoadMacro("ConfigJetAnalysisCDF.C");
    fJetFinder = (IlcCdfJetFinder*) gInterpreter->ProcessLine("ConfigJetAnalysis()");
    // Initialise Jet Analysis
    fJetFinder->Init();
    // Write header information to local file
    fJetFinder->WriteHeaders();
}

//______________________________________________________________________________
void IlcAnalysisTaskJetsCDF::UserExec(Option_t */*option*/)
{
// Execute analysis for current event
//

    fJetFinder->GetReader()->SetInputEvent(InputEvent(), AODEvent(), MCEvent());
    fJetFinder->ProcessEvent();

    // Post the data
    PostData(1, fListOfHistos);
}

//______________________________________________________________________________
void IlcAnalysisTaskJetsCDF::Terminate(Option_t */*option*/)
{
// Terminate analysis
//
    if (fDebug > 1) printf("IlcAnalysisJetsCDF: Terminate() \n");
//    if (fJetFinder) fJetFinder->FinishRun();
}

