#ifndef ILCTARGETCLUSTERFINDERV2_H
#define ILCTARGETCLUSTERFINDERV2_H
////////////////////////////////////////////////////////////////////
//                       TARGET clusterer V2                         //
//                                                                //
//                                                                //
//                                                                //
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch           //
////////////////////////////////////////////////////////////////////  
#include "IlcTARGETClusterFinder.h" 

class IlcTARGETRecPoint;
class IlcRawReader;
class IlcTARGETgeom;

class IlcTARGETClusterFinderV2 : public IlcTARGETClusterFinder {
public:
  IlcTARGETClusterFinderV2(IlcTARGETDetTypeRec* dettyp);
  virtual ~IlcTARGETClusterFinderV2() {;}

  void SetEvent(Int_t event) { fEvent=event; }
  virtual void RawdataToClusters(IlcRawReader* /*rawReader*/,TClonesArray** /*clusters*/) {
    Warning("RawdataToClusters","Method not implemented in this class ");}
  
protected:
  class Ilc1Dcluster {
  public:
    void SetY(Float_t y) {fY=y;}
    void SetQ(Float_t q) {fQ=q;}
    void SetNd(Int_t n)  {fNd=n;}
    void SetLabels(Int_t *lab) {fLab[0]=lab[0];fLab[1]=lab[1];fLab[2]=lab[2];}
    Float_t GetY() const {return fY;}
    Float_t GetQ() const {return fQ;}
    Int_t GetNd()const {return fNd;}
    Int_t GetLabel(Int_t lab) const { return fLab[lab]; }
  protected:
    Float_t fY; //cluster position
    Float_t fQ; //cluster charge
    Int_t fNd;  //number of digits
    Int_t fLab[3]; //track label
  };
  class IlcBin {
  public:
    IlcBin() {fIndex=0; fQ=0; fMask=0xFFFFFFFE;}
    void SetIndex(UInt_t idx) {fIndex=idx;}
    void SetQ(UShort_t q)  {fQ=q;}
    void SetMask(UInt_t m) {fMask=m;}

    void Use() {fMask&=0xFFFFFFFE;}
    Bool_t IsNotUsed() const {return (fMask&1);}
    Bool_t IsUsed() const {return !(IsNotUsed());}

    UInt_t   GetIndex() const {return fIndex;}
    UShort_t GetQ()     const {return fQ;}
    UInt_t   GetMask()  const {return fMask;}
  protected:
    UInt_t fIndex; //digit index
    UInt_t fMask; //peak mask
    UShort_t fQ;  //signal
  };
  void MakeCluster(Int_t k,Int_t max,IlcBin *bins,UInt_t m,IlcTARGETRecPoint &c);
  static Bool_t IsMaximum(Int_t k, Int_t max, const IlcBin *bins);
  static void FindPeaks(Int_t k,Int_t m,IlcBin*b,Int_t*idx,UInt_t*msk,Int_t&n);
  static void MarkPeak(Int_t k, Int_t max, IlcBin *bins, UInt_t m);
  static void FindCluster(Int_t k,Int_t maxz,IlcBin *bins,Int_t &n,Int_t *idx);
  IlcTARGETgeom *LoadGeometry();

protected:
  IlcTARGETClusterFinderV2(const IlcTARGETClusterFinderV2 &source); // copy constructor
  // assignment operator
  IlcTARGETClusterFinderV2& operator=(const IlcTARGETClusterFinderV2 &source);

  static void CheckLabels2(Int_t lab[10]);
  static void AddLabel(Int_t lab[10], Int_t label);      
   
  Int_t fNModules;             // total number of modules    
  Int_t fEvent;                //event number

  ClassDef(IlcTARGETClusterFinderV2,1)  // TARGET cluster finder V2
};
// Input and output functions for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETClusterFinderV2 &source);
istream &operator>>(istream &os,IlcTARGETClusterFinderV2 &source);
#endif
