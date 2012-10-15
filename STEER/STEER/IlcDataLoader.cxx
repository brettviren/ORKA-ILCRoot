/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//  class IlcDataLoader                                                                    //
//                                                                                         //
//  Container of all data needed for full                                                  //
//  description of each data type                                                          //
//  (Hits, Kine, ...)                                                                      //
//                                                                                         //
//  Each data loader has a basic standard setup of BaseLoaders                             //
//  which can be identuified by indexes (defined by EStdBasicLoaders)                      //
//  Data managed by these standard base loaders has fixed naming convention                //
//  e.g. - tree with hits is always named TreeH                                            //
//                     (defined in IlcLoader::fgkDefaultHitsContainerName)                 //
//                                                                                         //
//  EStdBasicLoaders   idx     Object Type        Description                              //
//      kData           0    TTree or TObject     main data itself (hits,digits,...)       //
//                                                                                         //
//                                                                                         //
//  User can define and add more basic loaders even Run Time.                              //
//  Caution: in order to save information about added base loader                          //
//  user must rewrite Run Loader to gilc.file, overwriting old setup                     //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

/* $Id: IlcDataLoader.cxx 52261 2011-10-23 15:46:57Z hristov $ */

#include "IlcDataLoader.h"
#include "IlcRunLoader.h"
#include "IlcLoader.h"
#include "IlcObjectLoader.h"
#include "IlcTreeLoader.h"
#include "IlcLog.h"

#include <TFile.h>
#include <TROOT.h>
#include <TString.h>
#include <TBits.h>

ClassImp(IlcDataLoader)

//______________________________________________________________________________
IlcDataLoader::IlcDataLoader():
 fFileName(0),
 fFile(0x0),
 fDirectory(0x0),
 fFileOption(),
 fCompressionLevel(2),
 fNEventsPerFile(0),
 fBaseLoaders(0x0),
 fEventFolder(0x0),
 fFolder(0x0)
{
  
}

//______________________________________________________________________________
IlcDataLoader::IlcDataLoader(const char* filename, const char* contname, 
			     const char* name, Option_t* opt):
 TNamed(name,name),
 fFileName(filename),
 fFile(0x0),
 fDirectory(0x0),
 fFileOption(0),
 fCompressionLevel(2),
 fNEventsPerFile(0),
 fBaseLoaders(new TObjArray(4)),
 fEventFolder(0x0),
 fFolder(0x0)
{
  //constructor
  // creates a 0 loader, depending on option, default "T" is specialized loader for trees
  // else standard object loader
  // trees needs special care, becouse we need to set dir before writing
  IlcDebug(1, Form("File name is %s",fFileName.Data()));
  
  TString option(opt);
  IlcBaseLoader* bl;
  if (option.CompareTo("T",TString::kIgnoreCase) == 0)
    bl = new IlcTreeLoader(contname,this);
  else 
    bl = new IlcObjectLoader(contname,this);
  fBaseLoaders->AddAt(bl,kData);
  
}

//______________________________________________________________________________
IlcDataLoader::~IlcDataLoader()
{
  //
  //dtor
  //
  UnloadAll();
}

//______________________________________________________________________________
Int_t  IlcDataLoader::SetEvent()
{
  //
  // The same that GetEvent but do not post data to folders
  //
  IlcRunLoader* rl = GetRunLoader();
  if (rl == 0x0)
    {
      IlcError("Can not get RunGettr");
      return 1;
    }
  
  Int_t evno = rl->GetEventNumber();
  
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  while ((bl = (IlcBaseLoader*)next()))
    {
      if (bl->DoNotReload() == kFALSE) bl->Clean();
    }
  
  if(fFile)
    {
      if (CheckReload())
	{
	  delete fFile;
	  fFile = 0x0;
	  IlcDebug(1, Form("Reloading new file. File opt is %s",fFileOption.Data()));
	  OpenFile(fFileOption);
	}
      
      fDirectory = IlcLoader::ChangeDir(fFile,evno);
      if (fDirectory == 0x0)
	{
	  IlcError(Form("Can not chage directory in file %s",fFile->GetName()));
	  return 1;
	}
    }
  return 0;
}

