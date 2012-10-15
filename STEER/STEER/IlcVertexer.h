#ifndef ILCVERTEXER_H
#define ILCVERTEXER_H

///////////////////////////////////////////////////////////////////
//                                                               //
// Base class for primary vertex reconstruction                  //
//                                                               //
///////////////////////////////////////////////////////////////////

#include<TObject.h>

class TTree;
class IlcESDVertex;
class IlcMultiplicity;


class IlcVertexer : public TObject {

 public:
    // default constructor
    IlcVertexer();  
 
    // destructor
    virtual ~IlcVertexer();
    // computes the vertex for the current event
    virtual IlcESDVertex* FindVertexForCurrentEvent(TTree *clustersTree)=0; 
    // computes the vertex for each event and stores it on file
    IlcMultiplicity* GetMultiplicity() const {return fMult;}
    virtual void PrintStatus() const = 0;
    void SetVtxStart(Double_t x,Double_t y,Double_t z) 
      { fNominalPos[0]=x; fNominalPos[1]=y; fNominalPos[2]=z; }
    void SetVtxStartSigma(Double_t sx,Double_t sy,Double_t sz) 
      { fNominalCov[0]=sx*sx; fNominalCov[2]=sy*sy; fNominalCov[5]=sz*sz;
      fNominalCov[1]=0.; fNominalCov[3]=0.; fNominalCov[4]=0.; }
    void SetVtxStart(IlcESDVertex *vtx);
    // the following method can be implemented in daughter classes 
    // (e.g. in IlcITSVertexer3D). It is intended to tag pile-up events
    // novertices is the total number of vertices (1 means no pileup)
    // The returned pointer points to an array of IlcESDVertx opbjects
    // with size=novertices
    virtual  IlcESDVertex* GetAllVertices(Int_t &novertices) const {novertices = 0; return NULL;}
    const Double_t* GetNominalPos() const {return fNominalPos;}
    const Double_t* GetNominalCov() const {return fNominalCov;}

 protected:
    IlcESDVertex *fCurrentVertex; //! pointer to the current vertex
    IlcMultiplicity *fMult;     //! Multiplicity object
 
 private:
    // copy constructor (NO copy allowed: the constructor is protected
    // to avoid misuse)
    IlcVertexer(const IlcVertexer& vtxr);
    // assignment operator (NO assignment allowed)
    IlcVertexer& operator=(const IlcVertexer& /* vtxr */);

    Double_t  fNominalPos[3];   // initial knowledge on vertex position
    Double_t  fNominalCov[6];   // initial knowledge on vertex position

  ClassDef(IlcVertexer,4);
};

#endif
