#ifndef ILCTARGETPREPROCESSORSPD_H
#define ILCTARGETPREPROCESSORSPD_H

/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTARGETPreprocessorSPD.h,v 1.2 2009/12/11 11:36:18 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////
// IlcTARGETPreprocessorSPD declaration by P. Nilsson 2005
// AUTHOR/CONTACT: Paul.Nilsson@cern.ch
//
// The purpose of this class is to provide algorithms for identification
// of "bad channels" such as dead channels and noisy channels in the SPD
///////////////////////////////////////////////////////////////////////////

#include <TTask.h>
#include <TH2F.h>
#include <TKey.h>
#include "IlcRunLoader.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETBadChannelsSPD.h"
#include "IlcTARGETBadChannelsAuxSPD.h"
#include "IlcCDBLocal.h"

class IlcCDBManager;
class IlcCDBMetaData;
class IlcCDBId;
class IlcCDBStorage;
class IlcTARGETChannelSPD;

class IlcTARGETPreprocessorSPD : public TTask {

 public:

  IlcTARGETPreprocessorSPD(void);                                         // Default constructor
  IlcTARGETPreprocessorSPD(const char *fileName, const char *mode,        // Standard constructor
			const char *fileNameg, Int_t maxNumberOfEvents);
  IlcTARGETPreprocessorSPD(const IlcTARGETPreprocessorSPD &prep);            // Default copy constructor
  IlcTARGETPreprocessorSPD& operator=(const IlcTARGETPreprocessorSPD &prep); // Assignment operator
  virtual ~IlcTARGETPreprocessorSPD(void);                                // Virtual destructor

  void Init(void);                                      // Initialization of the SPD preprocessor
  Bool_t Open(const char *fileName, const char *mode,   // Open the input file (of either "daq" or "vme" type)
	      const char *fileNameg);                   // and the gilc file
  Bool_t FindDeadChannels(void);                        // Find dead channels
  Bool_t FindDeadChannelsInModule(UInt_t module);       // Find dead channels in a module
  Bool_t FindNoisyChannels(void);                       // Locate the noisy channels among the digits
  Bool_t FindNoisyChannelsInModuleAlgo0(UInt_t module); // Locate the noisy channels in a module (for real data)
  Bool_t FindNoisyChannelsInModuleAlgo1(UInt_t module); // Locate the noisy channels in a module (for calibration data)
  Bool_t Store(IlcCDBId &Id, IlcCDBMetaData *md, Int_t runNumber);               // Write the final object to the calibration database
                                                        // Returns kTRUE if successful
  Bool_t GetVMEHistograms(TFile *vmeFile);              // Get pre-filled digit histograms from input VME file
  void MarkNoisyChannels(void);                         // Mark all found noisy channels among the digits
  void PrintChannels(void);                             // Print all found bad channels to stdout

  // Getters and setters
  void SetMaximumNumberOfEvents(UInt_t n)      // Set the maximum number of events
    { fMaximumNumberOfEvents = n; };           // (filling of noisy channel histograms will stop)
  UInt_t GetMaximumNumberOfEvents(void) const  // Get the maximum number of events
    { return fMaximumNumberOfEvents; };

  void SetGeometryMode(UInt_t mode);           // Set the geometry mode
  UInt_t GetGeometryMode(void) const           // Get the geometry mode
    { return fGeometryMode; };                 // (kILCGeometry is default, alt is kTestBeamGeometry)

  void SetThreshold(UInt_t t)                  // Set the noisy channel threshold
    { fThreshold = t; };                       // (A channel has to fire mode times than this value to be noisy)
  void SetThreshold(Double_t t)                // Set the noisy channel threshold (overloaded)
    { fThreshold = (UInt_t)t; };               // (A channel has to fire mode times than this value to be noisy)
  UInt_t GetThreshold(void) const              // Get the noisy channel threshold
    { return fThreshold; };                    // (A channel has to fire mode times than this value to be noisy)

  void SetThresholdRatio(Double_t r)           // Set the noisy channel threshold ratio
    { fThresholdRatio = r; };                  // (threshold to average neighboring channels)
  Double_t GetThresholdRatio(void) const       // Get the noisy channel threshold ratio
    { return fThresholdRatio; };               // (threshold to average neighboring channels)