//______________________________________________________________________________
Int_t  IlcDataLoader::GetEvent()
{
  // posts all loaded data from files to White Board
  // event number is defined in RunLoader
  // 
  //returns:
  //     0  - in case of no error
  //     1  - event not found
  //     
  //for each base laoder post, if was loaded before GetEvent
  
  //call set event to switch to new directory in file
  
  
  //post all data that were loaded before 
  // ->SetEvent does not call Unload, but only cleans White Board
  // such IsLoaded flag stays untached
  
  if ( IlcLoader::TestFileOption(fFileOption) == kTRUE ) //if file is read or update mode try to post
    {                                                     //in other case there is no sense to post: file is new
      TIter nextbl(fBaseLoaders);
      IlcBaseLoader* bl;
      while ((bl = (IlcBaseLoader*)nextbl()))
	{
	  if (bl->IsLoaded())
	    {
	      if (bl->DoNotReload() == kFALSE) bl->Post();
	    }
	} 
    }
  return 0;
}

//______________________________________________________________________________
Int_t IlcDataLoader::OpenFile(Option_t* opt)
{
  //Opens file named 'filename', and assigns pointer to it to 'file'
  //jumps to fDirectoryectory corresponding to current event and stores the pointer to it in 'fDirectory'
  //option 'opt' is passed to TFile::Open
  if (fFile)
    {
      if(fFile->IsOpen() == kTRUE)
	{
	  IlcWarning(Form(" File %s already opened. First close it.",fFile->GetName()));
	  return 0;
	}
      else
	{
	  IlcWarning(Form("Pointer to file %s is not null, but file is not opened",
			  fFile->GetName()));
	  delete fFile;
	  fFile = 0x0;
	}
    }
  
  TString fname(SetFileOffset(fFileName));
  
  fFile = (TFile *)(gROOT->GetListOfFiles()->FindObject(fname));
  if (fFile)
    {
      if(fFile->IsOpen() == kTRUE)
	{
	  TString option1 = fFile->GetOption();
	  if (option1.CompareTo("read",TString::kIgnoreCase) == 0)
	    {
	      IlcInfo(Form("File %s already opened in read mode.",fFile->GetName()));
	    }
	  else
	    {
	      TString option2 = opt;
	      if (option2.CompareTo("read",TString::kIgnoreCase) == 0)
		{
		  IlcInfo(Form("Open already opened file %s in read mode.",fFile->GetName()));
		}
	      else {
		IlcWarning(Form("File %s already opened by sombody else. First close it.",
				fFile->GetName()));
		return 0;
	      }
	    }
	}
    }
  
  fFileOption = opt;
  fFile = TFile::Open(fname,fFileOption);//open the file
  if (fFile == 0x0)
    {//file is null
      IlcError(Form("Can not open file %s",fname.Data()));
      return 1;
    }
  if (fFile->IsOpen() == kFALSE)
    {//file is null
      IlcError(Form("Can not open file %s",fname.Data()));
      return 1;
    }
  
  fFile->SetBit(TFile::kDevNull);
  fFile->SetCompressionLevel(fCompressionLevel);
  
  IlcRunLoader* rg = GetRunLoader();
  if (rg == 0x0)
    {
      IlcError("Can not find Run-Loader in folder.");
      return 2;
    }
  Int_t evno = rg->GetEventNumber();
  
  fDirectory = IlcLoader::ChangeDir(fFile,evno);
  if (fDirectory == 0x0)
    {
      IlcError(Form("Can not chage fDirectory in file %s.",fFile->GetName()));
      return 3; 
    }
  return 0;
}

