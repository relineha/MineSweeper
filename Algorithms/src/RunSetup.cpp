///////////////////////////////////////////////////////////////
//
// RunSetup.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This file holds functions for setting up the MineSweeper
// software. It is responsible for reading in the data from
// the config file and filelist and for initializing the
// Picture object vector and ParameterSet object.
// It also now (8/16/2018) initializes the geometry object
// that gets plugged into the grid.
// 
// Do not tamper with this file!
//
///////////////////////////////////////////////////////////////


//Standard includes
#include <iostream>
#include <fstream>
#include <cmath>

//MineSweeper includes
#include "ParameterSet.hh"
#include "Picture.hh"
#include "Geometry.hh"
#include "RunSetup.hh"
#include "RunSweep.hh"


//Globals
static ParameterSet pSetGlobal;

//The main function (called in MineSweeper.cpp)
void runMineSweeperSetup( ParameterSet & pSet, Geometry & geo, std::vector<Picture> & pictureVect )
{
  //Sanity check
  std::cout << "MineSweeper is being set up..." << std::endl;  
  
  //First things first: parse the config file and fill the parameter set object
  ParseConfigFile( pSet );
  pSetGlobal = pSet;

  //Initialize parameter set in the RunSweep file?
  InitializePSetForSweep( pSet );

  //Check to see if we're in production mode. If so, continue with the rest of the setup. If not, quit and go
  //to the analysis.
  if( pSetGlobal.GetRunMode() != "PRODUCTION" ){
    return;
  }
  

  
  //Next: read the fileList and push back the pictureVect with some limited picture info
  ReadPicFileList( pictureVect );

  //Select out those pictures that are too close to the ring - they will be mis-reconstructed because
  //of the presence of the ring. If the grid was asymmetrically photographed due to space constraints
  //due to the UV lamp, account for this.
  if( pSetGlobal.GetGridSymmetryStatus() == true ){
    FilterOutRingPics( pictureVect );
  }
  else{
    //For large grids, we won't have symmetry. In this case and this case only, we may also want to
    //eliminate reflection pics. 

    std::cout << "Grid scan finish status: " << pSetGlobal.GetGridScanFinishStatus() << std::endl;

    //This case is for successfully finished grids
    if( pSetGlobal.GetGridScanFinishStatus() == true ){
      std::pair<double,double> gridCenter = FilterOutRingPicsAsymmetric( pictureVect, geo );
      if( pSetGlobal.GetSupportRingReflectionStatus() == true && gridCenter.first != -999999. ){
	FilterOutReflectionPics( pictureVect, gridCenter );
      }
    }
    //This case is for aborted (or otherwise unsuccessful scans)
    else{
      std::pair<double,double> gridCenter = FilterOutRingPicsAsymmetricUnfinishedScan( pictureVect, geo );
      if( pSetGlobal.GetSupportRingReflectionStatus() == true && gridCenter.first != -999999. ){
	FilterOutReflectionPics( pictureVect,gridCenter );
      }
    }
  }    

      


  

  //print out basic geometry stuff. This should happen every time.
  std::cout << "\n----> Geometry Information <----\n";
  std::cout << "Grid Diameter: " << geo.GetGridDiameter() << std::endl;
  std::cout << "Grid Center X: " << geo.GetCenterXY().first << ", Y: " << geo.GetCenterXY().second << std::endl;
  std::cout << "Grid Clocking Phi: " << geo.GetClockPhi() << " deg from +X axis." << std::endl;
  std::cout << "--------------------------------\n";
  
  


}



