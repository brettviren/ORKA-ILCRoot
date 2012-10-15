#ifndef ILC_SURVEY_OBJ_H
#define ILC_SURVEY_OBJ_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////
//                                                                 //
//  class IlcSurveyObj						   //
//  Retrieve and Convert survey data into ROOT Objects   	   //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include <TObject.h>

#include <TString.h>
#include <TObjArray.h>

class IlcSurveyPoint;
class TGridResult;

class IlcSurveyObj: public TObject {

 public:
  IlcSurveyObj();
  ~IlcSurveyObj();
  Bool_t FillFromLocalFile(const Char_t* filename);
  Bool_t Fill(TString detector, Int_t reportNumber,
	      Int_t reportVersion, TString username = "");
  Bool_t Fill(TString detector, Int_t reportNumber,
	      TString username = "");

  static void ListValidDetectors();
  Int_t ListReports(TString detector = "", Int_t year = -1,
		    Int_t reportNumber = -1,
		    Int_t reportVersion = -1);

  void SetGridUser(TString username);
 
  // Number of points (IlcSurveyPoint) in the TObjArray
  Int_t GetEntries() const {return fDataPoints->GetEntries();};

  TString GetReportTitle() const {return fTitle;};
  TString GetReportDate() const {return fDate;};
  TString GetDetector() const {return fDetector;};
  TString GetURL() const {return fURL;};
  Int_t GetReportNumber() const {return fReportNr;};
  Int_t GetReportVersion() const {return fVersion;};

  // General comments and observations
  TString GetObservations() const {return fObs;};

  // Coordinate system used for the measurements
  TString GetCoordSys() const {return fCoordSys;};

  // Units used in the measurement
  TString GetUnits() const {return fUnits;};

  // Number of columns read from file (in the "Data" section)
  Int_t GetNrColumns() const {return fNrColumns;};

  // TObjArray with the names of the columns read
  TObjArray *GetColumnNames() const {return fColNames.Tokenize(',');};

  // TObjArray with the points read (IlcSurveyPoint)
  TObjArray *GetData() const {return fDataPoints;};

  Bool_t IsValid() const {return fIsValid;};

  
 private:
  TString fTitle;     // Report Title
  TString fDate;      // Report Date
  TString fDetector;  // Subdetector (or structure) surveyed
  TString fURL;       // Report URL in EDMS
  Int_t fReportNr;    // Report Number
  Int_t fVersion;     // Report Version
  TString fObs;       // General observations / comments
  TString fCoordSys;  // Measurements coordinate system
  TString fUnits;     // Measurements units
  Int_t fNrColumns;   // Number of columns in data values
  TString fColNames;  // Column names sepparated by commas
  Bool_t fIsValid;    // Is the data valid? (sucessfully parsed)
  TString fGridUser;  // Username to be used for the connection to GRID
  
  TObjArray *fDataPoints;	// Actual Data
  
  static const TString fgkStorage; // Storage
  static const TString fgkBaseFolder; // Base folder
  static const TString fgkValidDetectors;// Valid detectors
  static const TString fgkGRPDetectors;// GRP detectors
  static const TString fgkMUONDetectors;// MUON detectors
    
  Bool_t Connect(const char *gridUrl, const char *user);
  Bool_t OpenFile(TString openString);
  TString &Sanitize(TString str);
  Bool_t ParseBuffer(const Char_t* buf);
  void Reset();
  Bool_t IsValidDetector(TString detector) const;
  TString RealFolderName(TString detector) const;
  TString FileNamePathToDetector(TString filename) const;
  Int_t FileNamePathToReportYear(TString filename) const;
  Int_t FileNamePathToReportNumber(TString filename) const;
  Int_t FileNamePathToReportVersion(TString filename) const;
  TGridResult *QueryReports(TString detector, Int_t year,
			    Int_t reportNumber, Int_t reportVersion);

  IlcSurveyObj (const IlcSurveyObj& surveyObj);
  IlcSurveyObj& operator=(const IlcSurveyObj& surveyObj); 
  void AddPoint(IlcSurveyPoint* point);

  ClassDef(IlcSurveyObj, 1);
};

#endif
