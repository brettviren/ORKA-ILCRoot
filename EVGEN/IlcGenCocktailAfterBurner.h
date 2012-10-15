#ifndef IlcGenCocktailAfterBurner_H
#define IlcGenCocktailAfterBurner_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenCocktailAfterBurner.h 48105 2011-03-05 22:04:00Z morsch $ */

// Container class for IlcGenerator through recursion.
// (Container is itself an IlcGenerator)
// Author: piotr.skowronski@cern.ch 
//

#include <TMCProcess.h>

#include "IlcGenCocktail.h"
#include "IlcRunLoader.h"

class IlcGenCocktailEntry;
class IlcStack;
class IlcCollisionGeometry;
class IlcGenCocktailEventHeader;


class IlcGenCocktailAfterBurner : public  IlcGenCocktail
{
//container class for other generators
//extends IlcGenCocktail functionality
//with possiblity of adding after-burners

 public:
    IlcGenCocktailAfterBurner();
    virtual ~IlcGenCocktailAfterBurner();
    
    virtual void  Init();
    virtual void  Generate();
    virtual void  SetTracks(Int_t stackno);
    //
    // Add a new generator to the list
    virtual void  AddAfterBurner
	(IlcGenerator *Generator, char* Name, Float_t RateExp );
    
    IlcStack*     GetStack(Int_t n) const;
    IlcStack*     GetActiveStack() const{return fActiveStack;}

    IlcCollisionGeometry* GetCollisionGeometry(Int_t n) const;
 
    IlcGenerator* GetCurrentGenerator() const;
    virtual void  SetActiveEventNumber(Int_t actev);
    Int_t         GetActiveEventNumber() const {return fActiveEvent;}
    virtual Int_t GetNumberOfEvents() const {return IlcRunLoader::Instance()->GetNumberOfEventsPerRun() + fNBgEvents;}
    void          SetNBgEvents(Int_t nbg=0){fNBgEvents = nbg;}
    IlcGenCocktailEventHeader* GetActiveEventHeader() const { return fHeaders[fActiveEvent]; }

    static TMCProcess IntToMCProcess(Int_t no);

 protected:
    Int_t fNAfterBurners;       // Number of afterburners  
    TList  *fAfterBurnerEntries;// List of afterburners
    Bool_t fGenerationDone;     // flag if generation is already done 
                                //   during first call of Generate method
                                //   if true just return event to gIlc
                                //   
    TObjArray *fInternalStacks; //! List of internal stacks

    IlcCollisionGeometry**      fCollisionGeometries; //! List of Collision Geometries
    IlcGenCocktailEventHeader** fHeaders;             //! List of Headers
    
    Int_t fCurrentEvent;        //  Number of current event/stack
    

    IlcStack* fActiveStack;   //! pointer to the current stack
    Int_t fActiveEvent;       //HBT Processor needs more then one event to do correlations
                              //Due to complications in fortran, it first calls C routine
                              //that sets the active event to be read. All ilchbtp_gettrack
                              //are addressed to this event
    
    IlcGenerator *fCurrentGenerator;      // Current event generator 
    Int_t fNBgEvents;                     //Nuber of backgrouns events 
                                          //(events that are generated only temporarly)
                                          //needed by some afterburners that works better with higher statistics 
                                          //this generates such a artificial one
 private:
    IlcGenCocktailAfterBurner(const IlcGenCocktailAfterBurner& in);
    IlcGenCocktailAfterBurner & operator=(const IlcGenCocktailAfterBurner & rhs);

    ClassDef(IlcGenCocktailAfterBurner,2) // Particle cocktail generator a la SHAKER
                                          //background events added
};

inline  IlcGenerator*  
    IlcGenCocktailAfterBurner::GetCurrentGenerator() const
{
  return fCurrentGenerator;
}


#endif





