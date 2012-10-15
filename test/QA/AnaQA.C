/*
 *  AnaQA.C
 *  
 *
 *  Created by schutz on 29/09/08.
 *  Copyright 2008 CERN. All rights reserved.
 *
 */
#include <TFile.h>
#include <TList.h>
#include <TNamed.h>
#include "IlcQA.h"

void AnaQA(Int_t run) 
{
// Macro to analyse the output of the QAChecker
  
  // Open the file that holds the IlcQA object
  const char * resultFileName = "QA.root" ; //IlcQA::GetQAResultFileName() ; 
  const char * msgE = "QA ERROR: " ; 
  const char * msgS = "QA SIGNAL: " ; 

  TFile * inputQAFile = TFile::Open(resultFileName) ; 
  if ( ! inputQAFile ) {
    printf("QA ERROR: File %s not found\n", IlcQA::GetQAResultFileName()) ;
    exit(1) ; 
  }
  // Get the IlcQA object from the file 
 // inputQAFile.ls() ; 
  IlcQA * qa = dynamic_cast<IlcQA*>(inputQAFile->Get(IlcQA::GetQAName())) ; 
  // Show the status of all Detectors
  for (Int_t det = 0 ; det < IlcQA::kNDET ; det++) {
    for (Int_t task = 0 ; task < IlcQA::kNTASK ; task++) {
      if (qa->IsSetAny(IlcQA::DETECTORINDEX_t(det), IlcQA::ILCTASK_t(task))) {
        qa->ShowStatus(IlcQA::DETECTORINDEX_t(det), IlcQA::ILCTASK_t(task)) ;         
        // found a bad detector, open the QA data file and search for the faulty histogram
        TFile * dataQAFile = TFile::Open(IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ; 
        if ( ! dataQAFile ) {
          printf("%s File %s not found\n", msgE, IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
          exit(1) ; 
        }    
        dataQAFile->cd(IlcQA::GetDetName(IlcQA::DETECTORINDEX_t(det))) ; 
        TDirectory * saveDir = gDirectory ; 
        switch (task) {
          case IlcQA::kNULLTASK:
            break ; 
          case IlcQA::kRAW:
            Bool_t dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kRAWS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kRAWS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kRAWS).Data(), 
                           obj->GetName()); 
                }
              }
            }
            break ;  
          case IlcQA::kSIM:
            dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kHITS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kHITS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kHITS).Data(), 
                           obj->GetName()); 
                }
              }
            }
            dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kSDIGITS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kSDIGITS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kSDIGITS).Data(), 
                           obj->GetName()); 
                }
              }
            }
            
            dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kDIGITS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kDIGITS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kDIGITS).Data(), 
                           obj->GetName()); 
                }
              }
            }
            break ;
          case IlcQA::kREC:
            dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kRECPOINTS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kRECPOINTS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kRECPOINTS).Data(), 
                           obj->GetName()) ; 
                }
              }
            }
            break ;
            case IlcQA::kESD:
            dir = saveDir->cd(IlcQA::GetTaskName(IlcQA::kESDS)) ; 
            if ( ! dir ) {
              printf("%s Directory %s not found in %s\n", msgE, IlcQA::GetTaskName(IlcQA::kESDS).Data(), IlcQA::GetQADataFileName(IlcQA::GetDetName(det), run)) ;
            } else {
              TList * listofkeys = gDirectory->GetListOfKeys() ; 
              for (Int_t key = 0 ; key < listofkeys->GetEntries() ; key++) {
                TNamed * obj = dynamic_cast<TNamed*>(listofkeys->At(key)) ; 
                if (obj) {
                  Bool_t rv = obj->TestBit(IlcQA::GetQABit()) ;
                  if (rv)
                    printf("%s QA bit set in %s/%s/%s\n", 
                           msgS, 
                           IlcQA::GetDetName(det), 
                           IlcQA::GetTaskName(IlcQA::kESDS).Data(), 
                           obj->GetName()); 
                }
              }
            }
            break ;
            case IlcQA::kANA:
            break ;
          default:
             break ;
        }
        dataQAFile->Close() ; 
      }
    }
  }
  inputQAFile->Close() ; 
}
