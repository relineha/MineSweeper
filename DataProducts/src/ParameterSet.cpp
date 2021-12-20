///////////////////////////////////////////////////////////////
//
// ParameterSet.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the ParameterSet object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

//General includes
#include <iostream>

//MineSweeper Includes
#include "../include/ParameterSet.hh"
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(ParameterSet)



//Constructor
ParameterSet::ParameterSet()
{
}

//Destructor
ParameterSet::~ParameterSet()
{
}

//Set functions: verbosity level
void ParameterSet::SetVerbosity( int verbosity )
{
  f_Verbosity = verbosity;
}

//Set function: pixels per mm
void ParameterSet::SetPixelsPerMm( double pixelsPerMm )
{
  f_PixelsPerMm = pixelsPerMm;
}

//Set function: grid Name
void ParameterSet::SetGridName( std::string name )
{
  f_GridName = name;
}

//Set function: rgb weights
void ParameterSet::SetRGBWeights( int iRGB, double val )
{
  f_RGBWeights[iRGB] = val;
}

//Set function: fft negative threshold
void ParameterSet::SetFftNegThresh( double val )
{
  f_FftNegThresh = val;
}

//Set function: fft positive threshold
void ParameterSet::SetFftPosThresh( double val )
{
  f_FftPosThresh = val;
}

//Set function: simple hit finder brightness threshold
void ParameterSet::SetSimpleHitFinderBrightThresh( double val )
{
  f_SHFBrightThresh = val;
}

//Set function: simple hit finder fiducial area distance to edge of picture (in pixels)
void ParameterSet::SetSimpleHitFinderFiducialFromWall( int val )
{
  f_SHFFiducialFromWall = val;
}

//Set function: db cluster minimum points
void ParameterSet::SetDBClusterMinPts( size_t val )
{
  f_DBMinPts = val;
}

//Set function: db cluster epsilon
void ParameterSet::SetDBClusterEpsilon( double val )
{
  f_DBEpsilon = val;
}

//Set function: duplicate cluster distance
void ParameterSet::SetDuplicateClustDistance( double val )
{
  f_DuplicateClustDist = val;
}

//Set function: debug plots pic
void ParameterSet::SetDebugPlotsPicName( std::string name )
{
  f_DebugPlotsPicName = name;
}

//Set function: hough point cutoff
void ParameterSet::SetHoughPointCutoff( double val )
{
  f_HoughPointCutoff = val;
}

//Set function: baseline difference
void ParameterSet::SetBaselineDifference( double val )
{
  f_BaselineDifference = val;
}

//Set function: is pic inverted
void ParameterSet::SetIsPicInvertedXY( int val )
{
  f_IsPicInvertedXY = val;
}

//Set the run mode
void ParameterSet::SetRunMode( std::string name )
{
  f_RunMode = name;
}

//Set the grid symmetry status
void ParameterSet::SetGridSymmetryStatus( bool val )
{
  f_GridSymmetryStatus = val;
}

//Set the support ring reflection status
void ParameterSet::SetSupportRingReflectionStatus( bool val )
{
  f_SupportRingReflectionStatus = val;
}

//Set the duplicate cluster fiducialization fraction
void ParameterSet::SetDuplicateClustFiducialFraction( double val )
{
  f_DuplicateClustFiducialFraction = val;
}

//Set the phi of the clocking mark (in degrees) relative to the positive X axis.
void ParameterSet::SetClockPhi( double val )
{
  f_ClockPhi = val;
}

//Set the grid scan finish status
void ParameterSet::SetGridScanFinishStatus( bool val )
{
  f_GridScanFinishStatus = val;
}

//Set the max clusters per prod file
void ParameterSet::SetMaxClustersPerProdFile( int val )
{
  f_MaxClustersPerProdFile = val;
}

//Set the write mode (write being writing from production)
void ParameterSet::SetWriteMode( std::string name )
{
  f_WriteMode = name;
}

//Set the number of prod files to read (read being in analysis)
void ParameterSet::SetReadNumber( int val )
{
  f_ReadNumber = val;
}












//Get functions: verbosity level
int ParameterSet::GetVerbosity() const
{
  return f_Verbosity;
}

//Get function: pixels per mm
double ParameterSet::GetPixelsPerMm() const
{
  return f_PixelsPerMm;
}

//Get function: grid name
std::string ParameterSet::GetGridName() const
{
  return f_GridName;
}

//Get function: rgb weights
double ParameterSet::GetRGBWeights( int iRGB ) const
{
  return f_RGBWeights[iRGB];
}

//Get function: negative threshold for FFT cuts
double ParameterSet::GetFftNegThresh() const
{
  return f_FftNegThresh;
}

//Get function: positive threshold for FFT cuts
double ParameterSet::GetFftPosThresh() const
{
  return f_FftPosThresh;
}

//Get function: simple hit finder brightness threshold
double ParameterSet::GetSimpleHitFinderBrightThresh() const
{
  return f_SHFBrightThresh;
}

//Get function: simple hit finder fiducial area distance from wall (in pixels)
int ParameterSet::GetSimpleHitFinderFiducialFromWall() const
{
  return f_SHFFiducialFromWall;
}

//Get the min points for dbcluster
size_t ParameterSet::GetDBClusterMinPts() const
{
  return f_DBMinPts;
}

//Get the epsilon neighborhood for dbcluster
double ParameterSet::GetDBClusterEpsilon() const
{
  return f_DBEpsilon;
}

//Get the distance for duplicate clusters in different pics
double ParameterSet::GetDuplicateClustDistance() const
{
  return f_DuplicateClustDist;
}

//Get the name of the picture used for debugging plots (FFT, IFFT, hough, simple hit finder, etc.)
std::string ParameterSet::GetDebugPlotsPicName() const
{
  return f_DebugPlotsPicName;
}

//Get function: hough point cutoff
double ParameterSet::GetHoughPointCutoff() const
{
  return f_HoughPointCutoff;
}

//Get function: baseline difference value
double ParameterSet::GetBaselineDifference() const
{
  return f_BaselineDifference;
}

//Get function: is pic inverted
int ParameterSet::GetIsPicInvertedXY() const
{
  return f_IsPicInvertedXY;
}

//Get functions: run mode
std::string ParameterSet::GetRunMode() const
{
  return f_RunMode;
}

//Get function: symmetry status
bool ParameterSet::GetGridSymmetryStatus() const
{
  return f_GridSymmetryStatus;
}

//Get function: support ring reflection status
bool ParameterSet::GetSupportRingReflectionStatus() const
{
  return f_SupportRingReflectionStatus;
}

//Get function: duplicate cluster fiducial fraction
double ParameterSet::GetDuplicateClustFiducialFraction() const
{
  return f_DuplicateClustFiducialFraction;
}

//Get function: clock phi
double ParameterSet::GetClockPhi() const
{
  return f_ClockPhi;
}

//Get function: grid scan finish status
bool ParameterSet::GetGridScanFinishStatus() const
{
  return f_GridScanFinishStatus;
}

//Get function: max cluster per prod file
int ParameterSet::GetMaxClustersPerProdFile() const
{
  return f_MaxClustersPerProdFile;
}

//Get function: write mode
std::string ParameterSet::GetWriteMode() const
{
  return f_WriteMode;
}

//Get function: the number of prod files to read (read being in analysis)
int ParameterSet::GetReadNumber() const
{
  return f_ReadNumber;
}

