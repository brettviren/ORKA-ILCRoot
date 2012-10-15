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

/* $Id: IlcRunLoader.cxx 52261 2011-10-23 15:46:57Z hristov $ */

//____________________________________________________________________
//////////////////////////////////////////////////////////////////////
//                                                                  //
// class IlcRunLoader                                               //
//                                                                  //
// This class aims to be the unque interface for managing data I/O. //
// It stores Loaders for all modules which, knows names             //
// of the files were data are to be stored.                         //
//                                                                  //
// It aims to substitud IlcRun in automatic data managing           //
// thus there is no necessity of loading gIlc from file in order  //
// to get access to the data.                                       //
//                                                                  //
// Logical place to put the specific Loader to the given            //
// detector is detector  itself (i.e ITSLoader in ITS).             //
// But, to load detector object one need to load gIlc, and        //
// by the way all other detectors with their geometrieces and       //
// so on. So, if one need to open TPC clusters there is no          //
// principal need to read everything.                               //
//                                                                  //
//                                                                  //
// When RunLoader is read from the file it does not connect to      //
// the folder structure automatically. It must be connected         //
// (mounted) manualy. Default event folder is defined by            //
// IlcConfig::GetDefaultEventFolderName()                           //
// but can be mounted elsewhere. Usefull specially in merging case, //
// when more than pone session needs to be loaded                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#include "IlcRunLoader.h"
#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcLoader.h"
#include "IlcHeader.h"
#include "IlcStack.h"
#include "IlcDetector.h"
#include "IlcCDBManager.h"
#include "IlcCDBLocal.h"
#include "IlcCentralTrigger.h"

#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TFile.h>
#include <TFolder.h>
#include <TObjArray.h>
#include <TString.h>

ClassImp(IlcRunLoader)

IlcRunLoader* IlcRunLoader::fgRunLoader = 0x0;

const TString IlcRunLoader::fgkRunLoaderName("RunLoader");
const TString IlcRunLoader::fgkHeaderBranchName("Header");
const TString IlcRunLoader::fgkTriggerBranchName("ClassMask");
const TString IlcRunLoader::fgkHeaderContainerName("TE");
const TString IlcRunLoader::fgkTriggerContainerName("TreeCT");
const TString IlcRunLoader::fgkKineContainerName("TreeK");
const TString IlcRunLoader::fgkTrackRefsContainerName("TreeTR");
const TString IlcRunLoader::fgkKineBranchName("Particles");
const TString IlcRunLoader::fgkDefaultKineFileName("Kinematics.root");
const TString IlcRunLoader::fgkDefaultTrackRefsFileName("TrackRefs.root");
const TString IlcRunLoader::fgkGIlcName("gIlc");
const TString IlcRunLoader::fgkDefaultTriggerFileName("Trigger.root");
/**************************************************************************/

IlcRunLoader::IlcRunLoader():
 fLoaders(0x0),
 fEventFolder(0x0),
 fRun(-1),
 fCurrentEvent(0),
 fGAFile(0x0),
 fHeader(0x0),
 fStack(0x0),
 fCTrigger(0x0),
 fKineDataLoader(0x0),
 fTrackRefsDataLoader(0x0),
 fNEventsPerFile(1),
 fNEventsPerRun(0),
 fUnixDirName(".")
{
  IlcConfig::Instance();//force to build the folder structure
  if (!fgRunLoader) fgRunLoader = this;
}
/**************************************************************************/

IlcRunLoader::IlcRunLoader(const char* eventfoldername):
 TNamed(fgkRunLoaderName,fgkRunLoaderName),
 fLoaders(new TObjArray()),
 fEventFolder(0x0),
 fRun(-1),
 fCurrentEvent(0),
 fGAFile(0x0),
 fHeader(0x0),
 fStack(0x0),
 fCTrigger(0x0),
 fKineDataLoader(new IlcDataLoader(fgkDefaultKineFileName,fgkKineContainerName,"Kinematics")),
 fTrackRefsDataLoader(new IlcDataLoader(fgkDefaultTrackRefsFileName,fgkTrackRefsContainerName,"Track References")),
 fNEventsPerFile(1),
 fNEventsPerRun(0),
 fUnixDirName(".")
{
//ctor
  SetEventFolderName(eventfoldername);
 if (!fgRunLoader) fgRunLoader = this;
}
/**************************************************************************/

IlcRunLoader::~IlcRunLoader()
{
//dtor

  
  UnloadHeader();
  UnloadgIlc();
  if (fgRunLoader == this) fgRunLoader = 0x0;  
  if(fLoaders) {
    fLoaders->SetOwner();
    delete fLoaders;
  }
  
  delete fKineDataLoader;
  delete fTrackRefsDataLoader;
  
  
  RemoveEventFolder();
  
  //fEventFolder is deleted by the way of removing - TopIlcFolder owns it
  if( fCTrigger ) delete  fCTrigger;
  delete fStack;
  delete fGAFile;
}
/**************************************************************************/

IlcRunLoader::IlcRunLoader(TFolder* topfolder):
 TNamed(fgkRunLoaderName,fgkRunLoaderName),
 fLoaders(new TObjArray()),
 fEventFolder(topfolder),
 fRun(-1),
 fCurrentEvent(0),
 fGAFile(0x0),
 fHeader(0x0),
 fStack(0x0),
 fCTrigger(0x0),
 fKineDataLoader(new IlcDataLoader(fgkDefaultKineFileName,fgkKineContainerName,"Kinematics")),
 fTrackRefsDataLoader(new IlcDataLoader(fgkDefaultTrackRefsFileName,fgkTrackRefsContainerName,"Track References")),
 fNEventsPerFile(1),
 fNEventsPerRun(0),
 fUnixDirName(".")
{
//ctor
 if(topfolder == 0x0)
  {
    TString errmsg("Parameter is NULL");
    IlcError(errmsg.Data());
    throw errmsg;
    return;
  }
 
 TObject* obj = fEventFolder->FindObject(fgkRunLoaderName);
 if (obj)
  { //if it is, then sth. is going wrong... exits ilcroot session
    TString errmsg("In Event Folder Named ");
    errmsg+=fEventFolder->GetName();
    errmsg+=" object named "+fgkRunLoaderName+" already exists. I am confused ...";

    IlcError(errmsg.Data());
    throw errmsg;
    return;//never reached
  }

 if (!fgRunLoader) fgRunLoader = this;
   
 fEventFolder->Add(this);//put myself to the folder to accessible for all
  
}

