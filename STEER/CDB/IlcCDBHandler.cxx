/*************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * *                                                                        *
 * * Permission to use, copy, modify and distribute this software and its   *
 * * documentation strictly for non-commercial purposes is hereby granted   *
 * * without fee, provided that the above copyright notice appears in all   *
 * * copies and that both the copyright notice and this permission notice   *
 * * appear in the supporting documentation. The authors make no claims     *
 * * about the suitability of this software for any purpose. It is          *
 * * provided "as is" without express or implied warranty.                  *
 * **************************************************************************/

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//  The SAX XML file handler used in the CDBManager                       //
//                                                                        //
//  Author:                                                               //
//    Chiara Zampolli (Chiara.Zampolli@cern.ch)                           //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <Riostream.h>

#include <TList.h>
#include <TObject.h>
#include <TXMLAttr.h>
#include <TSAXParser.h>

#include "IlcLog.h"
#include "IlcCDBHandler.h"

ClassImp(IlcCDBHandler)

  
//_____________________________________________________________________________
IlcCDBHandler::IlcCDBHandler()
	:TObject(),
	 fRun(-1),
	 fStartRunRange(-1),
	 fEndRunRange(-1),
	 fOCDBFolder("")
{
	//
	// IlcCDBHandler default constructor
	//
}

//_____________________________________________________________________________
IlcCDBHandler::IlcCDBHandler(Int_t run)
	:TObject(),
	 fRun(run),
	 fStartRunRange(-1),
	 fEndRunRange(-1),
	 fOCDBFolder("")
{
	//
	// IlcCDBHandler constructor with requested run
	//
}

//_____________________________________________________________________________
IlcCDBHandler::IlcCDBHandler(const IlcCDBHandler &sh)
	:TObject(sh),
	 fRun(sh.fRun),
	 fStartRunRange(sh.fStartRunRange),
	 fEndRunRange(sh.fEndRunRange),
	 fOCDBFolder(sh.fOCDBFolder)
{
	//
	// IlcCDBHandler copy constructor
	//
}

//_____________________________________________________________________________
IlcCDBHandler &IlcCDBHandler::operator=(const IlcCDBHandler &sh)
{
	//
	// Assignment operator
	//
	if (&sh == this) return *this;
	
	new (this) IlcCDBHandler(sh);
	return *this;
}

//_____________________________________________________________________________
IlcCDBHandler::~IlcCDBHandler()
{
	//
	// IlcCDBHandler destructor
	//	
}

//_____________________________________________________________________________
void IlcCDBHandler::OnStartDocument()
{
	// if something should happen right at the beginning of the
	// XML document, this must happen here
	IlcInfo("Reading XML file for LHCPerdiod <-> Run Range correspondance");
}

//_____________________________________________________________________________
void IlcCDBHandler::OnEndDocument()
{
	// if something should happen at the end of the XML document
	// this must be done here
}

//_____________________________________________________________________________
void IlcCDBHandler::OnStartElement(const char *name, const TList *attributes)
{
	// when a new XML element is found, it is processed here

	// set the current system if necessary
	TString strName(name);
	IlcDebug(2,Form("name = %s",strName.Data()));
	Int_t startRun=-1;
	Int_t endRun=-1;
	TXMLAttr* attr;
	TIter next(attributes);
	while ((attr = (TXMLAttr*) next())) {
		TString attrName = attr->GetName();
		IlcDebug(2,Form("Name = %s",attrName.Data())); 
		if (attrName == "StartRunRange"){
			startRun = (Int_t)(((TString)(attr->GetValue())).Atoi());
			IlcDebug(2,Form("startRun = %d",startRun));
		}
		if (attrName == "EndRunRange"){
			endRun = (Int_t)(((TString)(attr->GetValue())).Atoi());
			IlcDebug(2,Form("endRun = %d",endRun));
		}				
		if (attrName == "OCDBFolder"){
			if (fRun>=startRun && fRun<=endRun && startRun!=-1 && endRun!=-1){
				fOCDBFolder = (TString)(attr->GetValue());
				IlcDebug(2,Form("OCDBFolder = %s",fOCDBFolder.Data()));
				fStartRunRange = startRun;
				fEndRunRange = endRun;
			}
		}
	}	
	return;
}
//_____________________________________________________________________________
void IlcCDBHandler::OnEndElement(const char *name)
{
	// do everything that needs to be done when an end tag of an element is found
	TString strName(name);
	IlcDebug(2,Form("name = %s",strName.Data()));
}

//_____________________________________________________________________________
void IlcCDBHandler::OnCharacters(const char *characters)
{
	// copy the text content of an XML element
	//fContent = characters;
	TString strCharacters(characters);
	IlcDebug(2,Form("characters = %s",strCharacters.Data()));
}

//_____________________________________________________________________________
void IlcCDBHandler::OnComment(const char* /*text*/)
{
	// comments within the XML file are ignored
}

//_____________________________________________________________________________
void IlcCDBHandler::OnWarning(const char *text)
{
	// process warnings here
	IlcInfo(Form("Warning: %s",text));
}

//_____________________________________________________________________________
void IlcCDBHandler::OnError(const char *text)
{
	// process errors here
	IlcError(Form("Error: %s",text));
}

//_____________________________________________________________________________
void IlcCDBHandler::OnFatalError(const char *text)
{
	// process fatal errors here
	IlcFatal(Form("Fatal error: %s",text));
}

//_____________________________________________________________________________
void IlcCDBHandler::OnCdataBlock(const char* /*text*/, Int_t /*len*/)
{
	// process character data blocks here
	// not implemented and should not be used here
}

