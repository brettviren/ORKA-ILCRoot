#ifndef ILCMDC_H
#define ILCMDC_H
// @(#)ilcmdc:$Name:  $:$Id: IlcMDC.h 32401 2009-05-15 14:01:03Z cvetan $
// Author: Fons Rademakers  26/11/99

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcMDC                                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#ifndef ROOT_TObjArray
#include <TObjArray.h>
#endif

#ifndef ROOT_TSysEvtHandler
#include <TSysEvtHandler.h>
#endif

// Forward class declarations
class IlcRawEventHeaderBase;
class IlcRawEquipmentHeader;
class IlcRawData;
class IlcRawDB;
class IlcTagDB;
class IlcRawEventTag;
class IlcESDEvent;

#include "IlcRawEventV2.h"
#include "IlcESDEvent.h"
#include "IlcRawDB.h"
#include "IlcTagDB.h"
#include "IlcRawData.h"
#include "IlcRawEventTag.h"

class IlcMDC : public TObject {

public:
   enum EWriteMode { kLOCAL, kRFIO, kROOTD, kCASTOR, kDEVNULL };
   enum EFilterMode { kFilterOff, kFilterTransparent, kFilterOn };
   enum EErrorCode { kErrStartEndRun = -1, 
		     kErrHeader = -2, 
		     kErrHeaderSize = -3, 
		     kErrSubHeader = -4, 
		     kErrDataSize = -5, 
		     kErrEquipmentHeader = -6, 
		     kErrEquipment = -7,
                     kErrFileSize = -8,
		     kFilterReject = -9,
                     kErrWriting = -10,
		     kErrTagFile = -11};

   IlcMDC(Int_t compress, Bool_t deleteFiles, 
	  EFilterMode filterMode = kFilterTransparent, 
	  Double_t maxSizeTagDB = -1, const char* fileNameTagDB = NULL,
	  const char* guidFileFolder = NULL,
	  Int_t basketsize = 32000);
   virtual ~IlcMDC();

   Int_t      Open(EWriteMode mode, const char* fileName,
		   Double_t maxFileSize = 0,
		   const char* fs1 = NULL, const char* fs2 = NULL);
   Int_t      ProcessEvent(void* event, Bool_t isIovecArray = kFALSE);
   Long64_t   GetTotalSize();
   Long64_t   Close();
   Long64_t   AutoSave();

   Int_t      Run(const char* inputFile, Bool_t loop,
	          EWriteMode mode, Double_t maxFileSize, 
		  const char* fs1 = NULL, const char* fs2 = NULL);
   void       Stop();

private:
   class IlcMDCInterruptHandler : public TSignalHandler {
   public:
     IlcMDCInterruptHandler(IlcMDC *mdc) : TSignalHandler(kSigUser1, kFALSE), fMDC(mdc) { }
     Bool_t Notify() {
       Info("Notify", "received a SIGUSR1 signal");
       fMDC->Stop();
       return kTRUE;
     }
   private:
     IlcMDC *fMDC;   // ilcmdc to signal

     IlcMDCInterruptHandler(const IlcMDCInterruptHandler& handler); // Not implemented
     IlcMDCInterruptHandler& operator=(const IlcMDCInterruptHandler& handler); // Not implemented
   };

   IlcRawEventV2 *fEvent;       // produced IlcRawEvent
   IlcESDEvent      *fESD;         // pointer to HLT ESD object
   IlcRawDB    *fRawDB;       // raw data DB
   IlcTagDB    *fTagDB;       // tag DB
   IlcRawEventTag *fEventTag; // raw-data event tag object
   Int_t        fCompress;    // compression factor used for raw output DB
   Int_t        fBasketSize;  // root i/o basket size (default = 32000)
   Bool_t       fDeleteFiles; // flag for deletion of files
   EFilterMode  fFilterMode;  // high level filter mode
   TObjArray    fFilters;     // filter algorithms
   Bool_t       fStop;        // stop execution (triggered by SIGUSR1)
   Bool_t       fIsTagDBCreated; // is tag db already created
   Double_t     fMaxSizeTagDB;// max size of the tag DB
   TString      fFileNameTagDB;// tag DB file name
   TString      fGuidFileFolder; // guid files folder

   // Filter names
   enum {kNFilters = 1};
   static const char* const fgkFilterName[kNFilters];

   IlcMDC(const IlcMDC& mdc);
   IlcMDC& operator = (const IlcMDC& mdc);

   Int_t     Read(const char *name) { return TObject::Read(name); }
   Int_t     Read(Int_t fd, void *buffer, Int_t length);
   Int_t     ReadEquipmentHeader(IlcRawEquipmentHeader &header,
                                 Bool_t isSwapped, char*& data);
   Int_t     ReadRawData(IlcRawData &raw, Int_t size, char*& data);

   ClassDef(IlcMDC,3)  // MDC processor
};

#endif
