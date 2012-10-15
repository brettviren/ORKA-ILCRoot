//-*- Mode: C++ -*-
// @(#) $Id: IlcRawHLTManager.h 23318 2008-01-14 12:43:28Z hristov $

#ifndef ILCRAWHLTMANAGER_H
#define ILCRAWHLTMANAGER_H
//* This file is property of and copyright by the ILC HLT Project        * 
//* 2012-2013 Infrastructure for Large Collider Experiment.       *
//* All rights reserved.                                                   *
//* See cxx source for full Copyright notice                               *

/** @file   IlcRawHLTManager.h
    @author Matthias Richter
    @date   
    @brief  dynamic generation of HLT RAW readers and streams
*/

// see below for class documentation
// or
// refer to README to build package
// or
// visit http://web.ift.uib.no/~kjeks/doc/ilc-hlt
 
class IlcRawReader;
#include "TObject.h"

extern "C" {
  typedef IlcRawReader* (*IlcRawReaderHLTCreateInstance_t)(IlcRawReader* pParentReader, const char* options);
}
                        
/**
 * @class IlcRawHLTManager
 * The class gives dynamic access to creater methods for HLT RAW readers and
 * streams without any library dependencies to HLT libraries.
 */
class IlcRawHLTManager {
 public:
  IlcRawHLTManager();
  virtual ~IlcRawHLTManager();

  /**
   * Create an instance of the IlcRawReaderHLT.
   * The IlcRawReaderHLT instance needs the parent RAW reader and a list
   * of detectors for which it should access data from the HLT stream.
   */
  static IlcRawReader* CreateRawReaderHLT(IlcRawReader* pParent, const char* detectors);

  /**
   * Create an instance of a RAW stream.
   * There is no common base class for RAW streams due to the different nature of the
   * detectors and the data. The least common class is the TObject. The calling code
   * should check if the right class has been created by
   * <pre>
   * TObject pObject=IlcRawHLTManager::CreateRawStream("MyClass");
   * MyClass* pClass=dynamic_cast<MyClass*>(pObject)
   * </pre>
   *
   * \b NOTE: The function redirects the request to the HLT framework, a handler
   * to actually create the RAW stream must be implemented in the corresponding
   * component library.
   */
  static TObject* CreateRawStream(const char* className);
 protected:
 private:
  enum {kUnloaded=0, kLoaded, kUnavailable};

  /**
   * Load the HLT interface library
   */
  static int LoadLibrary();

  /** status of the loading of the HOMER library */
  static int fLibraryStatus; //!transient

  /** entry in the HOMER library */
  static IlcRawReaderHLTCreateInstance_t fFctCreateRawReaderHLT; //!transient

  /** entry in the HOMER library */
  static void* fFctCreateRawStream; //!transient

  ClassDef(IlcRawHLTManager, 0)
};

// those definitions have been copied one to one from rec/IlcRawReaderHLT.h
// to avoid including this header file
#define ILCHLTREC_LIBRARY                   "libHLTrec.so"
#define ILCHLTREC_LIBRARY_VERSION           0
#define ILCRAWREADERHLT_CREATE_INSTANCE     "IlcRawReaderHLTCreateInstance"


#endif //ILCRAWHLTMANAGER_H
