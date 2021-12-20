///////////////////////////////////////////////////////////////
//
// RunSweep.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/31/2018
//
// This file contains the functions that do the sweep for 
// debris on the wires in the pictures input into MineSweeper.
// The stuff here are the workhorse functions for doing this
// particle identification.
// 
// Do not tamper with this file! 
//
///////////////////////////////////////////////////////////////

//Standard includes
#include <iostream>
#include <stdio.h>

//MineSweeper includes
#include "Particle.hh"
#include "Cluster.hh"
#include "ParameterSet.hh"
#include "Picture.hh"
#include "RunSweep.hh"
#include "DBCluster.hh"

//ROOT Includes
#include "TFile.h"
#include "TH2F.h"
#include "TMath.h"

//Additional library includes
#include <fftw3.h>

//Globals (for this file)
static ParameterSet pSetGlobal;
double baseline;
int PicWidthX = 512; //Really the half-width of the photo
int PicWidthY = 384; //Really the half-width of the photo

//Have to do this before any pic reading that happens in setup because I was stupid and put the 
//read bitmap pic function in this source file and not the setup one... guhhhh
void InitializePSetForSweep( const ParameterSet & pSet )
{
  //Set the global pSet for convenience
  pSetGlobal = pSet;
}


//This is the overarching function that is called from MineSweeper. It finds particles as well as generic clusters in the pictures. The particles have been classified as wire blemishes of sorts. The clusters are broader, and include particles, but may also include crosses and reflections that weren't elminated by the FFT recognition algorithm.
void RunParticleFinding( const ParameterSet & pSet, const std::vector<Picture> & picVect, std::vector<Particle> & particleVect, std::vector<Cluster> & clusterVect )
{
  //Pseudocode: highest level
  //Loop over pictures
  //- For each picture, run a particle finder on it to populate that picture with particles
  //- Then populate the particle vect with the particles in each pic

  //Set the global pSet for convenience
  //  pSetGlobal = pSet;

  //Loop over pictures
  for( size_t iPic = 0; iPic < picVect.size(); ++iPic ){
    Picture thePic = picVect[iPic];
    
    //Run the particle finder. This takes the picture and adds to it the particles and clusters found.
    //Note that this doesn't actually write to file - this is only a copy of the real picture written
    //to file. Also, we don't want to duplicate written information.
    FindParticles( thePic );


    //Now add the particles picked up by the picture to the vector of particles
    for( size_t iPart = 0; iPart < thePic.GetParticles().size(); iPart++ ){
      particleVect.push_back(thePic.GetParticles()[iPart]);
    }


    
    //Now add the clusters picked up by the picture to the vector of clusters. However, we want to put a
    //constraint on this. If the cluster is within some distance of another cluster from another picture,
    //we don't want to duplicate. (Later, we may want to try to better resolve things, but for now, we're
    //doing the simplest thing and just deleting duplicate clusters.
    if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
      std::cout << "Number of clusters in this pic: " << thePic.GetClusters().size() << std::endl;
    }
    
    //Push back all clusters for now, even if they are duplicates.
    for( size_t iC = 0; iC < thePic.GetClusters().size(); iC++ ){
      clusterVect.push_back(thePic.GetClusters()[iC]);
    }

  }

  


  
  /* This is problematic for the anode due to finite computing resources. Given that the overlap between pics is small and
     that duplicates just increase background and don't lose anything, I'm just going to kill these steps for now.
     - Feb 19, 2019
  */

  

  //Reorder cluster vect so that we start with all non-fiducial clusters and all other clusters appear after
  ReorderClusterVect( clusterVect );
  

  //Loop back through all of the clusters and look for duplicates. If there is one, remove it from the list.
  for( size_t iC = 0; iC < clusterVect.size(); ++iC ){
    Cluster clustToCheck = clusterVect[iC];

    //The first thing I want to do is see if this particle is anywhere near the edge of its picture. I've set up the 
    //camera motion so that there is a small overlap between pictures. This means that the central clusters should not
    //even be considered, because there will be no duplicates. First, let's check this.
    bool cif = ClusterIsFiducial(clustToCheck);
    if( cif ) break;

    
    

    //Check to see if this is a throwaway one.
    if( CheckForDuplicates(clusterVect,clustToCheck) ){
      clusterVect.erase(clusterVect.begin()+iC);
      std::cout << "This cluster is a duplicate. Position: " << clusterVect[iC].GetGlobalCenterXY().first << ", " << clusterVect[iC].GetGlobalCenterXY().second << std::endl;
      iC--;

      if( cif ){
	std::cout << "This cluster is also fiducial, for a fidFraction of " << pSetGlobal.GetDuplicateClustFiducialFraction() << std::endl;
      }
      
    }
    
  }
  //Done with anode debug

      
  /*
    for( size_t iC = 0; iC < thePic.GetClusters().size(); iC++ ){
      if( !CheckForDuplicates(clusterVect,thePic.GetClusters()[iC]) ){
	clusterVect.push_back(thePic.GetClusters()[iC]);
      }
      else{
	if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
	  std::cout << "This cluster is a duplicate. Position: " << clusterVect[iC].GetGlobalCenterXY().first << ", " << clusterVect[iC].GetGlobalCenterXY().second << std::endl;
	}
      }
    }
    }
  */
}


