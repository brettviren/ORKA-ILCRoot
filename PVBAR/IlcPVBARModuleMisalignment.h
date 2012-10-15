#ifndef ILCPVBARMODULEMISILCGNMENT_H
#define ILCPVBARMODULEMISILCGNMENT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: $ */

// Utility class to calculate PVBAR module's 
// misalignment from ideal geometry (defined by IlcPVBARGeometry)
// and survey data. 5 modules expected.
// Number of modules is specified by enumerator kModules here,
// if this number changes in IlcPVBARGeometry
// - CHANGE kModules constant here.
// We do not have such compile-time constant in
// IlcPVBARGeometry.
// I define module position by three points
// with names "name0", "name1", "name2".
//
// Module:
//                                 |
// <-------------Z-----------------|--
// name1 ------------------- name0 |
//                               | |
//                               | |
//                               | |
//                               | |
//                               | |
//                               | X
//                               | |
//                               | |
//                               | |
//                               | |
//                           name2 |
//
// For example, in
//    "PVBAR MODULE Position 2 (Production 2) 
//     MEASUREMENT AFTER FINAL INSTALLATION
//     Measurement on 19.05.2008": (https://edms.cern.ch/document/922320)
//
// name0 == "T2_10000"
// name1 == "T2_10027"
// name2 == "T2_24000"
//
// Usage:
//
// ...
// IlcPVBARGeometry * PVBARGeom = IlcPVBARGeometry::GetInstance("ORKA", "");
// ...
// //For IlcSurveyObj usage details see $ILC_ROOT/STEER/IlcSurveyObj.h,
// //here's example with local file.
// IlcSurveyObj survey;
// survey.FillFromLocalFile("local_file_name");
// ...
// IlcPVBARModuleMisalignment delta(*PVBARGeom);
// TGeoHMatrix m;//matrix to save transformations in.
// delta.DeltaTransformation(moduleNumber, survey.GetData(), "T2_10000",
//                           "T2_10027", "T2_24000", &m);
//
// Author: Timur Pocheptsov.
// 2 December 2008

//ROOT's typedefs.
#include <Rtypes.h>

class TGeoHMatrix;
class TString;

class IlcSurveyPoint;
class IlcPVBARGeometry;

class IlcPVBARModuleMisalignment {
  enum {
    kModules = 5
  };

public:
  IlcPVBARModuleMisalignment(const IlcPVBARGeometry & geom, Bool_t debug = kFALSE);
  //ClassDef macro will add virtual functions,
  //so, I have to define virtual dtor
  //to supress warnings.
  virtual ~IlcPVBARModuleMisalignment();
  //Module number "module" starts from zero.
  void DeltaTransformation(UInt_t module, const TObjArray * points, const TString & name0,
                           const TString & name1, const TString & name2, TGeoHMatrix * delta);
private:
  void FindIdealModule(UInt_t module);
  void FindRealModule(const IlcSurveyPoint * pt0, const IlcSurveyPoint * pt1,
                      const IlcSurveyPoint * pt2);
  void FindDelta(TGeoHMatrix * delta)const;

private:
  Bool_t   fDebug;  // debug level

  Double_t fAngles[kModules][3][2]; // Matrix of rotation angles
  Double_t fCenters[kModules][3];   // Matrix of center displacements
  Double_t fModule[3][3];  //Module's described by 3 points (non-positioned module)
  Double_t fU[3][3];       //Points from ideal module
  Double_t fV[3][3];       //Points from real module

  ClassDef(IlcPVBARModuleMisalignment, 1)//Utility class for module misalignment.
};

#endif
