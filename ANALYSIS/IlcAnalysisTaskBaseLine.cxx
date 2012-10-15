/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcAnalysisTaskBaseLine.cxx 46301 2011-01-06 14:25:27Z agheata $ */

//
//
// This empty task is used for the analysis train to estimate the memory and CPU consumption without any user code
//
//

#include "IlcAnalysisTaskBaseLine.h"

ClassImp(IlcAnalysisTaskBaseLine)

IlcAnalysisTaskBaseLine::IlcAnalysisTaskBaseLine() 
   :IlcAnalysisTaskSE()
{
}

IlcAnalysisTaskBaseLine::IlcAnalysisTaskBaseLine(const char *name)
   :IlcAnalysisTaskSE(name)
{
}

IlcAnalysisTaskBaseLine::~IlcAnalysisTaskBaseLine()
{
}

void IlcAnalysisTaskBaseLine::UserExec(Option_t *) 
{
}