  void SelectAlgorithm(UInt_t a)               // Select algorithm for read data or calibration data
    { fSelectedAlgorithm = a; };               // (either kOptimizedForRealData or kOptimizedForCalibrationData)
  UInt_t GetSelectedAlgorithm(void) const      // Get the selected algorithm for read data or calibration data
    { return fSelectedAlgorithm; };            // (either kOptimizedForRealData or kOptimizedForCalibrationData)

  // Geometry mode constants
  enum { kTestBeamGeometry, kILCGeometry };

  // Algorithm constants
  enum { kOptimizedForRealData, kOptimizedForRealDataRMS, kOptimizedForCalibrationData };

  // Noisy/dead pixel encoding (0001b, 0010b) used with the digit TObject statusbit
  enum { kNoisyChannel = 1, kDeadChannel = 2 };

  // Replace later with reading from geometry object
  // During the 2004 test beam, 4 modules were used but had internal hardware addresses of 0,1,4,5
  enum { kNumberOfTestBeamSPDModules = 6, kNumberOfSPDModules = 240 };
  enum { kNumberOfColumns = 160, kNumberOfRows = 256 }; // In one module/ladder
  enum { kNumberOfChannels = 40960 }; // 5*8192

 private:

  void ConvertObjToIntArray(void);             // Convert the bad channel TObjArray to an Int_t array
  void SetFakeNoisyChannel(Int_t module, Int_t column, Int_t row);
                                               // For testing purposes it is possible to add fake noisy channels
                                               // to the noisy pixel tree. These will be added to the hit histograms
  void CreateHistograms(void);                 // Create digit histograms
  void CreateNoisyChannelsTree(void);          // Create noisy channels tree
  TClonesArray *CreateDigitsArray(void) const; // Create the SPD digits array
  Bool_t CreateGeometryObj(void);              // Creation of geometry object
  Bool_t GetTARGETDigits(const char *fileName);   // Get the TARGET digits
  Bool_t GetgIlc(void);                      // Get the gIlc object
  Bool_t FillHistograms(void);                 // Fill the histograms with digits
  void SetNumberOfModules(UInt_t n)            // Set the number of modules
    { fNumberOfModules = n; };
  UInt_t GetNumberOfModules(void) const        // Get the number of modules
    { return fNumberOfModules; };

  // IlcTARGETGeometry *fGeometryObj;             //! Pointer to the geometry object
  IlcTARGETLoader *fTARGETLoader;                    //! TARGET loader
  IlcRunLoader *fRunLoader;                    //! Run Loader
  Double_t fThresholdRatio;                    //! Noisy channel ratio
  UInt_t fThreshold;                           //! Noisy channel threshold
  UInt_t fMaximumNumberOfEvents;               //! Maximum number of events per histograms
  UInt_t fNumberOfModules;                     //! Number of modules, used for digit histograms
  UInt_t fHighestModuleNumber;                 //! The highest module number with found bad channels
  UInt_t fNumberOfColumns;                     //! Number of SPD columns
  UInt_t fNumberOfRows;                        //! Number of SPD rows
  UInt_t fSelectedAlgorithm;                   //! Removal algorithm selection, either set to
                                               //! kOptimizedForRealData or kOptimizedForCalibrationData
  UInt_t fGeometryMode;                        //! Geometry mode (kILCGeometry is default, alt is kTestBeamGeometry)
  Int_t *fNumberOfBadChannels;                 //! Total number of bad channels counter
  Int_t fIndex;                                //! Bad channels array index to be stored in fBadChannelsIndexArray
  Bool_t fInit;                                //! Initialization boolean (true when histograms are created)
  Bool_t fVMEMode;                             //! Initialization boolean (true when using a VME file as input)
  TObjArray *fDigitsHistogram;                 //! Digits histogram array
  TObjArray *fBadChannelsObjArray;             //! Bad channels array (size unknown initially)
  Int_t *fBadChannelsIntArray;                 //! Bad channels array
  Int_t *fBadChannelsIndexArray;               //! Indices for the bad channels
  IlcTARGETBadChannelsSPD *fBadChannelsContainer; //! Container object for database storage

  ClassDef(IlcTARGETPreprocessorSPD,1)
};

#endif
