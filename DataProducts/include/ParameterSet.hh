///////////////////////////////////////////////////////////////
//
// ParameterSet.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the ParameterSet object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

#ifndef PARAMETERSET_HH
#define PARAMETERSET_HH


//Standard includes
#include <string>
#include "TObject.h"
#include "TBuffer.h"

class ParameterSet : public TObject
{


private:
  //Info on verbosity level:
  // 1: Only all setup info is printed
  // 2: Only limited sweep info is printed
  // 22: Extensive sweep info is printed
  // 3: DBCluster info is printed
  // 5: All info is printed.
  int f_Verbosity;
  double f_PixelsPerMm;
  std::string f_GridName;

  //Info on weights: These are the different factors that
  //are multiplied by the images' R/G/B values to get a 
  //grayscale value that we like
  double f_RGBWeights[3];

  //Parameters for the FFT cuts used in removing the wires from
  //the images
  double f_FftNegThresh;
  double f_FftPosThresh;

  //Parameters for the simple hit finder
  double f_SHFBrightThresh;
  int f_SHFFiducialFromWall; //in pixels

  //Parameters for the DBCluster algorithm
  size_t f_DBMinPts;         //nearby neighbors necessary to initiate a seed
  double f_DBEpsilon;        //epsilon distance (in pixels) for "proximity"

  //Duplicate cluster distance cutoff (for different pictures) 
  double f_DuplicateClustDist; //in mm

  //Debug plots picture
  std::string f_DebugPlotsPicName;
  
  //Hough transform parameters
  double f_HoughPointCutoff; //Currently, the value in hough space above which a point gets considered a line. This may cause duplicate lines, just fyi
  double f_BaselineDifference; //The deviation from the baseline necessary to be considered a valid pixel in the hough transform.

  //Picture inversion setting
  int f_IsPicInvertedXY; //-1 implies inversion. 1 implies non-inversion

  //Run mode. Are we running in production mode (creating a grid object from pictures and sending it to a root file), or
  //are we running in analysis mode (reading in the created grid object from the root file and analyzing the data).
  //The two options are PRODUCTION and ANALYSIS
  std::string f_RunMode;

  //Grid symmetry status: if we had to cut off some part of the grid due to space constraints imposed by the UV lamp position, this
  //value must be set to false. Otherwise, it is true.
  bool f_GridSymmetryStatus;

  //Support ring reflection status: if the support ring was too reflective in some spots and we want to cut out those images, this should be true.
  bool f_SupportRingReflectionStatus;

  //Fiducialization parameter for duplicate cluster elimination. Only check for duplicates in this outer fraction (all 4 sides) of the picture.
  double f_DuplicateClustFiducialFraction;

  //Phi of the clocking mark relative to the +X axis, in degrees
  double f_ClockPhi;
  
  //Grid scan completion status. If true, the scan was completed. If false, it is a partial (aborted) scan.
  bool f_GridScanFinishStatus;

  //Max number of clusters allowed per output file
  int f_MaxClustersPerProdFile;

  //Production file write mode: multifile or singlefile, with the number of files that are ultimately produced
  //(f_Readnumber is only useful for the analysis side)
  std::string f_WriteMode;
  int f_ReadNumber; 
  
public:
  ParameterSet();
  ~ParameterSet();

  //Set Functions
  void SetVerbosity( int verbosity );
  void SetPixelsPerMm( double pixelsPerMm );
  void SetGridName( std::string name );
  void SetRGBWeights( int iRGB, double val );
  void SetFftNegThresh( double val );
  void SetFftPosThresh( double val );
  void SetSimpleHitFinderBrightThresh( double val );
  void SetSimpleHitFinderFiducialFromWall( int val );
  void SetDBClusterMinPts( size_t val );
  void SetDBClusterEpsilon( double val );
  void SetDuplicateClustDistance( double val );
  void SetDebugPlotsPicName( std::string name );
  void SetHoughPointCutoff( double val );
  void SetBaselineDifference( double val );
  void SetIsPicInvertedXY( int val );
  void SetRunMode( std::string name );
  void SetGridSymmetryStatus( bool val );
  void SetSupportRingReflectionStatus( bool val );
  void SetDuplicateClustFiducialFraction( double val );
  void SetClockPhi( double val );
  void SetGridScanFinishStatus( bool val );
  void SetMaxClustersPerProdFile( int val );
  void SetWriteMode( std::string name );
  void SetReadNumber( int val );

  //Get Functions
  int GetVerbosity() const;
  double GetPixelsPerMm() const;
  std::string GetGridName() const;
  double GetRGBWeights( int iRGB ) const;
  double GetFftNegThresh() const;
  double GetFftPosThresh() const;
  double GetSimpleHitFinderBrightThresh() const;
  int GetSimpleHitFinderFiducialFromWall() const;
  size_t GetDBClusterMinPts() const;
  double GetDBClusterEpsilon() const;
  double GetDuplicateClustDistance() const;
  std::string GetDebugPlotsPicName() const;
  double GetHoughPointCutoff() const;
  double GetBaselineDifference() const;
  int GetIsPicInvertedXY() const;
  std::string GetRunMode() const;
  bool GetGridSymmetryStatus() const;
  bool GetSupportRingReflectionStatus() const;
  double GetDuplicateClustFiducialFraction() const;
  double GetClockPhi() const;
  bool GetGridScanFinishStatus() const;
  int GetMaxClustersPerProdFile() const;
  std::string GetWriteMode() const;
  int GetReadNumber() const;

  //Allows interface with ROOT I/O
  ClassDef(ParameterSet,1)


  

};


#endif
