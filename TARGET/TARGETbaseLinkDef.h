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


#endif