/**************************************************************************/

Int_t IlcRunLoader::GetEvent(Int_t evno)
{
//Gets event number evno
//Reloads all data properly
//PH  if (fCurrentEvent == evno) return 0;
  
  if (evno < 0)
   {
     IlcError("Can not give the event with negative number");
     return 4;
   }

  if (evno >= GetNumberOfEvents())
   {
     IlcError(Form("There is no event with number %d",evno));
     return 3;
   }
  
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  IlcDebug(1, Form("          GETTING EVENT  %d",evno));
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  IlcDebug(1, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
   
  fCurrentEvent = evno;

  Int_t retval;
  
  //Reload header (If header was loaded)
  if (GetHeader())
   {
     retval = TreeE()->GetEvent(fCurrentEvent);
     if ( retval == 0)
      {
        IlcError(Form("Cannot find event: %d\n ",fCurrentEvent));
        return 5;
      }
   }
  //Reload stack (If header was loaded)
  if (TreeE()) fStack = GetHeader()->Stack();
  //Set event folder in stack (it does not mean that we read kinematics from file)
   if( GetTrigger() && TreeCT() ) {
      retval = TreeCT()->GetEvent(fCurrentEvent);
      if ( retval < 0 )      {
         IlcError(Form("Error occured while GetEvent for Trigger. Event %d",evno));
         return 2;
      }
   }
  
  retval = SetEvent();
  if (retval)
   {
     IlcError(Form("Error occured while setting event %d",evno));
     return 1;
   }
   
  //Post Track References
  retval = fTrackRefsDataLoader->GetEvent();
  if (retval)
   {
     IlcError(Form("Error occured while GetEvent for Track References. Event %d",evno));
     return 2;
   }

  //Read Kinematics if loaded
  retval = fKineDataLoader->GetEvent();
  if (retval)
   {
     IlcError(Form("Error occured while GetEvent for Kinematics. Event %d",evno));
     return 2;
   }

  if (fStack && fKineDataLoader->GetBaseLoader(0)->IsLoaded())
    {
	fStack->ConnectTree(TreeK());
	
      if (fStack->GetEvent() == kFALSE)
	{
	  IlcError(Form("Error occured while GetEvent for Stack. Event %d",evno));
	  return 2;
	}
    }

  //Trigger data reloading in all loaders 
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
     retval = loader->GetEvent();
     if (retval)
      {
       IlcError(Form("Error occured while getting event for %s. Event %d.",
		     loader->GetDetectorName().Data(), evno));
       return 3;
      }
   }
  
  SetDetectorAddresses();
  
  return 0;
}
/**************************************************************************/
Int_t IlcRunLoader::SetEvent()
{
//if kinematics was loaded Cleans folder data

  Int_t retval;
  
  retval = fKineDataLoader->SetEvent();
  if (retval)
   {
     IlcError("SetEvent for Kinamtics Data Loader retutned error.");
     return retval;
   }
  retval = fTrackRefsDataLoader->SetEvent(); 
  if (retval)
   {
     IlcError("SetEvent for Track References Data Loader retutned error.");
     return retval;
   }

  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
     retval = loader->SetEvent();
     if (retval)
      {
        IlcError(Form("SetEvent for %s Data Loader retutned error.",loader->GetName()));
        return retval;
      }
   }

  return 0;
}
/**************************************************************************/

Int_t IlcRunLoader::SetEventNumber(Int_t evno)
{
  //cleans folders and sets the root dirs in files 
  if (fCurrentEvent == evno) return 0;
  fCurrentEvent = evno;
  return SetEvent();
}

/**************************************************************************/
IlcCDBEntry* IlcRunLoader::GetCDBEntry(const char* name) const
{
//Get an IlcCDBEntry from the run data storage
  
  IlcDebug(1, Form("Getting CDBEntry with name %s",name));

  if ( !(IlcCDBManager::Instance()->IsDefaultStorageSet()) ) {
    IlcError("No run data storage defined!");
    return 0x0;
  }
  return IlcCDBManager::Instance()->GetDefaultStorage()->Get(name, GetHeader()->GetRun());

}

