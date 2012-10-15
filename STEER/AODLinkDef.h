#ifdef __CINT__
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: AODLinkDef.h 58340 2012-08-28 15:56:24Z hristov $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ enum   IlcAODVertex::AODVtx_t;
#pragma link C++ enum   IlcAODTrack::AODTrk_t;
#pragma link C++ enum   IlcAODTrack::AODTrkPID_t;
//#pragma link C++ enum   IlcAODCluster::AODClu_t;
//#pragma link C++ enum   IlcAODCluster::AODCluPID_t;

#pragma link C++ class IlcAODEvent+;
#pragma link C++ class IlcAODHeader+;
#pragma link C++ class IlcAODTrack+;

#pragma read                                              \
    sourceClass="IlcAODPid"                               \
    targetClass="IlcAODPid"                               \
    source="UShort_t fTPCsignalN; Double_t fTPCmomentum; Float_t fTRDmomentum[6]"  \
    version="[-10]"                                       \
    target="fTPCsignalN, fTPCmomentum, fTRDnSlices, fTRDmomentum"                                          \
    code="{fTPCsignalN=(UChar_t)onfile.fTPCsignalN; fTPCmomentum=onfile.fTPCmomentum; for (Int_t i=0;i<6;++i) fTRDmomentum[i]=onfile.fTRDmomentum[i];}" 

#pragma link C++ class IlcAODPid+;
#pragma link C++ class IlcAODVertex+;
#pragma link C++ class IlcAODCluster+;
#pragma link C++ class IlcAODCaloCluster+;
#pragma link C++ class IlcAODPmdCluster+;
#pragma link C++ class IlcAODFmdCluster+;
#pragma link C++ class IlcAODJet+;
#pragma link C++ class IlcAODJetEventBackground+;
#pragma link C++ class IlcAODPhoton+;
#pragma link C++ class IlcAODRedCov<3>+;
#pragma link C++ class IlcAODRedCov<4>+;
#pragma link C++ class IlcAODRedCov<6>+;
#pragma link C++ class IlcAODRecoDecay+;
#pragma link C++ class IlcAODv0+;
#pragma link C++ class IlcAODcascade+;
#pragma link C++ class IlcAODHandler+;
#pragma link C++ class IlcAODExtension+;
#pragma link C++ class IlcAODBranchReplicator+;
#pragma link C++ class IlcAODInputHandler+;
#pragma link C++ class IlcAODTracklets+;
#pragma link C++ class IlcAODTagCreator+;
#pragma link C++ class IlcAODCaloCells+;
#pragma link C++ class IlcAODCaloTrigger+;
#pragma link C++ class IlcAODDiJet+;
#pragma link C++ class IlcAODMCParticle+;
#pragma link C++ class IlcAODMCHeader+;
#pragma link C++ class IlcAODPWG4Particle+;
#pragma link C++ class IlcAODPWG4ParticleCorrelation+;
#pragma link C++ class IlcAODDimuon+;
#pragma link C++ class IlcAODpidUtil+;
#pragma link C++ class IlcAODTZERO+;
#pragma link C++ class IlcAODVZERO+;
#pragma link C++ class IlcAODZDC+;
#pragma link C++ class IlcAODHMPIDrings+;


#endif
