#ifndef ILCLTUCONFIG_H
#define ILCLTUCONFIG_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Class that describes a detector LTU configuration                        //
//                                                                           //
//                                                                           //
//  Presently we store a subset of the LTU parameters:                       //
//  FineDelay1 3126 # picosec                                                //
//  FineDelay2 20459 # picosec                                               //
//  BC_DELAY_ADD 18 # ns                                                     //
//                                                                           //
//  cvetan.cheshkov@cern.ch 3/9/2010                                         //
///////////////////////////////////////////////////////////////////////////////


#include <TObject.h>

class IlcLTUConfig : public TObject {

public:
                          IlcLTUConfig(): TObject(),
                            fDetectorId(-1),
			    fFineDelay1(0.),
			    fFineDelay2(0.),
			    fBCDelayAdd(0.)
			    {}
                          IlcLTUConfig(UChar_t detectorId, Float_t fineDelay1, Float_t fineDelay2, Float_t bcDelayAdd): TObject(),
                            fDetectorId(detectorId),
			    fFineDelay1(fineDelay1),
			    fFineDelay2(fineDelay2),
			    fBCDelayAdd(bcDelayAdd)
			    {}
                          IlcLTUConfig(IlcLTUConfig & ltu): TObject(ltu),
                            fDetectorId(ltu.fDetectorId),
			    fFineDelay1(ltu.fFineDelay1),
			    fFineDelay2(ltu.fFineDelay2),
			    fBCDelayAdd(ltu.fBCDelayAdd)
			    {}
               virtual   ~IlcLTUConfig() {}

  //  Getters
	        Char_t    GetDetectorId() const { return fDetectorId; }
	    const char*   GetDetectorName() const;
	       Float_t    GetFineDelay1() const { return fFineDelay1; }
	       Float_t    GetFineDelay2() const { return fFineDelay2; }
	       Float_t    GetBCDelayAdd() const { return fBCDelayAdd; }

           virtual void   Print( const Option_t* opt ="" ) const;

private:
	   IlcLTUConfig & operator=(const IlcLTUConfig & );

                Char_t    fDetectorId; // Detector ID, see IlcDAQ class for more details
               Float_t    fFineDelay1; // Fine delay in ns
	       Float_t    fFineDelay2; // Fine delay in ns
	       Float_t    fBCDelayAdd; // BC_DELAY_ADD in ns 

   ClassDef( IlcLTUConfig, 1 )  // LTU Configuration class
};


#endif
