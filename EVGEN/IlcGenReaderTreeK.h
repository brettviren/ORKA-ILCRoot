#ifndef ILCGENREADERTreeK_H
#define ILCGENREADERTREEK_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenReaderTreeK.h 55910 2012-04-24 12:25:57Z hristov $ */
//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads events from a kinematics TreeK.
// Author: andreas.morsch@cern.ch
//
#include "IlcGenReader.h"

class TFile;
class IlcRunLoader;
class IlcStack;
class TString;
class TObjArray;

class IlcGenReaderTreeK : public IlcGenReader
{
 public:
    IlcGenReaderTreeK();
    IlcGenReaderTreeK(const IlcGenReaderTreeK &reader);
    virtual ~IlcGenReaderTreeK();
    // Initialise 
    virtual void Init();
    // Read
    virtual Int_t NextEvent();
    virtual TParticle*  NextParticle();
    virtual void RewindEvent();
    virtual void SetFirstEvent(Int_t evt)    {fNcurrent  = evt;};
    virtual void SetOnlyPrimaries(Bool_t flag) {fOnlyPrimaries = flag;}
    IlcGenReaderTreeK & operator=(const IlcGenReaderTreeK & rhs);
    void SetDirs(TObjArray* dirs){fDirs = dirs;} //sets array directories names
    void AddDir(const char* dirname);
    IlcRunLoader * GetRunLoader() const {return fInRunLoader;}

 protected:
    Int_t             fNcurrent;          // points to the next entry
    Int_t             fNparticle;         // Next particle in list
    Int_t             fNp;                // number of particles
    IlcRunLoader     *fInRunLoader;       //!Run Loader of the input event
    TFile            *fBaseFile;          //! pointer to base file
    IlcStack         *fStack;             //! Particle stack
    Bool_t            fOnlyPrimaries;     // Flag indicating wether only primaries are read
    TObjArray        *fDirs;              //arry with directories to read data from
    Int_t             fCurrentDir;        //Number of current directory
    static const TString fgkEventFolderName;//!name of folder where event to read is mounted
    
    TString&   GetDirName(Int_t entry);
    TParticle* GetParticle(Int_t i);
 private:
    void Copy(TObject&) const;    
    ClassDef(IlcGenReaderTreeK,1) // Read particles from TreeK
};



#endif






