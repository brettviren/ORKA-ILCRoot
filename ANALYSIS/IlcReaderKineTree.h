#ifndef IlcReaderKineTree_H
#define IlcReaderKineTree_H
//_______________________________________________________________________
/////////////////////////////////////////////////////////////////////////
//
// class IlcReaderKineTree
//
// Reader for Kinematics
//
// Piotr.Skowronski@cern.ch
//
/////////////////////////////////////////////////////////////////////////
#include "IlcReader.h"
#include <TString.h>

class TFile;
class IlcStack;
class IlcRunLoader;

class IlcReaderKineTree: public IlcReader
 {
   public:
    IlcReaderKineTree();
    
    IlcReaderKineTree(TString&);
    IlcReaderKineTree(TObjArray*,const Char_t *filename="gilc.root");
    IlcReaderKineTree(const IlcReaderKineTree& in);

    virtual ~IlcReaderKineTree();

    IlcReaderKineTree& operator=(const IlcReaderKineTree& in);
    
    void          Rewind();
    
    Bool_t        ReadsRec() const {return kFALSE;}
    Bool_t        ReadsSim() const {return kTRUE;}
    
   protected:
    Int_t         ReadNext();//reads tracks and particles and puts them in runs
    Int_t         OpenNextFile();
   
    TString       fFileName;//file name 
    IlcRunLoader* fRunLoader;//!Pointer to loader
    
    static const TString fgkEventFolderName; //Event folder name that session are mounter
    
   private:
     ClassDef(IlcReaderKineTree,2)
 };

#endif
