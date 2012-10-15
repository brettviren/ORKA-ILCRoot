#ifndef ILCTRIGGERDETECTOR_H
#define ILCTRIGGERDETECTOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTriggerDetector.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Base Class for Detector specific Trigger                                 //                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObjArray.h>
#include <TNamed.h>
class TString;
class IlcTriggerInput;


class IlcTriggerDetector : public TNamed {

public:
                          IlcTriggerDetector();
               virtual   ~IlcTriggerDetector();
	       IlcTriggerDetector(const IlcTriggerDetector & de );

          virtual void    AssignInputs(const TObjArray& inputs);
          virtual void    CreateInputs();
          virtual void    Trigger();
  //  Setters
                  void    AddInput( TObject * input ) { fInputs.AddLast( input ); }
                  void    SetInput( TString & name );
                  void    SetInput( const char * name );
  //  Getters
             TObjArray*   GetInputs() { return &fInputs; }
                Long_t    GetMask() const { return fMask; }

       IlcTriggerInput*   GetInput( TString & name ) {
                             return ((IlcTriggerInput*)fInputs.FindObject( name.Data() ));
                          }
       IlcTriggerInput*   GetInput( const char *  name ) {
                             return ((IlcTriggerInput*)fInputs.FindObject( name ));
                          }
          virtual void    Print( const Option_t* opt ="" ) const;

protected:
                Long_t    fMask;      // Trigger Mask ( bitwise OR of trigger inputs )
             TObjArray    fInputs;    // Array of Triggers Inputs (IlcTriggerInput class)

private:
	     IlcTriggerDetector&   operator=(const IlcTriggerDetector& de);

   ClassDef( IlcTriggerDetector, 1 )  // Base Class for Detector specific Trigger
};

#endif
