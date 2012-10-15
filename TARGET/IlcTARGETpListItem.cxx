/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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
#include <Riostream.h>
#include <TMath.h>
#include "IlcLog.h"
#include "IlcTARGETpListItem.h"
// ************************************************************************
// the data member "fa" of the IlcTARGETpList class
// is a TObjectArray of IlcTARGETpListItem objects
// each IlcTARGETpListItem object contains information related to
// the digitization such signal, noise, module number,...
// plus some info related to the simulation (hits/track)
// in order to allow efficiency studies
//************************************************************************
ClassImp(IlcTARGETpListItem)
//______________________________________________________________________
IlcTARGETpListItem::IlcTARGETpListItem(){
    // Default constructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A zeroed/empty IlcTARGETpListItem class.

    fmodule = -1;
    findex  = -1;
    for(Int_t i=0;i<this->fgksize;i++){
        this->fTrack[i]  = -2;
        this->fHits[i]   = -1;
        this->fSignal[i] = 0.0;
    } // end if i
    fTsignal = 0.0;
    fNoise   = 0.0;
    fSignalAfterElect = 0.0;
}
//______________________________________________________________________
IlcTARGETpListItem::IlcTARGETpListItem(Int_t module,Int_t index,Double_t noise){
    // Standard noise constructor
    // Inputs:
    //    Int_t module   The module where this noise occurred
    //    Int_t index    The cell index where this noise occurred
    //    Double_t noise The value of the noise.
    // Outputs:
    //    none.
    // Return:
    //    A setup and noise filled IlcTARGETpListItem class.

    this->fmodule    = module;
    this->findex     = index;
    for(Int_t i=0;i<this->fgksize;i++){
        this->fTrack[i]  = -2;
        this->fSignal[i] = 0.0;
        this->fHits[i]   = -1;
    } // end if i
    this->fTsignal = 0.0;
    this->fSignalAfterElect = 0.0;
    this->fNoise   = noise;
}
//______________________________________________________________________
IlcTARGETpListItem::IlcTARGETpListItem(Int_t track,Int_t hit,Int_t module,
                               Int_t index,Double_t signal){
    // Standard signal constructor
    // Inputs:
    //    Int_t track     The track number which produced this signal
    //    Int_t hit       The hit number which produced this signal
    //    Int_t module    The module where this signal occurred
    //    Int_t index     The cell index where this signal occurred
    //    Double_t signal The value of the signal (ionization)
    // Outputs:
    //    none.
    // Return:
    //    A setup and signal filled  IlcTARGETpListItem class.

    this->fmodule    = module;
    this->findex     = index;
    this->fTrack[0]  = track;
    this->fHits[0]   = hit;
    this->fSignal[0] = signal;
    for(Int_t i=1;i<this->fgksize;i++){
        this->fTrack[i]  = -2;
        this->fSignal[i] = 0.0;
        this->fHits[i]   = -1;
    } // end if i
    this->fTsignal = signal;
    this->fNoise   = 0.0;
    this->fSignalAfterElect   = 0.0;
}
//______________________________________________________________________
IlcTARGETpListItem::~IlcTARGETpListItem(){
    // Destructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A properly destroyed IlcTARGETpListItem class.

}
//______________________________________________________________________
IlcTARGETpListItem& IlcTARGETpListItem::operator=(const IlcTARGETpListItem &source){
    // = operator
    // Inputs:
    //    IlcTARGETpListItem &source   A IlcTARGETpListItem Object
    // Outputs:
    //    none.
    // Return:
    //    A copied IlcTARGETpListItem object
    Int_t i;

    if(this == &source) return *this;

    this->fmodule = source.fmodule;
    this->findex  = source.findex;
    for(i=0;i<this->fgksize;i++){
        this->fTrack[i]  = source.fTrack[i];
        this->fSignal[i] = source.fSignal[i];
        this->fHits[i]   = source.fHits[i];
    } // end if i
    this->fTsignal = source.fTsignal;
    this->fNoise   = source.fNoise;
    this->fSignalAfterElect   = source.fSignalAfterElect;
    /*
    cout <<"this fTrack[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<this->fTrack[i]<<",";
    cout <<" fHits[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<this->fHits[i]<<",";
    cout <<" fSignal[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<this->fSignal[i]<<",";
    cout << endl;
    cout <<"source fTrack[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<source.fTrack[i]<<",";
    cout <<" fHits[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<source.fHits[i]<<",";
    cout <<" fSignal[0-9]=";
    for(i=0;i<this->fgksize;i++) cout <<source.fSignal[i]<<",";
    cout << endl;
    */
    return *this;
}
//______________________________________________________________________
IlcTARGETpListItem::IlcTARGETpListItem(IlcTARGETpListItem &source) : TObject(source){
    // Copy operator
    // Inputs:
    //    IlcTARGETpListItem &source   A IlcTARGETpListItem Object
    // Outputs:
    //    none.
    // Return:
    //    A copied IlcTARGETpListItem object

    *this = source;
}
//______________________________________________________________________
void IlcTARGETpListItem::AddSignal(Int_t track,Int_t hit,Int_t module,
                               Int_t index,Double_t signal){
    // Adds this track number and signal to the pList and orders them
    // Inputs:
    //    Int_t track     The track number which produced this signal
    //    Int_t hit       The hit number which produced this signal
    //    Int_t module    The module where this signal occurred
    //    Int_t index     The cell index where this signal occurred
    //    Double_t signal The value of the signal (ionization)
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t    i,j,trk,hts;
    Double_t sig;
    Bool_t   flg=kFALSE;

    if (TMath::Abs(signal)>2147483647.0) {
      //PH 2147483647 is the max. integer
      //PH This apparently is a problem which needs investigation
      IlcWarning(Form("Too big or too small signal value %f",signal));
      signal = TMath::Sign((Double_t)2147483647,signal);
    }
    if(findex!=index || fmodule!=module) 
        Warning("AddSignal","index=%d != findex=%d or module=%d != fmodule=%d",
                 index,findex,module,fmodule);
    fTsignal += signal; // Keep track of sum signal.

    //    for(i=0;i<fgksize;i++) if( track==fTrack[i] && hit==fHits[i] ){
    for(i=0;i<fgksize;i++) if( track==fTrack[i]  ){
        fSignal[i] += signal;
        flg = kTRUE;
    } // end for i & if.
    //cout << "track="<<track<<endl;
    if(flg){ // resort arrays.  
        for(i=1;i<fgksize;i++){
            j = i;
            while(j>0 && fSignal[j]>fSignal[j-1]){
                trk = fTrack[j-1];
                hts = fHits[j-1];
                sig = fSignal[j-1];
                fTrack[j-1]  = fTrack[j];
                fHits[j-1]   = fHits[j];
                fSignal[j-1] = fSignal[j];                
                fTrack[j]  = trk;
                fHits[j]   = hts;
                fSignal[j] = sig;
		//cout << "#fTrack["<<j-1<<"]="<<fTrack[j-1]<< " fTrack["<<
		// j<<"]="<<fTrack[j]<<endl;
                j--;
            } // end while
        } // end if i
        return;
    } // end if added to existing and resorted array

    // new entry add it in order.
    // if this signal is <= smallest then don't add it.
    if(signal <= fSignal[fgksize-1]) return;
    for(i=fgksize-2;i>=0;i--){
        if(signal > fSignal[i]){
            fSignal[i+1] = fSignal[i];
            fTrack[i+1]  = fTrack[i];
            fHits[i+1]   = fHits[i];
        }else{
            fSignal[i+1] = signal;
            fTrack[i+1]  = track;
            fHits[i+1]   = hit;
            return; // put it in the right place, now exit.
        } //  end if
	//cout << "$fTrack["<<i+1<<"]="<<fTrack[i+1]<< " fTrack["<<i<<"]="
	//<<fTrack[i]<< " fHits["<<i+1<<"]="<<fHits[i+1]<< " fHits["<<i<<"]="
	//<<fHits[i]<< " fSignal["<<i+1<<"]="<<fSignal[i+1]<< " fSignal["<<i
	//<<"]="<<fSignal[i]<<endl;
    } // end if; end for i
    // Still haven't found the right place. Must be at top of list.
    fSignal[0] = signal;
    fTrack[0]  = track;
    fHits[0]   = hit;
    //cout << "$fTrack["<<0<<"]="<<fTrack[0]<<" fHits["<<0<<"]="<<fHits[0]
    //<<" fSignal["<<0<<"]="<<fSignal[0]<<endl;
    return;
}
//______________________________________________________________________
void IlcTARGETpListItem::AddNoise(Int_t module,Int_t index,Double_t noise){
    // Adds noise to this existing list.
    // Inputs:
    //    Int_t module   The module where this noise occurred
    //    Int_t index    The cell index where this noise occurred
    //    Double_t noise The value of the noise.
    // Outputs:
    //    none.
    // Return:
    //    none.

    if(findex!=index || fmodule!=module) 
        Warning("AddNoise","index=%d != findex=%d or module=%d != fmodule=%d",
            index,findex,module,fmodule);
    fNoise += noise; // Keep track of sum signal.
}
//______________________________________________________________________
void IlcTARGETpListItem::AddSignalAfterElect(Int_t module,Int_t index,Double_t signal){
    // Adds signal after electronics to this existing list.
    // Inputs:
    //    Int_t module   The module where this noise occurred
    //    Int_t index    The cell index where this noise occurred
    //    Double_t signal The value of the signal.
    // Outputs:
    //    none.
    // Return:
    //    none.

    if(findex!=index || fmodule!=module) 
        Warning("AddSignalAfterElect","index=%d != findex=%d or module=%d "
		"!= fmodule=%d",index,findex,module,fmodule);
    fSignalAfterElect += signal; // Keep track of sum signal.
}
//______________________________________________________________________
void IlcTARGETpListItem::Add(IlcTARGETpListItem *pl){
    // Adds the contents of pl to this
    // pl could come from different module and index 
    // Inputs:
    //    IlcTARGETpListItem *pl  an IlcTARGETpListItem to be added to this class.
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t i;
    Double_t sig  = 0.0;
    Double_t sigT = 0.0;

    for(i=0;i<pl->GetNsignals();i++){
        sig = pl->GetSignal(i); 
        if( sig <= 0.0 ) break; // no more signals
        AddSignal(pl->GetTrack(i),pl->GetHit(i),fmodule,findex,sig);
        sigT += sig;
    } // end for i
    fTsignal += (pl->fTsignal - sigT);
    fNoise   += pl->fNoise;
    return;
}
//______________________________________________________________________
void IlcTARGETpListItem::AddTo(Int_t fileIndex,IlcTARGETpListItem *pl){
    // Adds the contents of pl to this with track number off set given by
    // fileIndex.
    // Inputs:
    //    Int_t fileIndex      track number offset value
    //    IlcTARGETpListItem *pl  an IlcTARGETpListItem to be added to this class.
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t i,trk;
    Double_t sig  = 0.0;

    Int_t module = pl->GetModule();
    Int_t index  = pl->GetIndex();
    for(i=0;i<pl->GetNsignals();i++){
        sig = pl->GetSignal(i); 
        if( sig <= 0.0 ) break; // no more signals
        trk = pl->GetTrack(i);
        trk += fileIndex; 
        AddSignal(trk,pl->GetHit(i),module,index,sig);
    } // end for i
    fSignalAfterElect += (pl->fSignalAfterElect + pl->fNoise - fNoise);
    fNoise = pl->fNoise;
    return;
}
//______________________________________________________________________
Int_t IlcTARGETpListItem::ShiftIndex(Int_t in,Int_t trk) const {
    // Shift an index number to occupy the upper four bits. No longer used.
    // Inputs:
    //    Int_t in   The file number
    //    Int_t trk  The track number
    // Outputs:
    //    none.
    // Return:
    //    Int_t The track number with the file number in the upper bits.
    Int_t si = sizeof(Int_t) * 8;
    UInt_t uin,utrk; // use UInt_t to avoid interger overflow-> goes negative.

    uin = in;
    utrk = trk;
    for(Int_t i=0;i<si-4;i++) uin *= 2;
    uin += utrk;
    in = uin;
    return in;
}
//______________________________________________________________________
void IlcTARGETpListItem::Print(ostream *os) const {
    //Standard output format for this class
    // Inputs:
    //    ostream *os  The output stream
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t i;

    *os << fmodule <<","<<findex<<",";
    *os << fgksize <<",";
    for(i=0;i<fgksize;i++) *os << fTrack[i] <<",";
    for(i=0;i<fgksize;i++) *os << fHits[i] <<",";
    for(i=0;i<fgksize;i++) *os << fSignal[i] <<",";
    *os << fTsignal <<","<< fNoise << "," << fSignalAfterElect;
}
//______________________________________________________________________
void IlcTARGETpListItem::Read(istream *is){
    // Standard output streaming function.
    // Inputs:
    //    istream *is The input stream
    // Outputs:
    //    none.
    // Return:
    //    none.
    Int_t i,iss;

    *is >> fmodule >> findex;
    *is >> iss; // read in fgksize
    for(i=0;i<fgksize&&i<iss;i++) *is >> fTrack[i];
    for(i=0;i<fgksize&&i<iss;i++) *is >> fHits[i];
    for(i=0;i<fgksize&&i<iss;i++) *is >> fSignal[i];
    *is >> fTsignal >> fNoise >> fSignalAfterElect;
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETpListItem &source){
    // Standard output streaming function.
    // Inputs:
    //    ostream &os             The output stream
    //    IlcTARGETpListItem &source The IlcTARGETpListItem object to be written out.
    // Outputs:
    //    none.
    // Return:
    //    ostream  The output stream

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETpListItem &source){
    // Standard output streaming function.
    // Inputs:
    //    istream os              The input stream
    //    IlcTARGETpListItem &source The IlcTARGETpListItem object to be inputted
    // Outputs:
    //    none.
    // Return:
    //    istream The input stream.

    source.Read(&os);
    return os;
}
