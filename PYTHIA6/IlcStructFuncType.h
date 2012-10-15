#ifndef ILCSTRUCTFUNCTYPE_H
#define ILCSTRUCTFUNCTYPE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcStructFuncType.h 32487 2009-05-20 14:54:29Z hristov $ */

// Helper class to interface pdflib and the TPythia 
// the c++ interface for Pythia
// Author: andreas.morsch@cern.ch

#include <TObject.h>
#include <TString.h>
#include <Riostream.h>

using namespace std;

typedef enum
{
    kCTEQ4L,  // cteq4l.LHgrid   
    kCTEQ4M,  // cteq4m.LHgrid   
    kCTEQ5L,  // cteq5l.LHgrid
    kCTEQ5M,  // cteq5m.LHgrid
    kGRVLO98, // GRV98lo.LHgrid   
    kCTEQ6,   // cteq6.LHpdf
    kCTEQ61,  // cteq61.LHpdf
    kCTEQ6m,  // cteq6m.LHpdf
    kCTEQ6l,  // cteq6l.LHpdf 
    kCTEQ6ll  // cteq6ll.LHpdf
}
StrucFunc_t;


class IlcStructFuncType : public TObject {
    
 public:
    IlcStructFuncType(){;}
    virtual ~IlcStructFuncType(){;}
    static void PdfSet(char parm[20][20], Double_t value[20]){cout << Form("E-IlcStructFuncType::PDFset: Not Implemented\n");}
    static void StructA(Double_t xx, Double_t qq, Double_t a,
			Double_t& upv, Double_t& dnv, Double_t& usea,
			Double_t& dsea,
			Double_t& str, Double_t& chm, Double_t& bot,
			Double_t& top, Double_t& gl){cout << Form("E-IlcStructFuncType::StructA: Not Implemented\n");}
    static Int_t   PDFsetIndex(StrucFunc_t pdf){cout << Form("E-IlcStructFuncType::PDFsetIndex: Not Implemented\n"); return -1;}
    static TString PDFsetName(StrucFunc_t pdf){cout << Form("E-IlcStructFuncType::PDFsetName: Not Implemented\n"); return "";}
    ClassDef(IlcStructFuncType,1) // Library for partonic energy loss
};

#endif

