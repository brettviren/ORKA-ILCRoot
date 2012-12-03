// @(#)ilcmdc:$Name:  $:$Id: IlcMDC.cxx 54863 2012-02-28 12:10:29Z hristov $
// Author: Fons Rademakers  26/11/99
// Updated: Dario Favretto  15/04/2003

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

/* $Id: IlcMDC.cxx 54863 2012-02-28 12:10:29Z hristov $ */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// IlcMDC                                                               //
//                                                                      //
// Set of classes defining the ILC RAW event format. The IlcRawEventV2//
// class defines a RAW event. It consists of an IlcEventHeader object   //
// an IlcEquipmentHeader object, an IlcRawData object and an array of   //
// sub-events, themselves also being IlcRawEventV2s. The number of      //
// sub-events depends on the number of DATE LDC's.                      //
// The IlcRawEventV2 objects are written to a ROOT file using different //
// technologies, i.e. to local disk via IlcRawDB or via rfiod using     //
// IlcRawRFIODB or via rootd using IlcRawRootdDB or to CASTOR via       //
// rootd using IlcRawCastorDB (and for performance testing there is     //
// also IlcRawNullDB).                                                  //
// The IlcStats class provides statics information that is added as     //
// a single keyed object to each raw file.                              //
// The IlcTagDB provides an interface to a TAG database.                //
// The IlcMDC class is usid by the "ilcmdc" stand-alone program         //
// that reads data directly from DATE.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>

#include <TSystem.h>
#include <TROOT.h>
#include <TStopwatch.h>
#include <TPluginManager.h>
#include <TBufferFile.h>

#ifdef WIN32
#include <CVG.h>
#else
#include <sys/uio.h>
#endif
#ifdef USE_EB
#include "libDateEb.h"
#endif

#include "IlcMDC.h"

#include <IlcLog.h>
#include <IlcESDEvent.h>

#include "IlcRawEventV2.h"
#include "IlcRawEventHeaderBase.h"
#include "IlcRawEquipmentV2.h"
#include "IlcRawEquipmentHeader.h"
#include "IlcRawDataArrayV2.h"
#include "IlcRawData.h"
#include "IlcRawDB.h"
#include "IlcRawRFIODB.h"
#include "IlcRawCastorDB.h"
#include "IlcRawRootdDB.h"
#include "IlcRawNullDB.h"
#include "IlcTagDB.h"
#include "IlcRawEventTag.h"
#include "IlcFilter.h"



ClassImp(IlcMDC)


// Filter names
const char* const IlcMDC::fgkFilterName[kNFilters] = {"IlcHoughFilter"};