//______________________________________________________________________________
void IlcDataLoader::Unload()
{
  //
  //unloads main data -  shortcut method 
  //
  GetBaseLoader(0)->Unload();
}

//______________________________________________________________________________
void IlcDataLoader::UnloadAll()
{
  //
  // Unloads all data 
  //
  if ( fFile == 0x0 ) return; //nothing loaded
  
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  while ((bl = (IlcBaseLoader*)next()))
    {
      bl->Unload();
    }
}


//______________________________________________________________________________
Int_t IlcDataLoader::Reload()
{
  //
  // Unloads and loads data again
  //
  if ( fFile == 0x0 ) return 0;
  
  TBits loaded(fBaseLoaders->GetEntries());  
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  
  Int_t i = 0;
  while ((bl = (IlcBaseLoader*)next()))
    {
      if (bl->IsLoaded())
	{
	  loaded.SetBitNumber(i++,kTRUE);
	  bl->Unload();
	}
    }
  
  Int_t retval;
  i = 0;  
  next.Reset();
  while ((bl = (IlcBaseLoader*)next()))
    {
      if (loaded.TestBitNumber(i++))
	{
	  retval = bl->Load(fFileOption);
	  if (retval) 
	    {
	      IlcError(Form("Error occur while loading %s",bl->GetName()));
	      return retval;
	    }
	}
    }
  return 0;
}


//______________________________________________________________________________
Int_t IlcDataLoader::WriteData(Option_t* opt)
{
  //
  // Writes primary data ==  first BaseLoader
  //
  IlcDebug(1, Form("Writing %s container for %s data. Option is %s.",
		   GetBaseLoader(0)->GetName(),GetName(),opt));
  return GetBaseLoader(0)->WriteData(opt);
}

//______________________________________________________________________________
Int_t IlcDataLoader::Load(Option_t* opt)
{
  //
  // Writes primary data ==  first BaseLoader
  //
  return GetBaseLoader(0)->Load(opt);
}

//______________________________________________________________________________
Int_t  IlcDataLoader::SetEventFolder(TFolder* eventfolder)
{
  //
  // Sets the event folder
  //
  if (eventfolder == 0x0)
    {
      IlcError("Stupid joke. Argument is NULL");
      return 1;
    }
  IlcDebug(1, Form("name = %s Setting Event Folder named %s.",
		   GetName(),eventfolder->GetName()));
  
  fEventFolder = eventfolder;
  return 0;
}


//______________________________________________________________________________
Int_t  IlcDataLoader::SetFolder(TFolder* folder)
{
  // Sets the folder and the data loaders
  if (folder == 0x0)
    {
      IlcError("Stupid joke. Argument is NULL");
    return 1;
    }
  
  IlcDebug(1, Form("name = %s Setting folder named %s.",GetName(),folder->GetName()));
  
  fFolder = folder;
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  
  while ((bl = (IlcBaseLoader*)next()))
    {
      bl->SetDataLoader(this);
    }  
  
  return 0;
}

//______________________________________________________________________________
TFolder* IlcDataLoader::GetEventFolder()
{
  //
  // Get EVENT folder
  // Data that are changing from event to event, even in single run
  //
  IlcDebug(1, "EF = %#x");
  return fEventFolder;
}

//______________________________________________________________________________
IlcRunLoader* IlcDataLoader::GetRunLoader()
{
  //
  // Gets the run-loader from event folder
  //
  IlcRunLoader* rg = 0x0;
  TFolder* ef = GetEventFolder();
  if (ef == 0x0)
   {
     IlcError("Can not get event folder.");
     return 0;
   }
  rg = dynamic_cast<IlcRunLoader*>(ef->FindObject(IlcRunLoader::GetRunLoaderName()));
  return rg;
}

