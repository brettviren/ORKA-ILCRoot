// @(#)root/eve:$Id: IlcEveMacroExecutor.cxx 44943 2010-11-03 20:29:32Z cvetan $
// Author: Matevz Tadel 2007

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#include "IlcEveMacroExecutor.h"
#include "IlcEveMacro.h"
#include "IlcEveEventManager.h"
#include "IlcSysInfo.h"

#include <TEveUtil.h>
#include <TList.h>
#include <TROOT.h>

#include <TEveManager.h>
#include <TGFileDialog.h>
#include <TGMenu.h>

#include <TSystem.h>
#include <TPRegexp.h>
#include <RVersion.h>


//______________________________________________________________________________
//
// Contains a list of IlcEveMacros.
// The macros are added via AddMacro() and are owned by the executor.
// The macros can be executed via ExecMacros().
// They are executed in order in which they are registered.

ClassImp(IlcEveMacroExecutor)

//______________________________________________________________________________
IlcEveMacroExecutor::IlcEveMacroExecutor() :
  TObject(),
  fMacros(new TList)
{
  // Constructor.

  fMacros->SetOwner(kTRUE);
}

//______________________________________________________________________________
IlcEveMacroExecutor::~IlcEveMacroExecutor()
{
  // Destructor.

  delete fMacros;
}

/******************************************************************************/

void IlcEveMacroExecutor::AddMacro(IlcEveMacro* mac)
{
  // Add a new macro. Ownership transfered to the executor.

  static const TEveException kEH("IlcEveMacroExecutor::AddMacro ");

  const TString mname = mac->GetMacro();
  if ( ! mname.IsNull() && TEveUtil::CheckMacro(mname) == kFALSE)
  {
    TEveUtil::LoadMacro(mname);  
  }
  fMacros->Add(mac);
}

IlcEveMacro* IlcEveMacroExecutor::FindMacro(const TString& func)
{
  // Find macro with given function name (it is supposed to be unique).
  // Returns 0 if not found.

  TIter next(fMacros);
  IlcEveMacro* mac;
  while ((mac = (IlcEveMacro*) next()))
  {
    if (mac->GetFunc() == func)
      return mac;
  }
  return 0;
}

/******************************************************************************/

void IlcEveMacroExecutor::RemoveMacros()
{
  fMacros->Clear();
}

/******************************************************************************/

#include "Api.h"
#include "TInterpreter.h"

void IlcEveMacroExecutor::ExecMacros()
{
  // Execute registered macros.

  TIter next(fMacros);
  IlcEveMacro* mac;
  while ((mac = (IlcEveMacro*) next()))
  {
    // printf ("macro '%s'; func '%s'; args '%s'\n", mac->GetMacro().Data(), mac->GetFunc().Data(), mac->GetArgs().Data());

    mac->ResetExecState();
    
    if (mac->GetActive() == kFALSE || mac->GetFunc().IsNull())
    {
      continue;
    }

    if ((mac->RequiresRunLoader() && ! IlcEveEventManager::HasRunLoader()) ||
        (mac->RequiresESD()       && ! IlcEveEventManager::HasESD())       ||
        (mac->RequiresESDfriend() && ! IlcEveEventManager::HasESDfriend()) ||
        (mac->RequiresRawReader() && ! IlcEveEventManager::HasRawReader()) ||
	(mac->RequiresAOD()       && ! IlcEveEventManager::HasAOD()))
    {
      mac->SetExecNoData();
      continue;
    }

    TString cmd(mac->FormForExec());
    try
    {
      Long_t                   result = 0;
      TInterpreter::EErrorCode error  = TInterpreter::kNoError;

      IlcSysInfo::AddStamp(Form("%s_%s_before",mac->GetMacro().Data(), mac->GetFunc().Data()));
      result = gInterpreter->ProcessLine(cmd, &error);
      IlcSysInfo::AddStamp(Form("%s_%s_after",mac->GetMacro().Data(), mac->GetFunc().Data()));

      // Try to fix broken cint state? Code taken form pyroot.
      if (G__get_return(0) > G__RETURN_NORMAL)
      {
	printf ("*** FIXING CINT STATE AFTER RETURN ***\n");
	G__security_recover(0);
      }

      if (error != TInterpreter::kNoError)
      {
        mac->SetExecError();
        Error("ExecMacros", "Executing %s::%s, CINT error ... hopefully recovered.",
              mac->GetMacro().Data(), cmd.Data());
      }
      else
      {
        TEveElement *el  = (TEveElement*) result;
        TObject     *obj = dynamic_cast<TObject*>(el);
        if (el != 0 && obj == 0)
        {
          Warning("ExecMacros", "Executing %s::%s, returned TEveElement seems bad, setting it to 0.",
                  mac->GetMacro().Data(), cmd.Data());
          el = 0;
        }
        mac->SetExecOK(el);
      }
    }
    catch(TEveException& exc)
    {
      mac->SetExecException(exc);

      // Try to fix broken cint state? Code taken form pyroot.
      if (G__get_return(0) > G__RETURN_NORMAL)
      {
	printf ("*** FIXING CINT STATE AFTER EXCEPTION ***\n");
	G__security_recover(0);
      }

      Error("ExecMacros", "Executing %s::%s, caught exception: '%s'.",
	    mac->GetMacro().Data(), cmd.Data(), exc.Data());
    }
  }
}