//______________________________________________________________________________
IlcMDC::IlcMDC(Int_t compress, Bool_t deleteFiles, EFilterMode filterMode, 
	       Double_t maxSizeTagDB, const char* fileNameTagDB,
	       const char *guidFileFolder,
	       Int_t basketsize) :
  fEvent(new IlcRawEventV2),
  fESD(NULL),
  fRawDB(NULL),
  fTagDB(NULL),
  fEventTag(new IlcRawEventTag),
  fCompress(compress),
  fBasketSize(basketsize),
  fDeleteFiles(deleteFiles),
  fFilterMode(filterMode),
  fFilters(),
  fStop(kFALSE),
  fIsTagDBCreated(kFALSE),
  fMaxSizeTagDB(maxSizeTagDB),
  fFileNameTagDB(fileNameTagDB),
  fGuidFileFolder(guidFileFolder)
{
  // Create MDC processor object.
  // compress is the file compression mode.
  // If deleteFiles is kTRUE the raw data files will be deleted after they
  // were closed.
  // If the filterMode if kFilterOff no filter algorithm will be, if it is
  // kFilterTransparent the algorthims will be run but no events will be
  // rejected, if it is kFilterOn the filters will be run and the event will
  // be rejected if all filters return kFALSE.
  // If maxSizeTagDB is greater than 0 it determines the maximal size of the
  // tag DB and then fileNameTagDB is the directory name for the tag DB.
  // Otherwise fileNameTagDB is the file name of the tag DB. If it is NULL
  // no tag DB will be created.
  // Optional 'guidFileFolder' specifies the folder in which *.guid files
  // will be stored. In case this option is not given, the *.guid files
  // will be written to the same folder as the raw-data files.

  // Set the maximum tree size to 19GB
  // in order to allow big raw data files
  TTree::SetMaxTreeSize(20000000000LL);

  IlcRawEquipmentHeader::Class()->IgnoreTObjectStreamer();
  IlcRawEquipmentV2::Class()->IgnoreTObjectStreamer();
  IlcRawEventV2::Class()->IgnoreTObjectStreamer();
  IlcRawDataArrayV2::Class()->IgnoreTObjectStreamer();

  TBufferFile::SetGlobalReadParam(5);
 
  // This line is needed in case of a stand-alone application w/o
  // $ROOTSYS/etc/system.rootrc file
  gROOT->GetPluginManager()->AddHandler("TVirtualStreamerInfo",
					"*",
					"TStreamerInfo",
					"RIO",
					"TStreamerInfo()");

  if (fFilterMode != kFilterOff) {
    fESD = new IlcESDEvent();
  }

  // Create the guid files folder if it does not exist
  if (!fGuidFileFolder.IsNull()) {
    gSystem->ResetErrno();
    gSystem->MakeDirectory(fGuidFileFolder.Data());
    if (gSystem->GetErrno() && gSystem->GetErrno() != EEXIST) {
      SysError("IlcMDC", "mkdir %s", fGuidFileFolder.Data());
    }
  }

  // install SIGUSR1 handler to allow clean interrupts
  gSystem->AddSignalHandler(new IlcMDCInterruptHandler(this));

  // create the high level filters
  if (fFilterMode != kFilterOff) {
    for (Int_t iFilter = 0; iFilter < kNFilters; iFilter++) {
      TClass* filterClass = gROOT->GetClass(fgkFilterName[iFilter]);
      if (!filterClass) {
	Warning("IlcMDC", "no filter class %s found", fgkFilterName[iFilter]);
	continue;
      }
      IlcFilter* filter = (IlcFilter*) filterClass->New();
      if (!filter) {
	Warning("IlcMDC", "creation of filter %s failed", fgkFilterName[iFilter]);
	continue;
      }
      fFilters.Add(filter);
    }
  }
}

//______________________________________________________________________________
IlcMDC::~IlcMDC()
{
// destructor

  fFilters.Delete();
  if(fTagDB) delete fTagDB;
  delete fRawDB;
  delete fESD;
  delete fEvent;
  delete fEventTag;
}
 
//______________________________________________________________________________
Int_t IlcMDC::Open(EWriteMode mode, const char* fileName,
		   Double_t maxFileSize,
		   const char* fs1, const char* fs2)
{
// open a new raw DB file

  if (mode == kRFIO)
    fRawDB = new IlcRawRFIODB(fEvent, fESD, fCompress, fileName, fBasketSize);
  else if (mode == kROOTD)
    fRawDB = new IlcRawRootdDB(fEvent, fESD, fCompress, fileName, fBasketSize);
  else if (mode == kCASTOR)
    fRawDB = new IlcRawCastorDB(fEvent, fESD, fCompress, fileName, fBasketSize);
  else if (mode == kDEVNULL)
    fRawDB = new IlcRawNullDB(fEvent, fESD, fCompress, fileName, fBasketSize);
  else
    fRawDB = new IlcRawDB(fEvent, fESD, fCompress, fileName, fBasketSize);
  fRawDB->SetDeleteFiles(fDeleteFiles);

  if (fRawDB->IsZombie()) {
    delete fRawDB;
    fRawDB = NULL;
    return -1;
  }

  if (fileName == NULL) {
    fRawDB->SetMaxSize(maxFileSize);
    fRawDB->SetFS(fs1, fs2);
    if (!fRawDB->Create()) {
      delete fRawDB;
      fRawDB = NULL;
      return -1;
    }
  }

  if (!fRawDB->WriteGuidFile(fGuidFileFolder)) {
    delete fRawDB;
    fRawDB = NULL;
    return -2;
  }

  Info("Open", "Filling raw DB %s\n", fRawDB->GetDBName());

  return 0;
}

