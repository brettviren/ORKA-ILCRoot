//
//Class (base) responsible for management of data:
//    - opening and closing proper files
//    - posting data to folders
//    - writing data from folders to proper files
//
//Author: Ilc Offline Group http://ilcsoft.cern.ch
//Responsible: Piotr.Skowronski@cern.ch
//
#include "IlcLoader.h"

//Root includes
#include <TROOT.h>
#include <TFile.h>
#include <TFolder.h>
#include <TString.h>

//IlcRoot includes
#include "IlcConfig.h"
#include "IlcDetector.h"
#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcRunLoader.h"

const TString IlcLoader::fgkDefaultHitsContainerName("TreeH");
const TString IlcLoader::fgkDefaultDigitsContainerName = "TreeD";
const TString IlcLoader::fgkDefaultSDigitsContainerName = "TreeS";
const TString IlcLoader::fgkDefaultRecPointsContainerName = "TreeR";
const TString IlcLoader::fgkDefaultTracksContainerName = "TreeT";
const TString IlcLoader::fgkDefaultRecParticlesContainerName = "TreeP";
const TString IlcLoader::fgkLoaderBaseName("Loader");

ClassImp(IlcLoader)
//___________________________________________________________________
/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcLoader                                                //
//                                                                 //
//  Base class for Loaders.                                        //
//  Loader provides base I/O fascilities for "standard" data.      //
//  Each detector has a laoder data member                         //
//  loader is accessible via folder structure as well              //
//                                                                 //
/////////////////////////////////////////////////////////////////////
 
/*****************************************************************************/ 

IlcLoader::IlcLoader():
 fDataLoaders(0x0),
 fDetectorName(""),
 fEventFolder(0x0),
 fDataFolder(0x0),
 fDetectorDataFolder(0x0),
 fModuleFolder(0x0)
{
  //default constructor
  
}
/******************************************************************/

IlcLoader::IlcLoader(const Char_t* detname,const Char_t* eventfoldername):
 fDataLoaders(new TObjArray(kNDataTypes)),
 fDetectorName(""),
 fEventFolder(0x0),
 fDataFolder(0x0),
 fDetectorDataFolder(0x0),
 fModuleFolder(0x0)
{
  //ctor
   IlcDebug(1, Form("detname = %s eventfoldername = %s",detname,eventfoldername));

   //try to find folder eventfoldername in top ilc folder
   //safe because GetTopFolder will abort in case of failure

   fDetectorName = detname;
   fName = fDetectorName+"Loader";
   InitDefaults();

   TObject* fobj = GetTopFolder()->FindObject(eventfoldername);
   fEventFolder = (fobj)?dynamic_cast<TFolder*>(fobj):0x0;//in case FindObject returns NULL dynamic cast cause seg. fault
   SetEventFolder(fEventFolder);
   
 }
/*****************************************************************************/ 

IlcLoader::IlcLoader(const Char_t * detname,TFolder* eventfolder):
 fDataLoaders(new TObjArray(kNDataTypes)),
 fDetectorName(detname),
 fEventFolder(0x0),
 fDataFolder(0x0),
 fDetectorDataFolder(0x0),
 fModuleFolder(0x0)
{
//constructor
   fDetectorName = detname;
   fName = fDetectorName+"Loader";
   InitDefaults();
   SetEventFolder(eventfolder);
   //fileoption's don't need to initialized because default TString ctor does it correctly
}

/*****************************************************************************/ 
IlcLoader::~IlcLoader()
{
//detructor
  if (fDataLoaders) fDataLoaders->SetOwner();
  delete fDataLoaders;
}
/*****************************************************************************/ 

void IlcLoader::InitDefaults()
{
  // H I T S 
  IlcDataLoader* dl;
  dl = new IlcDataLoader(fDetectorName + ".Hits.root",fgkDefaultHitsContainerName, "Hits" );
  fDataLoaders->AddAt(dl,kHits);
  
  
  // S U M M A B L E   D I G I T S
  dl = new IlcDataLoader(fDetectorName + ".SDigits.root",fgkDefaultSDigitsContainerName, "Summable Digits");
  fDataLoaders->AddAt(dl,kSDigits);

  // D I G I T S  
  dl = new IlcDataLoader(fDetectorName + ".Digits.root",fgkDefaultDigitsContainerName, "Digits");
  fDataLoaders->AddAt(dl,kDigits);
  
  // R E C O N S T R U C T E D   P O I N T S aka C L U S T E R S 
  dl = new IlcDataLoader(fDetectorName + ".RecPoints.root",fgkDefaultRecPointsContainerName, "Reconstructed Points");
  fDataLoaders->AddAt(dl,kRecPoints);
  
  // T R A C K S
  dl = new IlcDataLoader(fDetectorName + ".Tracks.root",fgkDefaultTracksContainerName, "Tracks");
  fDataLoaders->AddAt(dl,kTracks);
  
  // R E C O N S T R U C T E D   P O I N T S aka C L U S T E R S 
  dl = new IlcDataLoader(fDetectorName + ".RecParticles.root",fgkDefaultRecParticlesContainerName, "Reconstructed Particles");
  fDataLoaders->AddAt(dl,kRecParticles);

 }
