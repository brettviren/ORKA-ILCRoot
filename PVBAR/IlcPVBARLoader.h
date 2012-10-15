#ifndef ILCPVBARLOADER_H
#define ILCPVBARLOADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARLoader.h 52261 2011-10-23 15:46:57Z hristov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.12  2006/08/25 16:00:53  kharlov
 * Compliance with Effective C++IlcPVBARHit.cxx
 *
 * Revision 1.11  2006/08/01 12:15:03  cvetan
 * Adding a constructor from TFolder. Needed by IlcReconstruction plugin scheme
 *
 * Revision 1.10  2005/05/28 14:19:04  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
//  A singleton that returns various objects 
//  Should be used on the analysis stage to avoid confusing between different
//  branches of reconstruction tree: e.g. reading RecPoints and TS made from 
//  another set of RecPoints.
// 
//  The objects are retrived from folders.  
//-- Author: Yves Schutz (SUBATECH) & Dmitri Peressounko (RRC KI & SUBATECH)
//    


// --- ROOT system ---
class TString ;
class TParticle ;
#include <TClonesArray.h>
#include <TTree.h>

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcRun.h"
#include "IlcLoader.h"
#include "IlcRunLoader.h"
#include "IlcPVBARClusterizer.h"
#include "IlcPVBARTrackSegmentMaker.h"
#include "IlcPVBARPID.h"
class IlcPVBAR ; 
class IlcPVBARHit ; 
class IlcPVBARDigit ; 
class IlcPVBAREmcRecPoint ; 
class IlcPVBARCpvRecPoint ; 
class IlcPVBARTrackSegment ;
class IlcPVBARRecParticle ;  
class IlcPVBARSDigitizer ; 
class IlcPVBARDigitizer ;
  
class IlcPVBARLoader : public IlcLoader {
  
public:

  IlcPVBARLoader();
  IlcPVBARLoader(const Char_t *detname,const Char_t *eventfoldername); 
  IlcPVBARLoader(const Char_t *detname,TFolder *topfolder);
  
  virtual ~IlcPVBARLoader() ; 

  Int_t   GetEvent();//extends the method on PVBAR RecPart posting
  Int_t   SetEvent();//extends the method on PVBAR RecPart posting
  
  Bool_t  BranchExists(const TString& recName);
  Int_t   LoadHits(Option_t* opt=""); //reads  from disk and sends them to folder; array as well as tree
  Int_t   LoadSDigits(Option_t* opt="");
  Int_t   LoadDigits(Option_t* opt=""); //reads Digits from disk and sends them to folder; array as well as tree
  Int_t   LoadRecPoints(Option_t* opt=""); //reads RecPoints from disk and sends them to folder; array as well as tree
  Int_t   LoadTracks(Option_t* opt="");  //reads Tracks from disk and sends them to folder; array as well as tree
  Int_t   LoadRecParticles(Option_t* opt="");
   
  Int_t   PostHits()const;  //Posts the 
  Int_t   PostSDigits()const;
  Int_t   PostDigits()const;
  Int_t   PostRecPoints()const;
  Int_t   PostTracks()const;
  Int_t   PostRecParticles()const;
  
  void    CleanFolders();//cleans all the stuff loaded by this detector + calls IlcLoader::Clean

  void    CleanHits()const;
  void    CleanSDigits()const;
  void    CleanDigits()const;
  void    CleanRecPoints()const;
  void    CleanTracks()const;
  void    CleanRecParticles();

  
/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

  TObject** HitsRef(){return GetDetectorDataRef(Hits());}
  TObject** SDigitsRef(){return GetDetectorDataRef(SDigits());}
  TObject** DigitsRef(){return GetDetectorDataRef(Digits());}
  TObject** EmcRecPointsRef(){return GetDetectorDataRef(EmcRecPoints());}
  TObject** CpvRecPointsRef(){return GetDetectorDataRef(CpvRecPoints());}
  TObject** TracksRef(){return GetDetectorDataRef(TrackSegments());}
  TObject** RecParticlesRef(){return GetDetectorDataRef(RecParticles());}
  TObject** AlarmsRef(){return GetDetectorDataRef(Alarms());}
  void   Track(Int_t itrack) ;

  static IlcPVBARLoader* GetPVBARLoader(const  char* eventfoldername);

  //Method to be used when digitizing under IlcDigitizationInput, who opens all files etc.
  Int_t  EventNumber()       { return (Int_t) GetRunLoader()->GetEventNumber();}
  Int_t  MaxEvent()          { return (Int_t) GetRunLoader()->TreeE()->GetEntries();}

/*   const IlcPVBAR *         PVBAR(); */
  // Alarms
  // TFolder * Alarms() const { return (TFolder*)(ReturnO("Alarms", 0)); }
  TObjArray *  Alarms() {return 0x0;}  

  /*********************************************/
  /************    TClonesArrays     ***********/
  /*********************************************/
  /****   H i t s  ****/
  TClonesArray*  Hits(void);
  const IlcPVBARHit*    Hit(Int_t index);
  void MakeHitsArray();
  /****   S D i g i t s  ****/ 
  TClonesArray*  SDigits();
  const IlcPVBARDigit*  SDigit(Int_t index);
  void MakeSDigitsArray();
  /****  D i g i t s  ****/
  TClonesArray*   Digits();
  const IlcPVBARDigit *  Digit(Int_t index);
  void MakeDigitsArray();
  /****  R e c P o i n t s  ****/
  TObjArray * EmcRecPoints();
  TObjArray * CpvRecPoints();
  const IlcPVBAREmcRecPoint * EmcRecPoint(Int_t index) ;
  const IlcPVBARCpvRecPoint * CpvRecPoint(Int_t index) ;
  void MakeRecPointsArray();
  /****   T r a c k S e g m e n t s ****/
  TClonesArray * TrackSegments();
  const IlcPVBARTrackSegment * TrackSegment(Int_t index);
  void MakeTrackSegmentsArray();
  /****  R e c P a r t ic l e s   ****/
  TClonesArray * RecParticles() ;
  const IlcPVBARRecParticle * RecParticle(Int_t index);
  void MakeRecParticlesArray();
  
