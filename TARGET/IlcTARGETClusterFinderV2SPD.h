#ifndef ILCTARGETCLUSTERFINDERV2SPD_H
#define ILCTARGETCLUSTERFINDERV2SPD_H
//--------------------------------------------------------------
//                       TARGET clusterer V2 for SPD
//
//   This can be a "wrapping" for the V1 cluster finding classes
//   if compiled with uncommented "#define V1" line 
//   in the IlcTARGETclustererV2.cxx file.
//
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//--------------------------------------------------------------
#include "IlcTARGETClusterFinderV2.h"
#include "IlcTARGETgeomSPD.h"
#include "IlcTARGETParam.h"

class TClonesArray;
class IlcRawReader;
class IlcTARGETRawStream;
class IlcTARGETgeom;

class IlcTARGETClusterFinderV2SPD : public IlcTARGETClusterFinderV2 {
public:
  IlcTARGETClusterFinderV2SPD(IlcTARGETDetTypeRec* dettyp);
  virtual ~IlcTARGETClusterFinderV2SPD();
  virtual void FindRawClusters(Int_t mod);
  virtual void RawdataToClusters(IlcRawReader* rawReader,TClonesArray** clusters);
  

 protected:

  void FindClustersSPD(Int_t lay,TClonesArray *digits);
  void FindClustersSPD(IlcTARGETRawStream* input,TClonesArray** clusters);
  Int_t ClustersSPD(Int_t lay,IlcBin* bins, TClonesArray* digits,TClonesArray* clusters,Int_t maxBins, Int_t nzbins,Int_t iModule,Bool_t rawdata=kFALSE);

  IlcTARGETParam *fParam;
  Int_t fLastSPD1;       //index of the last SPD1 detector
  Int_t *fNxSPD;          //number of pixels in X
  Int_t *fNySPD;          //number of pixels in Y
  Int_t *fNzSPD;          //number of pixels in Z
  Double_t *fXpitchSPD;    //pixel size in X
  Double_t *fYpitchSPD;    //pixel size in Y
  Double_t *fZ1pitchSPD,*fZ2pitchSPD;    //pixel sizes in Z
  Double_t *fHwSPD;        //half width of the SPD detector
  Double_t *fHlSPD;        //half length of the SPD detector
  Double_t fXSPD[32][100000];    //X-coordinates of pixel centers era Double_t fYSPD[13][10000]
  Double_t fYSPD[32][100000];    //Y-coordinates of pixel centers era Double_t fYSPD[13][10000]
  Double_t fZSPD[32][100000];    //Z-coordinates of pixel centers era Double_t fZSPD[13][10000]
  static IlcBin *fBins;
  static int fMaxNbins;

  ClassDef(IlcTARGETClusterFinderV2SPD,1)  // TARGET cluster finder V2 for SPD
};

#endif
