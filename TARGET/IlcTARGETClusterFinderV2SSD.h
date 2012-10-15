#ifndef ILCTARGETCLUSTERFINDERV2SSD_H
#define ILCTARGETCLUSTERFINDERV2SSD_H
//--------------------------------------------------------------
//                       TARGET clusterer V2 for SSD
//
//   This can be a "wrapping" for the V1 cluster finding classes
//   if compiled with uncommented "#define V1" line 
//   in the IlcTARGETclustererV2.cxx file.
//
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//--------------------------------------------------------------
#include "IlcTARGETClusterFinderV2.h"
#include "IlcTARGETParam.h"

class TClonesArray;
class IlcRawReader;
class IlcTARGETRawStream;

class IlcTARGETClusterFinderV2SSD : public IlcTARGETClusterFinderV2 {
public:
  IlcTARGETClusterFinderV2SSD(IlcTARGETDetTypeRec* dettyp);
  virtual ~IlcTARGETClusterFinderV2SSD();
  virtual void FindRawClusters(Int_t mod);
  virtual void RawdataToClusters(IlcRawReader* rawReader,TClonesArray** clusters);
 protected:

  void FindClustersSSD(Int_t lay,TClonesArray *digits);
  void FindClustersSSDv1Sens(Int_t lay,TClonesArray *digits);
  void FindClustersSSD(Int_t lay,Ilc1Dcluster* neg, Int_t nn, 
		       Ilc1Dcluster* pos, Int_t np,
		       TClonesArray *clusters=0x0);
  void FindClustersSSD(Int_t lay,Ilc1Dcluster* neg, Int_t nn, TClonesArray *clusters=0x0);
  

  void FindClustersSSD(IlcTARGETRawStream* input,TClonesArray** clusters);

  IlcTARGETParam *fParam;
  Int_t fLastSSD1;        //index of the last SSD1 detector   
  Float_t *fYpitchSSD;     //strip pitch (cm)
  Float_t *fHwSSD;         //half-width of an SSD detector (cm)
  Float_t *fHlSSD;         //half-length of an SSD detector (cm)
  Float_t *fTanP;          //tangent of the stereo angle on the P side
  Float_t *fTanN;          //tangent of the stereo angle on the N side


  ClassDef(IlcTARGETClusterFinderV2SSD,1)  // TARGET cluster finder V2 for SDD
};

#endif
