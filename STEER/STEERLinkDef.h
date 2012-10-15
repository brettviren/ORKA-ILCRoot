#ifdef __CINT__
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: STEERLinkDef.h 54207 2012-01-27 19:17:40Z hristov $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ global gIlc;
#pragma link C++ global gMC;
 
#pragma link C++ enum VertexSmear_t;
#pragma link C++ enum VertexSource_t;

#pragma link C++ class  IlcGenerator+;
#pragma link C++ class  IlcVertexGenerator+;
#pragma link C++ class  IlcRun-;
#pragma link C++ class  IlcModule+;
#pragma link C++ class  IlcDetector+;
#pragma link C++ class  IlcDigit+;
#pragma link C++ class  IlcHit+;
#pragma link C++ class  IlcLego+;
#pragma link C++ class  IlcLegoGenerator+;
#pragma link C++ class  IlcLegoGeneratorXYZ+;
#pragma link C++ class  IlcLegoGeneratorPhiZ+;
#pragma link C++ class  IlcLegoGeneratorEta+;
#pragma link C++ class  IlcLegoGeneratorEtaR+;
#pragma link C++ class  IlcDigitNew+;
#pragma link C++ class  IlcGeometry+;
#pragma link C++ class  IlcRecPoint+;
#pragma link C++ class  IlcHitMap+;
#pragma link C++ class  IlcRndm+;
#pragma link C++ class  IlcDebugVolume+;
#pragma link C++ class  IlcConfig+;
#pragma link C++ class  IlcDigitizer+;
#pragma link C++ class  IlcDigitizationInput+;
#pragma link C++ class  IlcStream+;
#pragma link C++ class  IlcMergeCombi+;
#pragma link C++ class  IlcGausCorr+;
#pragma link C++ class  IlcLoader+;
#pragma link C++ class  IlcDataLoader+;
#pragma link C++ class  IlcBaseLoader+;
#pragma link C++ class  IlcObjectLoader+;
#pragma link C++ class  IlcTreeLoader+;
#pragma link C++ class  IlcRunLoader+;
#pragma link C++ class  IlcReconstructor+;
#pragma link C++ class  IlcMC+;
#pragma link C++ class  IlcSimulation+;
#pragma link C++ class  IlcReconstruction+;
#pragma link C++ class  IlcRecoInputHandler+;
#pragma link C++ class  IlcVertexGenFile+;
#pragma link C++ class  IlcVertexer+;

#pragma link C++ class IlcTriggerDetector+;
#pragma link C++ class IlcCentralTrigger+;
#pragma link C++ class IlcTriggerUtils+;

#pragma link C++ class IlcGeomManager+;
#pragma link C++ class IlcAlignObj+;
#pragma link C++ class IlcAlignObjParams+;
#pragma link C++ class IlcAlignObjMatrix+;
#pragma link C++ class IlcMisAligner+;

#pragma link C++ class IlcTrackFitter+;
#pragma link C++ class IlcTrackFitterRieman+;
#pragma link C++ class IlcTrackFitterKalman+;
#pragma link C++ class IlcTrackFitterStraight+;
#pragma link C++ class IlcTrackResiduals+;
#pragma link C++ class IlcTrackResidualsChi2+;
#pragma link C++ class IlcTrackResidualsFast+;
#pragma link C++ class IlcTrackResidualsLinear+;
#pragma link C++ class IlcAlignmentTracks+;

#pragma link C++ class  IlcRieman;

#pragma link C++ class IlcTriggerDetector+;
#pragma link C++ class IlcCentralTrigger+;
#pragma link C++ class IlcCTPRawStream+;
#pragma link C++ class IlcSignalProcesor+;
#pragma link C++ class  IlcHelix+;
#pragma link C++ class  IlcCluster+;
#pragma link C++ class  IlcCluster3D+;
#pragma link C++ class  IlcTracker+;
#pragma link C++ class  IlcTrackleter+;
#pragma link C++ class  IlcV0+;
#pragma link C++ class  IlcKink+;

#pragma link C++ class  IlcSelectorRL+;

#pragma link C++ class IlcSurveyObj+;
#pragma link C++ class IlcSurveyPoint+;
#pragma link C++ class IlcSurveyToAlignObjs+;

#pragma link C++ class IlcFstream+;
#pragma link C++ class IlcCTPRawData+;

#pragma link C++ class IlcQADataMaker+;
#pragma link C++ class IlcQADataMakerSim+;
#pragma link C++ class IlcQADataMakerRec+;
#pragma link C++ class IlcCorrQADataMakerRec+;
#pragma link C++ class IlcGlobalQADataMaker+;
#pragma link C++ class IlcQAManager+;
#pragma link C++ class IlcQAChecker+;
#pragma link C++ class IlcCorrQAChecker+;
#pragma link C++ class IlcGlobalQAChecker+;
#pragma link C++ class IlcQACheckerBase+;
#pragma link C++ class IlcQAThresholds+;
#pragma link C++ class IlcMillepede+;

#pragma link C++ class IlcPlaneEff+;

#pragma link C++ class IlcTriggerRunScalers+;
#pragma link C++ class IlcGRPPreprocessor+;
#pragma link C++ class IlcGRPRecoParam+;

#pragma link C++ class IlcRelAlignerKalman+;

#pragma link C++ class IlcESDTagCreator+;

#pragma link C++ class IlcGRPObject+;

#pragma link C++ class IlcQAv1+;

#pragma link C++ class IlcRunInfo+;
#pragma link C++ class IlcEventInfo+;
#pragma link C++ class IlcDetectorRecoParam+;
#pragma link C++ class IlcRecoParam+;

#pragma link C++ class IlcMillePede2+;
#pragma link C++ class IlcMillePedeRecord+;
#pragma link C++ class IlcMinResSolve+;
#pragma link C++ class IlcMatrixSparse+;
#pragma link C++ class IlcVectorSparse+;
#pragma link C++ class IlcMatrixSq+;
#pragma link C++ class IlcSymMatrix+;
#pragma link C++ class IlcSymBDMatrix+;
#pragma link C++ class IlcRectMatrix+;
#pragma link C++ class IlcParamSolver+;

#pragma link C++ class IlcGRPManager+;
#pragma link C++ class IlcDCSArray+; 	 
#pragma link C++ class IlcLHCReader+;
#pragma link C++ class IlcCTPTimeParams+;
#pragma link C++ class IlcCTPInputTimeParams+;

#pragma link C++ class IlcLHCDipValT<Double_t>+; 	 
#pragma link C++ class IlcLHCDipValT<Int_t>+; 	 
#pragma link C++ class IlcLHCDipValT<Float_t>+; 	 
#pragma link C++ class IlcLHCDipValT<Char_t>+; 	 
#pragma link C++ class IlcLHCData+;
#pragma link C++ class IlcLHCClockPhase+;

#pragma link C++ class IlcLTUConfig+;

#pragma link C++ typedef IlcLHCDipValD; 	 
#pragma link C++ typedef IlcLHCDipValI; 	 
#pragma link C++ typedef IlcLHCDipValF; 	 
#pragma link C++ typedef IlcLHCDipValC;
#endif