/**************************************************************************/
IlcRunLoader* IlcRunLoader::Open
  (const char* filename, const char* eventfoldername, Option_t* option)
{
//Opens a desired file 'filename'
//gets the the run-Loader and mounts it desired folder
//returns the pointer to run Loader which can be further used for accessing data 
//in case of error returns NULL
 
 static const TString kwebaddress("http://ilcsoft.cern.ch/people/skowron/codedoc/split/index.html");
 IlcDebugClass(1,Form("\n\n\nNew I/O strcture: See more info:\n %s\n\n\n",kwebaddress.Data()));
 
 IlcRunLoader* result = 0x0;
 
 /* ************************************************ */
 /* Chceck if folder with given name already exists  */
 /* ************************************************ */
 
 TObject* obj = IlcConfig::Instance()->GetTopFolder()->FindObject(eventfoldername);
 if(obj)
  {
    TFolder* fold = dynamic_cast<TFolder*>(obj);
    if (fold == 0x0)
     {
      IlcErrorClass("Such a obejct already exists in top ilc folder and it is not a folder.");
      return 0x0;
     }
    
    //check if we can get RL from that folder
    result = IlcRunLoader::GetRunLoader(eventfoldername);
    if (result == 0x0)
     {
       IlcErrorClass(Form("Folder %s already exists, and can not find session there. Can not mount.",eventfoldername));
       return 0x0;
     }

    if (result->GetFileName().CompareTo(filename) != 0)
     {
       IlcErrorClass("Other file is mounted in demanded folder. Can not mount.");
       return 0x0;
     }

    //check if now is demanded (re)creation 
    if ( IlcLoader::TestFileOption(option) == kFALSE)
     {
       IlcErrorClass(Form("Session already exists in folder %s and this session option is %s. Unable to proceed.",
			  eventfoldername,option));
       return 0x0;
     }
     
    //check if demanded option is update and existing one 
    TString tmpstr(option);
    if ( (tmpstr.CompareTo("update",TString::kIgnoreCase) == 0) && 
         (result->fGAFile->IsWritable() == kFALSE) )
     { 
       IlcErrorClass(Form("Session already exists in folder %s and is not writable while this session option is %s. Unable to proceed.",
			  eventfoldername,option));
       return 0x0;
     }
     
    IlcWarningClass("Session is already opened and mounted in demanded folder");	
    if (!fgRunLoader) fgRunLoader = result; //PH get access from any place
    return result;
  } //end of checking in case of existance of object named identically that folder session is being opened
 
 
 TFile * gIlcFile = TFile::Open(filename,option);//open a file
 if (!gIlcFile) 
  {//null pointer returned
    IlcFatalClass(Form("Can not open file %s.",filename));
    return 0x0;
  }
  
 if (gIlcFile->IsOpen() == kFALSE)
  {//pointer to valid object returned but file is not opened
    IlcErrorClass(Form("Can not open file %s.",filename));
    return 0x0;
  }
 
 //if file is "read" or "update" than we try to find IlcRunLoader there - if not found cry and exit
 //else create new IlcRunLoader
 if ( IlcLoader::TestFileOption(option) )
  { 
    IlcDebugClass(1, "Reading RL from file");
    
    result = dynamic_cast<IlcRunLoader*>(gIlcFile->Get(fgkRunLoaderName));//get the run Loader from the file
    if (result == 0x0)
     {//didn't get
       IlcErrorClass(Form("Can not find run-Loader in file %s.",filename));
       delete gIlcFile;//close the file
       return 0x0;
     }
    Int_t tmp = result->SetEventFolderName(eventfoldername);//mount a event folder   
    if (tmp)//if SetEvent  returned error
     {
       IlcErrorClass(Form("Can not mount event in folder %s.",eventfoldername));
       delete result; //delete run-Loader
       delete gIlcFile;//close the file
       return 0x0;
     }
  }
 else
  {
    IlcDebugClass(1, Form("Creating new IlcRunLoader. Folder name is %s",eventfoldername));
    try
     {  
       result = new IlcRunLoader(eventfoldername);
     }
    catch (TString& errmsg)
     {
       IlcErrorClass(Form("IlcRunLoader constrcutor has thrown exception: %s\n",errmsg.Data()));
       delete result;
       delete gIlcFile;//close the file
       return 0x0;
     }
  }
 
//procedure for extracting dir name from the file name 
 TString fname(filename);
 Int_t  nsl = fname.Last('#');//look for hash in file name
 TString dirname;
 if (nsl < 0) {//hash not found
   nsl = fname.Last('/');// look for slash
   if (nsl < 0) 
     nsl = fname.Last(':');// look for colon e.g. rfio:gilc.root
 }

 if (nsl < 0) dirname = "./";      // no directory path, use "."
 else dirname = fname.Remove(nsl+1);// directory path
 
 IlcDebugClass(1, Form("Dir name is : %s",dirname.Data()));
 
 result->SetDirName(dirname); 
 result->SetGIlcFile(gIlcFile);//set the pointer to gIlcFile
 if (!fgRunLoader) fgRunLoader = result; //PH get access from any place
 return result;
}
/**************************************************************************/
Int_t IlcRunLoader::GetNumberOfEvents()
{
 //returns number of events in Run
 Int_t retval;
 if( TreeE() == 0x0 )
  {
    retval = LoadHeader();
    if (retval) 
     {
       IlcError("Error occured while loading header");
       return -1;
     }
  }
 return (Int_t)TreeE()->GetEntries();
}
/**************************************************************************/

void IlcRunLoader::MakeHeader()
{
 //Makes header and connects it to header tree (if it exists)
  IlcDebug(1, "");
  if(fHeader == 0x0)
   {
     IlcDebug(1, "Creating new Header Object");
     fHeader= new IlcHeader();
   }
  TTree* tree = TreeE();
  if (tree)
   {
     IlcDebug(1, "Got Tree from folder.");
     TBranch* branch = tree->GetBranch(fgkHeaderBranchName);
     if (branch == 0x0)
      {
        IlcDebug(1, "Creating new branch");
        branch = tree->Branch(fgkHeaderBranchName, "IlcHeader", &fHeader, 4000, 0);
        branch->SetAutoDelete(kFALSE);
      }
     else
      {
        IlcDebug(1, "Got Branch from Tree");
        branch->SetAddress(&fHeader);
        tree->GetEvent(fCurrentEvent);
        fStack = fHeader->Stack(); //should be safe - if we created Stack, header returns pointer to the same object
        if (fStack)
         {
           if (TreeK()) {
	       fStack->ConnectTree(TreeK());
	       fStack->GetEvent();
	   }
         }
        else
        {
          IlcDebug(1, "Header does not have a stack.");
        }
      }
   } 
  IlcDebug(1, "Exiting MakeHeader method");
}
/**************************************************************************/

void IlcRunLoader::MakeStack()
{
//Creates the stack object -  do not connect the tree
  if(fStack == 0x0)
   { 
     fStack = new IlcStack(10000);
   }
}
/**************************************************************************/

void IlcRunLoader::MakeTrigger()
{
 // Makes trigger object and connects it to trigger tree (if it exists)
   IlcDebug( 1, "" );
   if( fCTrigger == 0x0 ) {
      IlcDebug( 1, "Creating new Trigger Object" );
      fCTrigger = new IlcCentralTrigger();
   }
   TTree* tree = TreeCT();
   if( tree ) {
      fCTrigger->MakeBranch( fgkTriggerBranchName, tree );
      tree->GetEvent( fCurrentEvent );
   }

   IlcDebug( 1, "Exiting MakeTrigger method" );
}
/**************************************************************************/

void IlcRunLoader::MakeTree(Option_t *option)
{
//Creates trees
  const char *oK  = strstr(option,"K");  //Kine
  const char *oE  = strstr(option,"E");  //Header
  const char *oGG = strstr(option,"GG"); //Central TriGGer
  
  if(oK)
  { 
      if (fKineDataLoader->GetBaseLoader(0)->IsLoaded() == kFALSE)
      {
	  IlcError("Load Kinematics first");
      }
      else
      {
	  if (!TreeK()) {
	      fKineDataLoader->MakeTree();
	      MakeStack();
	  } 
	  fStack->ConnectTree(TreeK());
	  WriteKinematics("OVERWRITE");
      }
  } // TreeK
  
  if(oE && !TreeE())
   { 
     fGAFile->cd();
     TTree* tree = new TTree(fgkHeaderContainerName,"Tree with Headers");
     GetEventFolder()->Add(tree);
     MakeHeader();
     WriteHeader("OVERWRITE");
   }
  
   if(oGG && !TreeCT())
   {
      // create the CTP Trigger output file and tree
      TFile* file = gROOT->GetFile( fgkDefaultTriggerFileName );
      if( !file ) {
        char* tmp = gSystem->ConcatFileName( fUnixDirName.Data(), fgkDefaultTriggerFileName.Data() );
        file = TFile::Open(tmp , "RECREATE" ) ;
        delete[] tmp;
      }

      file->cd();
      TTree* tree = new TTree( fgkTriggerContainerName, "Tree with Central Trigger Mask" );
      GetEventFolder()->Add(tree);
      MakeTrigger();
  //    WriteHeader("OVERWRITE");
   }

  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
    loader->MakeTree(option);
   }

}
/**************************************************************************/
    
