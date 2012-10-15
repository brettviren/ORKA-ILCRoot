#ifndef ILCEXPRESSION_H
#define ILCEXPRESSION_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcExpression.h 50636 2011-07-18 11:27:31Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  IlcExpression Class                                                      //                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
class TString;
class TObjArray;

// These are the valid operators types.

enum { kOpAND=1,   // AND '&'
       kOpOR,      // OR '|'
       kOpNOT };   // Unary negation '!'

class IlcExpression : public TObject {

public:
                           IlcExpression() : fVname(0), fArg1(0), fArg2(0), fOperator(0)  {}
                           IlcExpression( TString exp );
               virtual    ~IlcExpression();
                           IlcExpression( const IlcExpression& exp ) : TObject( exp ),
                                          fVname(exp.fVname),
                                          fArg1(exp.fArg1), fArg2(exp.fArg2),
                                          fOperator(exp.fOperator)  {}
         IlcExpression&    operator=(const IlcExpression& exp);

        virtual Bool_t     Value( const TObjArray & vars );
       virtual TString     Unparse() const;

                TString    fVname;   // Variable name

private:
         IlcExpression*    fArg1;         // left argument
         IlcExpression*    fArg2;         // right argument
                 Int_t     fOperator;     // operator

                           IlcExpression( int op, IlcExpression* a );
                           IlcExpression( int op, IlcExpression* a, IlcExpression* b );

             TObjArray*    Tokenize( TString str ) const;
  static IlcExpression*    Element( TObjArray &st, Int_t &i );
  static IlcExpression*    Primary( TObjArray &st, Int_t &i );
  static IlcExpression*    Expression( TObjArray &st, Int_t &i );

   ClassDef( IlcExpression, 2 )  // Class to evaluate an expression
};




///////////////////////////////////////////////////////////////////////////

class IlcVariableExpression: public IlcExpression {
public:
                     IlcVariableExpression( TString a ): IlcExpression() { fVname = a; };
                    ~IlcVariableExpression() {}
   virtual Bool_t    Value( const TObjArray& pgm );
  virtual TString    Unparse() const { return fVname; }

   ClassDef( IlcVariableExpression, 2 )  // Class to define a variable expression
};

#endif
