/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//  class IlcObjectLoader                                                                  //
//                                                                                         //
//  Container of all data needed for full                                                  //
//  description of each data type                                                          //
//  (Hits, Kine, ...)                                                                      //
//                                                                                         //
//  Each data loader has a basic standard setup of BaseLoaders                             //
//  which can be identuified by indexes (defined by EStdBasicLoaders)                      //
//  Data managed by these standard base loaders has fixed naming convention                //
//  e.g. - tree with hits is always named TreeH                                            //
//                     (defined in IlcLoader::fgkDefaultHitsContainerName)                 //
//       - task DtectorName+Name defined                                                   //
//                                                                                         //
//  EStdBasicLoaders   idx     Object Type        Description                              //
//      kData           0    TTree or TObject     main data itself (hits,digits,...)       //
//      kTask           1        TTask            object producing main data               //
//      kQA             2        TTree                quality assurance tree               //
//      kQATask         3        TTask            task producing QA object                 //
//                                                                                         //
//                                                                                         //
//  User can define and add more basic loaders even Run Time.                              //
//  Caution: in order to save information about added base loader                          //
//  user must rewrite Run Loader to gilc.file, overwriting old setup                     //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

#include "IlcObjectLoader.h"
#include "IlcDataLoader.h"
#include "IlcLog.h"
#include <TFolder.h>

ClassImp(IlcObjectLoader)

//______________________________________________________________________________
IlcObjectLoader::IlcObjectLoader(const TString& name, IlcDataLoader* dl, Bool_t storeontop):
  IlcBaseLoader(name,dl,storeontop)
{
  //
  // Constructor
  //
}

//______________________________________________________________________________
TFolder* IlcObjectLoader::GetFolder() const
{
  //
  // Returns pointer to the object folder
  //
  TFolder* df = GetDataLoader()->GetFolder();
  if (df == 0x0)
    {
      IlcFatal("Data Folder is NULL");
    }
  return df;
}

//______________________________________________________________________________
void IlcObjectLoader::RemoveFromBoard(TObject* obj)
{
  //
  // Removes "obj" from the board
  //
  GetFolder()->Remove(obj);
}

//______________________________________________________________________________
Int_t IlcObjectLoader::AddToBoard(TObject* obj)
{
  //
  // Adds "obj" to the board
  //
  GetFolder()->Add(obj);
  return 0;
}

//______________________________________________________________________________
TObject* IlcObjectLoader::Get() const
{
  //
  // Returns pointer to the object loader
  //
  return (GetFolder()) ? GetFolder()->FindObject(GetName()) : 0x0;
}



