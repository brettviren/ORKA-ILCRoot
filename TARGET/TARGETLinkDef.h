#ifdef __CINT__
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: TARGETbaseLinkDef.h,v 1.2 2008/07/30 19:15:48 rucco Exp $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
//#pragma link C++ enum   Cluster_t;

//#pragma link C++ global gTARGETdisplay;  // global used by IlcTARGETdisplay

// Standard TARGET classes 

#pragma link C++ class  IlcTARGETRecPoint+;
#pragma link C++ class  IlcTARGETclusterV2+; 
#pragma link C++ class  IlcTARGETdigit+;
#pragma link C++ class  IlcTARGETdigitSPD+;
#pragma link C++ class  IlcTARGETdigitSDD+;
#pragma link C++ class  IlcTARGETdigitSSD+;
#pragma link C++ class  IlcTARGETTransientDigit+;
#pragma link C++ class  IlcTARGETgeom+;
#pragma link C++ class  IlcTARGETParam+;
#pragma link C++ class  IlcTARGETgeomMatrix-;
#pragma link C++ class  IlcTARGETgeomSPD+;
#pragma link C++ class  IlcTARGETgeomSDD+;
#pragma link C++ class  IlcTARGETgeomSSD+;
// Standard TARGET detector class initilizers
#pragma link C++ class IlcTARGETgeomSPD300+;
#pragma link C++ class IlcTARGETgeomSPD425Short+;
#pragma link C++ class IlcTARGETgeomSPD20Short+;
#pragma link C++ class IlcTARGETgeomSPD20Long+;
#pragma link C++ class IlcTARGETgeomSPD425Long+;
#pragma link C++ class IlcTARGETgeomSDD256+;
#pragma link C++ class IlcTARGETgeomSDD300+;
#pragma link C++ class IlcTARGETgeomSSD175+;
#pragma link C++ class IlcTARGETgeomSSD275and75+;
#pragma link C++ class IlcTARGETgeomSSD75and275+;
#pragma link C++ class IlcTARGETgeomSSDStripEnd+;
#pragma link C++ class IlcTARGETgeomSSDStripLay+;
#pragma link C++ class IlcTARGETgeomSPDPixel+;
#pragma link C++ class IlcTARGETgeomSPDFiberSegm+;
#pragma link C++ class IlcTARGETgeomSPDPixelEnd+;
#pragma link C++ class IlcTARGETgeomSPDPixelEndv4th+;
#pragma link C++ class  IlcTARGETMap+;
#pragma link C++ class  IlcTARGETMapA1+;
#pragma link C++ class  IlcTARGETMapA2+;
#pragma link C++ class  IlcTARGETsegmentation+;
#pragma link C++ class  IlcTARGETsegmentation2+;
#pragma link C++ class  IlcTARGETsegmentationSPD+;
#pragma link C++ class  IlcTARGETsegmentationSPD2+;
#pragma link C++ class  IlcTARGETsegmentationSDD+;
#pragma link C++ class  IlcTARGETsegmentationSSD+;
#pragma link C++ class  IlcTARGETresponse+;
#pragma link C++ class  IlcTARGETCalibration+;
#pragma link C++ class  IlcTARGETresponseSPD+;
#pragma link C++ class  IlcTARGETresponseSDD+;
#pragma link C++ class  IlcTARGETresponseSSD+;
#pragma link C++ class  IlcTARGETCalibrationSPD+;
#pragma link C++ class  IlcTARGETCalibrationSDD+;
#pragma link C++ class  IlcTARGETCalibrationSSD+;
#pragma link C++ class  IlcTARGETpList+;
#pragma link C++ class  IlcTARGETpListItem+;

#pragma link C++ class  IlcTARGETRawData+;
// These streamers must be formatted according to the raw data fromat
#pragma link C++ class  IlcTARGETInStream+;
#pragma link C++ class  IlcTARGETOutStream+;
//
// Raw data
#pragma link C++ class IlcTARGETDDLRawData+;

#pragma link C++ class IlcTARGETLoader+;

#pragma link C++ class IlcTARGETRawStream+;
#pragma link C++ class IlcTARGETRawStreamSDD+;
#pragma link C++ class IlcTARGETRawStreamSDDv2+;
#pragma link C++ class IlcTARGETRawStreamSDDv3+;
#pragma link C++ class IlcTARGETRawStreamSPD+;
#pragma link C++ class IlcTARGETRawStreamSSD+;
#pragma link C++ class IlcTARGETRawStreamSSDv1+;
#pragma link C++ class IlcTARGETEventHeader+;
// SPD preprocessing
#pragma link C++ class IlcTARGETBadChannelsAuxSPD+;
#pragma link C++ class IlcTARGETBadChannelsSPD+;
#pragma link C++ class IlcTARGETChannelSPD+;
#pragma link C++ class IlcTARGETPreprocessorSPD+;

// Standard TARGET classes

#pragma link C++ class  IlcTARGET+;
#pragma link C++ class  IlcTARGETvORKA+;
#pragma link C++ class  IlcTARGEThit+;
#pragma link C++ class  IlcTARGETGeant3Geometry+;
// Standard TARGET detector class initializers
#pragma link C++ class  IlcTARGETmodule+;
#pragma link C++ class  IlcTARGETsimulation+;
#pragma link C++ class  IlcTARGETsimulationSPD+;
#pragma link C++ class  IlcTARGETsimulationSDD+;
#pragma link C++ class  IlcTARGETsimulationSSD+;
#pragma link C++ class  IlcTARGETTableSSD+;
#pragma link C++ class  IlcTARGETsimulationFastPoints+;
#pragma link C++ class  IlcTARGETsimulationFastPointsV0+;
#pragma link C++ class  IlcTARGETDetTypeSim+;
#pragma link C++ class  IlcTARGETstatistics+;
#pragma link C++ class  IlcTARGETstatistics2+;
// These streamers must be formatted according to the raw data fromat

#pragma link C++ class  IlcTARGETHNode+;
#pragma link C++ class  IlcTARGETHTable+;
#pragma link C++ class  IlcTARGETetfSDD+;
// SSD simulation and reconstruction
#pragma link C++ class  IlcTARGETdcsSSD+;
#pragma link C++ class  IlcTARGETsDigitize+;
#pragma link C++ class  IlcTARGETDigitizer+;
#pragma link C++ class  IlcTARGETFDigitizer+;
// Raw data

#pragma link C++ class IlcTARGETTrigger+;
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
