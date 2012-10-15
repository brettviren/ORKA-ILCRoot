#ifndef ILCRSTACKSURVEY_H
#define ILCRSTACKSURVEY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKSurvey.h 20504 2007-08-28 13:12:19Z hristov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.3  2007/07/10 12:41:38  kharlov
 * Added a new class IlcRSTACKSurvet1 which read survey data from EDMS files
 *
 * Revision 1.2  2007/05/17 17:13:32  kharlov
 * Coding convensions satisfied (T.Pocheptsov)
 *
 * Revision 1.1  2007/04/19 15:47:20  kharlov
 * Add misalignment of strip units with IlcRSTACKSurvey class
 *
 */

#include <TObject.h>
#include <Rtypes.h>

class TClonesArray;
class TString;

class IlcRSTACKGeometry;

/*
  Objects of this class read txt file with survey (photogrammetry) data
  and convert the data into IlcAlignObjParams of alignable RSTACK volumes.
  It can be used as a base class, you need to override GetStripTransformation.
  IlcRSTACKSurvey inherits TObject only to use IlcLog "functions".
*/

class IlcRSTACKSurvey : public TObject {
public:
  IlcRSTACKSurvey();
  IlcRSTACKSurvey(const TString &txtFileName);

  virtual ~IlcRSTACKSurvey();

  //Create IlcAlignObjParams for strips.
  void CreateIlcAlignObjParams(TClonesArray &array);
  //Create IlcAlignObjParams with null shifts and rotations.
  void CreateNullObjects(TClonesArray &alObj, const IlcRSTACKGeometry *geom)const;

protected:

  struct IlcRSTACKStripDelta {
    Float_t fXShift; //x shift
    Float_t fYShift; //y shift
    Float_t fZShift; //z shift
    Float_t fPsi;    //psi
    Float_t fTheta;  //theta
    Float_t fPhi;    //phi
  };

  Int_t 	           fStrNum; // Number of strips.
  IlcRSTACKStripDelta *fStripData; // Strip unit transformation data

  void InitStripData(const Double_t *xReal, const Double_t *zReal);

private:
  //Calculate shifts and rotations for strip number stripIndex in a module moduleIndex.
  virtual IlcRSTACKStripDelta GetStripTransformation(Int_t stripIndex, Int_t moduleIndex)const;

  IlcRSTACKSurvey(const IlcRSTACKSurvey &);
  IlcRSTACKSurvey &operator = (const IlcRSTACKSurvey &);

  ClassDef(IlcRSTACKSurvey, 1) //Survey data reader
};

#endif
