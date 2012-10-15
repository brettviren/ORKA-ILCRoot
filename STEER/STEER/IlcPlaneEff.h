#ifndef ILCPLANEEFF_H
#define ILCPLANEEFF_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

#include <TObject.h>
#include <TString.h>
#include "IlcLog.h"

//class Ilcsegmentation;
//class Ilcgeom;

////////////////////////////////////////////////////
//                                                //
// Virtual base class for Plane Efficiency        //
// Origin: Giuseppe.Bruno@ba.infn.it              //
//                                                //
////////////////////////////////////////////////////

class IlcPlaneEff : public TObject {
 public:
 
    IlcPlaneEff();// Default constructor
    virtual ~IlcPlaneEff(){;}; 
    // copy constructor. See detector specific implementation.
    IlcPlaneEff(const IlcPlaneEff &source);
    // Assignment operator. See detector specific implementation.
    IlcPlaneEff& operator=(const IlcPlaneEff &source);
    // Average Plane efficiency (including dead/noisy)
    //Int_t   GetRunNumber() const {return fRunNumber;}
    //void    SetRunNumber(Int_t n) {fRunNumber=n;}
    //
    // Write into (read from) the data base 
    virtual Bool_t WriteIntoCDB() const
       {IlcError("This method must be implemented in a derived class"); return kFALSE;};
    virtual Bool_t ReadFromCDB()
       {IlcError("This method must be implemented in a derived class"); return kFALSE;};
    virtual Bool_t AddFromCDB()
       {IlcError("This method must be implemented in a derived class"); return kFALSE;};
    // Write/read Histograms to/from File
    virtual Bool_t WriteHistosToFile(TString ,Option_t* )
       {IlcError("This method must be implemented in a derived class"); return kFALSE; }
    virtual Bool_t ReadHistosFromFile(TString )
       {IlcError("This method must be implemented in a derived class"); return kFALSE; }
    virtual Bool_t GetCreateHistos() const
       {IlcError("This method must be implemented in a derived class"); return kFALSE; }
    virtual void InitCDB(){;};

 protected:

    virtual void Copy(TObject &obj) const;
    //Int_t	fRunNumber;	//! run number (to access CDB)
    //TString	fCDBUri;	//! Uri of the default CDB storage
    //Bool_t	fInitCDBCalled;	//! flag to check if CDB storages are already initialized
   
 private:
    //Int_t	fRunNumber;	// run number (to access CDB)

    ClassDef(IlcPlaneEff,2) // Plane Efficiency virtual base class 
};
#endif
