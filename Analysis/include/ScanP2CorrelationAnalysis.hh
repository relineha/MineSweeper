/////////////////////////////////////////////////////////
//
// ScanP2CorrelationAnalysis.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 8/18/2018
//
// This is a header file for the functions that run
// analysis of correlations between Phase 2 output and
// the scan data.
//
// 
//
/////////////////////////////////////////////////////////

#ifndef SCANP2CORRELATIONANALYSIS_HH
#define SCANP2CORRELATIONANALYSIS_HH

#include "TFile.h"

void AnalyzeCorrelations( const std::vector<Grid> & gridVect, const std::vector<TFile*> p2FileVect );

#endif