//______________________________________________________________________________
void IlcDataLoader::CloseFile()
{
  //
  // Closes file
  //
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  while ((bl = (IlcBaseLoader*)next()))
    {
      if (bl->IsLoaded()) return;
    }
  
  IlcDebug(1, "Closing (object) file.");
  
  if (fFile) {
    fFile->Close("R");
    delete fFile;
    fFile = 0x0;
  }
  fDirectory = 0x0;
}


//______________________________________________________________________________
void IlcDataLoader::Clean()
{
  //
  // Cleans main data
  //
  GetBaseLoader(0)->Clean();
}

//______________________________________________________________________________
void IlcDataLoader::CleanAll()
{
  //
  // Cleans all folders 
  //
  TIter next(fBaseLoaders);
  IlcBaseLoader* bl;
  while ((bl = (IlcBaseLoader*)next()))
    {
      bl->Clean();
    }
}

//______________________________________________________________________________
void IlcDataLoader::SetFileNameSuffix(const TString& suffix)
{
  //
  // adds the suffix before ".root", 
  // e.g. TPC.Digits.root -> TPC.DigitsMerged.root
  // made on Jiri Chudoba demand
  //
  IlcDebug(1, Form("suffix=%s",suffix.Data()));
  IlcDebug(1, Form("   Digits File Name before: %s",fFileName.Data()));
  
  static const TString dotroot(".root");
  const TString& suffixdotroot = suffix + dotroot;
  fFileName = fFileName.ReplaceAll(dotroot,suffixdotroot);
  
  IlcDebug(1, Form("                    after : %s",fFileName.Data()));
}

//______________________________________________________________________________
Bool_t IlcDataLoader::CheckReload()
{
  //
  // Checks if we have to reload given file
  //
  if (fFile == 0x0) return kFALSE;
  TString tmp = SetFileOffset(fFileName);
  if (tmp.CompareTo(fFile->GetName())) return kTRUE;  //file must be reloaded
  return  kFALSE;
}

//______________________________________________________________________________
const TString IlcDataLoader::SetFileOffset(const TString& fname)
{
  //
  // Return fname
  //
  Long_t offset = (Long_t)GetRunLoader()->GetFileOffset();
  if (fNEventsPerFile > 0) {
    offset = GetRunLoader()->GetEventNumber()/fNEventsPerFile;
  }
  if (offset < 1) return fname;
  
  TString soffset;
  soffset += offset;//automatic conversion to string
  TString dotroot(".root");
  const TString& offfsetdotroot = offset + dotroot;
  TString out = fname;
  out = out.ReplaceAll(dotroot,offfsetdotroot);
  IlcDebug(1, Form("in=%s  out=%s.",fname.Data(),out.Data()));
  return out;
}

//______________________________________________________________________________
void IlcDataLoader::SetFileOption(Option_t* newopt)
{
  //
  // Sets file option
  //
  if (fFileOption.CompareTo(newopt) == 0) return;
  fFileOption = newopt;
  Reload();
}

//______________________________________________________________________________
void IlcDataLoader::SetCompressionLevel(Int_t cl)
{
  //
  // Sets comression level for data defined by di
  //
  fCompressionLevel = cl;
  if (fFile) fFile->SetCompressionLevel(cl);
}

//______________________________________________________________________________
void IlcDataLoader::MakeTree()
{
  //
  // Makes tree for the current data loader
  //
  IlcTreeLoader* tl = dynamic_cast<IlcTreeLoader*>(fBaseLoaders->At(0));
  if (tl == 0x0)
   {
     IlcError("Can not make a tree because main base loader is not a tree loader");
     return;
   }
  tl->MakeTree();
}

//______________________________________________________________________________
Bool_t IlcDataLoader::IsFileWritable() const
{
  //
  // Returns true if file is writable
  //
  return (fFile)?fFile->IsWritable():kFALSE;
}

//______________________________________________________________________________
Bool_t IlcDataLoader::IsFileOpen() const
{
  //
  // Returns true if file is writable
  //
  return (fFile)?fFile->IsOpen():kFALSE;
}

