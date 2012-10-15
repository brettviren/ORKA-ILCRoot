#ifndef ILCRAWREADERDATEONLINE_H
#define ILCRAWREADERDATEONLINE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
///
/// This is a class for reading raw data from a date monitoring libraries.
/// It supports two modes - event taken from shared memory via DATE monitoring
/// libs, or an emulation mode when the events are taken from a DATE file using
/// the same monitoring libs.
/// The constructor requires an argument:
///
/// : - events are taken from shared memory
///  or
/// <DATE_filename> - events are taken from date file
///
/// Cvetan Cheshkov 1/04/2008
///////////////////////////////////////////////////////////////////////////////

#include <TSysEvtHandler.h>

#include "IlcRawReaderDate.h"

class IlcRawReaderDateOnline: public IlcRawReaderDate {
  public :
    IlcRawReaderDateOnline(const char* fileName, const Char_t** customTable = NULL);
    virtual ~IlcRawReaderDateOnline();

    virtual Bool_t   NextEvent();
    //    virtual Bool_t   RewindEvents();

    // Method which can be used in order to force the auto-save on
    // ESD tree inside IlcReconstruction. For the moment it will be
    // activated only for IlcRawReaderDateOnline.
    virtual Bool_t   UseAutoSaveESD() const { return kTRUE; }

    // Method triggered by signal hanlder
    // Set fStop to false in which case
    // NextEvent() returns fFALSE and the
    // processing of raw data stops
    virtual void     Stop();
    static Bool_t GetNoSleep()               {return fgNoSleep;}
    static void   SetNoSleep(Bool_t v=kTRUE) {fgNoSleep = v;}
    //
  protected:
    class IlcRawReaderDateIntHandler : public TSignalHandler {
    public:
    IlcRawReaderDateIntHandler(IlcRawReaderDateOnline *rawReader):
      TSignalHandler(kSigUser1, kFALSE), fRawReader(rawReader) { }
      Bool_t Notify() {
	Info("Notify", "received a SIGUSR1 signal");
	fRawReader->Stop();
	return kTRUE;
      }
    private:
      IlcRawReaderDateOnline *fRawReader;   // raw-reader to signal

      IlcRawReaderDateIntHandler(const IlcRawReaderDateIntHandler& handler); // Not implemented
      IlcRawReaderDateIntHandler& operator=(const IlcRawReaderDateIntHandler& handler); // Not implemented
    };

    virtual void     SelectEvents(Int_t type, ULong64_t triggerMask = 0, const char *triggerExpr = NULL);
  private:
    IlcRawReaderDateOnline(const IlcRawReaderDateOnline& rawReader);
    IlcRawReaderDateOnline& operator = (const IlcRawReaderDateOnline& rawReader);

    const Char_t**   fTable;// custom monitoring table
    Bool_t           fStop; // raw-reader signaled to stop
    static Bool_t    fgNoSleep; // do we freez the system while looping in event waiting? 
    ClassDef(IlcRawReaderDateOnline, 0) // class for reading DATE raw data from shared memory
};

#endif