//This finds particles for one picture
void FindParticles( Picture & thePic )
{

  //Pseudocode
  //Extract the bitmap info from the picture and put it into a vector of vectors (should be grayscale)
  //Pass the vector of vectors (grayscale) into a point-of-interest finder algorithm to find areas of interest
  //To double check, re-print out photos with the found points-of-interest marked in red on the bitmaps

  //Get the name of the picture and use it to read in the bitmap file
  std::string picName = thePic.GetPicName();

  //For keeping track of progress
  if( pSetGlobal.GetVerbosity() == 1 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Finding particles in picture: " << picName << std::endl;
  }
  

  
  //Extract the file info and put it into a vector of vectors. Also, read out the colors of the bitmap pic for use later
  std::vector<std::vector<int> > bwPic = ReadBitMapPic( picName );

  //Find the baseline (median background value) of this picture for use in hough
  FindBaseline( bwPic );

  //Go through and find the pixels that differ from the baseline by more than some value. Save them in a vector. This will be used for
  //the hough transform rather than the whole picture due to speed and other reasons
  std::vector<std::pair<double,std::pair<int,int> > > diffPixVector = FilterOutBackgroundPix( bwPic );
  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Size of baseline-reduced picture: " << diffPixVector.size() << std::endl;
  }

  //Run hough line finder. This will return a vector of pairs of rho, theta that define the lines in the image.
  //  std::vector<std::pair<double,double> > lines = FindHoughLines( diffPixVector, picName );
  
  //Point of interest finder structure:
  // - Take FT of the image
  // - Find high points of FFT and remove them to eliminate periodicity
  // - Take IFT of the modified FT
  // - Find hits
  // - Cluster hits
  // - Parametrize clusters and make them particles.
  //We'll be using the FFTW libraries for this. Credit: www.fftw.org. They like to take in C-like arrays of data
  //for the 2-d Fourier Transforms, so we're going to convert our vector to this.

  //Finding dimensions of the picture
  size_t dim1 = bwPic.size();
  if( dim1 == 0 ){
    std::cout << "Picture dimension 1 = 0. Returning without further progress on sweep." << std::endl;
    return;
  }
  size_t dim2 = bwPic[0].size();
  
  //Convert data from vector of vectors to normal c-like array
  double ** bwPic_arr;
  bwPic_arr = new double*[dim1];
  for( unsigned i = 0; i < dim1; ++i ){
    bwPic_arr[i] = new double[dim2];
    for( unsigned j = 0; j < dim2; ++j ){
      bwPic_arr[i][j] = bwPic[i][j];
    }
  }

  //Create a zero'd normal (non-row-major ordered) vector for output 
  double ** fft_output = new double*[dim1];
  double ** ifft_output = new double*[dim1];
  for( unsigned i = 0; i < dim1; i++ ){
    fft_output[i] = new double[dim2];
    ifft_output[i] = new double[dim2];
    for( unsigned j = 0; j < dim2; j++ ){
      fft_output[i][j] = 0;
      ifft_output[i][j] = 0;
    }
  }      



  //Run the FFT on the new 2d array
  RunTheFFTW(bwPic_arr,fft_output,dim1,dim2,FFTW_R2HC);
   
  //Now do the point-of-interest finding for the pieces of dust on the wire
  FourierSpaceCuts(fft_output,dim1,dim2);
    
  //Run the iFFT
  RunTheFFTW(fft_output,ifft_output,dim1,dim2,FFTW_HC2R);
  


  //Find "hits" by some metric. 

  //The first, simplest algorithm will just look for pixels that are both above a certain threshold and
  //inside a certain "fiducial area" - this fiducial area is necessary because of edge effects from the FFT
  //that cause the wire edges to fake feature hotspots. Each "hit Pixel" is just a pixel that is over threshold.
  //The algorithm stores a vector of these pixels, each with the brightness value and the X and Y values of
  //the pixel.
  //
  //This also takes the rgb bitmap image and logs the rgb values of the pixels that are identified as hit pixels.
  std::vector<std::pair<double,std::pair<int,int> > > hitPixels = simpleHitFinder(ifft_output,dim1,dim2);

  //Now we cluster the hits into reasonable sizes. Make sure to put these clusters into the picture as well.
  std::vector<Cluster> clusters = ClusterTheHits(hitPixels,pSetGlobal);

  //Compute the cluster positions (globally)
  ComputeClusterParams(clusters,pSetGlobal,thePic, dim1, dim2, bwPic_arr);

  //Sanity check
  //  std::cout << "Size of first cluster's rgb hit pix vect: " << clusters[0].GetHitPixelsRGB().size() << std::endl;
  //std::cout << "Size of first cluster's hit pix vect: " << clusters[0].GetHitPixels().size() << std::endl;



  //Compare clusters to hough lines and eliminate those that are close enough to the intersection of two hough lines
  //  CompareClustersToHoughLines(clusters,lines);


  //Add clusters to the picture
  for( int iClust = 0; iClust < clusters.size(); ++iClust ){

    //For the anode, I think we're running into i/o problems with root because there are so many clusters.
    //We'll temporarily try to cut away smaller clusters  //Anode debug
    //    if( clusters[iClust].GetArea() < 8 ) continue;

    thePic.AddCluster(clusters[iClust]);
  }

  
  
  


  //One thing we can do to get rid of the clusters on the crosses is to run a hough transform of the original picture before the FFT
  //is done and find the hough lines. If the cluster lies close enough to two hough lines and has a certain (calibrated) size, then
  //we don't consider it. Food for later thought.

  //One thing we can do to get rid of clusters from reflections is to define some "duplicate" metric and see if there are any duplicated
  //clusters in an image.


  //Run a test: plot original image, then fft, then ifft
  //if( picName == "/Users/ryanlinehan/Downloads/MicroscopeTestRun_958PM_4_2_2018/GridPoint_923.715_-488.55625_.bmp" ){
  //  if( picName == "/Users/ryanlinehan/Downloads/MicroscopeTestRun_958PM_4_2_2018/GridPoint_923.715_-503.43625_.bmp" ){
  //  if( picName == "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/TG25_SuccessRunSquare/ZZZGridPoint_897.27625_-541.56625_.bmp" ){
  //  if( picName == "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/TG25_SuccessRunSquare/ZZZGridPoint_897.27625_-581.24625_.bmp" ){
  //  if( picName == "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/TG25_SuccessRunSquare/ZZZGridPoint_907.19625_-541.56625_.bmp" ){
  if( picName == pSetGlobal.GetDebugPlotsPicName() ){

    //Original image
    TFile * f1 = new TFile("TestOutput.root","RECREATE");
    TH2F * h1 = new TH2F("map","map",dim2,0,dim2,dim1,0,dim1);
    for( size_t iY = 0; iY < bwPic.size(); ++iY ){
      for( size_t iX = 0; iX < bwPic[iY].size(); ++iX ){
	h1->Fill(iX,iY,bwPic[iY][iX]);
      }
    }
    h1->Write();
    
    //FFT
    TH2F * h1_fft = new TH2F("mapfft","mapfft",dim2,0,dim2,dim1,0,dim1);
    for( size_t iY = 0; iY < bwPic.size(); ++iY ){
      for( size_t iX = 0; iX < bwPic[iY].size(); ++iX ){
	//	std::cout << "iX: " << iX << ", iY: " << iY << ", value: " << fft_output[iY][iX] << std::endl;
	h1_fft->Fill(iX,iY,fft_output[iY][iX]);
      }
    }
    h1_fft->Write();

    //IFFT
    TH2F * h1_ifft = new TH2F("mapifft","mapifft",dim2,0,dim2,dim1,0,dim1);
    for( size_t iY = 0; iY < bwPic.size(); ++iY ){
      for( size_t iX = 0; iX < bwPic[iY].size(); ++iX ){
	//	std::cout << "iX: " << iX << ", iY: " << iY << ", value: " << ifft_output[iY][iX] << std::endl;
	//	if( (ifft_output[iY][iX]) > pSetGlobal.GetSimpleHitFinderBrightThresh()){
	  h1_ifft->Fill(iX,iY,ifft_output[iY][iX]);
	  //	}
      }
    }
    h1_ifft->Write();

    //Simple hits found
    TH2F * h1_SHF = new TH2F("mapSHF","mapSHF",dim2,0,dim2,dim1,0,dim1);
    for( int iPix = 0; iPix < hitPixels.size(); ++iPix ){
      int X = hitPixels[iPix].second.first;
      int Y = hitPixels[iPix].second.second;
      h1_SHF->Fill(X,Y);
    }
    h1_SHF->Write();

    

    //Clusters made
    TH2F * h1_Clusters = new TH2F("mapClusters","mapClusters",dim2,0,dim2,dim1,0,dim1);
    for( int iClust = 0; iClust < clusters.size(); ++iClust ){
      
      //Loop over hits and fill the histogram with the same weight
      size_t weight = clusters[iClust].GetID();
      std::vector<std::pair<double,std::pair<int,int> > > hitPixels = clusters[iClust].GetHitPixels();
      for( int iPix = 0; iPix < hitPixels.size(); ++iPix ){
	double x = hitPixels[iPix].second.first;
	double y = hitPixels[iPix].second.second;
	h1_Clusters->Fill(x,y,weight+1);
      }
      
    }
    h1_Clusters->Write();

    //OG Hit Pixels (sanity check - should be the same as hit pix)
    TH2F * h1_OGHitPix = new TH2F("mapOGHitPix","mapOGHitPix",dim2,0,dim2,dim1,0,dim1);
    for( int iClust = 0; iClust < clusters.size(); ++iClust ){
      
      //Loop over the og hit pixels and fill the histogram
      std::vector<std::pair<double,std::pair<int,int> > > ogHitPixels = clusters[iClust].GetOGHitPixels();
      for( int iPix = 0; iPix < ogHitPixels.size(); ++iPix ){
	double x = ogHitPixels[iPix].second.first;
	double y = ogHitPixels[iPix].second.second;
	h1_OGHitPix->Fill(x,y);
      }
      
    }
    h1_OGHitPix->Write();



    f1->Close();
  }


  //Free the memory associated with the C-like arrays
  for( int i = 0; i < dim1; ++i ){
    delete[] bwPic_arr[i];
  }
  delete bwPic_arr;
  for( int i = 0; i < dim1; ++i ){
    delete[] fft_output[i];
  }
  delete[] fft_output;
  for( int i = 0; i < dim1; ++i ){
    delete[] ifft_output[i];
  }
  delete[] ifft_output;

}