Int_t IlcRunLoader::LoadgIlc()
{
//Loads gIlc from file
 if (GetIlcRun())
  {
    IlcWarning("IlcRun is already in folder. Unload first.");
    return 0;
  }
 IlcRun* ilcrun = dynamic_cast<IlcRun*>(fGAFile->Get(fgkGIlcName));
 if (ilcrun == 0x0)
  {
    IlcError(Form("Can not find gIlc in file %s",fGAFile->GetName()));
    return 2;
  }
 ilcrun->SetRunLoader(this);
 if (gIlc)
  {
    IlcWarning(Form("gIlc already exists. Putting retrived object in folder named %s",
		    GetEventFolder()->GetName()));
  }
 else
  {
    gIlc = ilcrun;
  }
 SetDetectorAddresses();//calls SetTreeAddress for all detectors
 return 0; 
}
/**************************************************************************/

Int_t IlcRunLoader::LoadHeader()
{
//loads treeE and reads header object for current event
 if (TreeE())
  {
     IlcWarning("Header is already loaded. Use ReloadHeader to force reload. Nothing done");
     return 0;
  }
 
 if (GetEventFolder() == 0x0)
  {
    IlcError("Event folder not specified yet");
    return 1;
  }

 if (fGAFile == 0x0)
  {
    IlcError("Session not opened. Use IlcRunLoader::Open");
    return 2;
  }
 
 if (fGAFile->IsOpen() == kFALSE)
  {
    IlcError("Session not opened. Use IlcRunLoader::Open");
    return 2;
  }

 TTree* tree = dynamic_cast<TTree*>(fGAFile->Get(fgkHeaderContainerName));
 if (tree == 0x0)
  {
    IlcError(Form("Can not find header tree named %s in file %s",
		  fgkHeaderContainerName.Data(),fGAFile->GetName()));
    return 2;
  }

 if (tree == TreeE()) return 0;

 CleanHeader();
 GetEventFolder()->Add(tree);
 MakeHeader();//creates header object and connects to tree
 return 0; 

}
/**************************************************************************/

Int_t IlcRunLoader::LoadTrigger(Option_t* option)
{
   //Load treeCT
   static Bool_t warned = kFALSE;
   if( TreeCT() ) {
      if (!warned) IlcWarning("Trigger is already loaded. Nothing done. Message will not be repeated.");
      warned = kTRUE;
      return 0;
   }
 
   if( GetEventFolder() == 0x0 ) {
      IlcError("Event folder not specified yet");
      return 1;
   }
   // get the CTP Trigger output file and tree
  char* tmp = gSystem->ConcatFileName( fUnixDirName.Data(),
                                      fgkDefaultTriggerFileName.Data() );
  TString trgfile(tmp);
  delete[] tmp;
  
   TFile* file = gROOT->GetFile( trgfile );
   if( !file ) {
      file = TFile::Open( trgfile, option ) ;
      if (!file || file->IsOpen() == kFALSE ) {
         IlcError( Form( "Can not open trigger file %s", trgfile.Data() ) );
         return 2;
      }
   }
   file->cd();

   TTree* tree = dynamic_cast<TTree*>(file->Get( fgkTriggerContainerName ));
   if( !tree ) {
      IlcError( Form( "Can not find trigger tree named %s in file %s",
                      fgkTriggerContainerName.Data(), file->GetName() ) );
      return 2;
   }

   CleanTrigger();

   fCTrigger = dynamic_cast<IlcCentralTrigger*>(file->Get( "IlcCentralTrigger" ));
   GetEventFolder()->Add( tree );
   MakeTrigger();

   return 0;
}

/**************************************************************************/

Int_t IlcRunLoader::LoadKinematics(Option_t* option)
{
//Loads the kinematics 
 Int_t retval = fKineDataLoader->GetBaseLoader(0)->Load(option);
 if (retval)
  {
    IlcError("Error occured while loading kinamatics tree.");
    return retval;
  }
 if (fStack) 
  {
      fStack->ConnectTree(TreeK());
      retval = fStack->GetEvent();
    if ( retval == kFALSE)
     {
       IlcError("Error occured while loading kinamatics tree.");
       return retval;
     }
    
  }
 return 0;
}
/**************************************************************************/

Int_t IlcRunLoader::OpenDataFile(const TString& filename,TFile*& file,TDirectory*& dir,Option_t* opt,Int_t cl)
{
//Opens File with kinematics
 if (file)
  {
    if (file->IsOpen() == kFALSE)
     {//pointer is not null but file is not opened
       IlcWarning("Pointer to file is not null, but file is not opened");//risky any way
       delete file;
       file = 0x0; //proceed with opening procedure
     }
    else
     { 
       IlcWarning(Form("File  %s already opened",filename.Data()));
       return 0;
     }
  }
//try to find if that file is opened somewere else
 file = (TFile *)( gROOT->GetListOfFiles()->FindObject(filename) );
 if (file)
  {
   if(file->IsOpen() == kTRUE)
    {
     IlcWarning(Form("File %s already opened by sombody else.",file->GetName()));
     return 0;
    }
  }

 file = TFile::Open(filename,opt);
 if (file == 0x0)
  {//file is null
    IlcError(Form("Can not open file %s",filename.Data()));
    return 1;
  }
 if (file->IsOpen() == kFALSE)
  {//file is not opened
    IlcError(Form("Can not open file %s",filename.Data()));
   return 1;
  }
  
 file->SetCompressionLevel(cl);
 
 dir = IlcLoader::ChangeDir(file,fCurrentEvent);
 if (dir == 0x0)
  {
    IlcError(Form("Can not change to root directory in file %s",filename.Data()));
    return 3;
  }
 return 0; 
}
/**************************************************************************/

