#ifdef __CINT__
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: TARGETrecLinkDef.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
//#pragma link C++ enum   Cluster_t;

//#pragma link C++ global gTARGETdisplay;  // global used by IlcTARGETdisplay

// Standard TARGET classes 
 
#pragma link C++ class  IlcTARGETRawCluster+;
#pragma link C++ class  IlcTARGETRawClusterSPD+;
#pragma link C++ class  IlcTARGETRawClusterSDD+;
#pragma link C++ class  IlcTARGETRawClusterSSD+;
#pragma link C++ class  IlcTARGETClusterFinder+;
#pragma link C++ class  IlcTARGETClusterFinderSPD+;
//#pragma link C++ class  IlcTARGETClusterFinderSPDdubna+;
#pragma link C++ class  IlcTARGETClusterFinderSDD+;
#pragma link C++ class  IlcTARGETClusterFinderSSD+;
#pragma link C++ class  IlcTARGETDetTypeRec+;

#pragma link C++ class  IlcTARGETclusterSSD+;
#pragma link C++ class  IlcTARGETpackageSSD+;
// Classes used for Tracking
//#pragma link C++ class  IlcTARGETTrackV1+;
#pragma link C++ class  IlcTARGETRad+;
#pragma link C++ class  IlcTARGETIOTrack+;
//#pragma link C++ class  IlcTARGETTrackerV1+;
#pragma link C++ class  IlcTARGETRiemannFit+;

#pragma link C++ class IlcTARGETclustererV2+;
#pragma link C++ class IlcTARGETtrackV2+;
#pragma link C++ class IlcTARGETtrackerV2+;
#pragma link C++ class IlcTARGETtrackMI+;
#pragma link C++ class IlcTARGETtrackerMI+;
//#pragma link C++ class IlcTARGETRecV0Info+;

#pragma link C++ class  IlcTARGETVertexer+;
#pragma link C++ class  IlcTARGETVertexerIons+;
#pragma link C++ class  IlcTARGETVertexerPPZ+;
#pragma link C++ class  IlcTARGETVertexerZ+;
#pragma link C++ class  IlcTARGETVertexer3D+;

// Classes for neural tracking
#pragma link C++ class IlcTARGETNeuralPoint+;
#pragma link C++ class IlcTARGETNeuralTrack+;
#pragma link C++ class IlcTARGETNeuralTracker+;
#pragma link C++ class IlcTARGETtrackerANN+;
// Tasks
#pragma link C++ class IlcTARGETreconstruction+;
#pragma link C++ class IlcTARGETFindClustersV2+;
//#pragma link C++ class DisplayTARGETv11+;

#pragma link C++ class IlcTARGETclusterTable+;
#pragma link C++ class IlcTARGETtrackerSA+;
#pragma link C++ class IlcTARGETtrackSA+;
#pragma link C++ class IlcTARGETVertexerFast+;
#pragma link C++ class IlcTARGETReconstructor+;
#pragma link C++ class IlcTARGETClusterFinderV2+;
#pragma link C++ class IlcTARGETClusterFinderV2SDD+;
#pragma link C++ class IlcTARGETClusterFinderV2SPD+;
#pragma link C++ class IlcTARGETClusterFinderV2SSD+;

// Classes for PID
#pragma link C++ class  IlcTARGETPid+;
#pragma link C++ class  IlcTARGETtrackV2Pid+;
#pragma link C++ class IlcTARGETPidParItem+;
#pragma link C++ class IlcTARGETPident+;
#pragma link C++ class IlcTARGETSteerPid+;
#pragma link C++ class IlcTARGETpidESD+;
#pragma link C++ class IlcTARGETpidESD1+;
#pragma link C++ class IlcTARGETpidESD2+;
//beam test classes
#pragma link C++ class IlcTARGETBeamTestDig+;
#pragma link C++ class IlcTARGETBeamTestDigSPD+;
#pragma link C++ class IlcTARGETBeamTestDigSDD+;
#pragma link C++ class IlcTARGETBeamTestDigSSD+;
#pragma link C++ class IlcTARGETBeamTestDigitizer+;
//multiplicity
#pragma link C++ class IlcTARGETMultReconstructor+;

#endif
