#ifndef ILCLHCREADER_H
#define ILCLHCREADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Class to read the file coming from DCS containing the information        //
//  from LHC.                                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

class TString;
class TMap;
class TObjArray;

#include <TObject.h>

class IlcLHCReader : public TObject{
 public:
	IlcLHCReader(); // default ctor
	virtual ~IlcLHCReader();
	TMap* ReadLHCDP(TString lhcDP);

	UInt_t GetStartTime() const {return fStartTime;}
	UInt_t GetEndTime() const {return fEndTime;}

	void SetStartTime(UInt_t startTime) {fStartTime=startTime;}
	void SetEndTime(UInt_t endTime) {fEndTime = endTime;}
	TObjArray* ReadSingleLHCDP(TString filename, TString alias);

 private:
	IlcLHCReader& operator=(const IlcLHCReader *reader); // assignment operator
	IlcLHCReader(const IlcLHCReader *reader); // copy ctor

	UInt_t fStartTime;   // start time of data taking
	UInt_t fEndTime;     // end time of data taking

	ClassDef(IlcLHCReader,0)
		};
#endif