//______________________________________________________________________________
Int_t IlcMDC::ProcessEvent(void* event, Bool_t isIovecArray)
{
// Convert the DATE event to an IlcRawEventV2 object and store it in the raw DB,
// optionally also run the filter.
// event is either a pointer to the streamlined event 
// or, if isIovecArray is kTRUE, a pointer to an array of iovecs with one
// iovec per subevent (used by the event builder).
// The return value is the number of written bytes or an error code
  const Long64_t kFileSizeErrorLevel   = 19000000000LL;

  Long64_t currentFileSize = GetTotalSize();
  //  IlcDebug(1,Form("current file size is %lld bytes",currentFileSize));
  if(currentFileSize > kFileSizeErrorLevel) {
    Error("ProcessEvent", "file size (%lld) exceeds the limit "
	  , currentFileSize);
    return kErrFileSize;
  }

  Int_t status;
  char* data = (char*) event;
  if (isIovecArray) data = (char*) ((iovec*) event)[0].iov_base;

  // Shortcut for easy header access
  IlcRawEventHeaderBase *header = fEvent->GetHeader(data);

  // Read event header
  if ((status = header->ReadHeader(data)) != (Int_t)header->GetHeadSize()) {
    Error("ProcessEvent","Wrong event header format (%d != %d)",
	  status,(Int_t)header->GetHeadSize());
    return kErrHeader;
  }

  //  if (IlcDebugLevel() > 2) ToIlcDebug(3, header->Dump(););

  // Check event type and skip "Start of Run", "End of Run",
  // "Start of Run Files" and "End of Run Files"
  Int_t size = header->GetEventSize() - header->GetHeadSize();
  UInt_t eventType = header->Get("Type");  

  //  IlcDebug(1, Form("Processing %s (%d bytes)", header->GetTypeName(), size));

  // Amount of data left to read for this event
  Int_t toRead = size;

  // StartOfRun, EndOfRun etc. events have no payload
  // Nevertheless, store the event headers in the tree
  if (toRead > 0) {

    // If there is less data for this event than the next sub-event
    // header, something is wrong. Skip to next event...
    if (toRead < (Int_t)header->GetHeadSize()) {
      Error("ProcessEvent", "header size (%d) exceeds number of bytes "
	    "to read (%d)", header->GetHeadSize(), toRead);
      if (IlcDebugLevel() > 0) ToIlcDebug(1, header->Dump(););
      return kErrHeaderSize;
    }
  
    // Loop over all sub-events... (LDCs)
    Int_t nsub = 1;
    while (toRead > 0) {
      if (isIovecArray) data = (char*) ((iovec*) event)[nsub].iov_base;

      //      IlcDebug(1, Form("reading LDC %d", nsub));

      IlcRawEventV2 *subEvent = fEvent->NextSubEvent();

      // Read sub-event header
      IlcRawEventHeaderBase *subHeader = subEvent->GetHeader(data);
      if ((status = subHeader->ReadHeader(data)) != (Int_t)subHeader->GetHeadSize()) {
	return kErrSubHeader;
      }

      //      if (IlcDebugLevel() > 2) ToIlcDebug(3, subHeader->Dump(););

      toRead -= subHeader->GetHeadSize();

      Int_t rawSize = subHeader->GetEventSize() - subHeader->GetHeadSize();

      // Make sure raw data less than left over bytes for current event
      if (rawSize > toRead) {
	Warning("ProcessEvent", "raw data size (%d) exceeds number of "
		"bytes to read (%d)\n", rawSize, toRead);
	if (IlcDebugLevel() > 0) ToIlcDebug(1, subHeader->Dump(););
	return kErrDataSize;
      }

      // Read Equipment Headers (in case of physics or calibration event)
      if (eventType == IlcRawEventHeaderBase::kPhysicsEvent ||
	  eventType == IlcRawEventHeaderBase::kCalibrationEvent ||
	  eventType == IlcRawEventHeaderBase::kSystemSoftwareTriggerEvent ||
	  eventType == IlcRawEventHeaderBase::kDetectorSoftwareTriggerEvent ||
	  eventType == IlcRawEventHeaderBase::kStartOfData ||
	  eventType == IlcRawEventHeaderBase::kEndOfData) {
	while (rawSize > 0) {
	  IlcRawEquipmentV2 &equipment = *subEvent->NextEquipment();
	  IlcRawEquipmentHeader &equipmentHeader = 
	    *equipment.GetEquipmentHeader();
	  Int_t equipHeaderSize = equipmentHeader.HeaderSize();
	  if ((status = ReadEquipmentHeader(equipmentHeader, header->DataIsSwapped(),
					    data)) != equipHeaderSize) {
	    return kErrEquipmentHeader;
	  }

	  //	  if (IlcDebugLevel() > 2) ToIlcDebug(3, equipmentHeader.Dump(););

	  toRead  -= equipHeaderSize;
	  rawSize -= equipHeaderSize;

	  // Read equipment raw data
	  IlcRawDataArrayV2 *arr = fRawDB->GetRawDataArray(equipmentHeader.GetEquipmentSize(),
							   equipmentHeader.GetId());
	  IlcRawData &subRaw = *equipment.NextRawData(arr);

	  Int_t eqSize = equipmentHeader.GetEquipmentSize() - equipHeaderSize;
	  if ((status = ReadRawData(subRaw, eqSize, data)) != eqSize) {
	    return kErrEquipment;
	  }
	  toRead  -= eqSize;
	  rawSize -= eqSize;

	}

      } else {  // Read only raw data but no equipment header
	if (rawSize) {
	  IlcRawEquipmentV2 &equipment = *subEvent->NextEquipment();
	  IlcRawEquipmentHeader &equipmentHeader = 
	    *equipment.GetEquipmentHeader();
	  equipmentHeader.Reset();
	  IlcRawDataArrayV2 *arr = fRawDB->GetRawDataArray(equipmentHeader.GetEquipmentSize(),
							   equipmentHeader.GetId());
	  IlcRawData &subRaw = *equipment.NextRawData(arr);
	  if ((status = ReadRawData(subRaw, rawSize, data)) != rawSize) {
	    return kErrEquipment;
	  }
	  toRead  -= rawSize;
	}
      }

      nsub++;
    }
  }

  // High Level Event Filter
  if (fFilterMode != kFilterOff) {
    if (eventType == IlcRawEventHeaderBase::kPhysicsEvent ||
	eventType == IlcRawEventHeaderBase::kCalibrationEvent ||
	eventType == IlcRawEventHeaderBase::kSystemSoftwareTriggerEvent ||
	eventType == IlcRawEventHeaderBase::kDetectorSoftwareTriggerEvent ||
	eventType == IlcRawEventHeaderBase::kStartOfData ||
	eventType == IlcRawEventHeaderBase::kEndOfData) {
      Bool_t result = kFALSE;
      for (Int_t iFilter = 0; iFilter < fFilters.GetEntriesFast(); iFilter++) {
	IlcFilter* filter = (IlcFilter*) fFilters[iFilter];
	if (!filter) continue;
	if (filter->Filter(fEvent, fESD)) result = kTRUE;
      }
      if ((fFilterMode == kFilterOn) && !result) return kFilterReject;
    }
  }

  // Store raw event in tree
  Int_t nBytes = fRawDB->Fill();

  // Fill the event tag object
  fEventTag->SetHeader(header);
  fEventTag->SetGUID(fRawDB->GetDB()->GetUUID().AsString());
  fEventTag->SetEventNumber(fRawDB->GetEvents()-1);

  // Create Tag DB here only after the raw data header
  // version was already identified
  if (!fIsTagDBCreated) {
    if (!fFileNameTagDB.IsNull()) {
      if (fMaxSizeTagDB > 0) {
	fTagDB = new IlcTagDB(fEventTag, NULL);
	fTagDB->SetMaxSize(fMaxSizeTagDB);
	fTagDB->SetFS(fFileNameTagDB.Data());
	if (!fTagDB->Create()) return kErrTagFile;
      } else {
	fTagDB = new IlcTagDB(fEventTag, fFileNameTagDB.Data());
	if (fTagDB->IsZombie()) return kErrTagFile;
      }
    }
    fIsTagDBCreated = kTRUE;
  }

  // Store event tag in tree
  if (fTagDB) fTagDB->Fill();

  // Make top event object ready for next event data
  fEvent->Reset();
  fRawDB->Reset();

  // Clean up HLT ESD for the next event
  if (fESD) fESD->Reset();

  if(nBytes >= 0)
    return nBytes;
  else
    return kErrWriting;
}

