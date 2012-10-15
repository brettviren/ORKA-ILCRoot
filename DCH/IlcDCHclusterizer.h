#ifndef ILCDCHCLUSTERIZER_H
#define ILCDCHCLUSTERIZER_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHclusterizer.h,v 1.3 2008/04/24 08:45:12 tassiell Exp $ */

#include <TNamed.h>
//#include <TObjArray.h>
#include <TClonesArray.h>

#include "IlcLoader.h"
#include "IlcDCHcluster.h"



// class TFile;
// class TTree;
// class IlcRunLoader;
// class IlcDCH;
// class IlcDCHcluster;
///////////////////////////////////////////////////////
//  Finds and handles cluster                        //
///////////////////////////////////////////////////////

class IlcDCHclusterizer : public TNamed {

 public:

  IlcDCHclusterizer();
  IlcDCHclusterizer(const Text_t* name, const Text_t* title);
  IlcDCHclusterizer(const IlcDCHclusterizer &c);
  virtual ~IlcDCHclusterizer();
  IlcDCHclusterizer &operator=(const IlcDCHclusterizer &c);

  virtual void    Copy(TObject &c) const;
  virtual Bool_t  Open(const Char_t *name, Int_t nEvent = 0);
  
  virtual Bool_t  OpenInput(Int_t nEvent = 0);
  virtual Bool_t  OpenOutput();
  virtual Bool_t  MakeClusters(){return kTRUE;};
  virtual Bool_t  WriteClusters();
  void            SetVerbose(Int_t v = 1)          { fVerbose       = v;   };

//  TObjArray*      RecPoints() {if (!fRecPoints) fRecPoints = new TObjArray(400); return fRecPoints;}
  TClonesArray*   RecPoints() {if (!fRecPoints) fRecPoints = new TClonesArray("IlcDCHcluster"); return fRecPoints;}
  IlcDCHcluster*  AddCluster(Float_t *pos, Float_t *sig, Int_t *tracks, ULong_t Id, Float_t amp=0.0);
//  void            ResetRecPoints() {if (fRecPoints) fRecPoints->Clear();}
  void            ResetRecPoints() {if (fRecPoints) fRecPoints->Clear();}
  void 		  FillEv();

 protected:

//   Double_t CalcXposFromTimebin(Float_t timebin, Int_t idet, Int_t col, Int_t row);
       
  IlcRunLoader    *fRunLoader;     //! Run Loader
  IlcLoader       *fDCHLoader;     //! Detector Loader
  
  TTree           *fClusterTree;   //! Tree with the cluster

//  TObjArray       *fRecPoints;     //! Array of clusters
  TClonesArray     *fRecPoints;     //! Array of clusters
  Int_t            fVerbose;       //  Sets the verbose level
  Int_t	           fNclus;         // Number of Clusters per Event

  ClassDef(IlcDCHclusterizer,1)    //  DCH-Cluster manager rec class

};

#endif