//This reads in the bitmap. It's modularized in case we want to change the file format later
std::vector<std::vector<int> > ReadBitMapPic( std::string name )
{

  //Create an output vector with the grayscale info
  std::vector<std::vector<int> > outputVect;

  //Debug
  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Reading file: " << name << std::endl;
  }
  
  FILE* file = fopen(name.c_str(),"rb");

  //Read header info
  unsigned char info[66]; 
  fread(info, sizeof(unsigned char), 66, file);

  //Extract image height and width from the header
  int width = *(int*)&info[18];
  int height = *(int*)&info[22];
  short bitsPerPixel = *(short*)&info[28];
  int redMask = *(int*)&info[54];
  int greenMask = *(int*)&info[58];
  int blueMask = *(int*)&info[62];

  //Just freaking read out the full header for pete's sake
  int row_padded = (width*4 + 4) & (~4);
  if( pSetGlobal.GetVerbosity() == 22 || pSetGlobal.GetVerbosity() == 5 ){
    for( int iBit = 2; iBit < 66; iBit+=4 ){
      std::cout << "Bit: " << iBit << " in header: " << *(int*)&info[iBit] << std::endl;
    }
    std::cout << "Height: " << height << ", width: " << width << std::endl;
    std::cout << "Bits per pixel: " << bitsPerPixel << std::endl;
    std::cout << "RowPadded: " << row_padded << std::endl;
  }

  //Loop over horizontal lines
  for( int i = 0; i < height; i++ ){
    unsigned char data[row_padded];    
    fread( data, sizeof(unsigned char), row_padded, file);
    std::vector<int> horzLine;

    
    //By the compression level given, we have every four bytes correspond to a pixel.
    //To find the red, green, and blue contributions, we have to disentangle them
    //using the color masks given (red, green, and blue). This will involve some bitshifting...
    for(int j = 0; j < width*4; j += 4 ){
      
      //Get the int value corresponding to this pixel and do a bitwise AND
      int pixelVal = *(int*)&data[j];
      
      //Now take the bitwise AND of pixelVal and blue to get blue val
      int blueVal = blueMask & pixelVal;
      int blueVal_cor = blueVal;
      
      //Now take the bitwise AND of pixelVal and green to get green val (modulo a shift)
      int greenVal = greenMask & pixelVal;
      int greenVal_cor = greenVal >> 8;
      
      //Now take the bitwise AND of pixelVal and red to get red val (modulo a shift)
      int redVal = redMask & pixelVal;
      int redVal_cor = redVal >> 16;

     
      //Average RGB for particle finding
      double avg = ComputeBWFromRGB( (double)redVal_cor, (double)greenVal_cor, (double)blueVal_cor );
      if( pSetGlobal.GetVerbosity() == 22 || pSetGlobal.GetVerbosity() == 5 ){
	std::cout << "iX: " << j << ", R: " << redVal_cor << ", G: " << greenVal_cor << ", B: " << blueVal_cor << ", avg: " << avg << std::endl;      
      }

      //This actually pushes back into our vector
      horzLine.push_back((int)avg);
      
    }
    //Push our horizontal line back into our image vector
    outputVect.push_back(horzLine);
  }

  fclose(file);

  return outputVect;

}

