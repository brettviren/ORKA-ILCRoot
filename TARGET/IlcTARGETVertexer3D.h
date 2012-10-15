#ifndef ILCTARGETVERTEXER3D_H
#define ILCTARGETVERTEXER3D_H

#include<IlcTARGETVertexerZ.h>

///////////////////////////////////////////////////////////////////
//                                                               //
// Class for primary vertex finding  (3D reconstruction)         //
//                                                               //
///////////////////////////////////////////////////////////////////

class IlcVertex;
class TClonesArray;

class IlcTARGETVertexer3D : public IlcTARGETVertexer {

 public:

  IlcTARGETVertexer3D();
  IlcTARGETVertexer3D(TString filename);
  virtual ~IlcTARGETVertexer3D();
  virtual IlcESDVertex* FindVertexForCurrentEvent(Int_t evnumb);
  virtual void FindVertices();
  IlcVertex *GetVertex3D() const {return fVert3D;}
  virtual void MakeTracklet(Double_t *pA, Double_t *pB, Int_t &nolines);
  virtual void MakeTracklet(Float_t *pA, Float_t *pB, Int_t &nolines);
  virtual void PrintStatus() const;
  void SetCoarseDiffPhiCut(Float_t dphi = 0.5){fCoarseDiffPhiCut=dphi;}
  void SetMaxRCut(Float_t rad = 2.5){fMaxRCut=rad;}
  void SetZCutDiamond(Float_t zcut = 5.3){fZCutDiamond=zcut;}
  void SetDCAcut(Float_t dca=0.05){fDCAcut=dca;} 
  void SetDiffPhiMax(Float_t pm = 0.01){fDiffPhiMax = pm;}

protected:
  IlcTARGETVertexer3D(const IlcTARGETVertexer3D& vtxr);
  IlcTARGETVertexer3D& operator=(const IlcTARGETVertexer3D& /* vtxr */);
  Int_t FindTracklets(Int_t evnumber, Int_t optCuts);
  void Find3DVertex();
  Int_t Prepare3DVertex();

  TClonesArray *fLines;      //! array of tracklets
  IlcVertex *fVert3D;        // 3D Vertex
  Float_t fCoarseDiffPhiCut; // loose cut on DeltaPhi for RecPoint matching 
  Float_t fMaxRCut; // cut on tracklet DCA to Z axis
  Float_t fZCutDiamond;   // cut on +-Z of the diamond
  Float_t fDCAcut; // cut on tracklet to tracklet and tracklet to vertex DCA
  Float_t fDiffPhiMax;     // Maximum delta phi allowed among corr. pixels
  ClassDef(IlcTARGETVertexer3D,1);

};

#endif