  void   SetDebug(Int_t level) {fDebug = level;} // Set debug level
  void   SetBranchTitle(const TString& btitle);

  Int_t   GetDebug()                     const {return fDebug;      }
  TString GetBranchTitle()               const {return fBranchTitle;}
  
protected:
  TString fBranchTitle;            //Title of the branch

private:

  // assignement operator requested by coding convention, but not needed
  IlcPVBARLoader(const IlcPVBARLoader &);                //Not implemented
  IlcPVBARLoader & operator = (const IlcPVBARLoader & ); //Not implemented

  Int_t ReadHits();
  Int_t ReadDigits();
  Int_t ReadSDigits();
  Int_t ReadRecPoints();
  Int_t ReadTracks();
  Int_t ReadRecParticles();
  
  Int_t  fDebug ;             // Debug level
  TClonesArray *fTmpHits;     //! Temporary array of hits per track

  static const TString fgkHitsName;//Name for TClonesArray with hits from one event
  static const TString fgkSDigitsName;//Name for TClonesArray 
  static const TString fgkDigitsName;//Name for TClonesArray 
  static const TString fgkEmcRecPointsName;//Name for TClonesArray 
  static const TString fgkCpvRecPointsName;//Name for TClonesArray 
  static const TString fgkTracksName;//Name for TClonesArray 
  static const TString fgkRecParticlesName;//Name for TClonesArray

  static const TString fgkEmcRecPointsBranchName;//Name for branch
  static const TString fgkCpvRecPointsBranchName;//Name for branch
  static const TString fgkTrackSegmentsBranchName;//Name for branch
  static const TString fgkRecParticlesBranchName;//Name for branch
  
 
  ClassDef(IlcPVBARLoader,4)  // Algorithm class that provides methods to retrieve objects from a list knowing the index 

};

/******************************************************************************/
/****************    I N L I N E S     ****************************************/
/******************************************************************************/

inline TClonesArray* IlcPVBARLoader::Hits()  
{
 return (TClonesArray*)GetDetectorData(fgkHitsName);
}
/******************************************************************************/

inline const IlcPVBARHit* IlcPVBARLoader::Hit(Int_t index)  
{
  const TClonesArray* tcarr = Hits();
  if (tcarr)
    return (const IlcPVBARHit*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TClonesArray* IlcPVBARLoader::SDigits()
{
   return dynamic_cast<TClonesArray*>(GetDetectorData(fgkSDigitsName));
}
/******************************************************************************/

inline const IlcPVBARDigit*  IlcPVBARLoader::SDigit(Int_t index)
{
  const TClonesArray* tcarr = SDigits();
  if (tcarr)
    return (const IlcPVBARDigit*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TClonesArray* IlcPVBARLoader::Digits()
{
 return dynamic_cast<TClonesArray*>(GetDetectorData(fgkDigitsName));
}
/******************************************************************************/

inline const IlcPVBARDigit*  IlcPVBARLoader::Digit(Int_t index)
{
  const TClonesArray* tcarr = Digits();
  if (tcarr)
    return (const IlcPVBARDigit*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TObjArray * IlcPVBARLoader::EmcRecPoints()
{
 return dynamic_cast<TObjArray*>(GetDetectorData(fgkEmcRecPointsName));
}
/******************************************************************************/

inline const IlcPVBAREmcRecPoint * IlcPVBARLoader::EmcRecPoint(Int_t index)
{
  TObjArray* tcarr = EmcRecPoints();
  if (tcarr)
    return (const IlcPVBAREmcRecPoint*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TObjArray * IlcPVBARLoader::CpvRecPoints()
{
 return dynamic_cast<TObjArray*>(GetDetectorData(fgkCpvRecPointsName));
}
/******************************************************************************/

inline const IlcPVBARCpvRecPoint * IlcPVBARLoader::CpvRecPoint(Int_t index)
{
  TObjArray* tcarr = CpvRecPoints();
  if (tcarr)
    return (const IlcPVBARCpvRecPoint*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TClonesArray * IlcPVBARLoader::TrackSegments()
{
 return dynamic_cast<TClonesArray*>(GetDetectorData(fgkTracksName));
}
/******************************************************************************/

inline const IlcPVBARTrackSegment * IlcPVBARLoader::TrackSegment(Int_t index)
{
  const TClonesArray* tcarr = TrackSegments();
  if (tcarr)
    return (const IlcPVBARTrackSegment*) tcarr->At(index);
  return 0x0; 
}
/******************************************************************************/

inline TClonesArray * IlcPVBARLoader::RecParticles() 
{
 return dynamic_cast<TClonesArray*>(GetDetectorData(fgkRecParticlesName)); 
}
/******************************************************************************/

inline const IlcPVBARRecParticle* IlcPVBARLoader::RecParticle(Int_t index)
{
  TClonesArray* tcarr = RecParticles();
  if (tcarr)
    return (const IlcPVBARRecParticle*) tcarr->At(index);
  return 0x0;  
}

#endif // IlcPVBARLOADER_H
