#ifndef ILCDCHPARTID_H
#define ILCDCHPARTID_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

#include <TObject.h>

class IlcESDtrack;
class TProfile;
class TF1;


class IlcDCHPartID: public TObject {
  public: 
    IlcDCHPartID();
    IlcDCHPartID(TF1* betheBloch, Double_t res, Double_t range);
    virtual ~IlcDCHPartID();

    Bool_t          MakePID(IlcESDtrack* track);

    void            FitBetheBloch(TProfile* dEdxVsBetaGamma);
    inline TF1*     GetBetheBloch() {return fBetheBloch;};
    TF1*            CreateBetheBloch(Double_t mass);

    static IlcDCHPartID* GetFromFile(const char* fileName = "pid.root");

  private:
    static Double_t fcnBetheBloch(Double_t* xx, Double_t* par);
    static Double_t fcnBetheBlochMass(Double_t* xx, Double_t* par);

    TF1*            fBetheBloch;   // parametrized bethe bloch function
    Double_t        fRes;          // relative dE/dx resolution
    Double_t        fRange;        // cut off in standard deviations

    ClassDef(IlcDCHPartID,1)   // DCH PID class
};

#endif


