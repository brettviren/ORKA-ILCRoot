#ifndef ILCANALYSISTASKMCPARTICLEFILTER_H
#define ILCANALYSISTASKMCPARTICLEFILTER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//
//  Analysis task for Kinematic filtering
//  Fill AOD tracks from Kinematic stack
//

#include "IlcAnalysisTaskSE.h"

class IlcAnalysisFilter;
class TString;
class TList;
class IlcAODMCHeader;
class IlcMCParticle;
class TClonesArray;

class IlcAnalysisTaskMCParticleFilter : public IlcAnalysisTaskSE
{
 public:
                                  IlcAnalysisTaskMCParticleFilter();
                                  IlcAnalysisTaskMCParticleFilter( const char* name );
    virtual                      ~IlcAnalysisTaskMCParticleFilter();
    
    // Implementation of interface methods
    virtual                void   UserCreateOutputObjects();
    virtual                void   UserExec( Option_t *option );
    virtual                Bool_t Notify();
    virtual                void   Terminate( Option_t *option );
    // Setters
    virtual                void   SetTrackFilterMother(IlcAnalysisFilter* trackF) { fTrackFilterMother = trackF; }
    
 private:
    Bool_t Select(TParticle* part, Float_t rv, Float_t zv);                
    
    // pivate c'tors to prevent misuse
    IlcAnalysisTaskMCParticleFilter&   operator=(const IlcAnalysisTaskMCParticleFilter& other);
    IlcAnalysisTaskMCParticleFilter(const IlcAnalysisTaskMCParticleFilter& obj);
    void PrintMCParticle(const IlcMCParticle *mcp,Int_t np);


    IlcAnalysisFilter*  fTrackFilterMother;   //  Track Filter
    IlcAODMCHeader *fAODMcHeader;                 // ! MC Header
    TClonesArray *fAODMcParticles;            // ! tclones array of MC Particles
    TList *fHistList;                         // list to store e histograms, only as exchange

    ClassDef( IlcAnalysisTaskMCParticleFilter, 4 ); // Analysis task for Kinematic filtering
};
 
#endif
