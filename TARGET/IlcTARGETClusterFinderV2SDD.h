#ifndef ILCTARGETCLUSTERFINDERV2SDD_H
#define ILCTARGETCLUSTERFINDERV2SDD_H
//--------------------------------------------------------------
//                       TARGET clusterer V2 for SDD
//
//   This can be a "wrapping" for the V1 cluster finding classes
//   if compiled with uncommented "#define V1" line 
//   in the IlcTARGETclustererV2.cxx file.
//
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//--------------------------------------------------------------
#include "IlcTARGETClusterFinderV2.h"
#include "IlcTARGETDetTypeRec.h"

class TClonesArray;
class IlcRawReader;
class IlcTARGETRawStream;
class IlcTARGETCalibrationSDD;
class IlcTARGETsegmentationSDD;

class IlcTARGETClusterFinderV2SDD : public IlcTARGETClusterFinderV2 {
public:
  IlcTARGETClusterFinderV2SDD(IlcTARGETDetTypeRec* dettyp);
  virtual ~IlcTARGETClusterFinderV2SDD(){;}
  virtual void FindRawClusters(Int_t mod);
  virtual void RawdataToClusters(IlcRawReader* rawReader,TClonesArray** clusters);
 protected:

  void FindClustersSDD(TClonesArray *digits);
  void FindClustersSDD(IlcBin* bins[2], Int_t nMaxBin, Int_t nMaxZ,
		       TClonesArray *dig, TClonesArray *clusters=0x0);

  void FindClustersSDD(IlcTARGETRawStream* input,TClonesArray** clusters);
  void CorrectPosition(Float_t &z, Float_t&y);
  virtual IlcTARGETCalibrationSDD* GetResp(Int_t mod)const{
    return (IlcTARGETCalibrationSDD*) fDetTypeRec->GetCalibrationModel(mod);}
  virtual IlcTARGETsegmentationSDD* GetSeg()const{
    return (IlcTARGETsegmentationSDD*)fDetTypeRec->GetSegmentationModel(1);} 


  Int_t fNySDD;           //number of "pixels" in Y
  Int_t fNzSDD;           //number of "pixels" in Z
  Float_t fYpitchSDD;     //"pixel size" in Y (drift direction)
  Float_t fZpitchSDD;     //"pixel sizes" in Z
  Float_t fHwSDD;         //half width of the SDD detector
  Float_t fHlSDD;         //half length of the SDD detector
  Float_t fYoffSDD;       //some delay in the drift channel   

  ClassDef(IlcTARGETClusterFinderV2SDD,1)  // TARGET cluster finder V2 for SDD
};

#endif
