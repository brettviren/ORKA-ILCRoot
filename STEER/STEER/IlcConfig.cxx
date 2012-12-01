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

/* $Id: IlcConfig.cxx 52261 2011-10-23 15:46:57Z hristov $ */

//  Description:
//  This class is responsible for creating folder structure
//  All stuff of ilcroot sits in one folder with name defined by
//  fgkTopFolderName data wich do not very trough event to event are
//  sitting in directly in "top folder" all data which changes from
//  event to event are sitting in one folder (which has more subfolders)
//  Idea is to have more than one event in folder structure which allows
//  usage of standard procedures in merging
//  Add(IlcDetector*) calls Add(IlcModule*) as IlcDetector is a IlcModule
//  as well and should be listed in module list

#include <TDatabasePDG.h>
#include <TFolder.h>
#include <TInterpreter.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <TVirtualMC.h>

#include "IlcConfig.h"
#include "IlcDetector.h"
#include "IlcGenerator.h" 
#include "IlcLoader.h"
#include "IlcLog.h"

enum
 {
   kDetFolderData = 0,
   kDetFolderCalibration,
   kDetFolderAligmnet,
   kDetFolderLast
 };
ClassImp(IlcConfig)

IlcConfig* IlcConfig::fgInstance = 0;

//0 level folder
const TString IlcConfig::fgkTopFolderName("Folders");

//1st level folder
const TString IlcConfig::fgkConstantsFolderName("Constants");
const TString IlcConfig::fgkDefaultEventFolderName("Event");  //default folder for event, always used except merging

//2st level folder
//subfolder of event folder
const TString IlcConfig::fgkDataFolderName("Data");//folder for data (hits, digits, points, tracks) grouped by detectors
const TString IlcConfig::fgkModuleFolderName("Modules");//folder with modules objects
const TString IlcConfig::fgkConditionsFolderName("Conditions");//folder with conditions (mag. field etc.)
const TString IlcConfig::fgkConfigurationFolderName("Configuration");//folder with configuration (setup) of the detector
const TString IlcConfig::fgkHeaderFolderName("Header");//folder with header and other MC information

//3rd level folder
//fgkConditionsFolderName subfolders
const TString IlcConfig::fgkCalibrationFolderName("Calibration");
const TString IlcConfig::fgkAligmentFolderName("Aligment");
  
//3rd level folder
//fgkConfigurationFolderName subfolders
const TString IlcConfig::fgkFieldFolderName("Field");
const TString IlcConfig::fgkGeneratorsFolderName("Generators");
const TString IlcConfig::fgkVirtualMCFolderName("VirtualMC");


const TString IlcConfig::fgkPDGFolderName("Constants/DatabasePDG");//folder with PDG Database
const TString IlcConfig::fgkGeneratorFolderName("Configuration/Generators");//folder with generators
const TString IlcConfig::fgkMCFolderName("Configuration/VirtualMC");

//_____________________________________________________________________________
#ifdef WIN32
  const TString& IlcConfig::GetDefaultEventFolderName()
    {return fgkDefaultEventFolderName;}
#endif

//____________________________________________________________________________
IlcConfig* IlcConfig::Instance ()
{
  //
  // Instance method for singleton class
  //
   if(fgInstance == 0) 
    {
     fgInstance = new IlcConfig (fgkTopFolderName,"Ilc data exchange board");
    }
   return fgInstance;
}
//____________________________________________________________________________

IlcConfig::IlcConfig(const char *name, const char *title): 
  TNamed(name,title), 
  fTopFolder(gROOT->GetRootFolder()->AddFolder(name,title)),
  fConstFolder(0x0),
  fDetectorFolder(new TString[kDetFolderLast+1])
{
// Constructor

  //Main IlcRoot Folder
  if (fTopFolder == 0x0)
   {
     IlcFatal("Can not create Top Ilc Folder.");
     return;//never reached
   }
  fTopFolder->SetOwner();
  
  fDetectorFolder[kDetFolderData] = fgkDataFolderName;
  fDetectorFolder[kDetFolderCalibration] = fgkConditionsFolderName+"/"+fgkCalibrationFolderName;
  fDetectorFolder[kDetFolderAligmnet] = fgkConditionsFolderName+"/"+fgkAligmentFolderName;
  fDetectorFolder[kDetFolderLast] = "";
  
  gROOT->GetListOfBrowsables()->Add(fTopFolder, name);

  //Constants folder
  fConstFolder = fTopFolder->AddFolder (fgkConstantsFolderName, "Constant parameters");
  fConstFolder->AddFolder("DatabasePDG", "PDG database");
  
  fgInstance=this;
}

//____________________________________________________________________________
IlcConfig::~IlcConfig()
{ 
  // destructor
  delete [] fDetectorFolder ;  
  if (fTopFolder)
   {
    fTopFolder->SetOwner();
    delete fTopFolder; 
   }
}
//____________________________________________________________________________

void IlcConfig::AddInFolder (const char *dir, TObject *obj)
{
  // Adds object "obj" to folder "dir"
  TFolder *folder = dynamic_cast<TFolder *>(fTopFolder->FindObject(dir));
  if (folder)
    folder->Add (static_cast<TObject *>(obj));
}

