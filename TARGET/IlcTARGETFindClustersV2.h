#ifndef ILCTARGETFINDCLUSTERSV2_H
#define ILCTARGETFINDCLUSTERSV2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
 
/*
  $Id: IlcTARGETFindClustersV2.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $
 */
///////////////////////////////////////////////////////////////////
//Class for reconstruction of clusters V2                        //
///////////////////////////////////////////////////////////////////
#include <TTask.h>

class TString;
class TFile;
class IlcRun;
class IlcTARGETgeom;

class IlcTARGETFindClustersV2 : public TTask{
 public:
    IlcTARGETFindClustersV2(); // default constructor
    // standard constructor files not opened, by file name
    IlcTARGETFindClustersV2(const TString infile,const TString outfile = "");
    // standard constructor for files already opened.
    IlcTARGETFindClustersV2(TFile *in,TFile *out=0);
    // Standard constructor for IlcRun already read in.
    IlcTARGETFindClustersV2(IlcRun *ar, const TString outfile = "");
    IlcTARGETFindClustersV2(const IlcTARGETFindClustersV2& rec);
    IlcTARGETFindClustersV2& operator=(const IlcTARGETFindClustersV2 &source);

    virtual ~IlcTARGETFindClustersV2();//Destructor
    virtual Bool_t FastSimulation() const {return fSlowFast;}
    virtual void SetSlowSimulation(){fSlowFast = kFALSE;}
    virtual void SetFastSimulation(){fSlowFast = kTRUE;}
    virtual void Exec(const Option_t *opt="");
 private:
    IlcRun *fAr;           //! Pointer of type IlcRun
    Bool_t fDeletfAr;      //! Logical to indecate if fAr should be deleted.
    IlcTARGETgeom *fGeom;     //! Pointer to TARGET geometry
    TString *fInFileName;  //! Pointer to input file name string.
    TString *fOutFileName; //! Pointer to output file name string.
    TFile   *fIn;          //! pointer to input file
    TFile   *fOut;         //! pointer to output file
    Bool_t fInit;          //! true if Init was successfull
    Bool_t fSlowFast;      //! if true then using fast TARGET simulation.

    ClassDef(IlcTARGETFindClustersV2,1) // Task to Reconstruct TARGET from Digits.

};
#endif