/*****************************************************************************/ 

IlcDataLoader* IlcLoader::GetDataLoader(const char* name)
{
//returns Data Loader with specified name
  return dynamic_cast<IlcDataLoader*>(fDataLoaders->FindObject(name));
}
/*****************************************************************************/ 
void IlcLoader::AddDataLoader(IlcDataLoader* dl)
{
  //
  // Adds a data loader
  //
  if (dl == 0x0)
   {
     IlcError("Pointer is NULL");
     return;
   }
  if (fDataLoaders->FindObject(dl->GetName()))
   {
     IlcError("Such a loader exists");
     return;
   }
  fDataLoaders->AddLast(dl);
  dl->SetEventFolder(fEventFolder);
  dl->SetFolder(GetDetectorDataFolder()); //Must exists - ensure register is called before
}
/*****************************************************************************/ 

Int_t IlcLoader::SetEvent()
{
 //basically the same that GetEvent but do not post data to folders
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 while ((dl = (IlcDataLoader*)next()))
  {
    dl->SetEvent();
  }
 return 0;
}
/******************************************************************/

void IlcLoader::UnloadAll()
{
 //calls UnloadAll for all base laoders
 //Unloads everything
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 while ((dl = (IlcDataLoader*)next()))
  {
    dl->UnloadAll();
  }
}
/******************************************************************/

Int_t IlcLoader::GetEvent()
{
 //changes to proper root  directory and tries to load data from files to folders
 // event number is defined in RunLoader
 // 
 //returns:
 //     0  - in case of no error
 //     1  - event not found
 //     
 
 Int_t retval;   
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 while ((dl = (IlcDataLoader*)next()))
  {
    retval = dl->GetEvent();
    if (retval)
     {
       IlcError(Form("Error occured while GetEvent for %s",dl->GetName()));
       return retval;
     }
  }

 return 0;
}

/******************************************************************/

TFolder* IlcLoader::GetTopFolder()
{
//returns TOP ilcroot folder, just a simple interface to IlcConfig (gives shorter notation)
 return IlcConfig::Instance()->GetTopFolder();
}

/******************************************************************/

TFolder* IlcLoader::GetEventFolder()
{
//get EVENT folder (data that are changing from event to event, even in single run)
  return fEventFolder;
}
/******************************************************************/
TFolder* IlcLoader::GetDataFolder()
{
//returns the folder speciofic to given detector e.g. /Folders/Event/Data/
 if (!fDataFolder)
  {
   fDataFolder =  dynamic_cast<TFolder*>(GetEventFolder()->FindObject(IlcConfig::Instance()->GetDataFolderName()));
   
   if (!fDataFolder)
    {
     IlcFatal("Can not find IlcRoot data folder. Aborting");
     return 0x0;
    }
  }
  return fDataFolder;
}

/*****************************************************************************/ 

TFolder* IlcLoader::GetModulesFolder()
{
  //returns pointer to the folder containing modules
 if (!fModuleFolder)
  {
   fModuleFolder =  dynamic_cast<TFolder*>(GetEventFolder()->FindObjectAny(IlcConfig::GetModulesFolderName()));
   
   if (!fModuleFolder)
    {
     IlcFatal("Can not find modules folder. Aborting");
     return 0x0;
    }
  }
 return fModuleFolder;
   
}

/*****************************************************************************/ 

TDirectory* IlcLoader::ChangeDir(TFile* file, Int_t eventno)
{
//changes the root directory in "file" to "dirname" which corresponds to event 'eventno'
//in case of success returns the pointer to directory
//else NULL
 
 if (file == 0x0)
  {
    IlcErrorClass("File is null");
    return 0x0;
  }
 if (file->IsOpen() == kFALSE)
  {
    IlcErrorClass("File is not opened");
    return 0x0;
  }

 TString dirname("Event");
 dirname+=eventno;
 IlcDebugClass(1, Form("Changing Dir to %s in file %s.",dirname.Data(),file->GetName()));

 Bool_t result;
 
 TDirectory* dir = dynamic_cast<TDirectory*>(file->Get(dirname));

 if (dir == 0x0)
  {
    IlcDebugClass(1, Form("Can not find directory %s in file %s, creating...",
		     dirname.Data(),file->GetName()));
    
    if (file->IsWritable() == kFALSE)
     {
       IlcErrorClass(Form("Can not create directory. File %s in not writable.",
		     file->GetName()));
       return 0x0;
     }
            
    TDirectory* newdir = file->mkdir(dirname);
    if (newdir == 0x0)
     {
       IlcErrorClass(Form("Failed to create new directory in file %s.",
		     file->GetName()));
       return 0x0;
     }
    result = file->cd(dirname);
    if (result == kFALSE)
     {
       return 0x0;
     }
  }
 else
  {
   file->cd();//make a file active 
   file->cd(dirname);//cd event dir
  }

 return gDirectory;
}
/*****************************************************************************/ 