TTree* IlcRunLoader::TreeE() const
{
 //returns the tree from folder; shortcut method
 if (IlcDebugLevel() > 10) fEventFolder->ls();
 TObject *obj = fEventFolder->FindObject(fgkHeaderContainerName);
 return (obj)?dynamic_cast<TTree*>(obj):0x0;
}
/**************************************************************************/

TTree* IlcRunLoader::TreeCT() const
{
 //returns the tree from folder; shortcut method
   if (IlcDebugLevel() > 10) fEventFolder->ls();
   TObject *obj = fEventFolder->FindObject(fgkTriggerContainerName);
   return (obj)?dynamic_cast<TTree*>(obj):0x0;
}
/**************************************************************************/

IlcHeader* IlcRunLoader::GetHeader() const
{
//returns pointer header object
 return fHeader;
}
/**************************************************************************/

IlcCentralTrigger* IlcRunLoader::GetTrigger() const
{
//returns pointer trigger object
   return fCTrigger;
}

/**************************************************************************/
 
TTree* IlcRunLoader::TreeK() const
{
 //returns the tree from folder; shortcut method
 TObject *obj = GetEventFolder()->FindObject(fgkKineContainerName);
 return (obj)?dynamic_cast<TTree*>(obj):0x0;
}
/**************************************************************************/

TTree* IlcRunLoader::TreeTR() const
{
 //returns the tree from folder; shortcut method
 TObject* obj = GetEventFolder()->FindObject(fgkTrackRefsContainerName);
 return (obj)?dynamic_cast<TTree*>(obj):0x0;
}
/**************************************************************************/