//This determines how to weight R, G, B to get a grayscale image. It's separated because
//it would be good to parametrize these, and I can see my desire to change this parametrization
//popping up later.
double ComputeBWFromRGB( double R, double G, double B )
{
  double avg = R*pSetGlobal.GetRGBWeights(0) + G*pSetGlobal.GetRGBWeights(0) + B*pSetGlobal.GetRGBWeights(2);
  return avg;
}


//This runs the FFTW algorithm to convert the 2d array into a fftw_complex object that we will play with to remove
//periodicity in the images. Here, inStandard is the set of inputs (example: the picture in a 2-d array) that we
//have to assign to in, the input to the FFTW, AFTER calling the plan procedure on in. outStandard is the output
//in the same form (a normal 2-d array)
void RunTheFFTW( double ** inStandard, double ** outStandard, int dim1, int dim2, fftw_r2r_kind kind )
{
  //Create and allocate the input and output arrays
  double *in;
  double *out;
  int dim = dim1*dim2;
  in = new double[dim];
  out = new double[dim];
  
  //Create the plan  
  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Starting fftw plan creation." << std::endl;
    std::cout << "In/output array has dim: " << dim << std::endl;
  }
  fftw_plan p = fftw_plan_r2r_2d( dim1, dim2, in, out,kind,kind,FFTW_MEASURE);

  
  
  //Now fill the input vector with info (put into row-major order for the FFTW alg's use)
  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Finished plan creation and now filling the input array (row-major ordered)" << std::endl;
    std::cout << "Plan has dim1: " << dim1 << ", dim2: " << dim2 << std::endl;
  }
  for( unsigned i = 0; i < dim1; i++ ){
    int place = i*dim2;
    for( unsigned j = 0; j < dim2; j++ ){
      int fullPlace = place+j;
      in[fullPlace] = (double)inStandard[i][j];
    }
  }


  //Run the fft - this will need to be modified to speed things up
  fftw_execute(p); 
  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Done with fftw_execution." << std::endl;
  }


  
  //Destroy the plan
  fftw_destroy_plan(p);


  //Convert the output back to a normal 2-d array (as opposed to a row-major ordered array)
  //This is done so we can look at the FFT with ROOT
  int xInd = 0;
  int yInd = 0;
  for( int i = 0; i < dim; ++i ){
    
    //Go to next line
    if( i % dim2 == 0 && i != 0 ){
      xInd = 0;
      yInd++;
    }
    
    //Assign the value to the normal array
    outStandard[yInd][xInd] = out[i];
    xInd++;
  }

  delete in;
  delete out;
  

}