//____________________________________________________________________________
TObject* IlcConfig::FindInFolder (const char *dir, const char *name)
{
  // Finds object with name "name" in folder "dir"
  if(!name) return(fTopFolder->FindObject(name));
  TFolder * folder = dynamic_cast<TFolder *>(fTopFolder->FindObject(dir));
  if (!folder) return (NULL);
  return(folder->FindObject(name));
}

//____________________________________________________________________________
void    IlcConfig::Add (IlcGenerator * obj,const char* eventfolder)
{
  // Adds generator "obj" to the event folder "eventfolder"
  TString path(eventfolder);
  path = path + "/" + fgkGeneratorsFolderName;
  AddInFolder(path,obj);
}

//____________________________________________________________________________
void IlcConfig::Add (TVirtualMC * obj,const char* eventfolder)
{
  // Adds TVirtualMC object to the event folder
  TString path(eventfolder);
  path = path + "/" + fgkMCFolderName;
  AddInFolder(path, obj);
}

//____________________________________________________________________________
void  IlcConfig::Add (TDatabasePDG * obj)
{
  // Adds TDataBase object
  AddInFolder(fgkPDGFolderName, obj);
}

//____________________________________________________________________________
void IlcConfig::Add(IlcModule* obj,const char* eventfolder)
{
  // Adds module to the event folder
  TString path(eventfolder);
  path = path + "/" + fgkModuleFolderName;
  IlcDebug(1, Form("module name = %s, Ev. Fold. Name is %s.",
		   obj->GetName(),eventfolder));
  AddInFolder(path, obj);
}
//____________________________________________________________________________

Int_t IlcConfig::AddDetector(TFolder* evntfolder, const char *name, const char* title)
{
//creates folders for the detector 'name'
 Int_t retval;//returned value
 retval = CreateDetectorFolders(evntfolder,name,title);
 if (retval)
  {
    IlcError(Form("CreateDetectorFolders returned error for detector %s",name));
    return retval;
  }
 return 0; 
}
//____________________________________________________________________________

Int_t IlcConfig::AddDetector(const char* evntfoldername,const char *name, const char* title)
{
//creates folders for the detector 'name'
 Int_t retval;//returned value
 retval = CreateDetectorFolders(evntfoldername,name,title);
 if (retval)
  {
    IlcError(Form("CreateDetectorFolders returned error for detector %s",name));
    return retval;
  }
 return 0; 
}
//____________________________________________________________________________

void  IlcConfig::Add(IlcDetector * obj,const char* eventfolder)
{
  // Adds new IlcDetector objest to the correspondent event folder
  IlcDebug(1, Form("detector name = %s, Ev. Fold. Name is %s.",
		   obj->GetName(),eventfolder));

  TObject* foundobj = GetTopFolder()->FindObject(eventfolder);
  TFolder* evfolder = (foundobj)?dynamic_cast<TFolder*>(foundobj):0x0;
  if (evfolder == 0x0)
   {
     IlcFatal(Form("Can not find folder %s while adding detector %s",eventfolder,obj->GetName()));
     return;
   } 
  CreateDetectorFolders(evfolder, obj->GetName(), obj->GetTitle());
  
}
//____________________________________________________________________________

Int_t  IlcConfig::CreateDetectorFolders(const char* evntfoldername,const char *name, const char* title)
{
//creates a folders for detector named 'name' and titled 'title'
//in a event folder named 'evntfoldername'
//list of folder names where new folders are created is defined in fDetectorFolder array 
//detector folders are named 'name' and titled 'title' as well

 TFolder* evfolder = dynamic_cast<TFolder*>(GetTopFolder()->FindObject(evntfoldername));
 if (evfolder == 0x0)
  {
   IlcError(Form("Can not find folder %s while adding detector %s",evntfoldername,name));
   return 1;
  }
 return CreateDetectorFolders(evfolder,name,title);
}
//____________________________________________________________________________
Int_t  IlcConfig::CreateDetectorFolders(TFolder* evntfolder,const char *name, const char* title)
{
//creates a folders for detector named 'name' and titled 'title'
//in a event folder 'evntfolder'
//list of folder names where new folders are created is defined in fDetectorFolder array 
//detector folders are named 'name' and titled 'title' as well
//Here we add only detector not an modules
 
 Int_t tmp;
 Int_t i = 0;//iterator
 while(!fDetectorFolder[i].IsNull())
  {
    tmp = AddSubFolder(evntfolder,fDetectorFolder[i],name,title);
    if (tmp)
     {
      IlcError(Form("Failed to create subfolder of %s for detector %s",fDetectorFolder[i].Data(),name));
      return 1;
     }
    i++;
  }
 return 0;
}

/*****************************************************************************/