//______________________________________________________________________________
Bool_t IlcDataLoader::IsOptionContrary(const TString& option) const
{
  // Checks if passed option is contrary with file open option 
  // which is passed option "writable" and existing option not wriable
  // in reverse case it is no harm so it is NOT contrary
  if (fFile == 0x0) return kFALSE; //file is not opened - no problem
  
  if ( ( IlcLoader::IsOptionWritable(option)      == kTRUE  ) &&     // passed option is writable and 
       ( IlcLoader::IsOptionWritable(fFileOption) == kFALSE )    )   // existing one is not writable
    {
      return kTRUE;
    }
  
  return kFALSE;
}


//______________________________________________________________________________
void IlcDataLoader::AddBaseLoader(IlcBaseLoader* bl)
{
  //Adds a base loader to lits of base loaders managed by this data loader
  //Managed data will be stored in proper root directory,
  //and posted to 
  // - in case of tree/object - data folder connected with detector associated with this data loader
  
  if (bl == 0x0)
    {
      IlcWarning("Pointer is null.");
      return;
    }
  
  TObject* obj = fBaseLoaders->FindObject(bl->GetName());
  if (obj)
    {
      IlcError("Can not add this base loader.");
      IlcError(Form("There exists already base loader which manages data named %s for this detector.",obj->GetName()));
      return;
    }
 
  fBaseLoaders->Add(bl);
}

//______________________________________________________________________________
IlcBaseLoader* IlcDataLoader::GetBaseLoader(const TString& name) const
{
  //
  // Return pointer to base loader
  //
  return dynamic_cast<IlcBaseLoader*>(fBaseLoaders->FindObject(name));
}

//______________________________________________________________________________
IlcBaseLoader* IlcDataLoader::GetBaseLoader(Int_t n) const
{
  //
  // Gets the n-th base loader (what is n?)
  //
  return dynamic_cast<IlcBaseLoader*>(fBaseLoaders->At(n));
}

//______________________________________________________________________________
TTree* IlcDataLoader::Tree() const
{
  // Returns tree from the main base loader
  // it is just shortcut method for comfort of user
  // main storage object does not have to be Tree  - 
  // that is why first we need to check if it is a TreeLoader 
  IlcTreeLoader* tl = dynamic_cast<IlcTreeLoader*>(GetBaseLoader(0));
  if (tl == 0x0) return 0x0;
  return tl->Tree();
}

//______________________________________________________________________________
void  IlcDataLoader::SetDirName(TString& dirname)
{
  //
  // Sets the directory name where the files will be stored
  //
  IlcDebug(10, Form("FileName before %s",fFileName.Data()));
  Int_t n = fFileName.Last('/');
  IlcDebug(10, Form("Slash found on pos %d",n));
  if (n > 0) fFileName = fFileName.Remove(0,n+1);
  IlcDebug(10, Form("Core FileName %s",fFileName.Data()));
  fFileName = dirname + fFileName;
  IlcDebug(10, Form("FileName after %s",fFileName.Data()));
}

//______________________________________________________________________________
IlcObjectLoader* IlcDataLoader::GetBaseDataLoader()
{
  //
  // Gets the base data loader
  //
  return dynamic_cast<IlcObjectLoader*>(GetBaseLoader(kData));
}

//______________________________________________________________________________
void IlcDataLoader::SetBaseDataLoader(IlcBaseLoader* bl)
{
  //
  // Sets data base loader
  //
  if (bl == 0x0)
    {
      IlcError("Parameter is null");
      return;
    }
  if (GetBaseDataLoader()) delete GetBaseDataLoader();
  fBaseLoaders->AddAt(bl,kData);
}

//______________________________________________________________________________
void IlcDataLoader::Synchronize()
{
  //
  // Synchronizes all writable files 
  //
  if ( fFile ) fFile->Flush();
}