TString IlcLoader::GetUnixDir() const
 {
 //This Method will manage jumping through unix directories in case of 
 //run with more events per run than defined in gIlc
 
   TString dir;
   
   return dir;
 }
/*****************************************************************************/ 
/************************************************************/

void IlcLoader::MakeTree(Option_t *option)
 {
//Makes a tree depending on option 
//   H: - Hits
//   D: - Digits
//   S: - Summable Digits
//   R: - Reconstructed Points (clusters)
//   T: - Tracks (tracklets)

  const char *oH = strstr(option,"H");
  const char *oD = strstr(option,"D");
  const char *oS = strstr(option,"S");
  const char *oR = strstr(option,"R");
  const char *oT = strstr(option,"T");
  const char *oP = strstr(option,"P");
  
  if (oH) MakeHitsContainer();
  if (oD) MakeDigitsContainer();
  if (oS) MakeSDigitsContainer();
  if (oR) MakeRecPointsContainer();
  if (oT) MakeTracksContainer();
  if (oP) MakeRecParticlesContainer();
 }

/*****************************************************************************/ 
Int_t  IlcLoader::WriteHits(Option_t* opt) const
 {
   // Writes hits
   IlcDataLoader* dl = GetHitsDataLoader();
   Int_t ret = dl->WriteData(opt);
   return ret;
 }
/*****************************************************************************/ 

Int_t IlcLoader::WriteSDigits(Option_t* opt) const
 {
   // Writes summable digits
   IlcDataLoader* dl = GetSDigitsDataLoader();
   Int_t ret = dl->WriteData(opt);
   return ret;
 }
 
/*****************************************************************************/ 

TObject** IlcLoader::GetDetectorDataRef(TObject *obj)
{
  // Returns pointer to an entry in the list of folders pointing to "obj"
 if (obj == 0x0)
  {
    return 0x0;
  }
 return GetDetectorDataFolder()->GetListOfFolders()->GetObjectRef(obj) ;
}

/*****************************************************************************/ 
void IlcLoader::CleanFolders()
 {
 //Cleans all posted objects == removes from folders and deletes them
   TIter next(fDataLoaders);
   IlcDataLoader* dl;
   while ((dl = (IlcDataLoader*)next()))
    { 
      IlcDebug(1, Form("name = %s cleaning",dl->GetName()));
      dl->Clean();
    }
 }

/*****************************************************************************/ 

Int_t IlcLoader::ReloadAll()
{
  // Calling Reload function for all the data loaders
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 
 while((dl = (IlcDataLoader*)next()))
  {
   Int_t err = dl->Reload();
   if (err)
    {
      IlcError(Form("Reload returned error for %s",dl->GetName()));
      return err;
    }
  }
 return 0;
}
/*****************************************************************************/ 

void IlcLoader::CloseFiles()
{
//close files for data loaders
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 while((dl = (IlcDataLoader*)next()))
  {
   dl->CloseFile();
  }
}
/*****************************************************************************/ 

Int_t  IlcLoader::SetEventFolder(TFolder* eventfolder)
{
  //sets the event folder
 if (eventfolder == 0x0)
  {
    IlcError("Stupid joke. Argument is NULL");
    return 1;
  }

 fEventFolder = eventfolder;
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 
 while((dl = (IlcDataLoader*)next()))
  {
    dl->SetEventFolder(fEventFolder);
    dl->SetFolder(GetDetectorDataFolder()); //Must exists - ensure register is called before
  }

 return 0;
}//sets the event folder
/*****************************************************************************/ 

