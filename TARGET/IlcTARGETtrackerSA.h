#ifndef ILCTARGETTRACKERSA_H
#define ILCTARGETTRACKERSA_H 



#include "IlcTARGETtrackerMI.h"

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////
//  Stand alone tracker class                     //
//  Origin:  Elisabetta Crescio                   //
//  e-mail:  crescio@to.infn.it                   //
////////////////////////////////////////////////////

class IlcTARGETclusterTable;
class IlcTARGETtrackSA;
class IlcESDVertex;
class IlcTARGETVertexer;
class TTree;
class TArrayD;
class IlcTARGETParam;

#include <vector>

class IlcTARGETtrackerSA : public IlcTARGETtrackerMI {


 public:

  IlcTARGETtrackerSA();
  IlcTARGETtrackerSA(IlcTARGETgeom *geom);
  IlcTARGETtrackerSA(IlcTARGETgeom *geom,IlcESDVertex *vert);
  IlcTARGETtrackerSA(IlcTARGETgeom *geom,IlcTARGETVertexer *vertexer);
  IlcTARGETtrackerSA(const IlcTARGETtrackerSA& tracker);
  IlcTARGETtrackerSA& operator=(const IlcTARGETtrackerSA& source);
  virtual ~IlcTARGETtrackerSA();  
  virtual Int_t Clusters2Tracks(IlcESDEvent *event){Int_t rc = IlcTARGETtrackerMI::Clusters2Tracks(event); if(!rc) rc=FindTracks(event); return rc;}
  Int_t FindTracks(IlcESDEvent* event);

  IlcTARGETtrackV2* FitTrack(IlcTARGETtrackSA* tr,Double_t* primaryVertex);

  Int_t FindTrackLowChiSquare(TObjArray* tracklist, Int_t dim) const;
  Int_t LoadClusters(TTree *cf) {Int_t rc=IlcTARGETtrackerMI::LoadClusters(cf); SetClusterTree(cf);SetMinPoints(4); return rc;}
  void SetVertex(IlcESDVertex *vtx){fVert = vtx;}
  void SetClusterTree(TTree * itscl){fTARGETclusters = itscl;}
  void SetMinPoints(Int_t sp = 4){fMinPoints = sp;}
  Int_t GetMinPoints() const {return fMinPoints;}
  void SetWindowSizes(Int_t n=46, Double_t *phi=0, Double_t *lam=0);

  enum {kSAflag=0x8000}; //flag to mark clusters used in the SA tracker

  // protected:

  //Initialization
  void Init();
  void ResetForFinding();
  void UpdatePoints();

  static Double_t Curvature(Double_t x1,Double_t y1,Double_t x2,Double_t y2,
                     Double_t x3,Double_t y3);

  Double_t ChoosePoint(Double_t p1, Double_t p2, Double_t pp); 

  static Int_t   FindIntersection(Float_t a1, Float_t b1, Float_t c1, Float_t c2, 
                           Float_t& x1,Float_t& y1, Float_t& x2, Float_t& y2);
  static Int_t   FindEquation(Float_t x1, Float_t y1, Float_t x2, Float_t y2, 
                       Float_t x3, Float_t y3,Float_t& a, Float_t& b, 
                       Float_t& c);
 
  static Int_t Label(Int_t* gl0, Int_t* gl1, Int_t* gl2,Int_t numberofpoints,double &fakeratio);
 
  Int_t SearchClusters(Int_t layer,Double_t phiwindow,Double_t lambdawindow, 
                       IlcTARGETtrackSA* trs,Double_t zvertex,Int_t flagp); 

  void GetCoorAngles(IlcTARGETRecPoint* cl,Int_t module,Double_t &phi,Double_t &lambda,Float_t &x,Float_t &y,Float_t &z,Double_t* vertex);
  void GetCoorErrors(IlcTARGETRecPoint* cl, Int_t module,Float_t &sx,Float_t &sy, Float_t &sz);

  IlcTARGETclusterTable* GetClusterCoord(Int_t layer,Int_t n) const {return (IlcTARGETclusterTable*)fCluCoord[layer]->UncheckedAt(n);}
  void RemoveClusterCoord(Int_t layer, Int_t n) {fCluCoord[layer]->RemoveAt(n);fCluCoord[layer]->Compress();}

  inline int GetMapIndex(double phi,double lam){
    int i,j;
    return GetMapIndex(phi,lam,i,j);
  }
  inline int GetMapIndex(double phi,double lam, int& in, int &jn){
    phi=(phi+TMath::Pi())/TMath::Pi()/2;
    lam=(lam+TMath::Pi()/2)/TMath::Pi();
    if(lam<0.) lam=0.;
    if(lam>=1.) lam=0.9999999;
    in=(int)((phi-TMath::Floor(phi))*fNphiMap);
    jn=(int)(lam*fNlamMap);
    return in*fNlamMap+jn;
  }

  Double_t fPhiEstimate; //Estimation of phi angle on next layer
  Float_t fPoint1[3];   //! coord. of 1-st point to evaluate the curvature
  Float_t fPoint2[3];   //! coord. of 2-nd point to evaluate the curvature
  Float_t fPoint3[3];   //! coord. of 3-rd point to evaluate the curvature
  Float_t fPointc[3];   //! current point coord (for curvature eval.)
  Int_t     fLayer[4];   //! layer of 1,2,3,current points
  Double_t fLambdac;    //! current value of the Lambda angle in the window
  Double_t fPhic;       //! current value of the Phi angle in the window
  Float_t fCoef1;       //! param. of the equation of the circ. approx a layer
  Float_t fCoef2;       //! param. of the equation of the circ. approx a layer
  Float_t fCoef3;       //! param. of the equation of the circ. approx a layer
  Int_t fNloop;         //  Number of iterqations on phi and lambda windows
  Double_t *fPhiWin;    // phi window sizes
  Double_t *fLambdaWin; // lambda window sizes
  IlcESDVertex *fVert;        //! primary vertex
  IlcTARGETVertexer *fVertexer;  //! vertexer 
  IlcTARGETgeom *fGeom;          //! TARGET geometry
  TObjArray *fListOfTracks;   //! container for found tracks 
  TTree *fTARGETclusters;        //! pointer to TARGET tree of clusters
  Int_t fMinPoints;          // If true 6/6 points are required (default). 5/6 otherwise

  TClonesArray** fCluLayer; //! array with clusters 
  TClonesArray** fCluCoord; //! array with cluster info
  std::vector<std::vector<int> >* fClusterMap; 
  int fNphiMap;
  int fNlamMap;
  
  ClassDef(IlcTARGETtrackerSA,2)
};

#endif

