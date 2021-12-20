///////////////////////////////////////////////////////////////
//
// RunSetup.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This file defines functions for setting up the MineSweeper
// software. It is responsible for reading in the data from
// the config file and filelist and for initializing the
// Picture object vector and ParameterSet object.
// 
// Do not tamper with this file!
//
///////////////////////////////////////////////////////////////

#ifndef RUNSETUP_HH
#define RUNSETUP_HH

//MineSweeper includes
#include "ParameterSet.hh"
#include "Geometry.hh"
#include "Picture.hh"

//Main function
void runMineSweeperSetup( ParameterSet & pSet, Geometry & geo, std::vector<Picture> & pictureVect );
void ParseConfigFile( ParameterSet & pSet );
void ReadPicFileList( std::vector<Picture> & picVect );
std::pair<double,double> FindCrosshairXY( std::string pfn );
void FilterOutRingPics( std::vector<Picture> & pictureVect );
std::pair<double,double> FilterOutRingPicsAsymmetric( std::vector<Picture> & pictureVect, Geometry & geo );
void FilterOutReflectionPics( std::vector<Picture> & pictureVect, std::pair<double,double> gridCenter );
std::pair<double,double> FilterOutRingPicsAsymmetricUnfinishedScan( std::vector<Picture> & pictureVect, Geometry & geo );
std::pair<std::pair<double,double>,double> computeGridCenterSimple( std::vector<Picture> & pictureVect );
double findMedianOfSortedList( std::vector<double> list );

#endif
