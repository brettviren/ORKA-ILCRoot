#ifndef ILCCTPINPUTTIMEPARAMS_H
#define ILCCTPINPUTTIMEPARAMS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice */
/* $Id$ */

class TObject;

class IlcCTPInputTimeParams : public TObject {

public:
                         IlcCTPInputTimeParams();
                         IlcCTPInputTimeParams( TString& name, UInt_t& level, UInt_t delay, TString edge, UInt_t deltamin, UInt_t deltamax );   

              virtual   ~IlcCTPInputTimeParams() {}
                         IlcCTPInputTimeParams( const IlcCTPInputTimeParams &ctptime );
         IlcCTPInputTimeParams&   operator=(const IlcCTPInputTimeParams& ctptime);
              
      // Getters
              TString    GetInputName()      const { return fName; }
               UInt_t    GetLevel()     const { return fLevel; }       
               UInt_t    GetDelay() const { return fDelay; }  
              TString    GetEdge()    const { return fEdge; }      
               Int_t    GetDeltaMin() const { return fDeltaMin; }  
               Int_t    GetDeltaMax() const { return fDeltaMax; }  
     // Setters
                 void    SetCTPInputTimeParams( TString name, UInt_t level, 
                            UInt_t delay, TString edge, UInt_t deltamin, UInt_t deltamax );
              
         virtual void    Print( const Option_t* opt ="" ) const;
                               
              
protected:
	     TString	fName;  	
              UInt_t    fLevel; 
              UInt_t    fDelay;
	     TString	fEdge;
	      Int_t	fDeltaMin;
	      Int_t	fDeltaMax;
                         
private:                         

   ClassDef( IlcCTPInputTimeParams, 3 )  
};                                                                         


#endif
