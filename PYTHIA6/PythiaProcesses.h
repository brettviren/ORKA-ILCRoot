#ifndef PYTHIAPROCESS_H
#define PYTHIAPROCESS_H

typedef enum
{
    kPyCharm, kPyBeauty, kPyCharmUnforced, kPyBeautyUnforced,
    kPyJpsi, kPyJpsiChi, kPyMb, kPyMbWithDirectPhoton, kPyMbNonDiffr, kPyJets, kPyDirectGamma,
    kPyCharmPbPbMNR, kPyD0PbPbMNR, kPyDPlusPbPbMNR, kPyDPlusStrangePbPbMNR, kPyBeautyPbPbMNR,
    kPyCharmpPbMNR, kPyD0pPbMNR, kPyDPluspPbMNR, kPyDPlusStrangepPbMNR, kPyBeautypPbMNR,
    kPyCharmppMNR, kPyCharmppMNRwmi, kPyD0ppMNR, kPyDPlusppMNR, kPyDPlusStrangeppMNR, 
    kPyBeautyppMNR, kPyBeautyppMNRwmi, kPyBeautyJets, kPyW, kPyZ, kPyLambdacppMNR, kPyMbMSEL1,
    kPyOldUEQ2ordered, kPyOldUEQ2ordered2, kPyOldPopcorn,
    kPyLhwgMb, kPyMbDefault, kPyMbAtlasTuneMC09
}
Process_t;

#endif
