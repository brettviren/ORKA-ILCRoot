#ifndef ILCFASTEVENT_H
#define ILCFASTEVENT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastEvent.h 14855 2006-08-16 08:35:22Z morsch $ */

#include <TObject.h>
#include <TArrayF.h>

class IlcFastEvent : public TObject {
 public:
    IlcFastEvent();
    virtual ~IlcFastEvent(){;}
    virtual void  SetMultiplicty(Int_t mul) 
	{fMultiplicity = mul;}
    virtual Int_t GetMultiplicty() 
	{return fMultiplicity;}
    virtual void SetVertex(const TArrayF &o) 
	{
	    fEventVertex[0] = o.At(0);
	    fEventVertex[1] = o.At(1);
	    fEventVertex[2] = o.At(2);
	}

    virtual void GetVertex(TArrayF &o) const
	{
	    o[0] = fEventVertex.At(0);
	    o[1] = fEventVertex.At(1);
	    o[2] = fEventVertex.At(2);
	}

 protected:
    Int_t     fMultiplicity;    // Event Multiplicity
    TArrayF   fEventVertex;     // Event primary vertex
    
    ClassDef(IlcFastEvent,1) // Base class for fast event
};

#endif 