//______________________________________________________________________________
Long64_t IlcMDC::GetTotalSize()
{
// return the total current raw DB file size

  if (!fRawDB) return -1;

  return fRawDB->GetTotalSize();
}

//______________________________________________________________________________
Long64_t IlcMDC::Close()
{
// close the current raw DB file

  if (!fRawDB) return -1;

  Long64_t filesize = fRawDB->Close();
  delete fRawDB;
  fRawDB = NULL;
  return filesize;
}

//______________________________________________________________________________
Long64_t IlcMDC::AutoSave()
{
  // Auto-save the raw-data
  // and esd (if any) trees

  if (!fRawDB) return -1;

  return fRawDB->AutoSave();
}

//______________________________________________________________________________
Int_t IlcMDC::Run(const char* inputFile, Bool_t loop,
		  EWriteMode mode, Double_t maxFileSize, 
		  const char* fs1, const char* fs2)
{
  // Run the MDC processor. Read from the input stream and only return
  // when the input gave and EOF or a fatal error occured. On success 0
  // is returned, 1 in case of a fatality.
  // inputFile is the name of the DATE input file; if NULL the input will
  // be taken from the event builder.
  // If loop is set the same input file will be reused in an infinite loop.
  // mode specifies the type of the raw DB.
  // maxFileSize is the maximal size of the raw DB.
  // fs1 and fs2 are the file system locations of the raw DB.

  Info("Run", "input = %s, rawdb size = %f, filter = %s, "
       "looping = %s, compression = %d, delete files = %s",
       inputFile ? inputFile : "event builder", maxFileSize,
       fFilterMode == kFilterOff ? "off" : 
       (fFilterMode == kFilterOn ? "on" : "transparent"), 
       loop ? "yes" : "no", fCompress, fDeleteFiles ? "yes" : "no");

  // Open the input file
  Int_t fd = -1;
  if (inputFile) {
    if ((fd = open(inputFile, O_RDONLY)) == -1) {
      Error("Run", "cannot open input file %s", inputFile);
      return 1;
    }
  }

  // Used for statistics
  TStopwatch timer;
  timer.Start();
  Float_t  chunkSize = maxFileSize/100, nextChunk = chunkSize;

  // Create new raw DB.
  if (fRawDB) Close();

  if (Open(mode,NULL,maxFileSize,fs1,fs2) < 0) {
    if (fd>-1) close(fd);
    return 1;
  }
  // Process input stream
#ifdef USE_EB
  Int_t eorFlag = 0;
#endif
  char* event = NULL;
  UInt_t eventSize = 0;
  Int_t numEvents = 0;

  IlcRawEventHeaderBase header;
  IlcRawEventHeaderBase *hdr = NULL;

  while (kTRUE) {

    // If we were in looping mode stop directly after a SIGUSR1 signal
    if (fStop) {
      Info("Run", "Stopping loop, processed %d events", numEvents);
      break;
    }

    if (!inputFile) {  // get data from event builder
#ifdef USE_EB
      if ((eorFlag = ebEor())) break;
      if ((event = (char*)ebGetNextEvent()) == (char*)-1) {
	Error("Run", "error getting next event (%s)", ebGetLastError());
	break;
      }
      if (event == 0) {
	// no event, sleep for 1 second and try again
	gSystem->Sleep(1000);
	continue;
      }
#else
      Error("Run", "IlcMDC was compiled without event builder support");
      delete fRawDB;
      fRawDB = NULL;
      return 1;
#endif

    } else {  // get data from a file
      {
	Int_t nrecv;
	if ((nrecv = Read(fd, header.HeaderBaseBegin(), header.HeaderBaseSize())) !=
	    header.HeaderBaseSize()) {
	  if (nrecv == 0) {  // eof
	    if (loop) {
	      ::lseek(fd, 0, SEEK_SET);
	      continue;
	    } else {
	      break;
	    }
	  } else {
	    Error("Run", "error reading base header");
	    Close();
	    delete[] event;
	    if (fd>-1) close(fd);
	    return 1;
	  }
	}
      }
      char *data = (char *)header.HeaderBaseBegin();
      if (!hdr) {
	hdr = IlcRawEventHeaderBase::Create(data);
      }
      else {
	memcpy(hdr->HeaderBaseBegin(), header.HeaderBaseBegin(), header.HeaderBaseSize());
      }
      Int_t nrecv;
      if ((nrecv = Read(fd, hdr->HeaderBegin(), hdr->HeaderSize())) !=
	  hdr->HeaderSize()) {
	if (nrecv == 0) {  // eof
	  if (loop) {
	    ::lseek(fd, 0, SEEK_SET);
	    delete hdr;
	    continue;
	  } else {
	    delete hdr;
	    break;
	  }
	} else {
	  Error("Run", "error reading header");
	  Close();
	  delete[] event;
	  delete hdr;
	  if (fd>-1) close(fd);
	  return 1;
	}
      }
      if (eventSize < hdr->GetEventSize()) {
	delete[] event;
	eventSize = 2 * hdr->GetEventSize();
	event = new char[eventSize];
      }
      memcpy(event, header.HeaderBaseBegin(), header.HeaderBaseSize());
      memcpy(event+hdr->HeaderBaseSize(), hdr->HeaderBegin(), hdr->HeaderSize());
      if (hdr->GetExtendedDataSize() != 0)
	memcpy(event+hdr->HeaderBaseSize()+hdr->HeaderSize(),
	       hdr->GetExtendedData(), hdr->GetExtendedDataSize());
      Int_t size = hdr->GetEventSize() - hdr->GetHeadSize();
      if (Read(fd, event + hdr->GetHeadSize(), size) != size) {
	Error("Run", "error reading data");
	Close();
	delete[] event;
	delete hdr;
	if (fd>-1) close(fd);
	return 1;
      }
    }

    Int_t result = ProcessEvent(event, !inputFile);
    if(result < -1)
      Error("Run", "error writing data. Error code: %d",result);

    if (result >= 0) {
      numEvents++;
    }

    if (result > 0) {
      // Filling time statistics
      if (fRawDB->GetBytesWritten() > nextChunk) {
	nextChunk += chunkSize;
      }

      // Check size of raw db. If bigger than maxFileSize, close file
      // and continue with new file.
      if (fRawDB->GetBytesWritten() > maxFileSize) {

	printf("Written raw DB at a rate of %.1f MB/s\n",
	       fRawDB->GetBytesWritten() / timer.RealTime() / 1000000.);

	if (!fRawDB->NextFile()) {
	  Error("Run", "error opening next raw data file");
	  Close();
	  if (inputFile) delete[] event;
	  delete hdr;
	  if (fd>-1) close(fd);
	  return 1;
	}

	printf("Filling raw DB %s\n", fRawDB->GetDBName());
	timer.Start();
	nextChunk = chunkSize;
      }

      // Check size of tag db
      if (fTagDB && fTagDB->FileFull()) {
	if (!fTagDB->NextFile()) {
	  delete fTagDB;
	  fTagDB = 0;
	} else {
	  printf("Filling tag DB %s\n", fTagDB->GetDBName());
	}
      }
    }

    // Make top event object ready for next event data
    //printf("Event %d has %d sub-events\n", numEvents, fEvent->GetNSubEvents());
    //    fEvent->Reset();
    // Clean up HLT ESD for the next event
    //    if (fESD) fESD->Reset();

    if (!inputFile) {
#ifdef USE_EB
      if (!ebReleaseEvent((iovec*)event)) {
	Error("Run", "problem releasing event (%s)", ebGetLastError());
	break;
      }
#endif
    }
  }

  delete hdr;

  printf("Written raw DB at a rate of %.1f MB/s\n",
	 fRawDB->GetBytesWritten() / timer.RealTime() / 1000000.);

  // Write stats to raw db and run db and delete stats object
  Close();

  if (!inputFile) {
#ifdef USE_EB
    // Print eor flag
    if (eorFlag) {
      Info("Run", "event builder reported end of run (%d)", eorFlag);
    }
#endif
  } else {
    // Close input source
    if (fd>-1) close(fd);
    delete [] event;
  }

  return 0;
}