//This function takes the fft and makes cuts to eliminate the peaks. This will enable us to remove the
//periodicity inherent in the wires
void FourierSpaceCuts( double ** fft_output, int dim1, int dim2 )
{
  //Loop through the output array and make entries zero if they are extreme enough.
  for( int iEnt = 0; iEnt < dim1; ++iEnt ){
    for( int jEnt = 0; jEnt < dim2; ++jEnt ){
      if( fft_output[iEnt][jEnt] > pSetGlobal.GetFftPosThresh() || fft_output[iEnt][jEnt] < pSetGlobal.GetFftNegThresh() ){
	fft_output[iEnt][jEnt] = 0;
      }
    }
  }
}


//This function takes the ifft output and finds pixels over threshold. It then denotes them as hits, and saves them
//in a vector.
std::vector<std::pair<double,std::pair<int,int> > > simpleHitFinder( double ** ifft_output, int dim1, int dim2 )
{
  //output vector
  std::vector<std::pair<double,std::pair<int,int> > > output;
  
  
  //Loop over the ifft output image
  for( int iEnt = 0; iEnt < dim1; iEnt++ ){
    for( int jEnt = 0; jEnt < dim2; jEnt++ ){
      bool isInFiducialVol = true;
      int distToEdge = pSetGlobal.GetSimpleHitFinderFiducialFromWall();
      if( iEnt > dim1-distToEdge || iEnt < distToEdge ||
	  jEnt > dim2-distToEdge || jEnt < distToEdge ) isInFiducialVol = false;
      if( (ifft_output[iEnt][jEnt]) > pSetGlobal.GetSimpleHitFinderBrightThresh() &&  isInFiducialVol == true ){
	std::pair<int,int> xy(jEnt,iEnt);
	std::pair<double,std::pair<int,int> > hitPixel(fabs(ifft_output[iEnt][jEnt]),xy);
	output.push_back(hitPixel);
      }
    }
  }
  return output;
}