//The nitty gritty of the config file parsing. This reads in the values of the config file
//and passes them to the pSet object
void ParseConfigFile( ParameterSet & pSet )
{
  //Create the infile and open it
  std::ifstream infile;
  infile.open("Config/MineSweeper.config");
  
  //Variables
  std::string parName;
  std::string parVal;

  //Now read through the infile
  while(1){
    if( !infile.good() ) break;
    infile >> parName >> parVal;

    //Now try to understand what the variable is
    if( parName == "PIXELSPERMM" ){
      pSet.SetPixelsPerMm( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's PixelsPerMm to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "VERBOSITY" ){
      pSet.SetVerbosity( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's Verbosity to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "GRIDNAME" ){
      pSet.SetGridName( parVal );
      std::cout << "Initialized ParameterSet's GridName to " << parVal << std::endl;
    }
    //Also do pitch
    //Also do time/date
    //Also do wire diameter
    else if( parName == "RWEIGHT" ){
      pSet.SetRGBWeights( 0, atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's RWeight to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "GWEIGHT" ){
      pSet.SetRGBWeights( 1, atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's GWeight to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "BWEIGHT" ){
      pSet.SetRGBWeights( 2, atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's BWeight to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "FFTNEGTHRESH" ){
      pSet.SetFftNegThresh( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's FftNegThresh to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "FFTPOSTHRESH" ){
      pSet.SetFftPosThresh( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's FftPosThresh to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "SHFBRIGHTTHRESH" ){
      pSet.SetSimpleHitFinderBrightThresh(atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's SHF Brightness Thresh to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "SHFFIDUCIALFROMWALL" ){
      pSet.SetSimpleHitFinderFiducialFromWall(atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's SHF Fiducial Distance from Wall to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "DBMINPTS" ){
      pSet.SetDBClusterMinPts( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's DB Cluster Min Points to " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "DBEPSILON" ){
      pSet.SetDBClusterEpsilon( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's DB Cluster Epsilon to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "DUPLICATECLUSTDIST" ){
      pSet.SetDuplicateClustDistance( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's Duplicate Cluster Distance to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "DEBUGPLOTSFILENAME" ){
      pSet.SetDebugPlotsPicName( parVal );
      std::cout << "Initialized ParameterSet's Debug Plots Filename to " << parVal << std::endl;
    }
    else if( parName == "HOUGHPOINTCUTOFF" ){
      pSet.SetHoughPointCutoff( atof(parVal.c_str()));
      std::cout << "Initialized ParameterSet's Hough Point Cutoff to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "BASELINEDIFFERENCE" ){
      pSet.SetBaselineDifference( atof(parVal.c_str()));
      std::cout << "Initialized ParameterSet's Baseline Difference to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "ISPICINVERTEDXY" ){
      pSet.SetIsPicInvertedXY( atoi( parVal.c_str() ) );
      std::cout << "Initialized ParameterSet's IsPicInverted to " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "RUNMODE" ){
      pSet.SetRunMode( parVal );
      std::cout << "Initialized ParameterSet's RunMode to " << parVal << std::endl;
    }
    else if( parName == "GRIDSYMMETRYSTATUS" ){
      pSet.SetGridSymmetryStatus( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's GridSymmetryStatus to " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "SUPPORTRINGREFLECTIONSTATUS" ){
      pSet.SetSupportRingReflectionStatus( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's SupportRingReflectionStatus to " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "DUPLICATECLUSTFIDUCIALFRACTION" ){
      pSet.SetDuplicateClustFiducialFraction( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's DuplicateClustFiducialFraction to " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "CLOCKPHIDEG" ){
      pSet.SetClockPhi( atof(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's ClockPhiDeg to: " << atof(parVal.c_str()) << std::endl;
    }
    else if( parName == "GRIDSCANFINISHSTATUS" ){
      pSet.SetGridScanFinishStatus( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's GridScanFinishStatus to: " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "MAXCLUSTERSPERPRODFILE" ){
      pSet.SetMaxClustersPerProdFile( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's MaxClustersPerProdFile to: " << atoi(parVal.c_str()) << std::endl;
    }
    else if( parName == "WRITEMODE" ){
      pSet.SetWriteMode( parVal );
      std::cout << "Initialized ParameterSet's WriteMode to: " << parVal.c_str() << std::endl;
    }
    else if( parName == "READNUMBER" ){
      pSet.SetReadNumber( atoi(parVal.c_str()) );
      std::cout << "Initialized ParameterSet's ReadNumber to: " << atoi(parVal.c_str()) << std::endl;
    }
    else{ std::cout << "Warning: Parameter name not understood." << std::endl; }
    

   

  }

  infile.close();

}

//Read the file list containing the picture/image files, and then store some information
//about the pictures (like their filenames and the X/Y positions to which they correspond)
//in the picture vector
void ReadPicFileList( std::vector<Picture> & picVect )
{
  //For sanity's sake
  std::cout << "Reading Picture File List." << std::endl;
  //This name is hardcoded. Please remember this.
  std::ifstream infile;
  infile.open("Config/PicFileList.config");
  
  //Now loop through and find the files

  while(1){
    if( !infile.good() )break;

    std::string picFileName;
    infile >> picFileName;

    //Reading the file names out to cout
    if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
      std::cout << "Pic Filename: " << picFileName << std::endl;
    }

    //Parse the filename for the positions in X and Y of the crosshairs.
    std::pair<double,double> crosshairXY = FindCrosshairXY( picFileName );
    
    //Store the filename in a new picture object
    Picture newPicture;
    newPicture.SetPicName(picFileName);
    newPicture.SetCenterXY(crosshairXY.first,crosshairXY.second);
    
    //Push the picture back into the vector
    picVect.push_back(newPicture);
    
  } 
    
}



//The X and Y of the crosshairs should be stamped into the name of the picture in a 
//format like "STUFF_(X pos)_(Y pos)_. We're extracting X pos and Y pos
std::pair<double,double> FindCrosshairXY( std::string pfn )
{
  //Loop through the filename until you get to a "_"
  std::string X;
  std::string Y;
  bool readBool = false;
  size_t counter = 0;
  size_t iChar = 0;
  size_t lastSlashIndex = 0;

  //First off, look for the last "/" in the filename and start there.
  for( int iC = 0; iC < pfn.size(); ++iC ){
    if( pfn[iC] == '/' ) lastSlashIndex = iC;
  }
    
  //Now, starting at this position, look through the name for X, Y  
  iChar = lastSlashIndex+1;
  while(1){
    if( iChar == pfn.size() ) break;
    
    if( pfn[iChar] == '_' && readBool == false ){
      readBool = true;
      counter++;
      iChar++;
      continue;
    }

    if( readBool == true ){
      if( counter == 2 ){
	if( pfn[iChar] == '_' ){
	  counter++;
	  iChar++;
	  break;
	}
	
	Y.push_back(pfn[iChar]);
      }
      else if( counter == 1 ){
	if( pfn[iChar] == '_' ){
	  counter++;
	  iChar++;
	  continue;
	}
	
	X.push_back(pfn[iChar]);
      }
    }
    iChar++;
  }      
	
  //For double checking the X config
  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
    std::cout << "X: " << atof(X.c_str()) << ", Y: " << atof(Y.c_str()) << std::endl;
  }

  //Return this X and Y pair
  std::pair<double,double> output(atof(X.c_str()),atof(Y.c_str()));
  return output;
}


//Filter out the pictures that have the ring in them so that we don't mis-reconstruct things onto the ring.
void FilterOutRingPics( std::vector<Picture> & pictureVect )
{
  //Sanity check
  std::cout << "........ Centerfinding and ring pic filtering done assuming finished and symmetric scan ........" << std::endl;

  //Loop over the pictures and average to find the center. 
  double xAvg = 0;
  double yAvg = 0;
  double nPics = 0;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    
    //Find the pic center 
    double x = pictureVect[iPic].GetCenterXY().first;
    double y = pictureVect[iPic].GetCenterXY().second;
    
    xAvg+= x;
    yAvg+= y;

    nPics++;
  }
  xAvg /= nPics;
  yAvg /= nPics;

  //Loop back through the pics to find the one whose center is farthest from the center (average). This should
  //be close enough to the ring that we will consider it the ring radius for all practical purposes.
  double largestR = 0;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    double x = pictureVect[iPic].GetCenterXY().first;
    double y = pictureVect[iPic].GetCenterXY().second;

    if( pow(pow(xAvg - x,2)+pow(yAvg - y,2),0.5) > largestR ){
      largestR = pow(pow(xAvg - x,2)+pow(yAvg - y,2),0.5);
    }
  }

  //For sanity check
  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
    std::cout << "Largest R for this grid (in mm) is: " << largestR << std::endl;
  }

  //Finally, loop through the pics one more time to check for their radius.
  int dim1 = 0;
  int dim2 = 0;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    
    //Get the dimensions of the first one (and assume they all have the same dims)
    if( iPic == 0 ){
      std::vector<std::vector<int> > bwPic = ReadBitMapPic( pictureVect[iPic].GetPicName() );
      dim1 = bwPic.size();
      if( dim1 == 0 ){
	std::cout << "Picture dimension 1 = 0. Returning without further progress on filtering out ring pics." << std::endl;
	return;
      }
      dim2 = bwPic[0].size();
    }

    //Now check to see if this picture is too close to the wall. The metric is that the center-to-maxR distance has to be
    //greater than the half-diagonal distance of the picture.
    double ppmm = pSetGlobal.GetPixelsPerMm();
    double halfDiag = pow((double)(dim1*dim1 + dim2*dim2),0.5)/2/ppmm;
    double x = pictureVect[iPic].GetCenterXY().first;
    double y = pictureVect[iPic].GetCenterXY().second;
    double r = pow(pow(xAvg-x,2)+pow(yAvg-y,2),0.5);
    if( largestR - r < halfDiag ){
      if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
	std::cout << "Erasing ring picture (version 1): " << pictureVect[iPic].GetPicName() << std::endl;
      }
      pictureVect.erase(pictureVect.begin()+iPic);
      iPic--;
    }
  }

}


//Self explanatory
double findMedianOfSortedList( std::vector<double> list )
{
  int size = list.size();
  if( size % 2 == 0 ){
    double first = list[((double)size)/2.0-1.0];
    double second = list[((double)size)/2.0];
    return (first+second)/2.0;
  }
  else{
    return list[((double)size-1.0)/2.0];
  }
}



//This uses a more basic method for finding the circle center. It just finds the two most extreme pictures in the -X and +Y directions,
//then takes the center as the Y and X components of those, respectively. This only works for scans that are about 60% done or more,
//because it looks for the most extreme picture in Y but if there are degenerate pictures there, it chooses the middle one.
std::pair<std::pair<double,double>,double> computeGridCenterSimple( std::vector<Picture> & pictureVect )
{
  //First, loop through the pictures to find the extreme values in X and Y
  std::pair<double,double> center(-999999.,-999999);
  double minX = 9999999999;
  double maxY = -9999999999;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    double X = pictureVect[iPic].GetCenterXY().first;
    double Y = pictureVect[iPic].GetCenterXY().second;
    if( X < minX ) minX = X;
    if( Y > maxY ) maxY = Y;
  }
  
  //Next, loop again through the pictures to identify which ones have those extreme X and Y values.
  //Push them back into two vectors and identify which one of them has the middle Y (for X) and X (for Y) value.
  std::vector<Picture> picsWithMinX;
  std::vector<Picture> picsWithMaxY;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    double X = pictureVect[iPic].GetCenterXY().first;
    double Y = pictureVect[iPic].GetCenterXY().second;
    if( X-minX < 0.001 ) picsWithMinX.push_back(pictureVect[iPic]);
    if( Y-maxY < 0.001 ) picsWithMaxY.push_back(pictureVect[iPic]);
    
  }
  
  //Sanity check
  std::cout << "Number of pics with max Y: " << picsWithMaxY.size() << ", pics with min X: " << picsWithMinX.size() << std::endl;

  if( picsWithMinX.size() == 0 || picsWithMaxY.size() == 0 ){
    std::cout << "Pics with MinX or MaxY = 0. Aborting centerfinding." << std::endl;
    std::pair<std::pair<double,double>,double> output(center,99999999999);
    return output;
  }
  
  //Now find the median value of each dimension
  std::vector<double> xOfMaxY;
  std::vector<double> yOfMinX;
  for( int iP = 0; iP < picsWithMinX.size(); ++iP ){
    yOfMinX.push_back(picsWithMinX[iP].GetCenterXY().second);
  }
  for( int iP = 0; iP < picsWithMaxY.size(); ++iP ){
    xOfMaxY.push_back(picsWithMaxY[iP].GetCenterXY().first);
  }
  std::sort(xOfMaxY.begin(),xOfMaxY.end());
  std::sort(yOfMinX.begin(),yOfMinX.end());

  //Extract that value - note that segfaults could potentially pop up here if I've missed something...
  double medianXofMaxY = findMedianOfSortedList(xOfMaxY);
  double medianYofMinX = findMedianOfSortedList(yOfMinX);
  
  //Compute the radius from this value and the value of maxY
  double radius = fabs(medianYofMinX-maxY);


  //Build the center
  center.first = medianXofMaxY;
  center.second = medianYofMinX;
  std::pair<std::pair<double,double>,double> output(center,radius);
  return output;
}





//Use this for grid scans that have finished ~60% or more
//Later in this, I use another method to compute ring center for an unfinished scan. I can use these two to do a sanity check to make
//sure I'm doing things right.
std::pair<double,double> FilterOutRingPicsAsymmetricUnfinishedScan( std::vector<Picture> & pictureVect, Geometry & geo )
{
  std::cout << "........ Centerfinding and ring pic filtering done assuming unfinished scan ........" << std::endl;

  //Output
  std::pair<std::pair<double,double>,double> gridCenterOutput_Center_R = computeGridCenterSimple( pictureVect );
  std::pair<double,double> gridCenterOutput = gridCenterOutput_Center_R.first;
  double radius = gridCenterOutput_Center_R.second;
  double gridDiameter = radius * 2;

  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Grid Center X: " << gridCenterOutput.first << ", Grid Center Y: " << gridCenterOutput.second << ", Grid Radius: " << radius << std::endl;
  }

  //Set geometry information all in one place. This is kind of a weird place to do it, but whatever.
  geo.SetCenterXY(gridCenterOutput.first,gridCenterOutput.second);
  geo.SetGridDiameter(gridDiameter);
  geo.SetClockPhi(pSetGlobal.GetClockPhi());

  //Get info about the picture format
  std::vector<std::vector<int> > bwPic = ReadBitMapPic( pictureVect[0].GetPicName() );
  double dim1 = bwPic.size();
  if( dim1 == 0 ){
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPicture dimension 1 = 0. Returning bad center and without further progress on filtering out ring pics.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
    return gridCenterOutput;
  }
  double dim2 = bwPic[0].size();
  double ppmm = pSetGlobal.GetPixelsPerMm();
  double halfDiag = pow((double)(dim1*dim1 + dim2*dim2),0.5)/2/ppmm;


  //Now see if the picture is outside of (R-epsilon) from the center - kill these as they are ring pics. Here,
  //epsilon is the same as the prior algorithm.
  double A = gridCenterOutput.first;
  double B = gridCenterOutput.second;
  double R = radius;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    double x = pictureVect[iPic].GetCenterXY().first - A;
    double y = pictureVect[iPic].GetCenterXY().second - B;
    std::cout << "thisX/Y: " << x+A << "/" << y+B << ", thisR: " << (pow(x*x+y*y,0.5)) << ", bigR: " << R << ", halfDiag: " << halfDiag << std::endl;
    if( fabs(pow(x*x+y*y,0.5) - R) < halfDiag ){
      if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
	std::cout << "Erasing ring picture (version 2): " << pictureVect[iPic].GetPicName() << std::endl;
      }
      pictureVect.erase(pictureVect.begin()+iPic);
      iPic--;
      //std::cout << "PictureVect.size(): " << pictureVect.size() << ", iPic: " << iPic << std::endl;
    }
  }


  return gridCenterOutput;

}


//This gets a picture's 



//This uses Steffen's circle-computing algorithm. It basically finds the pictures that are at the extremities in 3 directions (+X, -X, and +Y), and uses them as points on the circle. This should get us a good enough center. Note that this should be used only for a finished scan.
std::pair<double,double> FilterOutRingPicsAsymmetric( std::vector<Picture> & pictureVect, Geometry & geo )
{

  std::cout << "........ Centerfinding and ring pic filtering done assuming finished but asymmetric scan ........" << std::endl;

  //Output
  std::pair<double,double> gridCenterOutput(-999999.,-9999999.);
  
  //First, loop through the pics to find the extreme ones
  double maxX = -99999999;
  int maxX_index = -1;
  double minX = 99999999;
  int minX_index = -1;
  double maxY = -99999999;
  int maxY_index = -1;
  int dim1 = 0;
  int dim2 = 0;
  double ppmm = 0;
  double halfDiag = 0;
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){

    //get picture dimensions so we can eliminate those with ring images in them
    if( iPic == 0 ){
      std::vector<std::vector<int> > bwPic = ReadBitMapPic( pictureVect[iPic].GetPicName() );
      dim1 = bwPic.size();
      if( dim1 == 0 ){
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPicture dimension 1 = 0. Returning bad center and without further progress on filtering out ring pics.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	return gridCenterOutput;
      }
      dim2 = bwPic[0].size();
    }
    ppmm = pSetGlobal.GetPixelsPerMm();
    halfDiag = pow((double)(dim1*dim1 + dim2*dim2),0.5)/2/ppmm;
    


    double x = pictureVect[iPic].GetCenterXY().first;
    double y = pictureVect[iPic].GetCenterXY().second;

    
    if( x > maxX ){
      maxX = x;
      maxX_index = iPic;
    }
    if( x < minX ){
      minX = x;
      minX_index = iPic;
    }
    if( y > maxY ){
      maxY = y;
      maxY_index = iPic;
    }
  }

  //Record the extreme points in XY space
  double point1[2] = {pictureVect[maxX_index].GetCenterXY().first,pictureVect[maxX_index].GetCenterXY().second};
  double point2[2] = {pictureVect[minX_index].GetCenterXY().first,pictureVect[minX_index].GetCenterXY().second};
  double point3[2] = {pictureVect[maxY_index].GetCenterXY().first,pictureVect[maxY_index].GetCenterXY().second};
  
  //Do a little bit of convenience relabeling
  double x1 = point1[0];
  double y1 = point1[1];
  double x2 = point2[0];
  double y2 = point2[1];
  double x3 = point3[0];
  double y3 = point3[1];
  double g1 = -1*(x1*x1+y1*y1);
  double g2 = -1*(x2*x2+y2*y2);
  double g3 = -1*(x3*x3+y3*y3);

  //Result of some linear algebra
  double Bprime = ((x3-x1)*(g2-g1)-(x2-x1)*(g3-g1))/((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1));
  double Aprime = (g3-g1-Bprime*(y3-y1))/(x3-x1);
  double Cprime = g1-y1*Bprime-x1*Aprime;
  double A = -1*Aprime/2.0;
  double B = -1*Bprime/2.0;
  double C = -1*Cprime;
  double R = pow(C+A*A+B*B,0.5);

  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Grid Center X: " << A << ", Grid Center Y: " << B << ", Grid Radius: " << R << std::endl;
  }

  //Setting grid center
  gridCenterOutput.first = A;
  gridCenterOutput.second = B;
  
  //Finding the diameter (for use in geometry)
  double gridDiameter = 0;
  double dX_gd = gridCenterOutput.first-point1[0];
  double dY_gd = gridCenterOutput.second-point1[1];
  double radius = pow(dX_gd*dX_gd+dY_gd*dY_gd,0.5);
  gridDiameter = radius*2;





  //Set geometry information all in one place. This is kind of a weird place to do it, but whatever.
  geo.SetCenterXY(gridCenterOutput.first,gridCenterOutput.second);
  geo.SetGridDiameter(gridDiameter);
  geo.SetClockPhi(pSetGlobal.GetClockPhi());
  
  


  //Now see if the picture is outside of (R-epsilon) from the center - kill these as they are ring pics. Here,
  //epsilon is the same as the prior algorithm.
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){
    double x = pictureVect[iPic].GetCenterXY().first - A;
    double y = pictureVect[iPic].GetCenterXY().second - B;
    

    std::cout << "thisX/Y: " << x+A << "/" << y+B << ", thisR: " << (pow(x*x+y*y,0.5)) << ", bigR: " << R << ", halfDiag: " << halfDiag << std::endl;
    if( fabs(pow(x*x+y*y,0.5) - R) < halfDiag ){
      if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
	std::cout << "Erasing ring picture (version 2): " << pictureVect[iPic].GetPicName() << std::endl;
      }
      pictureVect.erase(pictureVect.begin()+iPic);
      iPic--;
      std::cout << "PictureVect.size(): " << pictureVect.size() << ", iPic: " << iPic << std::endl;
    }
  }


  return gridCenterOutput;
}




//Filtr out the pictures that are too bright due to direct reflections of the UV lamp off of the support plate
void FilterOutReflectionPics( std::vector<Picture> & pictureVect, std::pair<double,double> gridCenter )
{
  //Printing for log
  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Filtering out reflection pics." << std::endl;
  }

  //Loop through the pictures. For each, find an average pixel value
  for( int iPic = 0; iPic < pictureVect.size(); ++iPic ){

    //Determine if the picture is outside of some R. This will help speed this process up.
    double x = pictureVect[iPic].GetCenterXY().first;
    double y = pictureVect[iPic].GetCenterXY().second;
    if( pow(pow(x-gridCenter.first,2)+pow(y-gridCenter.second,2),0.5) < 500. ) continue; //CAREFUL. THIS IS HARDCODED

    std::vector<std::vector<int> > bwPic = ReadBitMapPic( pictureVect[iPic].GetPicName() );
    
    //Loop through some of the picture's pixels in each dimension and compute an average. I don't want
    //to take all of the pixels because it'll take too long, but I want a uniform sampling, so I'll sample
    //every fifth pixel in each dimension.
    double avg = 0;
    for( int iPix1 = 0; iPix1 < bwPic.size(); iPix1+=1 ){
      for( int iPix2 = 0; iPix2 < bwPic[0].size(); iPix2+=1 ){
	avg+= (double)bwPic[iPix1][iPix2];
      }
    }
    avg/= ((double)bwPic.size()*(double)bwPic[0].size());

    //Printing for progress
    if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
      std::cout << "Reflection pic filter test: " << pictureVect[iPic].GetPicName() << ". Average pixel val is: " << avg << std::endl;
    }

    
    //Make the cut. I think this is probably a reasonable cut and shouldn't need tuning, but
    //I guess we'll see about that.
    if( avg > 3 ){
      if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5){
	std::cout << "Erasing reflection picture: " << pictureVect[iPic].GetPicName() << std::endl;
      }
      pictureVect.erase(pictureVect.begin()+iPic);
      iPic--;
    }
  }
}
