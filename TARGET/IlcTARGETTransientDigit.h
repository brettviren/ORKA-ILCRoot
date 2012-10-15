#ifndef ILCTARGETTRANSIENTDIGIT_H
#define ILCTARGETTRANSIENTDIGIT_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */

#include <IlcTARGETdigitSDD.h>

//______________________________________________________________________
class IlcTARGETTransientDigit : public IlcTARGETdigitSDD {

 public:
    IlcTARGETTransientDigit() {/*default constructor*/fTrackList=0;}
    // Standard constructor with digits and "phys"
    IlcTARGETTransientDigit(Float_t phys,const Int_t *digits);
    virtual ~IlcTARGETTransientDigit(){/*destructor delets TObjArray fTracklist */
	delete fTrackList;}
    //copy constructor
    IlcTARGETTransientDigit(const IlcTARGETTransientDigit &source);
    //assignment operator
    IlcTARGETTransientDigit& operator=(const IlcTARGETTransientDigit &source);
    // returns pointer to the TObjArray of tracks and associated charges
    TObjArray  *TrackList() const {return fTrackList;}
    //returns element i of fTrackList
    TObject *TrackItem(Int_t i) const {return fTrackList->At(i);}
    //put TObject into fTrackList at location i
    void PutTrackItem(TObject *obj,Int_t i){fTrackList->AddAt(obj,i);}
    void Print(ostream *os); // Class ascii print function
    void Read(istream *os);  // Class ascii read function
    virtual Int_t Read(const char *name) {return IlcTARGETdigitSDD::Read(name);}
    virtual void Print(Option_t *option="") const {IlcTARGETdigitSDD::Print(option);}
 protected:
    TObjArray *fTrackList;  // track list 

    ClassDef(IlcTARGETTransientDigit,1)  // Transient digit for set: TARGET

};
// Input and output functions for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETTransientDigit &source);
istream &operator>>(istream &os,IlcTARGETTransientDigit &source);

#endif
