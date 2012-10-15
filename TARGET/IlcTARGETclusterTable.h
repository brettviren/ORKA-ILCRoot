#ifndef ILCTARGETCLUSTERTABLE_H
#define ILCTARGETCLUSTERTABLE_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
//////////////////////////////////////////////////////////////////////////
// Class used to simplify some operations with clusters.                 //
// -Function FillArray fills an array wich contains, for each            //
//  TARGET module, an array with the indices of all the clusters detected   //
//  by the module. The indices correspond to the cluster indices in class// 
//  IlcTARGETlayer of IlcTARGETtrackerV2.                                      //
//  This function is used in IlcTARGETtrackerSA::FindTracks.                // 
// -Function FillArrayLabel fills an array wich contains, for each       //
//  particle label, and for each layer, the information on clusters:     //
//  0 if there is no cluster, 1 if there is a cluster with this label.   //
//  This function is used to define trackable tracks.                    //   
///////////////////////////////////////////////////////////////////////////


#include <TObject.h>


class IlcTARGETclusterTable : public TObject {


 public:

  IlcTARGETclusterTable();
  IlcTARGETclusterTable(Float_t x, Float_t y, Float_t z, Float_t sx, Float_t sy, Float_t sz, Double_t phi, Double_t lambda, Int_t index, int mapindex);
  virtual ~IlcTARGETclusterTable(){;}

  Int_t   GetOrInd() const {return fOrInd;}
  Int_t   GetMapIndex() const {return fMapIndex;}
  Float_t GetX() const {return fX;}
  Float_t GetY() const {return fY;}
  Float_t GetZ() const {return fZ;}
  Float_t GetSx() const {return fSx;}
  Float_t GetSy() const {return fSy;}
  Float_t GetSz() const {return fSz;}
  Float_t GetPhi() const {return fPhi;}
  Float_t GetLambda() const {return fLam;}

 protected: 

  Int_t   fOrInd; // original index in tracker
  Int_t   fMapIndex; // index in map
  Float_t fX;  //x of cluster 
  Float_t fY;  //y of cluster
  Float_t fZ;  //z of cluster
  Float_t fSx; // error on x
  Float_t fSy; // error on y
  Float_t fSz; // error on z
  Double_t fPhi; // azimuthal angle
  Double_t fLam; // lambda angle

  ClassDef(IlcTARGETclusterTable,2)
};

#endif



