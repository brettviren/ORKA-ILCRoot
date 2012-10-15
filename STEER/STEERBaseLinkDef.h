#ifdef __CINT__
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: STEERBaseLinkDef.h 58119 2012-08-07 10:32:21Z hristov $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ enum   IlcLog::EType_t;
 
#pragma link C++ class IlcVParticle+;
#pragma link C++ class IlcVTrack+;
#pragma link C++ class IlcVCluster+;
#pragma link C++ class IlcVCaloCells+;
#pragma link C++ class IlcVVertex+;
#pragma link C++ class IlcVEvent+;
#pragma link C++ class IlcVHeader+;
#pragma link C++ class IlcVEventHandler+;
#pragma link C++ class IlcVEventPool+;
#pragma link C++ class IlcVCuts+;
#pragma link C++ class IlcVVZERO+;
#pragma link C++ class IlcVZDC+;
#pragma link C++ class IlcCentrality+;
#pragma link C++ class IlcEventplane+;

#pragma link C++ class IlcMixedEvent+;

#pragma link C++ class IlcPID+;
#pragma link C++ class IlcLog+;

#pragma link C++ class IlcRunTag+;
#pragma link C++ class IlcLHCTag+;
#pragma link C++ class IlcDetectorTag+;
#pragma link C++ class IlcEventTag+;
#pragma link C++ class IlcFileTag+;

#pragma link C++ class IlcRunTagCuts+;
#pragma link C++ class IlcLHCTagCuts+;
#pragma link C++ class IlcDetectorTagCuts+;
#pragma link C++ class IlcEventTagCuts+;

#pragma link C++ class IlcTagCreator+;

#pragma link C++ class IlcHeader+;
#pragma link C++ class IlcGenEventHeader+;
#pragma link C++ class IlcDetectorEventHeader+;
#pragma link C++ class IlcGenCocktailEventHeader+;
#pragma link C++ class IlcGenPythiaEventHeader+;
#pragma link C++ class IlcGenHijingEventHeader+;
#pragma link C++ class IlcCollisionGeometry+;
#pragma link C++ class IlcGenDPMjetEventHeader+;
#pragma link C++ class IlcGenHerwigEventHeader+;
#pragma link C++ class IlcGenGeVSimEventHeader+;
#pragma link C++ class IlcGenEposEventHeader+;
#pragma link C++ class IlcStack+;
#pragma link C++ class IlcMCEventHandler+;
#pragma link C++ class IlcInputEventHandler+;

#pragma link C++ class IlcTrackReference+;
#pragma link C++ class IlcSysInfo+;

#pragma link C++ class IlcMCEvent+;
#pragma link C++ class IlcMCParticle+;
#pragma link C++ class IlcMCVertex+;

#pragma link C++ class  IlcMagF+;
#pragma link C++ class  IlcMagWrapCheb+;
#pragma link C++ class  IlcCheb3DCalc+;
#pragma link C++ class  IlcCheb3D+;

#pragma link C++ class  IlcNeutralTrackParam+;

#pragma link C++ class IlcCodeTimer+;
#pragma link C++ class IlcCodeTimer::IlcPair+;

#pragma link C++ class  IlcPDG+;

#pragma link C++ class IlcTimeStamp+;
#pragma link C++ class IlcTriggerScalers+;
#pragma link C++ class IlcTriggerScalersRecord+;

#pragma link C++ class  IlcExternalTrackParam+;
#pragma link C++ class IlcQA+;

#pragma link C++ class IlcTRDPIDReference+;
#pragma link C++ class IlcTRDPIDParams+;
#pragma link C++ class IlcTRDPIDParams::IlcTRDPIDThresholds+;
#pragma link C++ class IlcTRDPIDResponseObject+;
#pragma link C++ class IlcTRDTKDInterpolator+;
#pragma link C++ class IlcTRDTKDInterpolator::IlcTRDTKDNodeInfo+;
#pragma link C++ class IlcITSPidParams+;
#pragma link C++ class IlcPIDResponse+;
#pragma link C++ class IlcITSPIDResponse+;
#pragma link C++ class IlcTPCPIDResponse+;
#pragma link C++ class IlcTPCdEdxInfo+;
#pragma link C++ class IlcTOFPIDResponse+;
#pragma link C++ class IlcTRDPIDResponse+;
#pragma link C++ class IlcEMCALPIDResponse+;
#pragma link C++ class IlcPIDCombined+;
#pragma link C++ class IlcTOFHeader+;

#pragma link C++ class IlcDAQ+;
#pragma link C++ class IlcRefArray+;

#pragma link C++ class IlcOADBContainer+;

#pragma link C++ class IlcMathBase+;
#pragma link C++ class  TTreeDataElement+;
#pragma link C++ class  TTreeStream+;
#pragma link C++ class  TTreeSRedirector+;

#pragma link C++ class IlcVMFT+;
#pragma link C++ class IlcCounterCollection+;

#pragma link C++ class IlcTOFPIDParams+;

#endif
