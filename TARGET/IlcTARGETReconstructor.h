#ifndef ILCTARGETRECONSTRUCTOR_H
#define ILCTARGETRECONSTRUCTOR_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETReconstructor.h,v 1.1.1.1 2008/03/11 14:52:51 vitomeg Exp $ */
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for TARGET reconstruction                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcRunLoader.h"
#include "IlcReconstructor.h"

class IlcTARGETgeom;
class IlcLoader;
class IlcTracker;
class IlcTARGETtrackerMI;
class IlcTARGETpidESD;

class IlcTARGETReconstructor: public IlcReconstructor {
public:
  IlcTARGETReconstructor();
  virtual ~IlcTARGETReconstructor();
// // // //   IlcTARGETReconstructor(const IlcTARGETReconstructor &ob); // copy constructor
  IlcTARGETReconstructor& operator=(const IlcTARGETReconstructor & /* source */); // ass. op.

  virtual void         Reconstruct(IlcRunLoader* runLoader) const;
  virtual void         Reconstruct(IlcRunLoader* runLoader,
				   IlcRawReader* rawReader) const;
  virtual void         Reconstruct(IlcRawReader* rawReader, TTree* clustersTree) const 
    {IlcReconstructor::Reconstruct(rawReader,clustersTree);}
  virtual void         Reconstruct(TTree* digitsTree, TTree* clustersTree) const
  {
    IlcRunLoader* runLoader = IlcRunLoader::Instance();
    Reconstruct(runLoader );
  }

  virtual IlcTracker*  CreateTracker(IlcRunLoader* runLoader) const;
  virtual IlcTracker*  CreateTracker() const;
  virtual IlcVertexer* CreateVertexer(IlcRunLoader* runLoader) const;
  virtual void         FillESD(IlcRunLoader* runLoader, IlcESDEvent* esd) const;

  virtual void         FillESD(TTree* digitsTree, TTree* clustersTree, 
                               IlcESDEvent* esd) const
    {IlcReconstructor::FillESD(digitsTree, clustersTree, esd);}
  virtual void         FillESD(IlcRawReader* rawReader, TTree* clustersTree, 
                               IlcESDEvent* esd) const
    {IlcReconstructor::FillESD(rawReader, clustersTree, esd);}
// // // //   virtual void         FillESD(IlcRunLoader* runLoader, 
// // // //                                IlcRawReader* rawReader, IlcESDEvent* esd) const
// // // //     {IlcReconstructor::FillESD(runLoader,rawReader, esd);}

private:
  // methods
  IlcTARGETgeom*          GetTARGETgeom(IlcRunLoader* runLoader) const;
  //data
  IlcTARGETpidESD *fItsPID; //Pid for TARGET
  ClassDef(IlcTARGETReconstructor, 1)   // class for the TARGET reconstruction
};

#endif
