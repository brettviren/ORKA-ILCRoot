#ifndef ILCTARGETVERTEXERIONS_H
#define ILCTARGETVERTEXERIONS_H

#include <IlcTARGETVertexer.h>

//////////////////////////////////////////////////////////////////////
// IlcTARGETVertexerIons  is a class for full 3D primary vertex        //
// finding optimized for Ion-Ion interactions                       //
//                                                                  // 
//                                                                  //
//                                                                  //
//                                                                  //
// Written by Giuseppe Lo Re and Francesco Riggi                    //
// Giuseppe.Lore@ct.infn.it                                         //
// Franco.Riggi@ct.infn.it                                          //
//                                                                  //
// Release date: Mar 2004                                           //
//                                                                  //
//                                                                  //       
//////////////////////////////////////////////////////////////////////


class TH1F;

class IlcTARGETVertexerIons : public IlcTARGETVertexer {

 public:
  IlcTARGETVertexerIons();
  IlcTARGETVertexerIons(TString fn);
  virtual ~IlcTARGETVertexerIons(); // destructor
  virtual IlcESDVertex* FindVertexForCurrentEvent(Int_t event);
  virtual IlcESDVertex* FindVertexForCurrentEvent(TTree*){ Fatal("FindVertexForCurrentEvent","FindVertexForCurrentEvent(TTree*) not yet implemented\n");}
  virtual void FindVertices();
  virtual void PhiFunc(Double_t &x,Double_t &y,Double_t &phi);
  virtual void PrintStatus() const;
  Int_t GetNpThreshold() const {return fNpThreshold;}
  void SetNpThreshold(Int_t t = 500){fNpThreshold = t;}
  Double_t GetMaxDeltaPhi() const {return fMaxDeltaPhi;}
  void SetMaxDeltaPhi(Double_t dphi=0.45) {fMaxDeltaPhi=dphi;}
  Double_t GetMaxDeltaZ() const {return fMaxDeltaPhi;}
  void SetMaxDeltaZ(Double_t dz=0.15) {fMaxDeltaZ=dz;}
  Double_t FindMaxAround(Double_t point, TH1F *h, Double_t distance);
  void SetFirstEvent(Int_t ev){fFirstEvent = ev;}
  void SetLastEvent(Int_t ev){fLastEvent = ev;}

 protected:

  Int_t fNpThreshold;      // minimum number of rec points for vertexing
  Double_t fMaxDeltaPhi;   // Maximum phi difference for rec points correlation
  Double_t fMaxDeltaZ;     // Maximum z difference for rec points correlation
// // // //   IlcTARGETVertexerIons(const IlcTARGETVertexerIons &source); // copy constructor (NO copy allowed: the constructor is protected to avoid misuse)   
  IlcTARGETVertexerIons& operator=(const IlcTARGETVertexerIons &source); // assignment operator (NO assignment allowed)

  Int_t fFirstEvent;          // First event to be processed by FindVertices
  Int_t fLastEvent;           // Last event to be processed by FindVertices

  ClassDef(IlcTARGETVertexerIons,4);
};

#endif