Int_t IlcLoader::Register(TFolder* eventFolder)
{
//triggers creation of subfolders for a given detector
//this method is called when session is read from disk
//
//warning: IlcDetector in constructor (not default) calls
//creation of folder structure as well (some detectors needs folders 
//alrady in constructors)

 IlcDebug(1, Form("Name is %s.",GetName()));
 if (eventFolder == 0x0)
  {
    IlcError("Event folder is not set.");
    return 1;
  }
 Int_t retval = IlcConfig::Instance()->AddDetector(eventFolder,fDetectorName,fDetectorName);
 if(retval)
  {
    IlcError(Form("Can not create folders for %s. Event folder name is %s",
		  fDetectorName.Data(),eventFolder->GetName()));
    return retval;
  }
 SetEventFolder(eventFolder);
 return 0;
}
/*****************************************************************************/ 
IlcRunLoader* IlcLoader::GetRunLoader()
{
//gets the run-loader from event folder
  IlcRunLoader* rg = 0x0;
  TObject * obj = GetEventFolder()->FindObject(IlcRunLoader::GetRunLoaderName());
  if (obj) rg = dynamic_cast<IlcRunLoader*>(obj);
  return rg;
}
/*****************************************************************************/ 
Bool_t  IlcLoader::TestFileOption(Option_t* opt)
{
//tests the TFile::Option
//if file is truncated at opening moment ("recreate", "new" or "create") returns kFALSE;
//else kTRUE (means opened with "read" or "update" mode)
  TString option(opt);
  if (option.CompareTo("recreate",TString::kIgnoreCase) == 0) return kFALSE;
  if (option.CompareTo("new",TString::kIgnoreCase) == 0) return kFALSE;
  if (option.CompareTo("create",TString::kIgnoreCase) == 0) return kFALSE;
  return kTRUE;
}
/*****************************************************************************/ 
void  IlcLoader::SetDirName(TString& dirname)
{
//adds "dirname" to each file. Dirname should end with "#","/", or ":" 
  TIter next(fDataLoaders);
  IlcDataLoader* dl;
  while((dl = (IlcDataLoader*)next()))
   {
    dl->SetDirName(dirname);
   }
}

/*****************************************************************************/ 

void IlcLoader::SetDigitsFileNameSuffix(const TString& suffix) const
{
  //adds the suffix before ".root", 
  //e.g. TPC.Digits.root -> TPC.DigitsMerged.root
  //made on Jiri Chudoba demand
  GetDigitsDataLoader()->SetFileNameSuffix(suffix);
}
/*****************************************************************************/ 

void IlcLoader::SetCompressionLevel(Int_t cl)
{
//sets comression level for data defined by di
  TIter next(fDataLoaders);
  IlcDataLoader* dl;
  while((dl = (IlcDataLoader*)next()))
   {
     dl->SetCompressionLevel(cl);
   }
}
/*****************************************************************************/ 

void IlcLoader::Clean()
{
//Cleans all data loaders
  TIter next(fDataLoaders);
  IlcDataLoader* dl;
  while((dl = (IlcDataLoader*)next()))
   {
     dl->Clean();
   }
}
/*****************************************************************************/

void IlcLoader::Clean(const TString& name)
{
  // Removes object with "name" from the detector's data folder
  // and from the memory
  TObject* obj = GetDetectorDataFolder()->FindObject(name);
  if(obj)
   {
     IlcDebug(1, Form("name=%s, cleaning %s.",GetName(),name.Data()));
     GetDetectorDataFolder()->Remove(obj);
     delete obj;
   }
}

/*****************************************************************************/ 

Bool_t IlcLoader::IsOptionWritable(const TString& opt)
{
  // Returns "true" if the option means also "writable"
  if (opt.CompareTo("recreate",TString::kIgnoreCase)) return kTRUE;
  if (opt.CompareTo("new",TString::kIgnoreCase)) return kTRUE;
  if (opt.CompareTo("create",TString::kIgnoreCase)) return kTRUE;
  if (opt.CompareTo("update",TString::kIgnoreCase)) return kTRUE;
  return kFALSE;
}

/*****************************************************************************/ 

void IlcLoader::SetDebug(Int_t deb)
{
  // Sets debug level
  IlcLog::SetClassDebugLevel("IlcRunLoader", deb);
  IlcLog::SetClassDebugLevel("IlcLoader", deb);
  IlcLog::SetClassDebugLevel("IlcDataLoader", deb);
  IlcLog::SetClassDebugLevel("IlcBaseLoader", deb);
  IlcLog::SetClassDebugLevel("IlcObjectLoader", deb);
  IlcLog::SetClassDebugLevel("IlcTreeLoader", deb);
  IlcLog::SetClassDebugLevel("IlcConfig", deb);
}
/*****************************************************************************/ 

void IlcLoader::SetTAddrInDet()
{
  //calls SetTreeAddress for corresponding detector
  IlcRunLoader* rl = GetRunLoader();   
  if (rl == 0x0) return;
  IlcRun* ar = rl->GetIlcRun();
  if (ar == 0x0) return;
  IlcDetector* det = ar->GetDetector(fDetectorName);  
  if (det == 0x0) return;
  det->SetTreeAddress();
}
/*****************************************************************************/ 

void IlcLoader::Synchronize()
{
  //synchrinizes all writtable files 
 TIter next(fDataLoaders);
 IlcDataLoader* dl;
 while ((dl = (IlcDataLoader*)next()))
  {
    dl->Synchronize();
  }
  
}
/*****************************************************************************/ 
/*****************************************************************************/ 
/*****************************************************************************/ 

