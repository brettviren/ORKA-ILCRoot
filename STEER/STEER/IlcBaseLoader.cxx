/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//  class IlcBaseLoader                                                                    //
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
//       - task DtectorName+Name defined                                                   //
//                                                                                         //
//  EStdBasicLoaders   idx     Object Type        Description                              //
//      kData           0    TTree or TObject     main data itself (hits,digits,...)       //
//      kTask           1        TTask            object producing main data               //
//      kQA             2        TTree                quality assurance tree               //
//      kQATask         3        TTask            task producing QA object                 //
//                                                                                         //
//                                                                                         //
//  User can define and add more basic loaders even Run Time.                              //
//  Caution: in order to save information about added base loader                          //
//  user must rewrite Run Loader to gilc.file, overwriting old setup                     //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

/* $Id: IlcBaseLoader.cxx 50615 2011-07-16 23:19:19Z hristov $ */

#include "IlcBaseLoader.h"
#include "IlcTreeLoader.h"
#include "IlcDataLoader.h"
#include "IlcLoader.h"
#include "IlcLog.h"

#include <TFile.h>
#include <TString.h>

ClassImp(IlcBaseLoader)

//______________________________________________________________________________
IlcBaseLoader::IlcBaseLoader():
 fIsLoaded(kFALSE),
 fStoreInTopOfFile(kFALSE),
 fDoNotReload(kFALSE),
 fDataLoader(0x0)
{
  //
  // default constructor
  //
}

//______________________________________________________________________________
IlcBaseLoader::IlcBaseLoader(const TString& name,  IlcDataLoader* dl, Bool_t storeontop):
 TNamed(name,name+" Base Loader"),
 fIsLoaded(kFALSE),
 fStoreInTopOfFile(storeontop),
 fDoNotReload(storeontop),//if stored on top of file - this object is loaded ones pe
 fDataLoader(dl)
{
  //
  // constructor
  //
}

//______________________________________________________________________________
Int_t IlcBaseLoader::Load(Option_t* opt)
{
  //
  // Loads and posts the data
  //
  IlcDebug(1, Form("data type = %s, option = %s",GetName(),opt));
  
  if (Get())
    {
      IlcDebug(1,Form("Data <<%s>> are already loaded. Use ReloadData to force reload. Nothing done",GetName()));
      return 0;
    }
  
  Int_t retval;
  
  if (GetDataLoader()->IsFileOpen() == kTRUE)
    {
      if (GetDataLoader()->IsOptionContrary(opt) == kTRUE)
	{
	  IlcError(Form("Data Type %s, Container Name %s", GetDataLoader()->GetName(),GetName()));
	  IlcError("File was already opened in READ-ONLY mode, while now WRITEABLE access is requested.");
	  IlcError("Use IlcDataLoader::SetOption to enforce change of access mode OR");
	  IlcError("Load previosly loaded data with coherent option.");
	  return 10;
	}
    }
  else
    {
      retval = GetDataLoader()->OpenFile(opt);
      if (retval) 
	{
	  IlcError(Form("Error occured while opening <<%s>> file",GetName()));
	  return retval;
	}
    }
  //if file is recreated there is no sense to search for data to post and get Error message
  if (IlcLoader::TestFileOption(opt) == kFALSE)
    {
      IlcTreeLoader* tl = dynamic_cast<IlcTreeLoader*>(this);
      if (tl) tl->MakeTree();
      fIsLoaded = kTRUE;
      return 0;
    }
  
  retval = Post();
  if (retval)
    {
      IlcError(Form("Error occured while posting %s from file to folder.",GetName()));
      return retval;
    }
  
  fIsLoaded = kTRUE;
  return 0;
}

//______________________________________________________________________________
Int_t IlcBaseLoader::Post()
{
  //
  // Posts data container to proper folders
  //
  
  if ( GetDirectory() == 0x0)
    {
      IlcError(Form("%s directory is NULL. Load before.",GetDataLoader()->GetName()));
      return 2; 
    }
  
  TObject* data = GetFromDirectory(fName);
  if(data)
    {
      //if such an obejct already exists - remove it first
      return Post(data);
    }
  else
    {
      //check if file is in update mode
      Int_t fileupdate = GetDataLoader()->GetFileOption().CompareTo("update",TString::kIgnoreCase);
      if ( fileupdate == 0)
	{ //if it is, it is normal that there is no data yet
	  IlcDebug(1, Form("Can not find %s in file %s (file is opened in UPDATE mode).",
			   GetName(),GetDataLoader()->GetFile()->GetName()));
	}
      else
	{
	  IlcError(Form("Can not find %s in file %s", GetName(),GetDataLoader()->GetFile()->GetName()));
	  return 5;
	}
    }
  return 0;
}

