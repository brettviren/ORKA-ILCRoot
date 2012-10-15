#ifndef ILCTARGETVERTEXER_H
#define ILCTARGETVERTEXER_H

#include<IlcVertexer.h>

///////////////////////////////////////////////////////////////////
//                                                               //
// Base class for primary vertex reconstruction  for TARGET         //
//                                                               //
///////////////////////////////////////////////////////////////////

class TString;
class TClonesArray;


class IlcTARGETVertexer : public IlcVertexer {

 public:
    // default constructor
    IlcTARGETVertexer();   
    // standard constructor     
    IlcTARGETVertexer(TString filename); 
    virtual ~IlcTARGETVertexer(){;}
    virtual void FindMultiplicity(Int_t evnumber);
    virtual void WriteCurrentVertex();
 
    void SetFirstEvent(Int_t ev){fFirstEvent = ev;}
    void SetLastEvent(Int_t ev){fLastEvent = ev;}

 
 protected:
    // copy constructor (NO copy allowed: the constructor is protected
    // to avoid misuse)
// // // //     IlcTARGETVertexer(const IlcTARGETVertexer& vtxr);
    // assignment operator (NO assignment allowed)
    IlcTARGETVertexer& operator=(const IlcTARGETVertexer& /* vtxr */);

    Int_t fFirstEvent;          // First event to be processed by FindVertices
    Int_t fLastEvent;           // Last event to be processed by FindVertices

 
  ClassDef(IlcTARGETVertexer,3);
};

#endif
