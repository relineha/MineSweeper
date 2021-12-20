/////////////////////////////////////////////////////////
//
// AnalysisFunctions.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/31/2018
//
// This is a test analysis function definition file for
// for the output of minesweeper. You'll need to add
// your forward declarations to this file to do analysis.
// 
//
/////////////////////////////////////////////////////////

#ifndef ANALYSISFUNCTIONS_HH
#define ANALYSISFUNCTIONS_HH


void Analyze( const Grid theTestGrid );
void PlotPicLocations();
void PlotClusterInfo();
bool CXBadPixelClusters( const Cluster clu );
void RunMetrology();
double computeMaxPixelVal( Cluster clu );
double computeSigmaPixelVal( Cluster clu );
double computeOGMaxPixelVal( Cluster clu );
double computeOGSigmaPixelVal( Cluster clu );


#endif
