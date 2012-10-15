// Author: Mihai Niculescu 2012

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          										 *
 * full copyright notice.                                                																						 *
 **************************************************************************/

#include <TInterpreter.h>
#include <TSystem.h>
#include <TString.h>
#include <TROOT.h>

#include <IlcLog.h>

#include <IlcEveApplication.h>
#include <IlcEveManager.h>


ClassImp(IlcEveApplication)

IlcEveApplication::IlcEveApplication(const char* appClassName, int* argc, char** argv, void* options, int numOptions, Bool_t noLogo)
	: TRint(appClassName, argc, argv, options, numOptions, noLogo)
{
  Init();
}

IlcEveApplication::~IlcEveApplication()
{}

void  IlcEveApplication::Init()
{

	static const TEveException kEH("ilceve::main");

  TString evedir(Form("%s/EVE", gSystem->Getenv("ILC_ROOT")));

  TString macPath(gROOT->GetMacroPath());
  macPath += Form(":%s/macros", evedir.Data());
  gInterpreter->AddIncludePath(evedir);

  macPath += Form(":%s/ilc-macros", evedir.Data());
  gInterpreter->AddIncludePath(Form("%s/EVE", gSystem->Getenv("ILC_ROOT")));
//   gInterpreter->AddIncludePath(Form("%s/PWG0", gSystem->Getenv("ILC_ROOT")));
  gInterpreter->AddIncludePath(Form("%s/include", gSystem->Getenv("ILC_ROOT")));
  gInterpreter->AddIncludePath(gSystem->Getenv("ILC_ROOT"));
 
  gROOT->SetMacroPath(macPath);

  // make sure logger is instantiated
  IlcLog::GetRootLogger();


}
