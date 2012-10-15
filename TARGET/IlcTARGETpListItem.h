#ifndef ILCTARGETPLISTITEM_H
#define ILCTARGETPLISTITEM_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */

/* $Id: IlcTARGETpListItem.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */

#include <TObject.h>


class IlcTARGETpListItem: public TObject {
 public:
    // Default Constructor
    IlcTARGETpListItem();
    // Standard Signal Constructor
    IlcTARGETpListItem(Int_t track,Int_t hit,Int_t module,Int_t index,
		   Double_t signal);
    // Standard Noise Constructor
    IlcTARGETpListItem(Int_t module,Int_t index,Double_t signal);
    // Class destrutor
    virtual ~IlcTARGETpListItem();
    // Copy Oporator
    IlcTARGETpListItem(IlcTARGETpListItem &source);
    // = Opoerator
    virtual IlcTARGETpListItem& operator=(const IlcTARGETpListItem &source);
    // Returns the signal value in the list of signals
    virtual Double_t GetSignal(Int_t i) const {
	                    return ( (i>=0&&i<fgksize) ? fSignal[i] : 0.0);}
    virtual Double_t GetSignal() const {
	                    return fTsignal;}
    virtual Double_t GetSignalAfterElect() const {
	                    return fSignalAfterElect;}
    // Returns the Sum/Total signal
    virtual Double_t GetSumSignal() const {return fTsignal+fNoise;}
    // Returns the  noise
    virtual Double_t GetNoise() const {return fNoise;}
    // Returns the number of stored singals.
    virtual Int_t GetNsignals() const {return fgksize;}
    // Addes track number and signal to this existing list.
    virtual void AddSignal(Int_t track,Int_t hit,Int_t module,
			   Int_t index,Double_t signal);
    // Adds signal after electronics to this existing list.
    virtual void AddSignalAfterElect(Int_t module,Int_t index,Double_t signal);
    // Addes noise to this existing list.
    virtual void AddNoise(Int_t module,Int_t index,Double_t noise);
    // Returns track number.
    virtual Int_t GetTrack(Int_t i) const {
	                    return ((i>=0&&i<fgksize) ? fTrack[i] : 0);}
    // Returns hit number.
    virtual Int_t GetHit(Int_t i) const {
	                    return ((i>=0&&i<fgksize) ? fHits[i] : 0);}
    // Returns module number.
    virtual Int_t GetModule() const {
	                    return fmodule;}
    // Returns index number.
    virtual Int_t GetIndex() const {
	                    return findex;}
    // Adds the contents of pl to this 
    virtual void Add(IlcTARGETpListItem *pl);
    // Adds the contents of pl to this with track number off set given by
    // fileIndex.
    virtual void AddTo(Int_t fileIndex,IlcTARGETpListItem *pl);
    // Shift an index number to occupy the upper four bits.
    virtual Int_t ShiftIndex(Int_t in,Int_t trk) const;
    // Standard ascii class print function
    void Print(ostream *os) const;
    // Standard ascii class read function
    void Read(istream *is);
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}

    // Returns max size of array for for Tracks, Hits, and signals.
    static Int_t GetMaxKept() {return fgksize;};

 private:
    static const Int_t fgksize = 10; // Array sizes
    Int_t    fmodule;         // module number
    Int_t    findex;          // Strip/row,col number linearlized.
    Int_t    fTrack[fgksize];  //[fgksize] track Number
    Int_t    fHits[fgksize];   //[fgksize] hit number
    Double_t fSignal[fgksize]; //[fgksize] Signals
    Double_t fTsignal;        // Total signal (no noise)
    Double_t fNoise;          // Total noise, coupling, ...
    Double_t fSignalAfterElect; // Signal after electronics

    ClassDef(IlcTARGETpListItem,3) // Item list of signals and track numbers
};	
// Input and output functions for standard C++ input/output.
ostream & operator<<(ostream &os,IlcTARGETpListItem &source);
istream & operator>>(istream &is,IlcTARGETpListItem &source);


#endif