/******************************************************************************/

#include <iostream>
#include <fstream>
using namespace std;

namespace
{
const char *gMacroSaveAsTypes[] = {"CINT Macro", "*.C",
                                   0, 0};
}

void IlcEveMacroExecutor::SaveAddedMacros()
{

  TGFileInfo fi;
  fi.fFileTypes   = gMacroSaveAsTypes;
  fi.fIniDir      = StrDup(""); // current directory
  fi.fFileTypeIdx = 0;
  fi.fOverwrite   = kTRUE;
  new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDSave, &fi);
  if (!fi.fFilename) return;

  TPMERegexp filere(".*/([^/]+$)");
  if (filere.Match(fi.fFilename) != 2)
  {
    Warning("IlcEvePopupHandler", "file '%s' bad.", fi.fFilename);
    return;
  }
  printf("Saving...\n");

  TString file(filere[1]);
  TString file1;
  if (!file.EndsWith(".C"))
  file1 = file + ".C";
  gSystem->ChangeDirectory(fi.fIniDir);
  ofstream myfile;
  myfile.open (file1);

  TIter next(fMacros);
  IlcEveMacro* mac;


  myfile <<"//Macro generated automatically by IlcEveMacroExecutor\n\n";

  myfile <<"void "<<file<<"(){\n\n";
  myfile <<"  IlcEveMacroExecutor *exec = IlcEveEventManager::GetMaster()->GetExecutor();\n";
  myfile <<"  exec->RemoveMacros();\n";
  myfile <<"  TEveBrowser *browser = gEve->GetBrowser();\n";
  myfile <<"  browser->ShowCloseTab(kFALSE);\n";
      
  while ((mac = (IlcEveMacro*) next()))
  {
    myfile <<"  exec->AddMacro(new IlcEveMacro("<<mac->GetSources()<<", "<<char(34)<<mac->GetTags()<<char(34)<<", "
     <<char(34)<<mac->GetMacro()<<char(34)<<", "<<char(34)<<mac->GetFunc()<<char(34)<<", "<<char(34)<<mac->GetArgs()
     <<char(34)<<", "<<mac->GetActive()<<"));\n\n";
  }

  myfile <<"  TEveWindowSlot *slot = TEveWindow::CreateWindowInTab(browser->GetTabRight());\n";
  myfile <<"  slot->StartEmbedding();\n";
  myfile <<"  IlcEveMacroExecutorWindow* exewin = new IlcEveMacroExecutorWindow(exec);\n";
  myfile <<"  slot->StopEmbedding("<<char(34)<<"DataSelection"<<char(34)<<");\n";
  myfile <<"  exewin->PopulateMacros();\n\n";

  myfile <<"\n}";
  myfile.close();
  printf("Saved...\n");

}