TFolder* IlcConfig::BuildEventFolder(const char* name,const char* title)
{
/*
 creates the folder structure for one event
 TopFolder
         |_
         | \
         |  Constants
         |_
         | \
         |  Event_
         |      | \
         |      |  Modules(detector objects)
         |      |_
         |      | \              
         |      |  Header
         |      |_
         |      | \              
         |      |  Data_
         |      |     | \ 
         |      |     |  TPC_
         |      |     |    | \
         |      |     |    |  Hits(object;e.g. tree)
         |      |     |    |_  
         |      |     |    | \ 
         |      |     |    |  SDigits(object)
         |      |     |    |_
         |      |     |    | \ 
         |      |     |    |  Digits(object)
         |      |     |    |_
         |      |     |    | \ 
         |      |     |    |  RecPoints(object)
         |      |     |    |_
         |      |     |      \ 
         |      |     |       Tracks(object)
         |      |     |_ 
         |      |       \
         |      |        ITS_
         |      |          | \
         |      |          |  Hits(object;e.g. tree)
         |      |          |_  
         |      |          | \ 
         |      |          |  SDigits(object)
         |      |          |_
         |      |          | \ 
         |      |          |  Digits(object)
         |      |          |_
         |      |          | \ 
         |      |          |  RecPoints(object)
         |      |          |_
         |      |            \ 
         |      |             Tracks(object)
         |      |_         
         |        \       
         |         Configuration
         |               
         |_
           \
            Event2_  (to be merged with event)
                |  \
                |   Modules(detector objects)
                |_
                | \              
                |  Header
                |_
                | \              
                |  Data_
                |     | \ 
                |     |  TPC_
                |     |    | \
                |     |    |  Hits(object;e.g. tree)
                |     |    |_  
                |     |    | \ 
                |     |    |  SDigits(object)
                |     |    |_
                |     |    | \ 
                |     |    |  Digits(object)
                |     |    |_
                |     |    | \ 
                |     |    |  RecPoints(object)
                |     |    |_
                |     |      \ 
                |     |       Tracks(object)
                |     |_ 
                |       \
                |        ITS_
                |          | \
                |          |  Hits(object;e.g. tree)
                |          |_  
                |          | \ 
                |          |  SDigits(object)
                |          |_
                |          | \ 
                |          |  Digits(object)
                |          |_
                |          | \ 
                |          |  RecPoints(object)
                |          |_
                |            \ 
                |             Tracks(object)
                |_         
                  \       
                   Configuration
                         
*/
  TFolder* eventfolder = fTopFolder->AddFolder(name,title); 
   
  //modules
  eventfolder->AddFolder(fgkModuleFolderName, "Detector objects");
  //event data
  eventfolder->AddFolder(fgkDataFolderName, "Detector data");
  
    //Conditions
  TFolder *conditions = eventfolder->AddFolder(fgkConditionsFolderName, "Run conditions");
  conditions->AddFolder(fgkCalibrationFolderName,"Detector calibration data");
  conditions->AddFolder(fgkAligmentFolderName,"Detector aligment");
  //Configuration
  TFolder *configuration = eventfolder->AddFolder(fgkConfigurationFolderName, "Run configuration");
  configuration->AddFolder(fgkFieldFolderName, "Magnetic field maps");
  configuration->AddFolder(fgkGeneratorsFolderName,"list of generator objects");
  //PH configuration->AddFolder(fgkVirtualMCFolderName,"the Virtual MC");

  eventfolder->AddFolder(fgkHeaderFolderName,"MonteCarlo event header");

  eventfolder->SetOwner();

  return eventfolder;
}

/*****************************************************************************/

const TString& IlcConfig::GetDataFolderName() const
{
//returns name of data folder path relative to event folder
 return fgkDataFolderName;
}

/*****************************************************************************/

Int_t IlcConfig::AddSubFolder(TFolder* topfolder, const char* infoler, 
                     const char* newfoldname, const char* newfoldtitle)
{
//helper method
//in topfolder looks for folder named 'infolder'
//and if it exist it creates inside a new folder named 'newfoldname' and titled 'newfoldtitle'

 if (topfolder == 0x0)//check if exists top folder
  {
   IlcError("Parameter TFolder* points to NULL.");
   return 1;
  }
 
 TObject *obj;
 TFolder* folder;
 
 folder = dynamic_cast<TFolder*>(topfolder->FindObject(infoler));
 if (folder == 0x0) //check if we got inolder
  {
   IlcError(Form("Can not find folder %s in folder %s.",infoler,topfolder->GetName()));
   return 1;
  }
 obj = folder->FindObject(newfoldname); //see if such a subfolder already exists
 if (obj == 0x0) //nope
  {
   TFolder *newfolder = folder->AddFolder(newfoldname,newfoldtitle);//add the desired subfolder
   if (newfolder == 0x0) //check if we managed
    {
     IlcError(Form("Can not create folder %s in folder %s",newfoldname,infoler));
     return 2;
    }
   else return 0;//success
  }
 //such an object already exists
 TFolder* fol = dynamic_cast<TFolder*>(obj);
 if (fol == 0x0)
   {
     IlcError(Form("Object named %s already exists in folder %s AND IT IS NOT A FOLDER",newfoldname,infoler));
     return 3;
   }
 IlcWarning(Form("Folder named %s already exists in folder %s",newfoldname,infoler));
 return 0;
}
