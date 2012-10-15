#ifndef ILCDATALOADER_H
#define ILCDATALOADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDataLoader.h 52261 2011-10-23 15:46:57Z hristov $ */

//__________________________________________
////////////////////////////////////////////
//                                        //
//  class IlcDataLoader                   //
//                                        //
//  Loader responsible for one data type  //
//  i.e. Hits, Kine, etc.                 //
//  many objects type can be assciated    //
//  with one data type: storing object    //
//  (usually tree)                        //
//                                        //
////////////////////////////////////////////

class TTree;
class TFile;
class TFolder;

class IlcBaseLoader;
class IlcObjectLoader;
class IlcRunLoader;

#include <TDirectory.h>
#include <TNamed.h>
#include <TString.h>

class IlcDataLoader: public TNamed
 {
  public:
   IlcDataLoader();
   IlcDataLoader(const char* filename, const char* contname, const char* name, Option_t* opt = "t");
   virtual ~IlcDataLoader();

   virtual Int_t      SetEvent();
   virtual Int_t      GetEvent();

   //shrtcuts method to basic data base loader 0  
   virtual Int_t      Load(Option_t* opt="");
   virtual void       Unload();
   virtual Int_t      Reload(); 
   virtual Int_t      WriteData(Option_t* opt="");
   virtual TTree*     Tree() const;
   virtual void       Clean();
   virtual void       MakeTree();
   virtual Int_t      OpenFile(Option_t* opt);

   virtual void       CloseFile();
   void               UnloadAll();
   void               CleanAll();
   const TString&     GetFileName() const {return fFileName;}
   TFile*             GetFile() const {return fFile;}
   TDirectory*        GetDirectory() const {return fDirectory;}
   const TString&     GetFileOption() const {return fFileOption;}
   const Int_t&       GetCompressionLevel() const {return fCompressionLevel;}
   
   Bool_t             Cd(){return (fDirectory)?fDirectory->cd():kFALSE;}
   
   virtual void       SetFileName(const TString& filename){fFileName = filename;}
   virtual void       SetFileOption(const Option_t* fileopt);
   virtual void       SetCompressionLevel(Int_t cl);

   Int_t              SetEventFolder(TFolder* eventfolder);//sets the event folder
   Int_t              SetFolder(TFolder* folder);//sets the data folder ??????
   TFolder*           GetEventFolder();
   TFolder*           GetFolder() const {return fFolder;}
   
//   TObject*           GetFromDirectory(const char *name){return (fDirectory)?fDirectory->Get(name):0x0;}
   void               SetFileNameSuffix(const TString& suffix);//adds the suffix before ".root", 
                                                               //e.g. TPC.Digits.root -> TPC.DigitsMerged.root
                                                               //made on Jiri Chudoba demand
   void               SetNumberOfEventsPerFile(Int_t nevpf) 
     {fNEventsPerFile = nevpf;}
   const TString      SetFileOffset(const TString& fname);//adds the proper number before .root extension suffix
   void               SetDirName(TString& dirname);

   void               AddBaseLoader(IlcBaseLoader* bl);
   enum EStdBasicLoaders {kData = 0};//standard basic loaders identifiers

   IlcBaseLoader*     GetBaseLoader(const TString& name) const;
   IlcBaseLoader*     GetBaseLoader(Int_t n) const;
   IlcObjectLoader*   GetBaseDataLoader();
   
   void               SetBaseDataLoader(IlcBaseLoader* bl);
   
   Bool_t             CheckReload();//checks if we have to reload given file
   Bool_t             IsFileWritable() const;
   Bool_t             IsFileOpen() const;
   Bool_t             IsOptionContrary(const TString& option) const;
   
   void Synchronize();

  protected:
   IlcRunLoader*      GetRunLoader();//gets the run-loader from event folder

  private:
   IlcDataLoader(const IlcDataLoader&); //Not implemented
   IlcDataLoader& operator=(const IlcDataLoader&); //Not implemented
      
   TString      fFileName; //name of the file 
   TFile*       fFile; //! pointer to file 
   TDirectory*  fDirectory; //!pointer to TDirectory
   TString      fFileOption; //!file option while opened 
   Int_t        fCompressionLevel; //Compression Level of File
   Int_t        fNEventsPerFile;  //defines number of events stored per one file
   
   TObjArray*   fBaseLoaders;//base loaders

   TFolder*     fEventFolder;//!event folder
   TFolder*     fFolder;//! folder with data
   
   ClassDef(IlcDataLoader,3)
 };

#endif