//This goes through all of the clusters and converts their locations in the picture to global locations
void ComputeClusterParams(std::vector<Cluster> & clusters, const ParameterSet pS, const Picture pic, const int dim1, const int dim2, double ** bwPic_arr )
{
  //Get the global position of the picture and the center of the picture in pixels
  std::pair<double,double> picGlobalXY = pic.GetCenterXY();
  std::pair<int,int> picLocalCenterPix(dim2/2,dim1/2);

  



  //Get the relationship between pixel and distance
  double mmPerPixel = 1/(double)pS.GetPixelsPerMm();
  
  //Now loop over clusters and convert
  for( int iClust = 0; iClust < clusters.size(); ++iClust ){
    
    //Find the spatial extent by looking at all of the pixels in the cluster. Also, compute the integral of brightness.
    int locXMin = 10000000;
    int locXMax = -10000000;
    int locYMin = 10000000;
    int locYMax = -10000000;
    double bIntegral = 0;
    double ogIntegral = 0;
    std::vector<std::pair<double,std::pair<int,int> > > hitPix = clusters[iClust].GetHitPixels();
    std::vector<std::pair<double,std::pair<int,int> > > ogHitPix;
    for( int iP = 0; iP < hitPix.size(); ++iP ){

      std::pair<int,int> xy = hitPix[iP].second;
      int x = xy.first;
      int y = xy.second;
      
      if( x < locXMin ) locXMin = x;
      if( x > locXMax ) locXMax = x;
      if( y < locYMin ) locYMin = y;
      if( y > locYMax ) locYMax = y;

      //This adds to the integral of the hit pixels as they have been created with respect to the IFT values
      bIntegral += hitPix[iP].first;

      //Now deal with OG hit pix stuff
      //This adds to the integral of the hit pixels as they have been created with respect to the original (bw) image
      ogIntegral += bwPic_arr[y][x];
      


      //Make an ogHitPixel and push it into the vector
      std::pair<double,std::pair<int,int> > ogHitPixel(bwPic_arr[y][x],xy);
      ogHitPix.push_back(ogHitPixel);

    }

    //Set the cluster's area, integral, average brightness
    clusters[iClust].SetIntegral(bIntegral);
    clusters[iClust].SetArea((int)hitPix.size());
    clusters[iClust].SetAvgBrightness(bIntegral/(double)hitPix.size());


    //Set the cluster's ogHitPix, ogIntegral, and ogAverage brightness
    clusters[iClust].SetOGHitPixels(ogHitPix);
    clusters[iClust].SetOGIntegral(ogIntegral);
    clusters[iClust].SetOGAvgBrightness(ogIntegral/(double)ogHitPix.size());
    
    //Sanity check
    if( ogHitPix.size() != hitPix.size() ){
      std::cout << "Hit pix and og hit pix are not the same size vectors. Problem!!!" << std::endl;
    }
    
						   
    
    
    

    //Get the local center (in pixels relative to the 0,0 pixel) as an average of the sides
    int localCenterX = (locXMax + locXMin)/2;
    int localCenterY = (locYMax + locYMin)/2;

    clusters[iClust].SetLocalCenterXY(localCenterX,localCenterY);

    //Get the half width as a subtraction of the sides
    double halfWidthX = ((double)locXMax-(double)locXMin+1.)/2.;
    double halfWidthY = ((double)locYMax-(double)locYMin+1.)/2.;
    
    clusters[iClust].SetHalfWidthsXY(halfWidthX,halfWidthY);


    //Now convert this stuff to clobal center. Note that the pictures may be inverted in X and Y relative to the loom coordinate system
    std::pair<int,int> xy = clusters[iClust].GetLocalCenterXY();
    int xOffset = xy.first - picLocalCenterPix.first;
    int yOffset = xy.second - picLocalCenterPix.second;
    double gXOffset = xOffset*mmPerPixel;
    double gYOffset = yOffset*mmPerPixel;
    double globalX = pSetGlobal.GetIsPicInvertedXY()*gXOffset + picGlobalXY.first;
    double globalY = pSetGlobal.GetIsPicInvertedXY()*gYOffset + picGlobalXY.second;
    clusters[iClust].SetGlobalCenterXY(globalX,globalY);
    
    if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
      std::cout << "Cluster center: " << clusters[iClust].GetGlobalCenterXY().first << ", " << clusters[iClust].GetGlobalCenterXY().second << std::endl;
      std::cout << "GlobalX: " << globalX << std::endl;
      std::cout << "gXOffset: " << gXOffset << std::endl;
      std::cout << "xOffset: " << xOffset << std::endl;
      std::cout << "LocXMax: " << locXMax << ", locXMin: " << locXMin << std::endl;
      std::cout << "LocYMax: " << locYMax << ", locYMin: " << locYMin << std::endl;
      std::cout << "HalfwidthX: " << halfWidthX << std::endl;
      std::cout << "HalfwidthY: " << halfWidthY << std::endl;
      std::cout << "Pic global X: " << picGlobalXY.first << std::endl;
      std::cout << "^Picname: " << pic.GetPicName() << std::endl;
      
    }


    //Finally, give this cluster an owning picture name
    clusters[iClust].SetOwnerPicture(pic.GetPicName());

  }
}


