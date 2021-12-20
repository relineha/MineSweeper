///////////////////////////////////////////////////////////////
//
// RunSweep.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/31/2018
//
// This file contains the definitions for the functions that 
// do the debris sweep in MineSweeper.
// 
// Do not tamper with this file!
//
///////////////////////////////////////////////////////////////

#ifndef RUNSWEEP_HH
#define RUNSWEEP_HH

//Standard includes
#include <vector>

//MineSweeper Includes
#include "ParameterSet.hh"
#include "Particle.hh"
#include "Picture.hh"

//Additional library includes
#include <fftw3.h>

void InitializePSetForSweep( const ParameterSet & pSet );
void RunParticleFinding( const ParameterSet & pSet, const std::vector<Picture> & picVect, std::vector<Particle> & particleVect, std::vector<Cluster> & clusterVect );
void FindParticles( Picture & thePic );
std::vector<std::vector<int> > ReadBitMapPic( std::string name );
double ComputeBWFromRGB( double R, double G, double B );
void RunTheFFTW( double ** inStandard, double ** outStandard, int dim1, int dim2, fftw_r2r_kind kind );
void FourierSpaceCuts( double ** fft_output, int dim1, int dim2 );
std::vector<std::pair<double,std::pair<int,int> > > simpleHitFinder( double ** ifft_output, int dim1, int dim2 );
void ComputeClusterParams(std::vector<Cluster> & clusters, const ParameterSet pS, const Picture pic, const int dim1, const int dim2, double ** bwPic_arr );
bool CheckForDuplicates( std::vector<Cluster> clustVect, Cluster clust );
std::vector<std::pair<double,double> > FindHoughLines( std::vector<std::pair<double,std::pair<int,int> > > reducedPic, std::string picName );
void FindBaseline( std::vector<std::vector<int> > picture );
std::vector<std::pair<double,std::pair<int,int> > > FilterOutBackgroundPix( std::vector<std::vector<int> > picture );
bool ClusterIsFiducial( Cluster theClust );
void ReorderClusterVect( std::vector<Cluster> & clustVect );

#endif
