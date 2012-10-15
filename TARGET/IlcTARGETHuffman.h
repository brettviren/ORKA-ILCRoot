#ifndef IlcTARGETHUFFMAN_H
#define IlcTARGETHUFFMAN_H

///////////////////////////////////////////////////
//  Huffman Table associated classes for set:TARGET //
///////////////////////////////////////////////////

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Attention! Two classes in this file.
// They have to stay in the same file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <TObject.h>

class IlcTARGETInStream;
class TObjectArray;
class IlcTARGETHNode: public TObject  {

 public:
  IlcTARGETHNode();
  IlcTARGETHNode(UChar_t symbol, ULong_t freq);
  virtual   ~IlcTARGETHNode() {
    // destructor
  }
  IlcTARGETHNode(const IlcTARGETHNode &source); // copy constructor
  IlcTARGETHNode& operator=(const IlcTARGETHNode &source); // ass. op.

  Bool_t IsSortable() const {
    // is sortable
    return kTRUE;
  }
  Int_t Compare(const TObject *obj) const;
  
  ClassDef(IlcTARGETHNode,1)     //HuffT node object for set:TARGET

 public:

  UChar_t    fSymbol;        // comment to be written
  ULong_t    fFrequency;     // comment to be written
  IlcTARGETHNode     *fLeft;    // comment to be written
  IlcTARGETHNode     *fRight;   // comment to be written
  IlcTARGETHNode     *fFather;  // not used
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  Attention! Next class has kept deliberaty in 
//  the same file as the previous one
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
//___________________________________________
class IlcTARGETHTable: public TObject{
  
public:
  IlcTARGETHTable(); 
  IlcTARGETHTable(Int_t size);
  virtual   ~IlcTARGETHTable();
  IlcTARGETHTable(const IlcTARGETHTable &source); // copy constructor
  IlcTARGETHTable& operator=(const IlcTARGETHTable &source); // ass. op.
  
  Int_t  Size() {
    // size
    return fSize;
  }
  UChar_t   *CodeLen() {
    // code len
    return fCodeLen;
  }
  ULong_t *Code() {
    // code
    return fCode;
  }
  TObjArray  *HNodes() {
    // HNodes
    return fHNodes;
  }
  
  
  void GetFrequencies(Int_t len, UChar_t *stream);
  void BuildHTable();   
  Bool_t SpanTree(IlcTARGETHNode*start, ULong_t code, UChar_t len);
  void ResetHNodes();
  void ClearTable();
  
 protected:

  Int_t          fSize;     // size of the arrays
  UChar_t       *fCodeLen;  //![fSize] number of bits array
  ULong_t       *fCode;     //![fSize] coded symbols array
  
  Short_t       *fSym;      //![fSize] array of input symbols
  TObjArray     *fHNodes;   // array of nodes
  Int_t          fNnodes;   // number of nodes

  ClassDef(IlcTARGETHTable,1)     //Huffman Table  object for set:TARGET
    };

#endif