//This checks to see if the cluster clust is near any other clusters in clustVect that are NOT IN THE SAME PICTURE.
//If it is, it returns true, and clust is not saved. (It's a duplicate.)
bool CheckForDuplicates( std::vector<Cluster> clustVect, Cluster clust )
{
  


  //Loop through cluster vect and check for duplicates
  for( int iC = 0; iC < clustVect.size(); ++iC ){



    Cluster clust2 = clustVect[iC];

    //Break once we get to the fiducial clusters
    bool cif = ClusterIsFiducial(clust2);
    if( cif ) break;


    double x2 = clust2.GetGlobalCenterXY().first;
    double y2 = clust2.GetGlobalCenterXY().second;

    double x = clust.GetGlobalCenterXY().first;
    double y = clust.GetGlobalCenterXY().second;

    if( pSetGlobal.GetVerbosity() == 22 || pSetGlobal.GetVerbosity() == 5 ){
      std::cout << "Duplication Check. Dist ctrs: " << pow(pow(x-x2,2) + pow(y-y2,2),0.5) << std::endl; 
      std::cout << "Name1: " << clust.GetOwnerPicture() << ", Name2: " << clust2.GetOwnerPicture() << std::endl;
    }
    
    if( pow(pow(x-x2,2) + pow(y-y2,2),0.5) < pSetGlobal.GetDuplicateClustDistance() &&
	clust2.GetOwnerPicture() != clust.GetOwnerPicture() ){
      
      //One additional thing: to make sure we get the best image possible, we want to "trust" clusters that are closer to their image's
      //respective center (due to lighting, etc.) So we do this here.
      double lx2 = clust2.GetLocalCenterXY().first;
      double ly2 = clust2.GetLocalCenterXY().second;
      double lx = clust.GetLocalCenterXY().first;
      double ly = clust.GetLocalCenterXY().second;
      double lr2 = pow(pow(ly2-PicWidthY,2)+pow(lx2-PicWidthX,2),0.5); //HARDCODED!!!! BE VERY CAREFUL WITH PICTURE SIZE
      double lr = pow(pow(ly-PicWidthY,2)+pow(lx-PicWidthX,2),0.5);
      if( lr > lr2 ){ //If this cluster is farther from the center of its picture than the other is from its own, dump it
	return true;
      }
    }
  }
  return false;
}


//Okay, I'm going to be pretty lazy with this. I'm going to find peaks by finding rho, theta points above some threshold
//in rho, theta space. There will be giant blobs because there are several lines per wire, but that's okay. I'll include
//all of the values in my output vector. Then, when I come to testing grid clusters for compatibility, I'll just test the
//cluster against all of the lines generated. If my cuts are tight enough, this should be enough to get just the crosses.
//It also avoids clustering and peakfinding here at the cost of a bit of redundant rho-theta checking for each point. I will
//need to add one parameter in for that process, though...
std::vector<std::pair<double,double> > FindHoughLines( std::vector<std::pair<double,std::pair<int,int> > > reducedPic, std::string picName )
{

  if( pSetGlobal.GetVerbosity() == 2 || pSetGlobal.GetVerbosity() == 5 ){
    std::cout << "Starting Hough Line Finder." << std::endl;
  }

  //Parameter (probably shouldn't mess with this too much)
  int nBinsTheta = 200;
  int nBinsRho = 200;
  double houghPointCutoff = pSetGlobal.GetHoughPointCutoff(); //This is unfortunately a parameter, but I can't really see how not to use a parameter in this step. It kinda blows.



  //TH2F For visualizing hough space
  TH2F * houghSpace = new TH2F("houghSpace","Hough Space",nBinsTheta,0,TMath::Pi(),nBinsRho,-1000,1000);
  

  //Output vect of rhos and thetas
  std::vector<std::pair<double,double> > output;

    
  //Loop through the points in the reduced picture
  for( int iPix = 0; iPix < reducedPic.size(); ++iPix ){
    double x = reducedPic[iPix].second.first;
    double y = reducedPic[iPix].second.second;
    double val = reducedPic[iPix].first;

    //For each point, loop through theta and compute rho
    for( int iTheta = 0; iTheta < nBinsTheta; ++iTheta ){
      double theta = TMath::Pi()*((double)iTheta)/((double)nBinsTheta);
      double rho = x*cos(theta) + y*sin(theta);
      houghSpace->Fill(theta,rho,val);
    }
  }
  
  if( picName == pSetGlobal.GetDebugPlotsPicName() ){
    TFile * f1 = new TFile("TestOutput.root","RECREATE");
    houghSpace->Write();
    f1->Close();
  }


  /*  START WORKING AGAIN HERE
  //Loop through houghspace and find the high points (above some threshold)
  for( int iX = 1; iX <= houghSpace->GetNbinsX(); ++iX ){
    for( int iY = 1; iY <= houghSpace->GetNbinsY(); ++iY ){
      houghSpace->GetBinContent(iX,iY); //I think this is indexed starting with 1...
      if( houghSpace->GetBinContent(iX,iY) > houghPointCutoff ){
	std::pair<double,double> aPair(houghSpace->GetBinX()->GetBinCenter()
    }
  }
  */
  
  //Free the memory for houghspace
  delete houghSpace;


  return output;
}



