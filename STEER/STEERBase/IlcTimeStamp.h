#ifndef ILCTIMESTAMP_H
#define ILCTIMESTAMP_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTimeStamp.h 22322 2007-11-22 11:43:14Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define Event Timestamp from : 
//
//               Orbit
//               Period counter
//               Seconds    |
//                  +       | ===> Bunch cross 
//               Microsecs  | 
//
//////////////////////////////////////////////////////////////////////////////
#include <cmath>

class TObject;

class IlcTimeStamp : public TObject {

public:
                         IlcTimeStamp();
                         IlcTimeStamp( UInt_t orbit, UInt_t period, ULong64_t bunchCross );   
                         IlcTimeStamp( UInt_t orbit, UInt_t period, 
                                       UInt_t seconds, UInt_t microsecs );   
              virtual   ~IlcTimeStamp() {}
                         IlcTimeStamp( const IlcTimeStamp &timestamp );
         IlcTimeStamp&   operator=(const IlcTimeStamp& timestamp);
              
      // Getters
               UInt_t    GetOrbit()      const { return fOrbit;     }        
               UInt_t    GetPeriod()     const { return fPeriod;    }       
            ULong64_t    GetBunchCross() const { return fBunchCross; }  
               UInt_t    GetSeconds()    const { return (UInt_t)(fBunchCross/1000000000.*fNanosecPerBC);   }      
               UInt_t    GetMicroSecs()  const { return (UInt_t)(fmod(fBunchCross *fNanosecPerBC, 1000000000.)/1000 ); }     
       virtual Bool_t    IsSortable()    const { return kTRUE; }
     // Setters
                 void    SetTimeStamp( UInt_t orbit, UInt_t period, ULong64_t bunchcross );
                 void    SetTimeStamp( UInt_t orbit, UInt_t period, 
                                       UInt_t seconds, UInt_t microsecs );
              
              
        virtual Int_t    Compare( const TObject* obj ) const;
         virtual void    Print( const Option_t* opt ="" ) const;
                               
   static const Int_t    fNanosecPerBC;   //! nanosecs per bunch cross
              
protected:
              UInt_t    fOrbit;         // Orbit
              UInt_t    fPeriod;        // Period counter
           ULong64_t    fBunchCross;    // Bunch Cross 
//              UInt_t    fSeconds;       // Seconds 
//              UInt_t    fMicroSecs;     // Microsecs  
                         
private:                         

   ClassDef( IlcTimeStamp, 1 )  // Define a timestamp
};                                                                         


#endif
