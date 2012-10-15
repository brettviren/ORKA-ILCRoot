// $Id: IlcEveEventManager.h 58244 2012-08-21 12:57:19Z hristov $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

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

#ifndef IlcEveEventManager_H
#define IlcEveEventManager_H

#include <TEveEventManager.h>
#include <TQObject.h>
#include <TObjArray.h>

class IlcEveMacroExecutor;
class IlcEveEventSelector; 

class IlcRunLoader;
class IlcESDEvent;
class IlcESDfriend;
class IlcAODEvent;
class IlcRawReader;

class IlcGRPObject;
class IlcMagF;

class TEveElement;
class TFile;
class TTree;
class TGeoManager;
class TString;
class TMap;

//==============================================================================
//
// IlcEveEventManager
//
// Interface to ILC event-data (RunLoader, ESD), magnetic field and
// geometry.
//


class IlcEveEventManager : public TEveEventManager,
                           public TQObject
{
public:
  static void SetESDFileName(const TString& esd);
  static void SetAODFileName(const TString& aod);
  static void AddAODfriend  (const TString& friendFileName);
  static void SetRawFileName(const TString& raw);
  static void SetCdbUri     (const TString& cdb);
  static void SetAssertElements(Bool_t assertRunloader, Bool_t assertEsd,
				Bool_t assertAod, Bool_t assertRaw);
  static void SearchRawForCentralReconstruction();

  IlcEveEventManager(const TString& name="Event");
  IlcEveEventManager(const TString& name, const TString& path, Int_t ev=0);
  virtual ~IlcEveEventManager();

  virtual void  Open();
  void          SetEvent(IlcRunLoader *runLoader, IlcRawReader *rawReader, IlcESDEvent *esd, IlcESDfriend *esdf);
  virtual Int_t GetMaxEventId(Bool_t refreshESD=kFALSE) const;
  virtual void  GotoEvent(Int_t event);
  virtual void  NextEvent();
  virtual void  PrevEvent(); 
  virtual void  Close();
  void Timeout(); // * SIGNAL*

  Int_t         GetEventId()         const { return fEventId; }
  IlcRunLoader* GetRunLoader()       const { return fRunLoader; }
  TFile*        GetESDFile()         const { return fESDFile; }
  TTree*        GetESDTree()         const { return fESDTree; }
  IlcESDEvent*  GetESD()             const { return fESD;     }
  IlcESDfriend* GetESDfriend()       const { return fESDfriend; }
  Bool_t        GetESDfriendExists() const { return fESDfriendExists; }
  TFile*        GetAODFile()         const { return fAODFile; }
  TTree*        GetAODTree()         const { return fAODTree; }
  IlcAODEvent*  GetAOD()             const { return fAOD;     }
  virtual const Text_t* GetTitle()   const { return fPath.Data(); }
  IlcEveEventSelector* GetEventSelector() const { return fPEventSelector; }
  TString       GetEventInfoHorizontal() const;
  TString       GetEventInfoVertical()   const;

  static Int_t  CurrentEventId();

  static Bool_t HasRunLoader();
  static Bool_t HasESD();
  static Bool_t HasESDfriend();
  static Bool_t HasAOD();
  static Bool_t HasRawReader();

  static IlcRunLoader* AssertRunLoader();
  static IlcESDEvent*  AssertESD();
  static IlcESDfriend* AssertESDfriend();
  static IlcAODEvent*  AssertAOD();
  static IlcRawReader* AssertRawReader();

  static IlcMagF*      AssertMagField();
  static TGeoManager*  AssertGeometry();

  static IlcEveEventManager* AddDependentManager(const TString& name, const TString& path);
  static IlcEveEventManager* GetDependentManager(const TString& name);

  static IlcEveEventManager* GetMaster();
  static IlcEveEventManager* GetCurrent();

  static void                RegisterTransient    (TEveElement* element);
  static void                RegisterTransientList(TEveElement* element);


  Double_t      GetAutoLoadTime()        const { return fAutoLoadTime; }
  Bool_t        GetAutoLoad()            const { return fAutoLoad;     }
  void          SetAutoLoadTime(Float_t time);
  void          SetAutoLoad(Bool_t autoLoad);
  void          SetTrigSel(Int_t trig);
  void          AutoLoadNextEvent();

  Bool_t        AreEventFilesOpened()    const { return fIsOpen;       }
  Bool_t        IsEventAvailable()       const { return fHasEvent;     }
  Bool_t        IsUnderExternalControl() const { return fExternalCtrl; }
  
  Bool_t        InsertGlobal(const TString& tag, TEveElement* model);
  Bool_t        InsertGlobal(const TString& tag, TEveElement* model,
                             Bool_t replace, Bool_t update);
  TEveElement*  FindGlobal(const TString& tag);

  virtual void  AfterNewEventLoaded();
  void          NewEventDataLoaded();  // *SIGNAL*
  void          NewEventLoaded();      // *SIGNAL*

  IlcEveMacroExecutor* GetExecutor() const { return fExecutor; }

protected:
  TString       fPath;			// URL to event-data.
  Int_t         fEventId;		// Id of current event.

  IlcRunLoader* fRunLoader;		// Run loader.

  TFile        *fESDFile;		// ESD file.
  TTree        *fESDTree;		// ESD tree.
  IlcESDEvent  *fESD;			// ESDEvent object.
  IlcESDfriend *fESDfriend;		// ESDfriend object.
  Bool_t        fESDfriendExists;	// Flag specifying if ESDfriend was found during opening of the event-data.
  TFile        *fAODFile;		// AOD file.
  TTree        *fAODTree;		// AOD tree.
  IlcAODEvent  *fAOD;			// AODEvent object.

  IlcRawReader *fRawReader;             // Raw-data reader.

  Bool_t        fAutoLoad;              // Automatic loading of events (online)
  Float_t       fAutoLoadTime;          // Auto-load time in seconds
  TTimer       *fAutoLoadTimer;         // Timer for automatic event loading

  Bool_t        fIsOpen;                // Are event-files opened.
  Bool_t        fHasEvent;              // Is an event available.
  Bool_t        fExternalCtrl;          // Are we under external event-loop.

  TMap*         fGlobal;
  Bool_t        fGlobalReplace;         // Are global replace
  Bool_t        fGlobalUpdate;          // Are global updates

  IlcEveMacroExecutor *fExecutor;       // Executor for std macros

  TEveElementList     *fTransients;     // Container for additional transient (per event) elements.
  TEveElementList     *fTransientLists; // Container for lists of transient (per event) elements.

  IlcEveEventSelector* fPEventSelector; // Event filter

  TList        *fSubManagers;           // Dependent event-managers, used for event embedding.

  static TString  fgESDFileName;        // Name by which to open ESD.
  static TString  fgAODFileName;        // Name by which to open AOD.
  static TString  fgRawFileName;        // Name by which to open raw-data file.
  static TString  fgCdbUri;		// Global URI to CDB.
  static Bool_t   fgAssertRunLoader;	// Global flag specifying if IlcRunLoader must be asserted during opening of the event-data.
  static Bool_t   fgAssertESD;		// Global flag specifying if ESDEvent must be asserted during opening of the event-data.
  static Bool_t   fgAssertAOD;		// Global flag specifying if AODEvent must be asserted during opening of the event-data.
  static Bool_t   fgAssertRaw;		// Global flag specifying if raw-data presence must be asserted during opening of the event-data.

  static TList   *fgAODfriends;         // Global list of AOD friend names to be attached during opening of the event-data (empty by default).

  static Bool_t   fgRawFromStandardLoc; // Global flag to enable looking for raw data in ../../../raw/, as it is stored for central reco.

  static Bool_t        fgGRPLoaded;     // Global run parameters loaded?
  static IlcMagF      *fgMagField;      // Global pointer to magnetic field.
  static Bool_t        fgUniformField;  // Track with uniform field.

private:
  IlcEveEventManager(const IlcEveEventManager&);            // Not implemented
  IlcEveEventManager& operator=(const IlcEveEventManager&); // Not implemented

  void InitInternals();
  void StartAutoLoadTimer();
  void StopAutoLoadTimer();

  static Bool_t InitGRP();

  Bool_t fAutoLoadTimerRunning; // State of auto-load timer.

  static IlcEveEventManager* fgMaster;
  static IlcEveEventManager* fgCurrent;

  ClassDef(IlcEveEventManager, 0); // Interface for getting all event components in a uniform way.
};

#endif
