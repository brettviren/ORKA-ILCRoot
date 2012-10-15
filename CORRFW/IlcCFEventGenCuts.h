#ifndef ILCCFEVENTGENCUTS_H
#define ILCCFEVENTGENCUTS_H
/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
// Cut on the Event at generator level: for the moment just
// the requirements on the MB process type, on the number of charged tracks 
// and on the vertex 3-D position are implemented
// The argument of IsSelected member function (passed object) is cast into 
// an IlcMCEvent. In the future may be modified to use IlcVEvent interface
// and to include more cut variables.
// The class derives from IlcCFCutBase
// Author:S.Arcelli Silvia.Arcelli@cern.ch


#include "IlcCFCutBase.h"
class TBits;
class IlcGenEventHeader;
class IlcMCEvent;
//____________________________________________________________________________
class IlcCFEventGenCuts: public IlcCFCutBase 
{
 public :
  IlcCFEventGenCuts() ;
  IlcCFEventGenCuts(Char_t* name, Char_t* title) ;
  IlcCFEventGenCuts(const IlcCFEventGenCuts& c) ;
  IlcCFEventGenCuts& operator=(const IlcCFEventGenCuts& c) ;
  ~IlcCFEventGenCuts();
  Bool_t IsSelected(TObject* obj);
  Bool_t IsSelected(TList* /*list*/) {return kTRUE;}

  //number of embedded cuts
  enum{kNCuts=5};

  //Label the MB MC processes
  enum PrType { 
   kND, kSD, kDD 
  }; 


  static Int_t ProcType(IlcGenEventHeader *genHeader); 
  static Bool_t IsMBProcType(IlcMCEvent *ev, PrType iproc);


  void   SetMBSelProcType(PrType iproc = kND) {fMBProcType=iproc;} // cut values setter
  Int_t  GetMBSelProcType()const {return fMBProcType;} // cut values getter



  void SetNTracksCut(Int_t xMin=-1, Int_t xMax=1000000) {fNTracksMin=xMin; fNTracksMax=xMax;} // cut values setter

  void SetRequireVtxCuts(Bool_t vtx=kFALSE) {fRequireVtxCuts=vtx;} // cut values setter
  void SetVertexXCut(Double_t xMin=-1.e99, Double_t xMax=1.e99) { fVtxXMin=xMin; fVtxXMax=xMax;} // cut values setter
  void SetVertexYCut(Double_t yMin=-1.e99, Double_t yMax=1.e99) { fVtxYMin=yMin; fVtxYMax=yMax;} // cut values setter
  void SetVertexZCut(Double_t zMin=-1.e99, Double_t zMax=1.e99) { fVtxZMin=zMin; fVtxZMax=zMax;} // cut values setter

  Int_t    GetNTracksMin() const {return fNTracksMin;} // cut values getter
  Int_t    GetNTracksMax() const {return fNTracksMax;} // cut values getter
  Bool_t   GetRequireVtxCuts() const {return fRequireVtxCuts;} // cut value getter
  Double_t GetVertexXMax() const {return fVtxXMax;} // cut values getter
  Double_t GetVertexYMax() const {return fVtxYMax;} // cut values getter
  Double_t GetVertexZMax() const {return fVtxZMax;} // cut values getter
  Double_t GetVertexXMin() const {return fVtxXMin;} // cut values getter
  Double_t GetVertexYMin() const {return fVtxYMin;} // cut values getter
  Double_t GetVertexZMin() const {return fVtxZMin;} // cut values getter
  

 protected:

  void SelectionBitMap(TObject* obj);
  
  Int_t fMBProcType ; //the type of selected MB process 
  Int_t fNTracksMin; //minimum number of particles in the event
  Int_t fNTracksMax; //maximum number of particles in the event
  Bool_t fRequireVtxCuts ; //The type of trigger to be checked
  Double_t fVtxXMax ; //X vertex position, maximum value
  Double_t fVtxYMax ; //Y vertex position, maximum value 
  Double_t fVtxZMax ; //Z vertex position, maximum value
  Double_t fVtxXMin ; //X vertex position, minimum value
  Double_t fVtxYMin ; //Y vertex position, minimum value
  Double_t fVtxZMin ; //Z vertex position, minimum value

  TBits *fBitMap ; //cut mask

  ClassDef(IlcCFEventGenCuts,2);
};

#endif