//______________________________________________________________________________
Int_t IlcBaseLoader::Post(TObject* data)
{
  //
  // Posts data container to proper folders
  //
  if (data == 0x0)
    {
      IlcError("Pointer to object is NULL");
      return 1;
    }
  
  if ( fName.CompareTo(data->GetName()) != 0)
    {
      IlcFatal(Form("Object name is <<%s>> while <<%s>> expected",data->GetName(),GetName()));
      return -1;//pro forma
    }
  
  TObject* obj = Get();
  if (data == obj)
    {
      IlcWarning("This object was already posted.");
      return 0;
    }
  if (obj)
    {
      IlcWarning(Form("Object named %s already exitsts in data folder. Removing it",GetName()));
      Clean();
    }
  return AddToBoard(data);
}

//______________________________________________________________________________
Int_t IlcBaseLoader::WriteData(Option_t* opt)
{
  //
  // Writes data defined by di object
  // opt might be "OVERWRITE" in case of forcing overwriting
  //
  IlcDebug(1, Form("Writing %s container for %s data. Option is %s.",
		   GetName(),GetDataLoader()->GetName(),opt));
  
  TObject *data = Get();
  if(data == 0x0)
    {//did not get, nothing to write
      IlcWarning(Form("Tree named %s not found in folder. Nothing to write.",GetName()));
      return 0;
    }
  
  //check if file is opened
  if (GetDirectory() == 0x0)
    { 
      //if not try to open
      GetDataLoader()->SetFileOption("UPDATE");
      if (GetDataLoader()->OpenFile("UPDATE"))
	{  
	  //oops, can not open the file, give an error message and return error code
	  IlcError(Form("Can not open hits file. %s ARE NOT WRITTEN",GetName()));
	  return 1;
	}
    }
  
  if (GetDataLoader()->IsFileWritable() == kFALSE)
    {
      IlcError(Form("File %s is not writable",GetDataLoader()->GetFileName().Data()));
      return 2;
    }
  
  GetDirectory()->cd(); //set the proper directory active
  
  //see if hits container already exists in this (root) directory
  TObject* obj = GetFromDirectory(GetName());
  if (obj)
    { //if they exist, see if option OVERWRITE is used
      const char *oOverWrite = strstr(opt,"OVERWRITE");
      if(!oOverWrite)
	{//if it is not used -  give an error message and return an error code
	  IlcError("Tree already exisists. Use option \"OVERWRITE\" to overwrite previous data");
	  return 3;
	}
    }
  
  IlcDebug(1, Form("DataName = %s, opt = %s, data object name = %s",
                   GetName(),opt,data->GetName()));
  IlcDebug(1, Form("File Name = %s, Directory Name = %s Directory's File Name = %s",
                   GetDataLoader()->GetFile()->GetName(),GetDirectory()->GetName(),
                   GetDirectory()->GetFile()->GetName()));
  
  IlcDebug(1, "Writing data");
  data->Write(0,TObject::kOverwrite);
  
  fIsLoaded = kTRUE;  // Just to ensure flag is on. Object can be posted manually to folder structure, not using loader.
  
  return 0;
  
}

//______________________________________________________________________________
Int_t IlcBaseLoader::Reload()
{
  //
  // Unloads and loads datat again - if loaded before
  //
  if (IsLoaded())
    {
      Unload();
      return Load(GetDataLoader()->GetFileOption());
    }
  return 0;
}

//______________________________________________________________________________
void IlcBaseLoader::Clean()
{
  //
  // Removes objects from folder/task
  //
  IlcDebug(1, Form("%s %s",GetName(),GetDataLoader()->GetName()));
  TObject* obj = Get();
  if(obj)
    { 
      IlcDebug(1, Form("cleaning %s.",GetName()));
      RemoveFromBoard(obj);
      delete obj;
    }
}

//______________________________________________________________________________
void IlcBaseLoader::Unload()
{
  // Unloads data and closes the files
  Clean();
  fIsLoaded = kFALSE;
  GetDataLoader()->CloseFile();
}

//______________________________________________________________________________
IlcDataLoader* IlcBaseLoader::GetDataLoader() const
{
  //
  // Returns pointer to the data loader
  //
  if (fDataLoader == 0x0) 
    {
      IlcFatal("Pointer to Data Loader is NULL");
    }
  return fDataLoader;
}

//______________________________________________________________________________
TDirectory* IlcBaseLoader::GetDirectory() const
{
  //
  // returnd TDirectory where data are to be saved
  // if fStoreInTopOfFile flag is true - returns pointer to file
  //
  return (fStoreInTopOfFile)?GetDataLoader()->GetFile():GetDataLoader()->GetDirectory();
}



