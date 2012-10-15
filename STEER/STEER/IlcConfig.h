#ifndef ILCCONFIG_H
#define ILCCONFIG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcConfig.h 52261 2011-10-23 15:46:57Z hristov $ */

//
// Class to name the folders
// This class contains 
// the names of the top level folders
// 

//
// Class to name the folders
// This class contains 
// the names of the top level folders
// 

#include <TNamed.h>
class TDatabasePDG;
class TFolder;
class TString;
class TVirtualMC;

class IlcConfig;
class IlcDetector;
class IlcGenerator;
class IlcModule;

class IlcConfig : public TNamed {
  
public:
  
  virtual ~ IlcConfig (); 

  void       Add(TDatabasePDG *pdg);
  
  void       Add(IlcGenerator *generator,const char* eventfolder = fgkDefaultEventFolderName);
  void       Add (TVirtualMC *mc,const char* eventfolder = fgkDefaultEventFolderName);
  void       Add (IlcModule *module,const char* eventfolder = fgkDefaultEventFolderName);
  void       Add (IlcDetector *detector,const char* eventfolder = fgkDefaultEventFolderName);

  Int_t      AddDetector(const char* evntfoldername,const char *name, const char* title);
  Int_t      AddDetector(TFolder* evntfolder,const char *name, const char* title);
  
  Int_t      CreateDetectorFolders(const char* evntfoldername,const char *name, const char* title);//Used by IlcRunGetter
  Int_t      CreateDetectorFolders(TFolder* evntfolder,const char *name, const char* title);//Used by IlcRunGetter
  
  static     IlcConfig* Instance();
  
  TFolder*              BuildEventFolder(const char* name,const char* tilte);
  
  TFolder*              GetTopFolder(){return fTopFolder;}
  TFolder*              GetConstFolder(){return fConstFolder;}

  static const TString& GetModulesFolderName(){return fgkModuleFolderName;}
  static const TString& GetDefaultEventFolderName()
    {return fgkDefaultEventFolderName;}
    
  const TString&        GetDataFolderName() const;//returns name of data folder

private:  
  IlcConfig(const IlcConfig&); // Not implemented
  IlcConfig& operator=(const IlcConfig&); // Not implemented
  IlcConfig(); // Not implemented
  IlcConfig(const char * name, const char * title );
  
  static const TString  fgkTopFolderName; //name of top IlcRoot folder
 
  static const TString  fgkDefaultEventFolderName; //name of event folder
  static const TString  fgkConstantsFolderName;    //name of constants folder
  
  static const TString  fgkDataFolderName;         //name of data folde
  static const TString  fgkConditionsFolderName;   //name of conditions folder
  static const TString  fgkConfigurationFolderName;//name of configuration foolder
  static const TString  fgkHeaderFolderName;       //name of header folder
    
  static const TString  fgkCalibrationFolderName;  //name of calibration folder
  static const TString  fgkAligmentFolderName;     //name of alignment folder
  
  static const TString  fgkFieldFolderName;        //name of magn.field folder
  static const TString  fgkGeneratorsFolderName;   //name of generator folder
  static const TString  fgkVirtualMCFolderName;    //name of virtual MC folder

  void          AddInFolder (const char * dir, TObject *obj);
  Int_t         AddSubFolder(TFolder* topfolder, const char* infoler, //helper method
                     const char* newfoldname, const char* newfoldtitle);
  TObject*      FindInFolder (const char *dir, const char *name);
  
  // folders
  TFolder*              fTopFolder;    //pointer to top folder
  TFolder*              fConstFolder;  //pointer to constants folder

  static const TString  fgkPDGFolderName; //name of PDG folder
  static const TString  fgkGeneratorFolderName; //name of generator name
  static const TString  fgkMCFolderName;        //name of MC folder
  static const TString  fgkModuleFolderName;    //name of module folder
  
  TString              *fDetectorFolder;//!array with names for detector folders (where detector is going to be put)
  
  static IlcConfig*     fgInstance; //pointer to the IlcConfig instance
  
  
  ClassDef(IlcConfig,3) //Configuration class for IlcRun
};				// end class IlcConfig

#endif