//This loops through the pixels in a picture and finds the baseline. It should use the median
//value so that the wires (outliers) don't matter all that much. Mean is used now because I'm lazyyyyy
void FindBaseline( std::vector<std::vector<int> > picture )
{
  double mean = 0;
  int counter = 0;
  for( int iX = 0; iX < picture.size(); ++iX ){
    for( int iY = 0; iY < picture[iX].size(); ++iY ){
      mean += picture[iX][iY];
      counter++;
    }
  }

  mean/=((double)counter);

  baseline = mean;
}



//Look for the pixels that are not background and keep those for use in the hough transform so we can speed it up.
std::vector<std::pair<double,std::pair<int,int> > > FilterOutBackgroundPix( std::vector<std::vector<int> > picture )
{
  std::vector<std::pair<double,std::pair<int,int> > > output;
  
  //Threshold for baseline cut (in grayscale units)
  double blineCutThresh = 60;
  
  //Loop over X and Y
  for( int iX = 0; iX < picture.size(); ++iX ){
    for( int iY = 0; iY < picture[iX].size(); ++iY ){
      if( fabs((double)picture[iX][iY] - baseline) < blineCutThresh ) continue;
      std::pair<int,int> thePair(iY,iX);
      std::pair<double,std::pair<int,int> > thePair2(picture[iX][iY],thePair);
      output.push_back(thePair2);
    }
  }

  return output;
}


/*
  void CompareClustersToHoughLines( std::vector<Cluster> clusters, std::vector<std::pair<double,double> > lines )
  {
  //Here, we unpack the lines from the line parameters. We then loop through each cluster and see if it is on
  //at least two lines that are separated by 90 degrees in hough space. This corresponds to a cross. If it does, we tag it as such.
  for( int iClu = 0; iClu < clusters.size(); ++iClu ){
  
  
  
  
  
  
  }
*/


//This checks to see if a cluster is within the "fiducial area" set by the duplicate cluster fiducial fraction.
//If it is, then this function returns true.
bool ClusterIsFiducial( Cluster theClust )
{
  //Get the local X and Y of the cluster, in pixels
  int X = theClust.GetLocalCenterXY().first;
  int Y = theClust.GetLocalCenterXY().second;
  
  //Find the distance from the edge in X
  int edgeDistX = 0;
  int edgeDistY = 0;
  if( X < PicWidthX )
    edgeDistX = X;
  else{ edgeDistX = 2*PicWidthX-X; }
  if( Y < PicWidthY )
    edgeDistY = Y;
  else{ edgeDistY = 2*PicWidthY-Y; }
  
  //Now find the fraction of the total width in X, Y that the edge dist is
  if( ((double)edgeDistX)/((double)PicWidthX*2.) < pSetGlobal.GetDuplicateClustFiducialFraction() ||
      ((double)edgeDistY)/((double)PicWidthY*2.) < pSetGlobal.GetDuplicateClustFiducialFraction() ){
    return false;
  }

  return true;

}



//Reorder the cluster vect so that we break it into chunks of fiducial clusters and non-fiducial clusters.
//The non-fiducial ones come first so that we can break when we get to fiducials
void ReorderClusterVect( std::vector<Cluster> & clustVect )
{
  
  //Sanity check
  std::cout << "Reordering clusters into non-fiducial and fiducial clusters" << std::endl;


  //Loop through the vector
  bool firstOne = false;
  std::string ownerPic = "BLAH";
  int ID = -1;
  for( int iC = 0; iC < clustVect.size(); ++iC ){

    std::cout << "iC: " << iC << ", clusterVectSize: " << clustVect.size() << std::endl;
    
    if( ClusterIsFiducial( clustVect[iC] ) ){
      std::string thisOwnerPic = clustVect[iC].GetOwnerPicture();
      int thisID = clustVect[iC].GetID();

      //We've finished the loop over clusters at this point. If this wasn't here, this would
      //be an infinite loop.
      if( firstOne == true && thisOwnerPic == ownerPic && thisID == ID ) break;

      //Set settings of the first moved cluster (fiducial cluster)
      if( firstOne == false ){
	ownerPic = clustVect[iC].GetOwnerPicture();
	ID = clustVect[iC].GetID();
	firstOne = true;
      }
      clustVect.push_back(clustVect[iC]);
      clustVect.erase(clustVect.begin()+iC);
      iC--;
    }
  }
}


