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

#include "IlcTriggerUtils.h"
#include "IlcTriggerConfiguration.h"
#include "IlcTriggerClass.h"
#include "IlcTriggerInput.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"
#include "IlcCDBManager.h"
#include "IlcPDG.h"
#include "IlcMC.h"
#include "IlcModule.h"
#include "IlcLog.h"
#include <TROOT.h>
#include <TInterpreter.h>
#include <TString.h>

ClassImp(IlcTriggerUtils)

//_____________________________________________________________________________
Bool_t IlcTriggerUtils::CheckConfiguration( TString& configfile, IlcTriggerConfiguration * cfg )
{
   // To be used on the pre-creation of Configurations to check if the
   // conditions have valid inputs names.
   //
   // Initiate detectors modules from a Config file
   // Ask to each active module present in the fDetectorCluster
   // to create a Trigger detector and retrive the inputs from it
   // to create a list of inputs.
   // Each condition in the configuration is then checked agains 
   // the list of inputs


   if (!gIlc) {
      IlcError( "no gIlc object. Restart ilcroot and try again." );
      return kFALSE;
   }
   if (gIlc->Modules()->GetEntries() > 0) {
      IlcError( "gIlc was already run. Restart ilcroot and try again." );
      return kFALSE;
   }

   IlcInfo( Form( "initializing gIlc with config file %s",
            configfile.Data() ) );
//_______________________________________________________________________
   gIlc->Announce();
   
   gROOT->LoadMacro(configfile.Data());
   gInterpreter->ProcessLine(gIlc->GetConfigFunction());
   
   if(IlcCDBManager::Instance()->GetRun() >= 0) { 
     IlcRunLoader::Instance()->SetRunNumber(IlcCDBManager::Instance()->GetRun());
   } else {
     IlcWarning("Run number not initialized!!");
   }
  
   IlcRunLoader::Instance()->CdGAFile();
    
   IlcPDG::AddParticlesToPdgDataBase();  

   gIlc->GetMCApp()->Init();
   
   //Must be here because some MCs (G4) adds detectors here and not in Config.C
   gIlc->InitLoaders();
   IlcRunLoader::Instance()->MakeTree("E");
   IlcRunLoader::Instance()->LoadKinematics("RECREATE");
   IlcRunLoader::Instance()->LoadTrackRefs("RECREATE");
   IlcRunLoader::Instance()->LoadHits("all","RECREATE");
   //
   // Save stuff at the beginning of the file to avoid file corruption
   IlcRunLoader::Instance()->CdGAFile();
   gIlc->Write();

   IlcRunLoader* runLoader = IlcRunLoader::Instance();
   if( !runLoader ) {
      IlcError( Form( "gIlc has no run loader object. "
                      "Check your config file: %s", configfile.Data() ) );
      return kFALSE;
   }

   // get the possible inputs to check the condition
   TObjArray inputs;
   TObjArray* detArray = runLoader->GetIlcRun()->Detectors();

   TString detStr = cfg->GetTriggeringModules();

   for( Int_t iDet = 0; iDet < detArray->GetEntriesFast(); iDet++ ) {
      IlcModule* det = (IlcModule*) detArray->At(iDet);
      if( !det || !det->IsActive() ) continue;
      if( cfg->IsSelected( det->GetName(), detStr ) ) {
         IlcInfo( Form( "Creating inputs for %s", det->GetName() ) );
         IlcTriggerDetector* dtrg = det->CreateTriggerDetector();
         dtrg->AssignInputs(cfg->GetInputs());
         TObjArray* detInp = dtrg->GetInputs();
         for( Int_t i=0; i<detInp->GetEntriesFast(); i++ ) {
            IlcInfo( Form( "Adding input %s", ((IlcTriggerInput*)detInp->At(i))->GetName() ) );
            inputs.AddLast( detInp->At(i) );
         }
      }
   }

   // check if the condition is compatible with the triggers inputs
   Int_t ndesc = cfg->GetClasses().GetEntriesFast();
   Bool_t check = kTRUE;
   ULong64_t mask = 0L;
   for( Int_t j=0; j<ndesc; j++ ) {
     IlcTriggerClass *trclass = (IlcTriggerClass*)cfg->GetClasses().At( j );
     if( !(trclass->CheckClass( cfg )) ) check = kFALSE;
     else {
       if (trclass->IsActive(cfg->GetInputs(),cfg->GetFunctions())) {
	 IlcInfo( Form( "Trigger Class (%s) OK, class mask (0x%llx)",
			trclass->GetName(), trclass->GetMask( ) ) );
       }
       else {
	 IlcWarning( Form( "Trigger Class (%s) is NOT active, class mask (0x%llx)",
			   trclass->GetName(), trclass->GetMask( ) ) );
       }
     }
     // check if condition mask is duplicated
     if( mask & trclass->GetMask() ) {
       IlcError( Form("Class (%s). The class mask (0x%llx) is ambiguous. It was already defined",
		      trclass->GetName(), trclass->GetMask()  ) );
       check = kFALSE;
     }
     mask |= trclass->GetMask();
   }

   return check;
}