IlcRun* IlcRunLoader::GetIlcRun() const
{
//returns IlcRun which sits in the folder
 if (fEventFolder == 0x0) return 0x0;
 TObject *obj = fEventFolder->FindObject(fgkGIlcName);
 return (obj)?dynamic_cast<IlcRun*>(obj):0x0;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteHeader(Option_t* opt)
{
//writes treeE
  IlcDebug(1, "WRITING HEADER");
  
  TTree* tree = TreeE();
  if ( tree == 0x0)
   {
     IlcWarning("Can not find Header Tree in Folder");
     return 0;
   } 
  if (fGAFile->IsWritable() == kFALSE)
   {
     IlcError(Form("File %s is not writable",fGAFile->GetName()));
     return 1;
   }

  TObject* obj = fGAFile->Get(fgkHeaderContainerName);
  if (obj)
   { //if they exist, see if option OVERWRITE is used
     TString tmp(opt);
     if(tmp.Contains("OVERWRITE",TString::kIgnoreCase) == 0)
      {//if it is not used -  give an error message and return an error code
        IlcError("Tree already exisists. Use option \"OVERWRITE\" to overwrite previous data");
        return 3;
      }
   }
  fGAFile->cd();
  tree->SetDirectory(fGAFile);
  tree->Write(0,TObject::kOverwrite);

  IlcDebug(1, "WRITTEN\n\n");
  
  return 0;
}

/**************************************************************************/

Int_t IlcRunLoader::WriteTrigger(Option_t* opt)
{
   //writes TreeCT
   IlcDebug( 1, "WRITING TRIGGER" );
  
   TTree* tree = TreeCT();
   if ( tree == 0x0) {
      IlcWarning("Can not find Trigger Tree in Folder");
      return 0;
   }

   TFile* file = gROOT->GetFile( gSystem->ConcatFileName( fUnixDirName.Data(), fgkDefaultTriggerFileName.Data() ) ) ;
   if( !file || !file->IsOpen() ) {
      IlcError( "can't write Trigger, file is not open" );
      return kFALSE;
   }

   TObject* obj = file->Get( fgkTriggerContainerName );
   if( obj ) { //if they exist, see if option OVERWRITE is used
      TString tmp(opt);
      if( tmp.Contains( "OVERWRITE", TString::kIgnoreCase ) == 0) {
         //if it is not used -  give an error message and return an error code
         IlcError( "Tree already exisists. Use option \"OVERWRITE\" to overwrite previous data" );
         return 3;
      }
   }
   file->cd();
   fCTrigger->Write( 0, TObject::kOverwrite );
   tree->Write( 0, TObject::kOverwrite );
   file->Flush();

   IlcDebug(1, "WRITTEN\n\n");
  
   return 0;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteIlcRun(Option_t* /*opt*/)
{
//writes IlcRun object to the file
  fGAFile->cd();
  if (GetIlcRun()) GetIlcRun()->Write();
  return 0;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteKinematics(Option_t* opt)
{
//writes Kinematics
  return fKineDataLoader->GetBaseLoader(0)->WriteData(opt);
}
/**************************************************************************/
Int_t IlcRunLoader::WriteTrackRefs(Option_t* opt)
{
//writes Track References tree
  return fTrackRefsDataLoader->GetBaseLoader(0)->WriteData(opt);
}
/**************************************************************************/

Int_t IlcRunLoader::WriteHits(Option_t* opt)
{
//Calls WriteHits for all loaders
  Int_t res;
  Int_t result = 0;
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     res = loader->WriteHits(opt);
     if (res)
      {
        IlcError(Form("Failed to write hits for %s (%d)",loader->GetDetectorName().Data(),res));
        result = 1;
      }
   }
  return result;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteSDigits(Option_t* opt)
{
//Calls WriteSDigits for all loaders
  Int_t res;
  Int_t result = 0;
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     res = loader->WriteSDigits(opt);
     if (res)
      {
        IlcError(Form("Failed to write summable digits for %s.",loader->GetDetectorName().Data()));
        result = 1;
      }
   }
  return result;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteDigits(Option_t* opt)
{
//Calls WriteDigits for all loaders
  Int_t res;
  Int_t result = 0;
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   { 
     res = loader->WriteDigits(opt);
     if (res)
      {
        IlcError(Form("Failed to write digits for %s.",loader->GetDetectorName().Data()));
        result = 1;
      }
   }
  return result;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteRecPoints(Option_t* opt)
{
//Calls WriteRecPoints for all loaders
  Int_t res;
  Int_t result = 0;
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     res = loader->WriteRecPoints(opt);
     if (res)
      {
        IlcError(Form("Failed to write Reconstructed Points for %s.",
		      loader->GetDetectorName().Data()));
        result = 1;
      }
   }
  return result;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteTracks(Option_t* opt)
{
//Calls WriteTracks for all loaders
  Int_t res;
  Int_t result = 0;
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     res = loader->WriteTracks(opt);
     if (res)
      {
        IlcError(Form("Failed to write Tracks for %s.",
		      loader->GetDetectorName().Data()));
        result = 1;
      }
   }
  return result;
}
/**************************************************************************/

Int_t IlcRunLoader::WriteRunLoader(Option_t* /*opt*/)
{
//Writes itself to the file
  CdGAFile();
  this->Write(0,TObject::kOverwrite);
  return 0;
}
/**************************************************************************/

Int_t IlcRunLoader::SetEventFolderName(const TString& name)
{  
//sets top folder name for this run; of alread
  if (name.IsNull())
   {
     IlcError("Name is empty");
     return 1;
   }
  
  //check if such a folder already exists - try to find it in ilc top folder
  TObject* obj = IlcConfig::Instance()->GetTopFolder()->FindObject(name);
  if(obj)
   {
     TFolder* fold = dynamic_cast<TFolder*>(obj);
     if (fold == 0x0)
      {
       IlcError("Such a obejct already exists in top ilc folder and it is not a folder.");
       return 2;
      }
     //folder which was found is our folder
     if (fEventFolder == fold)
      {
       return 0;
      }
     else
      {
       IlcError("Such a folder already exists in top ilc folder. Can not mount.");
       return 2;
      }
   }

  //event is alredy connected, just change name of the folder
  if (fEventFolder) 
   {
     fEventFolder->SetName(name);
     return 0;
   }

  if (fKineDataLoader == 0x0)
    fKineDataLoader = new IlcDataLoader(fgkDefaultKineFileName,fgkKineContainerName,"Kinematics");

  if ( fTrackRefsDataLoader == 0x0)
    fTrackRefsDataLoader = new IlcDataLoader(fgkDefaultTrackRefsFileName,fgkTrackRefsContainerName,"Track References");
   
  //build the event folder structure
  IlcDebug(1, Form("Creating new event folder named %s",name.Data()));
  fEventFolder = IlcConfig::Instance()->BuildEventFolder(name,"Event Folder");
  fEventFolder->Add(this);//put myself to the folder to accessible for all
  
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     loader->Register(fEventFolder);//build folder structure for this detector
   }
  
  fKineDataLoader->SetEventFolder(GetEventFolder());
  fTrackRefsDataLoader->SetEventFolder(GetEventFolder());
  fKineDataLoader->SetFolder(GetEventFolder());
  fTrackRefsDataLoader->SetFolder(GetEventFolder());
  
  fEventFolder->SetOwner();
  return 0;
}
/**************************************************************************/

void IlcRunLoader::AddLoader(IlcLoader* loader)
 {
 //Adds the Loader for given detector 
  if (loader == 0x0) //if null shout and exit
   {
     IlcError("Parameter is NULL");
     return;
   }
  loader->SetDirName(fUnixDirName);
  if (fEventFolder) loader->SetEventFolder(fEventFolder); //if event folder is already defined, 
                                                          //pass information to the Loader
  fLoaders->Add(loader);//add the Loader to the array
 }
/**************************************************************************/

void IlcRunLoader::AddLoader(IlcDetector* det)
 {
//Asks module (detector) ro make a Loader and stores in the array
   if (det == 0x0) return;
   IlcLoader* get = det->GetLoader();//try to get loader
   if (get == 0x0)  get = det->MakeLoader(fEventFolder->GetName());//if did not obtain, ask to make it

   if (get) 
    {
      IlcDebug(1, Form("Detector: %s   Loader : %s",det->GetName(),get->GetName()));
      AddLoader(get);
    }
 }

/**************************************************************************/

IlcLoader* IlcRunLoader::GetLoader(const char* detname) const
{
//returns loader for given detector
//note that naming convention is TPCLoader not just TPC
  return (IlcLoader*)fLoaders->FindObject(detname);
}

/**************************************************************************/

IlcLoader* IlcRunLoader::GetLoader(IlcDetector* det) const
{
//get loader for detector det
 if(det == 0x0) return 0x0;
 TString getname(det->GetName());
 getname+="Loader";
 IlcDebug(1, Form(" Loader name is %s",getname.Data()));
 return GetLoader(getname);
}

/**************************************************************************/

void IlcRunLoader::CleanFolders()
{
//  fEventFolder->Add(this);//put myself to the folder to accessible for all

  CleanDetectors();
  CleanHeader();
  CleanKinematics();
  CleanTrigger();
}
/**************************************************************************/

void IlcRunLoader::CleanDetectors()
{
//Calls CleanFolders for all detectors
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
     loader->CleanFolders();
   }
}
/**************************************************************************/

void IlcRunLoader::RemoveEventFolder()
{
//remove all the tree of event 
//all the stuff changing EbE stays untached (PDGDB, tasks, etc.)

 if (fEventFolder == 0x0) return;
 fEventFolder->SetOwner(kFALSE);//don't we want to deleted while removing the folder that we are sitting in
 fEventFolder->Remove(this);//remove us drom folder
 
 IlcConfig::Instance()->GetTopFolder()->SetOwner(); //brings ownership back for fEventFolder since it sits in top folder
 IlcConfig::Instance()->GetTopFolder()->Remove(fEventFolder); //remove the event tree
 delete fEventFolder;
}
/**************************************************************************/

void IlcRunLoader::SetGIlcFile(TFile* gafile)
{
//sets pointer to gilc.root file
 fGAFile = gafile;
}

/**************************************************************************/

Int_t IlcRunLoader::LoadHits(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  IlcDebug(1, "Loading Hits");
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     IlcDebug(1, "Option is All");
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer array
   }   

  IlcDebug(1, Form("For detectors. Number of detectors chosen for loading %d",loaders->GetEntries()));
  
  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    IlcDebug(1, Form("    Calling LoadHits(%s) for %s",opt,loader->GetName()));
    loader->LoadHits(opt);
   }
  IlcDebug(1, "Done");
  return 0;
} 

/**************************************************************************/

Int_t IlcRunLoader::LoadSDigits(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->LoadSDigits(opt);
   }
  return 0;
} 

/**************************************************************************/

Int_t IlcRunLoader::LoadDigits(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->LoadDigits(opt);
   }
  return 0;
} 
/**************************************************************************/

Int_t IlcRunLoader::LoadRecPoints(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->LoadRecPoints(opt);
   }
  return 0;
} 
/**************************************************************************/

Int_t IlcRunLoader::LoadRecParticles(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->LoadRecParticles(opt);
   }
  return 0;
} 
/**************************************************************************/

