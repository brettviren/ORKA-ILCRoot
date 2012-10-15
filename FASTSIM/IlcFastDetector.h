#ifndef ILCFASTDETECTOR_H
#define ILCFASTDETECTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastDetector.h 13627 2006-04-27 08:42:35Z morsch $ */

//
// Base class for fast simulation of a detctor
// or a system of subdetectors.
// The detector response is described by resolution and efficiency.
// Author:
// Andreas Morsch
// andreas.morsch@cern.ch

#include <TNamed.h>
class TList;
class TObjLink;
class IlcFastResponse;
class IlcFastParticle;
class IlcGeometry;

class IlcFastDetector : public TNamed {
    
 public:
    IlcFastDetector();
    IlcFastDetector(char* Name, char* Title);
    IlcFastDetector(const IlcFastDetector& det);    
    virtual ~IlcFastDetector();
    virtual void Init();
    virtual void SetGeometry(IlcGeometry* geom) 
	{fGeometry = geom;}
    
    virtual IlcGeometry* GetGeometry() const  
	{return fGeometry;}
    //
    // Add a new subdetector 
    virtual void AddSubdetector(IlcFastDetector *Detector, char* Name);
    virtual TList* Subdetectors() {return fSubdetectors;}
    //
    // Add a new response
    virtual void AddResponse(IlcFastResponse *Response);
    virtual TList* Responses() {return fResponses;}
    virtual Float_t EvaluateEfficiency(IlcFastParticle* part);
    virtual Bool_t  EvaluateAcceptance(IlcFastParticle* part);
    virtual void    EvaluateResponse(IlcFastParticle* part);
    
    // Iterators
    IlcFastDetector*  FirstSubdetector();
    IlcFastDetector*  NextSubdetector();
    IlcFastResponse*  FirstResponse();
    IlcFastResponse*  NextResponse();
    // Copy
    IlcFastDetector& operator=(const IlcFastDetector & rhs);
    void Copy(TObject&) const;
 protected:
    TList            *fSubdetectors;      // List of Subdetectors
    TList            *fResponses;         // Responses
    TObjLink         *fLnkD;              // Pointer to detector in list 
    TObjLink         *fLnkR;              // Pointer to response in list
    IlcFastResponse  *fEfficiency;        // Efficiency Simulation
    IlcFastResponse  *fResolution;        // Resolution Simulation
    IlcGeometry      *fGeometry;          // Geometry 
    ClassDef(IlcFastDetector,1) // Base class for fast detector
};

#endif 



