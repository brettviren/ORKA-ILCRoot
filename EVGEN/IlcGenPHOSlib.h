#ifndef ILCGENPHOSLIB_H
#define ILCGENPHOSLIB_H
#include "IlcGenLib.h"
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenPHOSlib.h 48521 2011-03-17 21:02:01Z morsch $ */

//======================================================================
//  IlcGenPHOSlib class contains parameterizations of the
//  pion, kaon, eta, omega, etaprime, phi and baryon (proton, 
//  antiproton, neutron and anti-neutron) particles for the 
//  study of the neutral background in PHOS detector. 
//  Additional particle species simulation options has been added:
//  Charged Pion, Charged Kaons, KLong Proton, Anti-Proton, Neutron,
//  Anti-Neutron --> Changes made by Gustavo Conesa in November 2004
//======================================================================

class TRandom;

class IlcGenPHOSlib :
public IlcGenLib
{
 public:
    enum constants{kPion, kChargedPion, kPi0, kPi0Flat, kKaon, kChargedKaon, kKaon0L,
		   kEta, kEtaFlat,kOmega, kOmegaFlat, kEtaPrime, kPhi, 
		   kBaryon, kProton, kAProton, kNeutron, kANeutron};
    GenFunc   GetPt(Int_t param, const char* tname=0) const;
    GenFunc   GetY (Int_t param, const char* tname=0) const;
    GenFuncIp GetIp(Int_t param, const char* tname=0) const;   
 private:
// pions
    static Double_t PtPion(const Double_t *px, const Double_t *dummy);
    static Double_t PtScal(Double_t pt, Int_t np);
    static Double_t YPion( const Double_t *py, const Double_t *dummy);
    static Int_t    IpPion(TRandom* ran);
    static Int_t    IpChargedPion(TRandom* ran);
//  pi0 Distribution
    static Double_t PtPi0(const Double_t *px, const Double_t *dummy);
//  pi0 Flat Distribution
    static Double_t PtPi0Flat(const Double_t *px, const Double_t *dummy);
    static Double_t YPi0Flat( const Double_t *py, const Double_t *dummy);
    static Int_t    IpPi0Flat(TRandom* ran); 
    
// kaons
    static Double_t PtKaon(const Double_t *px, const Double_t *dummy);
    static Double_t YKaon( const Double_t *py, const Double_t *dummy);
    static Int_t    IpKaon(TRandom* ran);
    static Int_t    IpChargedKaon(TRandom* ran);
    static Int_t    IpKaon0L(TRandom* ran);
// etas
    static Double_t PtEta(const Double_t *px, const Double_t *dummy);
    static Double_t YEta( const Double_t *py, const Double_t *dummy);
    static Int_t    IpEta(TRandom *ran);
    
// etas Flat Distribution
    static Double_t PtEtaFlat(const Double_t *px, const Double_t *dummy);
    static Double_t YEtaFlat( const Double_t *py, const Double_t *dummy);
    static Int_t    IpEtaFlat(TRandom *ran);

// omegas
    static Double_t PtOmega(const Double_t *px, const Double_t *dummy);
    static Double_t YOmega( const Double_t *py, const Double_t *dummy);
    static Int_t    IpOmega(TRandom *ran);
   
// omegas  Flat Distribution
    static Double_t PtOmegaFlat(const Double_t *px, const Double_t *dummy);
    static Double_t YOmegaFlat( const Double_t *py, const Double_t *dummy);
    static Int_t    IpOmegaFlat(TRandom *ran); 

// etaprime
    static Double_t PtEtaprime(const Double_t *px, const Double_t *dummy);
    static Double_t YEtaprime( const Double_t *py, const Double_t *dummy);
    static Int_t    IpEtaprime(TRandom* ran);
    
// phis
    static Double_t PtPhi(const Double_t *px, const Double_t *dummy);
    static Double_t YPhi( const Double_t *py, const Double_t *dummy);
    static Int_t    IpPhi(TRandom* ran);
    
// baryons
    static Double_t PtBaryon(const Double_t *px, const Double_t *dummy);
    static Double_t YBaryon( const Double_t *py, const Double_t *dummy);
    static Int_t    IpBaryon(TRandom *ran);
    static Int_t    IpProton(TRandom *ran);
    static Int_t    IpAProton(TRandom *ran);
    static Int_t    IpNeutron(TRandom *ran);
    static Int_t    IpANeutron(TRandom *ran);

    ClassDef(IlcGenPHOSlib,0) // Library providing y and pT parameterisations
};
#endif