Int_t IlcRunLoader::LoadTracks(Option_t* detectors,Option_t* opt)
{
//LoadHits in selected detectors i.e. detectors="ITS TPC TRD" or "all"

  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->LoadTracks(opt);
   }
  return 0;
} 
/**************************************************************************/

void IlcRunLoader::UnloadHits(Option_t* detectors)
{
  //unloads hits for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadHits();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadSDigits(Option_t* detectors)
{
  //unloads SDigits for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadSDigits();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadDigits(Option_t* detectors)
{
  //unloads Digits for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadDigits();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadRecPoints(Option_t* detectors)
{
  //unloads RecPoints for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadRecPoints();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadAll(Option_t* detectors)
{
  //calls UnloadAll for detectors names specified in parameter
  // option "all" passed can be passed
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadAll();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadTracks(Option_t* detectors)
{
  //unloads Tracks for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadTracks();
   }
}
/**************************************************************************/

void IlcRunLoader::UnloadRecParticles(Option_t* detectors)
{
  //unloads Particles for detectors specified in parameter
  TObjArray* loaders;
  TObjArray arr;

  const char* oAll = strstr(detectors,"all");
  if (oAll)
   {
     loaders = fLoaders;
   }
  else
   {
     GetListOfDetectors(detectors,arr);//this method looks for all Loaders corresponding to names (many) specified in detectors option
     loaders = &arr;//get the pointer to array
   }   

  TIter next(loaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
    loader->UnloadRecParticles();
   }
}
/**************************************************************************/

IlcRunLoader* IlcRunLoader::GetRunLoader(const char* eventfoldername)
{
//returns RunLoader from folder named eventfoldername
  TFolder* evfold= dynamic_cast<TFolder*>(IlcConfig::Instance()->GetTopFolder()->FindObject(eventfoldername));
  if (evfold == 0x0)
   {
     return 0x0;
   }
  IlcRunLoader* runget = dynamic_cast<IlcRunLoader*>(evfold->FindObject(IlcRunLoader::fgkRunLoaderName));
  return runget;
  
}
/**************************************************************************/

IlcLoader* IlcRunLoader::GetDetectorLoader(const char* detname, const char* eventfoldername)
{
//get the loader of the detector with the given name from the global
//run loader object
  IlcRunLoader* runLoader = GetRunLoader(eventfoldername);
  if (!runLoader) {
    IlcErrorClass("No run loader found");
    return NULL;
  }
  return runLoader->GetDetectorLoader(detname);
}
/**************************************************************************/

IlcLoader* IlcRunLoader::GetDetectorLoader(const char* detname)
{
//get the loader of the detector with the given name from the global
//run loader object
  
  char loadername[256];
  snprintf(loadername, 255, "%sLoader", detname);
  IlcLoader* loader = GetLoader(loadername);
  if (!loader) {
    IlcError(Form("No loader for %s found", detname));
    return NULL;
  }
  return loader;
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeH(const char* detname, Bool_t maketree, const char* eventfoldername)
{
//get the tree with hits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeH() && maketree) loader->MakeTree("H");
  return loader->TreeH();
}

/**************************************************************************/

TTree* IlcRunLoader::GetTreeH(const char* detname, Bool_t maketree)
{
//get the tree with hits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeH() && maketree) loader->MakeTree("H");
  return loader->TreeH();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeS(const char* detname, Bool_t maketree,const char* eventfoldername)
{
//get the tree with summable digits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeS() && maketree) loader->MakeTree("S");
  return loader->TreeS();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeS(const char* detname, Bool_t maketree)
{
//get the tree with summable digits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeS() && maketree) loader->MakeTree("S");
  return loader->TreeS();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeD(const char* detname, Bool_t maketree,const char* eventfoldername)
{
//get the tree with digits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeD() && maketree) loader->MakeTree("D");
  return loader->TreeD();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeD(const char* detname, Bool_t maketree)
{
//get the tree with digits of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeD() && maketree) loader->MakeTree("D");
  return loader->TreeD();
}
/**************************************************************************/
TTree* IlcRunLoader::GetTreeR(const char* detname, Bool_t maketree,const char* eventfoldername)
{
//get the tree with clusters of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeR() && maketree) loader->MakeTree("R");
  return loader->TreeR();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeR(const char* detname, Bool_t maketree)
{
//get the tree with clusters of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeR() && maketree) loader->MakeTree("R");
  return loader->TreeR();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeT(const char* detname, Bool_t maketree,const char* eventfoldername)
{
//get the tree with tracks of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeT() && maketree) loader->MakeTree("T");
  return loader->TreeT();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeT(const char* detname, Bool_t maketree)
{
//get the tree with tracks of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeT() && maketree) loader->MakeTree("T");
  return loader->TreeT();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeP(const char* detname, Bool_t maketree,const char* eventfoldername)
{
//get the tree with particles of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname,eventfoldername);
  if (!loader) return NULL;
  if (!loader->TreeP() && maketree) loader->MakeTree("P");
  return loader->TreeP();
}
/**************************************************************************/

TTree* IlcRunLoader::GetTreeP(const char* detname, Bool_t maketree)
{
//get the tree with particles of the detector with the given name
//if maketree is true and the tree does not exist, the tree is created
  IlcLoader* loader = GetDetectorLoader(detname);
  if (!loader) return NULL;
  if (!loader->TreeP() && maketree) loader->MakeTree("P");
  return loader->TreeP();
}

/**************************************************************************/

void IlcRunLoader::CdGAFile()
{
//sets gDirectory to gilc file
//work around 
  if(fGAFile) fGAFile->cd();
}
 
/**************************************************************************/

void IlcRunLoader::GetListOfDetectors(const char * namelist,TObjArray& pointerarray) const
 {
//this method looks for all Loaders corresponding 
//to names (many) specified in namelist i.e. namelist ("ITS TPC TRD")
  
   char buff[10];
   char dets [200];
   strncpy(dets,namelist,199);//compiler cries when char* = const Option_t*;
   //   dets[strlen(dets)+1] = '\n';//set endl at the end of string 
   char* pdet = dets;
   Int_t tmp;
   for(;;)
    {
      tmp = sscanf(pdet,"%9s",buff);//read the string from the input string pdet into buff
      if ( (buff[0] == 0) || (tmp == 0) ) break; //if not read
     
      pdet = strstr(pdet,buff) + strlen(buff);//move the input pointer about number of bytes (letters) read
      //I am aware that is a little complicated. I don't know the number of spaces between detector names
      //so I read the string, than I find where it starts (strstr) and move the pointer about length of a string
      //If there is a better way, please write me (Piotr.Skowronski@cern.ch)
      //construct the Loader name
      TString getname(buff);
      getname+="Loader";
      IlcLoader* loader = GetLoader(getname);//get the Loader
      if (loader)
       {
        pointerarray.Add(loader);
       }
      else
       {
        IlcError(Form("Can not find Loader for %s",buff));
       }
        
      buff[0] = 0;
    }
 }
/*****************************************************************************/ 

void IlcRunLoader::Clean(const TString& name)
{
//removes object with given name from event folder and deletes it
  if (GetEventFolder() == 0x0) return;
  TObject* obj = GetEventFolder()->FindObject(name);
  if(obj)
   {
     IlcDebug(1, Form("name=%s, cleaning %s.",GetName(),name.Data()));
     GetEventFolder()->Remove(obj);
     delete obj;
     obj = 0x0;
   }
}

/*****************************************************************************/ 

void IlcRunLoader::SetCompressionLevel(Int_t cl)
{
//Sets Compression Level in all files
 if (fGAFile) fGAFile->SetCompressionLevel(cl);
 SetKineComprLevel(cl);
 SetTrackRefsComprLevel(cl);
 TIter next(fLoaders);
 IlcLoader *loader;
 while((loader = (IlcLoader*)next()))
  {
   loader->SetCompressionLevel(cl);
  }
}
/**************************************************************************/

void IlcRunLoader::SetKineComprLevel(Int_t cl)
{
//Sets comression level in Kine File
  fKineDataLoader->SetCompressionLevel(cl);
}
/**************************************************************************/

void IlcRunLoader::SetTrackRefsComprLevel(Int_t cl)
{
//Sets comression level in Track Refences File
  fTrackRefsDataLoader->SetCompressionLevel(cl);
}
/**************************************************************************/

void IlcRunLoader::UnloadHeader()
{
 //removes TreeE from folder and deletes it
 // as well as fHeader object
 CleanHeader();
 fHeader = 0x0;
}
/**************************************************************************/

void IlcRunLoader::UnloadTrigger()
{
 //removes TreeCT from folder and deletes it
 // as well as fHeader object
   CleanTrigger();
   delete fCTrigger;
   fCTrigger = 0x0;
}

/**************************************************************************/

void IlcRunLoader::UnloadKinematics()
{
//Unloads Kinematics
 fKineDataLoader->GetBaseLoader(0)->Unload();
}
/**************************************************************************/

void IlcRunLoader::UnloadTrackRefs()
{
//Unloads Track Refernces
 fTrackRefsDataLoader->GetBaseLoader(0)->Unload();
}
/**************************************************************************/

void IlcRunLoader::UnloadgIlc()
{
//Unloads gIlc
 if (gIlc == GetIlcRun())
  {
   IlcDebug(1, "Set gIlc = 0x0");
   gIlc = 0x0;//if gIlc is the same that in folder (to be deleted by the way of folder)
  }
 IlcRun* ilcrun = GetIlcRun();
 if (GetEventFolder()) GetEventFolder()->Remove(ilcrun);
 delete ilcrun;
}
/**************************************************************************/

void  IlcRunLoader::MakeTrackRefsContainer()
{
// Makes a tree for Track References
  fTrackRefsDataLoader->MakeTree();
}
/**************************************************************************/

Int_t IlcRunLoader::LoadTrackRefs(Option_t* option)
{
//Load track references from file (opens file and posts tree to folder)

 return fTrackRefsDataLoader->GetBaseLoader(0)->Load(option);
}
/**************************************************************************/

void  IlcRunLoader::SetDirName(TString& dirname)
{
//sets directory name 
  if (dirname.IsNull()) return;
  fUnixDirName = dirname;
  fKineDataLoader->SetDirName(dirname);
  fTrackRefsDataLoader->SetDirName(dirname);
  
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
    loader->SetDirName(dirname);
   }

}
/*****************************************************************************/ 

Int_t IlcRunLoader::GetFileOffset() const
{
//returns the file number that is added to the file name for current event
  return Int_t(fCurrentEvent/fNEventsPerFile);
}

/*****************************************************************************/ 
const TString IlcRunLoader::SetFileOffset(const TString& fname)
{
//adds the the number to the file name at proper place for current event
  Long_t offset = (Long_t)GetFileOffset();
  if (offset < 1) return fname;
  TString soffset;
  soffset += offset;//automatic conversion to string
  TString dotroot(".root");
  const TString& offfsetdotroot = offset + dotroot;
  TString out = fname;
  out = out.ReplaceAll(dotroot,offfsetdotroot);
  IlcDebug(1, Form(" in=%s out=%s",fname.Data(),out.Data()));
  return out;
}
/*****************************************************************************/ 

void IlcRunLoader::SetDigitsFileNameSuffix(const TString& suffix)
{
//adds the suffix before ".root", 
//e.g. TPC.Digits.root -> TPC.DigitsMerged.root
//made on Jiri Chudoba demand

  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next())) 
   {
     loader->SetDigitsFileNameSuffix(suffix);
   }
}
/*****************************************************************************/ 

TString IlcRunLoader::GetFileName() const
{
//returns name of gilc file
 TString result;
 if (fGAFile == 0x0) return result;
 result = fGAFile->GetName();
 return result;
}
/*****************************************************************************/ 

void IlcRunLoader::SetDetectorAddresses()
{
 //calls SetTreeAddress for all detectors
  if (GetIlcRun()==0x0) return;
  TIter next(GetIlcRun()->Modules());
  IlcModule* mod;
  while((mod = (IlcModule*)next())) 
   {
     IlcDetector* det = dynamic_cast<IlcDetector*>(mod);
     if (det) det->SetTreeAddress();
   }
}
/*****************************************************************************/ 

void IlcRunLoader::Synchronize()
{
  //synchrinizes all writtable files 
  TIter next(fLoaders);
  IlcLoader *loader;
  while((loader = (IlcLoader*)next()))
   {
     loader->Synchronize();
   }
  
  fKineDataLoader->Synchronize();
  fTrackRefsDataLoader->Synchronize();

  TFile* file = gROOT->GetFile( gSystem->ConcatFileName( fUnixDirName.Data(), fgkDefaultTriggerFileName.Data() ) ) ;
  if( file ) file->Flush();
  
  if (fGAFile) fGAFile->Flush();
}
/*****************************************************************************/ 
/*****************************************************************************/ 
