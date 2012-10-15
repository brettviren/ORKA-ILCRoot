#if !defined(__CINT__) || defined(__MAKECINT__)
  #include <TStopwatch.h>
  #include "IlcRawReaderRoot.h"
  #include "IlcRawReaderDate.h"
  #include "IlcAltroRawStreamFast.h"
  #include "IlcLog.h"
#endif


void altroFast(const char *fileName)
{
  //  IlcLog::SetGlobalLogLevel(IlcLog::kFatal);

  //  IlcRawReader *reader = new IlcRawReaderRoot(fileName);
  IlcRawReader *reader = new IlcRawReaderDate(fileName);
  reader->Reset();

  TStopwatch timer;
  timer.Start();

  IlcAltroRawStreamFast* stream = new IlcAltroRawStreamFast(reader);
  stream->SelectRawData("TPC");

  while (reader->NextEvent()) {

    while (stream->NextDDL()) {

      while (stream->NextChannel()) {

	while (stream->NextBunch()) {
	  const UInt_t *adc = stream->GetSignals();
	  for(UInt_t i = stream->GetStartTimeBin(); i <= stream->GetEndTimeBin(); i++) {
	    // cout i - timebin, *adc - ADC signal, ...
	    adc++;
	  }
	}
      }
    }
  }

  timer.Stop();
  timer.Print();

  delete stream;

  return;
}