//______________________________________________________________________________
Int_t IlcMDC::Read(Int_t fd, void *buffer, Int_t length)
{
   // Read exactly length bytes into buffer. Returns number of bytes
   // received, returns -1 in case of error and 0 for EOF.

   errno = 0;

   if (fd < 0) return -1;

   Int_t n, nrecv = 0;
   char *buf = (char *)buffer;

   for (n = 0; n < length; n += nrecv) {
      if ((nrecv = read(fd, buf+n, length-n)) <= 0) {
         if (nrecv == 0)
            break;        // EOF
         if (errno != EINTR)
            SysError("Read", "read");
         return -1;
      }
   }
   return n;
}

//______________________________________________________________________________
Int_t IlcMDC::ReadEquipmentHeader(IlcRawEquipmentHeader &header,
                                  Bool_t isSwapped, char*& data)
{
  // Read equipment header info from DATE data stream. Returns bytes read
  // (i.e. IlcRawEquipmentHeader::HeaderSize()), -1 in case of error and
  // 0 for EOF. If isSwapped is kTRUE the event data is byte swapped
  // and we will swap the header to host format.

  memcpy(header.HeaderBegin(), data, header.HeaderSize());
  data += header.HeaderSize();

  // Swap equipment header data if needed
  if (isSwapped)
    header.Swap();

  if (header.GetEquipmentSize() < (UInt_t)header.HeaderSize()) {
    Error("ReadEquipmentHeader", "invalid equipment header size");
    // try recovery... how?
    return -1;
  }

  return header.HeaderSize();
}

//______________________________________________________________________________
Int_t IlcMDC::ReadRawData(IlcRawData &raw, Int_t size, char*& data)
{
  // Read raw data from DATE data stream. Returns bytes read (i.e.
  // size), -1 in case of error and 0 for EOF.

  raw.SetBuffer(data, size);
  data += size;

  return size;
}

//______________________________________________________________________________
void IlcMDC::Stop()
{
  // Stop the event loop
 
  fStop = kTRUE; 
  if (fRawDB) fRawDB->Stop(); 
}


