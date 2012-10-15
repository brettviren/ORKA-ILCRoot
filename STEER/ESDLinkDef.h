#ifdef __CINT__
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: ESDLinkDef.h 54863 2012-02-28 12:10:29Z hristov $ */

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ enum   IlcESDEvent::ESDListIndex;


#pragma link C++ class  IlcESD+;
#pragma link C++ class  IlcESDEvent+;
#pragma link C++ class  IlcESDInputHandler+;
#pragma link C++ class  IlcESDInputHandlerRP+;
#pragma link C++ class  IlcESDRun+;
#pragma link C++ class  IlcESDHeader+;
#pragma link C++ class  IlcESDHLTDecision+;
#pragma link C++ class  IlcESDZDC+;
#pragma link C++ class  IlcESDCaloTrigger+;

#pragma read sourceClass="IlcESDCaloTrigger" targetClass="IlcESDCaloTrigger" source="Char_t fTriggerBits[48][64]" version="[2]" \
  target="fNEntries, fColumn, fRow, fTriggerBits" targetType="Int, Int_t*, Int_t*, Int_t*" code="{fTriggerBits = new Int_t[fNEntries]; for (Int_t i=0; i<fNEntries; ++i) fTriggerBits[i]=onfile.fTriggerBits[fColumn[i]][fRow[i]];}"

#pragma link C++ class  IlcESDfriend+;                                                                                                           
#pragma read sourceClass="IlcESDtrack" targetClass="IlcESDtrack" source="UChar_t fTRDpidQuality"  version="[-49]" target="fTRDntracklets" targetType="UChar_t" code="{fTRDntracklets = onfile.fTRDpidQuality;}"
// see http://root.cern.ch/svn/root/trunk/io/doc/DataModelEvolution.txt
#pragma link C++ class  IlcESDtrack+;
#pragma read sourceClass="IlcESDfriendTrack" targetClass="IlcESDfriendTrack" source="Int_t fITSindex" version="[-3]" \
        target="fnMaxITScluster, fITSindex" targetType="Int_t, Int_t*" code="{fnMaxITScluster = 12; fITSindex= new Int_t[fnMaxITScluster]; memcpy(fITSindex, &(onfile.fITSindex), fnMaxITScluster*sizeof(Int_t));}"
#pragma read sourceClass="IlcESDfriendTrack" targetClass="IlcESDfriendTrack" source="Int_t fTPCindex" version="[-3]" \
        target="fnMaxTPCcluster, fTPCindex" targetType="Int_t, Int_t*" code="{fnMaxTPCcluster = 160; fTPCindex= new Int_t[fnMaxTPCcluster]; memcpy(fTPCindex, &(onfile.fTPCindex), fnMaxTPCcluster*sizeof(Int_t));}"
#pragma read sourceClass="IlcESDfriendTrack" targetClass="IlcESDfriendTrack" source="Int_t fTRDindex" version="[-3]" \
        target="fnMaxTRDcluster, fTRDindex" targetType="Int_t, Int_t*" code="{fnMaxTRDcluster = 180; fTRDindex= new Int_t[fnMaxTRDcluster]; memcpy(fTRDindex, &(onfile.fTRDindex), fnMaxTRDcluster*sizeof(Int_t));}"

#pragma link C++ class  IlcESDfriendTrack+;
#pragma link C++ class  IlcESDMuonTrack+;
#pragma link C++ class  IlcESDPmdTrack+;
#pragma link C++ class  IlcESDTrdTrigger+;
#pragma link C++ class  IlcESDTrdTrack+;
#pragma link C++ class  IlcESDTrdTracklet+;
#pragma link C++ class  IlcESDHLTtrack+;
#pragma link C++ class  IlcESDv0+;
#pragma link C++ class  IlcESDcascade+;
#pragma link C++ class  IlcVertex+;
#pragma link C++ class  IlcESDVertex+;
#pragma link C++ class  IlcESDpid+;
#pragma link C++ class  IlcESDkink+;
#pragma link C++ class  IlcESDV0Params+;
#pragma link C++ class  IlcESDCaloCluster+;
#pragma link C++ class  IlcESDMuonCluster+;
#pragma link C++ class  IlcESDMuonPad+;

#pragma link C++ class  IlcKFParticleBase+;
#pragma link C++ class  IlcKFParticle+;
#pragma link C++ class  IlcKFVertex+;

#pragma link C++ class  IlcKalmanTrack+;
#pragma link C++ class  IlcVertexerTracks+;
#pragma link C++ class  IlcStrLine+;
#pragma link C++ class  IlcTrackPointArray+;
#pragma link C++ class  IlcTrackPoint+;

#pragma link C++ class IlcTrackPointArray+;
#pragma link C++ class IlcTrackPoint+;

#pragma link C++ class  IlcESDFMD+;
#pragma link C++ class  IlcFMDMap+;
#pragma link C++ class  IlcFMDFloatMap+;

#pragma link C++ class  IlcESDVZERO+;
#pragma link C++ class  IlcESDTZERO+;
#pragma link C++ class  IlcESDACORDE+;
#ifdef MFT_UPGRADE
//#pragma link C++ class  IlcESDMFT+;
#endif

#pragma link C++ class  IlcESDMultITS+;
#pragma link C++ class  IlcMultiplicity+;

#pragma link C++ class  IlcSelector+;

#pragma link C++ class  IlcRawDataErrorLog+;

#pragma link C++ class  IlcMeanVertex+;
#pragma link C++ class  IlcESDCaloCells+;

#pragma link C++ class  IlcESDVZEROfriend+;
#pragma link C++ class  IlcESDTZEROfriend+;

#pragma link C++ class  IlcESDHandler+;
#pragma link C++ class  IlcTrackerBase+;

#pragma link C++ namespace IlcESDUtils;

#pragma link C++ class  IlcTriggerIR+;
#pragma link C++ class  IlcTriggerScalersESD+;
#pragma link C++ class  IlcTriggerScalersRecordESD+;
#pragma link C++ class IlcTriggerCluster+;
#pragma link C++ class IlcTriggerDescriptor+;
#pragma link C++ class IlcTriggerInput+;
#pragma link C++ class IlcTriggerInteraction+;
#pragma link C++ class IlcTriggerPFProtection+;
#pragma link C++ class IlcTriggerBCMask+;
#pragma link C++ class IlcTriggerClass+;
#pragma link C++ class IlcTriggerConfiguration+;
#pragma link C++ class IlcExpression+;
#pragma link C++ class IlcVariableExpression+;
#pragma link C++ class IlcESDCosmicTrack+;

#pragma link C++ class  IlcV0vertexer+;
#pragma link C++ class  IlcCascadeVertexer+;

#pragma link C++ function IlcESDUtils::GetCorrV0(const IlcESDEvent*,Float_t &);
#pragma link C++ function IlcESDUtils::GetCorrSPD2(Float_t,Float_t);
#pragma link C++ function operator*(const IlcFMDMap&,const IlcFMDMap&);
#pragma link C++ function operator/(const IlcFMDMap&,const IlcFMDMap&);
#pragma link C++ function operator+(const IlcFMDMap&,const IlcFMDMap&);
#pragma link C++ function operator-(const IlcFMDMap&,const IlcFMDMap&);
  
#endif
