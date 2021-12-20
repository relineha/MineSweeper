///////////////////////////////////////////////////////
//
// AnalysisFunctions.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is an implementation file for the analysis
// functions to be used in minesweeper.
// 
//
/////////////////////////////////////////////////////////

//Standard includes
#include <iostream>
#include <fstream>

//MineSweeper includes
#include "Grid.hh"
#include "Cluster.hh"
#include "Picture.hh"
#include "ParameterSet.hh"
#include "DBCluster.hh"
#include "../include/AnalysisFunctions.hh"

//ROOT includes
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TMath.h"
#include "TF1.h"

//Global grid object
static Grid theGlobalGrid;
static ParameterSet pSetGlobal;
const int histXBins = 100;
const int histYBins = 100;
int histClustXBins = 0;//6000;
int histClustYBins = 0;//6000;
const int histClustAreaBins = 320;
const int brightnessThresh = 10000000;
const int areaThresh = 120;
const int histClustHWBins = 160;



//////////////////////////////////////////////////
//             Struct Definitions               //
//////////////////////////////////////////////////
struct RecoWire
{
  int XorY; //0 for X=Constant, 1 for Y=Constant
  double theta;
  double rho;
  double nPix;
};






//////////////////////////////////////////////////
//               Cut Definitions                //
//////////////////////////////////////////////////
bool CXPop1D_75( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double a = clu.GetArea()/pixPerMm/pixPerMm;
  double sq = std::min(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second)/
    std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
 
  if( b > 5e+6 && b < 9e+6 &&
      a > 10000 && 
      sq > 0.51 ){
    return true;
  }
  return false;
}

bool CXPop1D_100( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double a = clu.GetArea()/pixPerMm/pixPerMm;
  double sq = std::min(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second)/
    std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
 
  if( b > 5e+6 && b < 9e+6 &&
      a > 18000 && 
      sq > 0.51 ){
    return true;
  }
  return false;
}



bool CXPop2B_75( Cluster clu, const double pixPerMm )
{
  double xhw = clu.GetHalfWidthsXY().first/pixPerMm;
  double yhw = clu.GetHalfWidthsXY().second/pixPerMm;
  if( xhw > 80 && yhw > 80 ) return true;
  return false;

}

bool CXPop2B_100( Cluster clu, const double pixPerMm )
{
  double xhw = clu.GetHalfWidthsXY().first/pixPerMm;
  double yhw = clu.GetHalfWidthsXY().second/pixPerMm;
  if( xhw > 100 && yhw > 100 ) return true;
  return false;

}


bool CXPop3_75( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double a = clu.GetArea();
  double a_fake = clu.GetHalfWidthsXY().first*clu.GetHalfWidthsXY().second;
  double r = a/a_fake;
  if( r < 0.4 )return true;
  return false;
}


bool CXPop3_100( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double a = clu.GetArea();
  double a_fake = clu.GetHalfWidthsXY().first*clu.GetHalfWidthsXY().second;
  double r = a/a_fake;
  if( r < 0.3 )return true;
  return false;
}


bool CXPop8_75( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double sq = std::min(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second)/
    std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 9e+6 && md > 5 && sq > 0.5 ) return true;
  return false;
}

bool CXPop8_100( Cluster clu, const double pixPerMm )
{
  double b = clu.GetAvgBrightness();
  double sq = std::min(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second)/
    std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 11e+6 && md > 5 && sq > 0.5 ) return true;
  return false;
}


//This looks at a cluster and identifies if it is a case where you have
//reflections in a corner. If it is, return true.
//Note to self: this cut looks like it has a purity of well over 90%. 
bool CXCornerCases( Cluster clu, const double pixPerMm )
{
  //Find the cluster center in XY
  int ctrX = clu.GetLocalCenterXY().first;
  int ctrY = clu.GetLocalCenterXY().second;
  int hwX = clu.GetHalfWidthsXY().first;
  int hwY = clu.GetHalfWidthsXY().second;
  
  //Check all four corner cases
  if( ctrX + hwX >= 1022 && ctrY + hwY >= 766 ) return true;
  if( ctrX - hwX <= 2 && ctrY - hwY <= 2 ) return true;
  if( ctrX + hwX >= 1022 && ctrY - hwY <= 2 ) return true;
  if( ctrX - hwX <= 2 && ctrY + hwY >= 766 ) return true;
  return false;

}

//This looks at a cluster and cuts out the population of cross reflections.
//It also eliminates some non-cross-reflections. Efficiency will be examined
//in a second.
bool CXCrossReflectionCases( Cluster clu, const double pixPerMm )
{
  //Find the cluster area. If the area is consistent with the cross reflection peak,
  //return true.
  if( clu.GetArea()/pixPerMm/pixPerMm > 2000 && clu.GetArea()/pixPerMm/pixPerMm < 5500 ) return true;
  return false;

}

//This is a cut that eliminates small things (things smaller than the corner reflections in size)
bool CXSmallClusters( Cluster clu, const double pixPerMm )
{
  if( clu.GetArea()/pixPerMm/pixPerMm <= 2000 ) return true;
  return false;
}



//Brutalize the search (last resort)
bool CXPop10_Brutal( Cluster clu, const double pixPerMm )
{
  //  double 
  double b = clu.GetAvgBrightness();
  if( b > 10e+6 ) return true;
  return false;
		 
}

//FOr post-AP bottom scan
bool CXPop10_Constrained( Cluster clu )
{
  //  double 
  double b = clu.GetAvgBrightness();
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 10e+6 && md > 5 ) return true;
  return false;
		 
}

//FOr post-AP bottom scan
bool CXPop10_Constrained_FinalBottom( Cluster clu )
{
  //  double 
  double b = clu.GetAvgBrightness();
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 10e+6 && md > 5 ) return true;
  return false;
		 
}

//FOr post-AP bottom scan
bool CXPop10_Constrained_FinalBottomPostBlowdown( Cluster clu )
{
  //  double 
  double b = clu.GetAvgBrightness();
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 9e+6 && md > 5 ) return true;
  return false;
		 
}

//FOr pre-AP scan of Bottom 2
bool CXPop10_Constrained_Bottom2_OriginalScan( Cluster clu )
{
  //  double 
  double b = clu.GetAvgBrightness();
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 6e+6 && md > 4 ) return true;
  return false;
		 
}

//FOr pre-AP scan of Bottom 2
bool CXPop10_Constrained_GateScan2( Cluster clu )
{
  //  double 
  double b = clu.GetAvgBrightness();
  double md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( b > 8e+6 && md > 5 ) return true;
  return false;
		 
}


//OG average brightness vs average brightness
bool CXPop1_OGABvAB( Cluster clu )
{
  double b = clu.GetAvgBrightness();
  double b2 = clu.GetOGAvgBrightness();
  if( b2 > 20 && b < 6e6 ) return true;
  return false;
}

//Saame as above, but for original bottom 2 scan
bool CXPop1_OGABvAB_Bottom2_OriginalScan( Cluster clu )
{
  double b = clu.GetAvgBrightness();
  double b2 = clu.GetOGAvgBrightness();
  if( b2 > 13 && b < 5.5e6 ) return true;
  return false;
}


//OG max pixel val vs. max pixel val
bool CXPop1_OGMPvMP( double maxPixel, double ogMaxPixel )
{
  if( maxPixel < 7e6 && ogMaxPixel > 25 )return true;
  return false;
}

//OG integral vs. integral
bool CXPop1_OGIntvInt( Cluster clu )
{
  if( clu.GetIntegral() < 700e6 && clu.GetOGIntegral() > 2000 )return true;
  return false;
}

//This is for the post-AP scan of the bottom2
bool CXPop1_ABvMD( Cluster clu )
{
  int md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( clu.GetAvgBrightness() < 10e6 && md > 20 )return true;
  return false;
}

//This is for the original scan of the bottom2
bool CXPop1_ABvMD_Bottom2_OriginalScan( Cluster clu )
{
  int md = std::max(clu.GetHalfWidthsXY().first,clu.GetHalfWidthsXY().second);
  if( clu.GetAvgBrightness() < 10e6 && md > 15 )return true;
  return false;
}




//////////////////////////////////////////////////
//////////////////////////////////////////////////
//                                              //
//           MAIN ANALYSIS FUNCTION             //
//                                              //
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//Analyze should always just take one argument: a single grid object. This should contain
//everything we need it to contain.
void Analyze( const Grid theGrid ) 
{
  std::cout << "\n------------------------------------------------" << std::endl;
  std::cout << "Starting Analysis Run. Red 5 Standing By." << std::endl;
  std::cout << "------------------------------------------------" << std::endl;

  //Set the global grid object to the local one
  theGlobalGrid = theGrid;

  //Create the output file
  TFile * file1 = new TFile("MineSweeperAnalysisOutput.root","RECREATE");


  //Get the pictures
  std::vector<Picture> picVect = theGrid.GetPictures();

  //for( size_t iPic = 0; iPic < picVect.size(); ++iPic ){
  //  std::cout << "Pic name from grid: " << picVect[iPic].GetPicName() << std::endl;
  // }

  //Make a 2-d plot of pic locations
  PlotPicLocations();

  //Run Metrology
  //  RunMetrology();

  //Now make a scatter plot of the cluster locations. If I've done this right, a grid should appear.
  PlotClusterInfo();

  



  //Wrap up filestream
  file1->Close();

}



//Just reads in the pictures analyzed and plots their positions on a histogram
void PlotPicLocations()
{
  //Get the list of the grid pictures
  std::vector<Picture> picVect = theGlobalGrid.GetPictures();
  std::vector<std::string> flags;
  
  //Create the histogram
  TH2F * picPositions = new TH2F("picPositions","picPositions",histXBins,1,0,histYBins,1,0);
  
  //For each of them, plot the location.
  for( int iPic = 0; iPic < picVect.size(); ++iPic ){
    std::pair<double,double> picXY = picVect[iPic].GetCenterXY();
    picPositions->Fill(picXY.first,picXY.second);
  }
  
  //Save
  picPositions->Write();


  
  /*
  //Print out a list of pictures whose positions pass a spatial cut
  for( int iPic = 0; iPic < picVect.size(); ++iPic ){
    std::pair<double,double> picXY = picVect[iPic].GetCenterXY();
    double gridOffsetX = theGlobalGrid.GetGeometry().GetCenterXY().first;
    double gridOffsetY = theGlobalGrid.GetGeometry().GetCenterXY().second;
    double x = picXY.first - gridOffsetX;
    double y = picXY.second - gridOffsetY;
    if( x > 100 && x < 200 && y > 100 && y < 100 ){
      flags.push_back(picVect[iPic].GetPicName());
      flags
  */



  
}


//Self explanatory.
void PlotClusterInfo()
{

  std::vector<Cluster> clusterVect = theGlobalGrid.GetClusters();
  std::vector<std::pair<int, std::string>> flags;
  double picHeight = 768;

  //Get parameter set info
  ParameterSet pSet = theGlobalGrid.GetParameterSet();
  double pixPerMm = pSet.GetPixelsPerMm()/1000.; //Really now pixels per micron

  //Loop over clusters to identify the extreme positions of clusters
  double epsilon = 10;
  double minX = 999999;
  double maxX = -999999;
  double minY = 999999;
  double maxY = -999999;
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    if( clustXY.first < minX ) minX = clustXY.first;
    if( clustXY.first > maxX ) maxX = clustXY.first;
    if( clustXY.second < minY ) minY = clustXY.second;
    if( clustXY.second > maxY ) maxY = clustXY.second;
  }

  //Identify the number of bins you want for X and Y, based on the size of the grid.
  //Let's choose the number of bins so that we get 10 bins per pitch.
  histClustYBins = 130; //Hardcoded for little grids because we can't measure diameter well..
  histClustXBins = 130; //Hardcoded for little grids becaause ...

  
  //Create the histograms
  TH2F* clustPositions = new TH2F("clustPositions","Cluster Positions on the Grid", histClustXBins,minX-epsilon,maxX+epsilon,histClustYBins,minY-epsilon,maxY+epsilon );
  TH1F* clustPositionsX = new TH1F("clustPositionsX","Cluster Positions on the Grid, X", histClustXBins,minX-epsilon,maxX+epsilon);
  TH1F* clustPositionsY = new TH1F("clustPositionsY","Cluster Positions on the Grid, Y", histClustYBins,minY-epsilon,maxY+epsilon);
  TH1F* clustAreas = new TH1F("clustAreas","Cluster Areas (computed by halfWidths), in um^2",histClustAreaBins,0,50000);
  TH2F* clust2DSize = new TH2F("clust2DSize","Cluster Half Widths (X and Y), in um",histClustHWBins,0,1000,histClustHWBins,0,1000);
  TH1F* clustAreasReal = new TH1F("realClustAreas","Real Cluster Areas, in um^2",histClustAreaBins,0,50000);
  TH1F* clustIntegral = new TH1F("clustIntegral","Cluster Integral, (au)",histClustAreaBins,0,3e9);
  TH1F* clustAvgBrightness = new TH1F("clustAvgBrightness","Cluster Avg Brightness",histClustAreaBins,4e+6,20e+6);
  TH2F* clustAvgBrightnessVArea = new TH2F("clustAvgBrightnessVArea","Cluster Avg Brightness versus Area",histClustAreaBins,4e+6,20e+6,histClustAreaBins,0,50000);
  TH2F* clustIntegralVArea = new TH2F("clustIntegralVArea","Cluster Integral versus Area",histClustAreaBins,1,0,histClustAreaBins,0,50000);
  TH1F* clustRectangularity = new TH1F("clustRectangularity","Cluster Rectangularity (fraction)",100,0,1);
  TH2F* clustAvgBrightnessVRectangularity = new TH2F("clustAvgBrightnessVRectangularity","Cluster Avg Brightness vs. Rectantularity",histClustAreaBins,4e+6,20e+6,100,0,1);
  TH2F* clustRealAreaVRectangularity = new TH2F("clustRealAreaVRectangularity","Cluster Real Area vs. Rectangularity",histClustAreaBins,0,50000,100,0,1);
  TH2F* clustMaxSizePos = new TH2F("clustMaxSize","Cluster Max Size by Bin (DO NOT REBIN)",100,-1373,0,100,-660,620);
  TH1F* clustMaxPixelVal = new TH1F("clustMaxPixelVal","Cluster Max Pixel Val; Val [arb units]; Clusters",200,0,50e+6);
  TH1F* clustSigmaPixelVal = new TH1F("clustSigmaPixelVal","Cluster Sigma Pixel Val; Sigma [arb units]; Clusters",200,0,10e+6);
  TH2F* clustMaxVSigmaPixelVal = new TH2F("clustMaxVSigmaPixelVal","Cluster Max vs. Sigma Pixel Val; Max Pixel Val [arb units]; Sigma Pixel Val [arb units]",200,0,50e+6,200,0,10e+6);
  TH2F* clustMaxVAvgPixelVal = new TH2F("clustMaxVAvgPixelVal","Cluster Average Brightness vs. Max Pixel Val; Avg Brightness [arb units]; Max Pixel Val [arb units]",200,0,30e+6,200,0,50e+6);
  TH2F* clustSigmaVAvgPixelVal = new TH2F("clustSigmaVAvgPixelVal","Cluster Average Brightness vs. Sigma Pixel Val; Avg Brightness [arb units]; Sigma Pixel Val [arb units]",200,0,30e+6,200,0,10e+6);
  TH1F* clustOGIntegral = new TH1F("clustOGIntegral","Cluster OG Integral, (au)",histClustAreaBins,0,-1);
  TH1F* clustOGAvgBrightness = new TH1F("clustOGAvgBrightness","Cluster OG Average Brightness, (au)",histClustAreaBins,4,50);
  TH2F* clustOGIntegralVsIntegral = new TH2F("clustOGIntegralVsIntegral","Cluster OG Integral vs. Integral; Original Integral [pixel vals]; Integral [IFT pixel vals]",histClustAreaBins,1,0,histClustAreaBins,1,0);
  TH2F* clustOGMaxPixelVsMaxPixel = new TH2F("clustOGMaxPixelVsMaxPixel","Cluster OG MaxPixel vs. MaxPixel; Original MaxPixel [pixel vals]; MaxPixel [IFT pixel vals]",histClustAreaBins,1,0,histClustAreaBins,1,0);
  TH2F* clustOGSigmaPixelVsSigmaPixel = new TH2F("clustOGSigmaPixelVsSigmaPixel","Cluster OG Sigma Pixel vs. Sigma Pixel",histClustAreaBins,1,0,histClustAreaBins,1,0);
  TH2F* clustOGAvgBrightnessVsArea = new TH2F("clustOGAvgBrightnessVsArea","Cluster OG Average Brightness vs. Area",histClustAreaBins,4,50,histClustAreaBins,0,50000);
  TH2F* clustOGIntegralVsArea = new TH2F("clustOGIntegralVsArea","Cluster OG Integral vs. Area",histClustAreaBins,0,-1,histClustAreaBins,0,50000);
  TH2F* clustOGAvgBrightnessVsRectangularity = new TH2F("clustOGAvgBrightnessVsRectangularity","Cluster OG Avg Brightness vs. Rectangularity",histClustAreaBins,4,50,100,0,1);
  TH2F* clustOGAvgBrightnessVsAvgBrightness = new TH2F("clustOGAvgBrightnessVsAvgBrightness","Cluster OG Avg Brightness vs. Brightness",histClustAreaBins,4,50,histClustAreaBins,4e+6,20e+6);
  TH1F* edgeArtifactAreas = new TH1F("edgeArtifactAreas","Edge artifact cluster areas (mostly edge artifacts)",histClustAreaBins,0,-1);
  TH2F* edgeArtifactLocalPositions = new TH2F("edgeArtifactLocalPositions","Edge artifacts' local positions",1024,0,1024,768,0,768);
  TH2F* clustSizeVsMaxPixelVal = new TH2F("clustSizeVsMaxPixelVal","Cluster Size Vs. Max Pixel Val",200,0,50e+6,histClustAreaBins,0,-1);
  TH2F* clustAvgBrightnessVsMaxDim = new TH2F("clustAvgBrightnessVsMaxDim","Average Brightness vs. Max Dimension; Average Brightness [au]; Max Dimension [pixels]",histClustAreaBins,4e+6,20e+6,100,0,100);
  TH2F* clustIntegralVsAvgBrightness = new TH2F("clustIntegralVsAvgBrightness","Integral vs. Avg Brightness; Integral; Avg Brightness",histClustAreaBins,1,0,histClustAreaBins,4e+6,20e+6);

  //////////////////////////////////
  // Cleanliness metric histograms
  //--------------------------------
  TH1F* fitCor_clustIntegral = new TH1F("fitCor_clustIntegral","Cluster integral, cross population bulk corrected; Integral [IFT units]; Clusters",histClustAreaBins,0,3e9);
  TH2F* postCleanlinessCut_clusterXY = new TH2F("postCleanlinessCut_clusterXY","Post-Cleanliness Cut, Cluster XY; X [mm]; Y [mm]",histClustXBins,minX-epsilon,maxX+epsilon,histClustYBins,minY-epsilon,maxY+epsilon );
    
  clustPositions->GetXaxis()->SetTitle("X (mm)");
  clustPositions->GetYaxis()->SetTitle("Y (mm)");

  

  //Loop over clusters
  int cornerCounter = 0;
  int crossCounter = 0;
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){

    //From the cluster hitpix, build a few pieces of information
    double sigmaPixelVal = computeSigmaPixelVal( clusterVect[iClust] );
    double maxPixelVal = computeMaxPixelVal( clusterVect[iClust] );
    double OGMaxPixelVal = computeOGMaxPixelVal( clusterVect[iClust] );
    double OGSigmaPixelVal = computeOGSigmaPixelVal( clusterVect[iClust] );





    /////////////////////////////
    // Quality control cuts 
    //---------------------------

    //Here, do a cut on whether a cluster is a bad-pixel artifact or not. There are quite a few instances where the same
    //pixel fired on several pictures, creating a small cluster. This creates the appearance of periodicity in our 
    //cluster locations where there should be none.
    if( CXBadPixelClusters(clusterVect[iClust]) ) continue;

    //Here, do a cut on the relationship between average brightness and
    //original average brightness. This cuts out some ring and straight-wire
    //clusters that are hard to associate anything physical to.
    //    if( CXPop1_OGABvAB_Bottom2_OriginalScan(clusterVect[iClust]) ) continue;

    //Here, do a cut on integral
    //    if( clusterVect[iClust].GetIntegral() < 7e8 ) continue;
    //if( clusterVect[iClust].GetAvgBrightness() < 16e6 ) continue;
    //    if( clusterVect[iClust].GetArea()/pixPerMm/pixPerMm < 7000 ) continue; 
    
    







    /////////////////////////////
    // Filling of Histograms
    //---------------------------


    clustMaxPixelVal->Fill(maxPixelVal);
    clustSigmaPixelVal->Fill(sigmaPixelVal);
    clustMaxVSigmaPixelVal->Fill(maxPixelVal,sigmaPixelVal);
    clustMaxVAvgPixelVal->Fill(clusterVect[iClust].GetAvgBrightness(),maxPixelVal);    
    clustSigmaVAvgPixelVal->Fill(clusterVect[iClust].GetAvgBrightness(),sigmaPixelVal);    
    clustSizeVsMaxPixelVal->Fill(maxPixelVal,((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm);
    clustAvgBrightnessVsMaxDim->Fill(clusterVect[iClust].GetAvgBrightness(),std::max(clusterVect[iClust].GetHalfWidthsXY().first,clusterVect[iClust].GetHalfWidthsXY().second));
    clustIntegralVsAvgBrightness->Fill(clusterVect[iClust].GetIntegral(),clusterVect[iClust].GetAvgBrightness());

    
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    //    std::cout << "Cluster location: " << clustXY.first << ", " << clustXY.second << std::endl;
    std::pair<double,double> clustHWs = clusterVect[iClust].GetHalfWidthsXY();
    double area = clustHWs.first*clustHWs.second*4.0;

    //For clustmaxsizepos, only fill the histogram bins with the clusters of largest size in those bins
    if( clustMaxSizePos->GetBinContent(clustMaxSizePos->FindBin(clustXY.first,clustXY.second)) < area ){
      clustMaxSizePos->SetBinContent(clustMaxSizePos->FindBin(clustXY.first,clustXY.second),area);
    }

    //   if( clusterVect[iClust].GetAvgBrightness() > 14000000 ){
    clustPositions->Fill(clustXY.first,clustXY.second,1); //Weight by area to see how long the thing is. (currently not applied)
    clustPositionsY->Fill(clustXY.second);
    clustPositionsX->Fill(clustXY.first);
      // }
    clustAreas->Fill(area/pixPerMm/pixPerMm);
    clust2DSize->Fill(clustHWs.first/pixPerMm,clustHWs.second/pixPerMm);
    clustAreasReal->Fill(((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm);
    clustIntegral->Fill(clusterVect[iClust].GetIntegral());
    clustAvgBrightness->Fill(clusterVect[iClust].GetAvgBrightness());
    clustAvgBrightnessVArea->Fill(clusterVect[iClust].GetAvgBrightness(),((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm);
    clustIntegralVArea->Fill(clusterVect[iClust].GetIntegral(),(double)clusterVect[iClust].GetArea()/pixPerMm/pixPerMm);

    //We define a rectangularity by the ratio of hit pixels to not-hit pixels within the area bounded by the full widths in XY of the cluster
    clustRectangularity->Fill(clusterVect[iClust].GetArea()/area);
    clustAvgBrightnessVRectangularity->Fill(clusterVect[iClust].GetAvgBrightness(),clusterVect[iClust].GetArea()/area);
    clustRealAreaVRectangularity->Fill(clusterVect[iClust].GetArea()/pixPerMm/pixPerMm,clusterVect[iClust].GetArea()/area);

    




    if( clusterVect[iClust].GetArea() > areaThresh && clusterVect[iClust].GetArea()/area < 0.5 ){
      std::cout << "Rectangularity and area thresholds passed for cluster at x: " << clustXY.first << ", y: " << clustXY.second << std::endl;
    }

    //Sanity check
    if( area <= 0 ){
      std::cout << "Cluster Area is zero?" << std::endl;
      std::cout << "Cluster area is: " << area << std::endl;
    }

    //For kicks and gigs
    if( clustHWs.first > 20 && clustHWs.second > 20 ){
      std::cout << "WE'VE GOT A BIG SUCKER HERE! Location: " << clustXY.first << ", " << clustXY.second << std::endl;
    }


    //Fill various OG Hit Pixel-related quantity histos.
    clustOGIntegral->Fill(clusterVect[iClust].GetOGIntegral());
    clustOGAvgBrightness->Fill(clusterVect[iClust].GetOGAvgBrightness());
    clustOGAvgBrightnessVsArea->Fill(clusterVect[iClust].GetOGAvgBrightness(),((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm);
    clustOGIntegralVsArea->Fill(clusterVect[iClust].GetOGIntegral(),((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm);
    clustOGAvgBrightnessVsRectangularity->Fill(clusterVect[iClust].GetOGAvgBrightness(),clusterVect[iClust].GetArea()/area);
    clustOGAvgBrightnessVsAvgBrightness->Fill(clusterVect[iClust].GetOGAvgBrightness(),clusterVect[iClust].GetAvgBrightness());
    clustOGIntegralVsIntegral->Fill(clusterVect[iClust].GetOGIntegral(),clusterVect[iClust].GetIntegral());
    clustOGMaxPixelVsMaxPixel->Fill(OGMaxPixelVal,maxPixelVal);
    clustOGSigmaPixelVsSigmaPixel->Fill(OGSigmaPixelVal,sigmaPixelVal);





    
    //////////////////////////////////////////////////////////////
    ///////////// MAKE CUT ON CLUSTERS FOR FIBER ID //////////////
    //////////////////////////////////////////////////////////////
    //This pushes back all of the pertinent clusters into a flags vector, which is then read out into a
    //"dust file" (text) that can be read by cropduster.
    if( //CXPop1D_100( clusterVect[iClust], pixPerMm ) ||
       	//CXPop2B_100( clusterVect[iClust], pixPerMm ) ||
       	//CXPop3_100( clusterVect[iClust], pixPerMm ) ||
       	//CXPop8_100( clusterVect[iClust], pixPerMm ) ){
        //CXPop10_Constrained( clusterVect[iClust] ) ){
       CXPop10_Constrained_FinalBottomPostBlowdown( clusterVect[iClust])  ){
      flags.push_back(std::make_pair(5,clusterVect[iClust].GetOwnerPicture() + "," + std::to_string(clusterVect[iClust].GetLocalCenterXY().first) + ","+ std::to_string(picHeight-clusterVect[iClust].GetLocalCenterXY().second)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().first)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().second)));
    }
    
       


    /*
    //This is cropDuster's cut
    double rectangularity = clusterVect[iClust].GetArea()/area;
    //if( clusterVect[iClust].GetAvgBrightness() > 15000000 ){
    if( rectangularity < 0.3 ){
      flags.push_back(std::make_pair(5,clusterVect[iClust].GetOwnerPicture() + "," + std::to_string(clusterVect[iClust].GetLocalCenterXY().first) + ","+ std::to_string(picHeight-clusterVect[iClust].GetLocalCenterXY().second)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().first)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().second)));
    }
    */    

    /////////////////////////////////////////
    // Crop duster population searches, REL
    //---------------------------------------
      
    if( CXPop1_ABvMD_Bottom2_OriginalScan( clusterVect[iClust] )){
      flags.push_back(std::make_pair(5,clusterVect[iClust].GetOwnerPicture() + "," + std::to_string(clusterVect[iClust].GetLocalCenterXY().first) + ","+ std::to_string(picHeight-clusterVect[iClust].GetLocalCenterXY().second)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().first)+","+std::to_string(clusterVect[iClust].GetHalfWidthsXY().second)));
    }
    
    
    
    
  } //Loop over clusters


  ///////////////////////////
  // Cropduster output flags
  //-------------------------

  std::ofstream dust_file;
  dust_file.open("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/CropDusterStuff/IdentifiedDust.csv");
  //Print out all flagged pictures and coordinates for easy reference, optionally restricting by type
  for (int i=0; i<flags.size(); i++)
    {
      if(flags[i].first == 5)
	{
	  std::cout << flags[i].second << "\n";
	  dust_file << flags[i].second << "\n";
	}

    }
  std::cout << "No of flags: " << flags.size() << "\n";
  dust_file.close();













  ///////////////////////////////////////////////////////////////////
  // Part two of the analysis: quantitative cleanliness comparisons
  //-----------------------------------------------------------------

  //We've somewhat motivated a fit and cut on integral to identify
  //clusters of interest that are separate from the calibration 
  //(cross) clusters. First, fit the integral histo to a gaussian
  //plus a power law drop. This allows us to identify the calibration
  //population
  
  TF1 * gausPlusPower = new TF1("gausPlusPower","[0]*exp(-0.5*((x-[1])/[2])**2) + [3]*x**[4]",30e6,600e6);
  gausPlusPower->SetParameter(0,1000);
  gausPlusPower->SetParameter(1,500e6);
  gausPlusPower->SetParameter(2,100e6);
  gausPlusPower->SetParameter(3,10000);
  gausPlusPower->SetParameter(4,-1);
  gausPlusPower->SetParLimits(0,10,10000);
  gausPlusPower->SetParLimits(1,100e6,650e6);//300e6,500e6);//1,100e6,300e6);
  gausPlusPower->SetParLimits(2,10e6,150e6);
  //  gausPlusPower->SetParLimits(3,10e6,100e16);
  clustIntegral->Fit(gausPlusPower,"R","",30e6,600e6);
  
  //Get the bin associated with the gaussian peak+3sigma so we can identify the cleanliness and plot its
  //clusters' positions on the grid
  int peakBinPlus3sigma = clustIntegral->FindBin(gausPlusPower->GetParameter(1)+3*gausPlusPower->GetParameter(2));
  double integral = clustIntegral->Integral(peakBinPlus3sigma,clustIntegral->GetNbinsX());
  std::cout << "Integral of fitCor_ClustIntegral from peak+3sigma upward: " << integral << std::endl;
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){  
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    if( clusterVect[iClust].GetIntegral() > gausPlusPower->GetParameter(1)+3*gausPlusPower->GetParameter(2) ){
      postCleanlinessCut_clusterXY->Fill(clustXY.first,clustXY.second,1);
    }
  }
  
  

  
  































  /*
  ///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////
  ////////////// PART TWO OF ANALYSIS: CALIBRATION ////////////// 
  ///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////

  //Do some fitting of these histos for various reasons
  
  //First, fit the cluster area histogram to a gaussian tailored to find the calibration population peak.
  double upEnd = 700;//3000; //700
  double lowEnd = 1600;//5000; //1600
  double wireDiam = 100; //in um
  
  TF1 * f1 = new TF1("f1","gaus",lowEnd,upEnd);
  clustAreasReal->Fit(f1,"R","",lowEnd,upEnd);
  double calibMean = f1->GetParameter(1);
  double calibSigma = f1->GetParameter(2);
  std::cout << "Real Area mean: " << calibMean << ", sigma: " << calibSigma << std::endl;

  //Second set of fit histograms
  TH2F* clustXYHW_calibCut = new TH2F("clustXYHW_calibCut","Cluster XY Halfwidths, calibration population; X Half Width [um]; Y Half Width [um]",histClustHWBins,0,1000,histClustHWBins,0,1000);
  TH2F* clustXYHW_calibCutOver = new TH2F("clustXYHW_calibCutOver","Cluster XY Halfwidths, clusters larger than calibration population; X Half Width [um]; Y Half Width [um]",histClustHWBins,0,1000,histClustHWBins,0,1000);
  TH2F* clustXYHW_HWCut = new TH2F("clustXYHW_HWCut","Cluster XY Halfwidths, non-calib clusters passing absolute xyhw cut; X Half Width [um]; Y Half Width [um]",histClustHWBins,0,1000,histClustHWBins,0,1000);
  TH2F* clustXYHW_calibCutXYOver = new TH2F("clustXYHW_calibCutXYOver","Cluster XY halfwidths, non-calib clusters passing relative xyhw cut;X Half Width [um]; Y Half Width [um]",histClustHWBins,0,1000,histClustHWBins,0,1000);
  TH2F* clustPositions_calibCutXYOver = new TH2F("clustPositions_calibCutXYOver","Cluster Positions, non-calib clusters passing relative xyhw cut; X position [mm]; y position [mm]",histClustXBins,minX-epsilon,maxX+epsilon,histClustYBins,minY-epsilon,maxY+epsilon);
  

  
  //Useful variables
  double meanXHW = 0;
  double meanYHW = 0;
  int counterHW = 0;

  //Second loop over clusters - used to find calibration population in another space
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){

    //Same cut on bad pixels
    if( CXBadPixelClusters(clusterVect[iClust]) ) continue;
    
    //Now identify if a cluster is more-or-less in the calibration sample
    double realArea = ((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm;
    double xhw = clusterVect[iClust].GetHalfWidthsXY().first/pixPerMm;
    double yhw = clusterVect[iClust].GetHalfWidthsXY().second/pixPerMm;
    if( realArea > calibMean+2*calibSigma ) clustXYHW_calibCutOver->Fill(xhw,yhw);

    //Here, we will only use the top half of the calibration population, because the lower
    //half will bleed into the rising spectrum at low areas.
    if( realArea-calibMean < 2*calibSigma && realArea > calibMean ){
      clustXYHW_calibCut->Fill(xhw,yhw);
      meanXHW+=xhw;
      meanYHW+=yhw;
      counterHW++;
    }

    //We want to set a lower bound on the x and y half widths. Very small half widths may have sizes that fluctuate up
    //and down drastically (fractionally) with changes in illumination, etc. So we demand that the cluster has a half
    //width in either direction that is at least as large as half of the wire diameter.
    if( xhw >= wireDiam/2 && yhw >= wireDiam/2 ) clustXYHW_HWCut->Fill(xhw,yhw);
    
    
    

  }

  //Algebra
  meanXHW/=counterHW;
  meanYHW/=counterHW;


  //Third loop over clusters: used to apply calibration population
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){

    //Same cut on bad pixels
    if( CXBadPixelClusters(clusterVect[iClust]) ) continue;
    
    //Now identify if a cluster is more-or-less in the calibration sample
    double realArea = ((double)clusterVect[iClust].GetArea())/pixPerMm/pixPerMm;
    double xhw = clusterVect[iClust].GetHalfWidthsXY().first/pixPerMm;
    double yhw = clusterVect[iClust].GetHalfWidthsXY().second/pixPerMm;
    double gx = clusterVect[iClust].GetGlobalCenterXY().first;
    double gy = clusterVect[iClust].GetGlobalCenterXY().second;
  
    if( xhw > meanXHW && yhw > meanYHW && realArea > calibMean+2*calibSigma ){
      clustXYHW_calibCutXYOver->Fill(xhw,yhw);
      clustPositions_calibCutXYOver->Fill(gx,gy);
    }
    

  }
  */


  

  //Finding the calibration population for a rectilinear grid. The way we're going to do this is
  //by looking for all bins in X, Y that have a bin occupancy greater than the expected # of crossings for
  //a square grid. Note that this only works for 5mm grids so far.
  int expectedNumCrossings = 12;
  std::vector<double> crossVectX;
  std::vector<double> crossVectY;
  for( int iBin = 1; iBin <= clustPositionsX->GetNbinsX(); ++iBin ){
    if( clustPositionsX->GetBinContent(iBin) > (double)expectedNumCrossings ){
      crossVectX.push_back(clustPositionsX->GetBinCenter(iBin));
      std::cout << "Position of wire: " << clustPositionsX->GetBinCenter(iBin) << std::endl;
    }
  }
  for( int iBin = 1; iBin <= clustPositionsY->GetNbinsX(); ++iBin ){
    if( clustPositionsY->GetBinContent(iBin) > (double)expectedNumCrossings ){
      crossVectY.push_back(clustPositionsY->GetBinCenter(iBin));
      std::cout << "Position of wire: " << clustPositionsY->GetBinCenter(iBin) << std::endl;
    }
  }
  
  
  
  
  
  



      



  
  

  
  





  //Save
  clustPositions->Write();
  clustPositionsX->Write();
  clustPositionsY->Write();
  clustAreas->Write();
  clust2DSize->Write();
  clustAreasReal->Write();
  clustIntegral->Write();
  clustAvgBrightness->Write();
  clustAvgBrightnessVArea->Write();
  clustIntegralVArea->Write();
  clustRectangularity->Write();
  clustAvgBrightnessVRectangularity->Write();
  clustRealAreaVRectangularity->Write();
  clustMaxSizePos->Write();
  clustMaxPixelVal->Write();
  clustSigmaPixelVal->Write();
  clustMaxVSigmaPixelVal->Write();
  clustSigmaVAvgPixelVal->Write();
  clustMaxVAvgPixelVal->Write();
  clustOGIntegral->Write();
  clustOGAvgBrightness->Write();
  clustOGAvgBrightnessVsArea->Write();
  clustOGIntegralVsArea->Write();
  clustOGAvgBrightnessVsRectangularity->Write();
  clustOGAvgBrightnessVsAvgBrightness->Write();
  edgeArtifactAreas->Write();  
  edgeArtifactLocalPositions->Write();
  clustSizeVsMaxPixelVal->Write();
  clustOGIntegralVsIntegral->Write();
  clustOGMaxPixelVsMaxPixel->Write();
  clustOGSigmaPixelVsSigmaPixel->Write();
  clustAvgBrightnessVsMaxDim->Write();
  clustIntegralVsAvgBrightness->Write();
  fitCor_clustIntegral->Write();
  postCleanlinessCut_clusterXY->Write();

  /*
  clustXYHW_calibCut->Write();
  clustXYHW_calibCutOver->Write();
  clustXYHW_HWCut->Write();
  clustXYHW_calibCutXYOver->Write();
  clustPositions_calibCutXYOver->Write();
  */
}


//This takes the clusters returned from the clustering and 
std::vector<RecoWire> reconstructWires( std::vector<Cluster> clusters, TH2F* houghSpace )
{
  //Output vector
  std::vector<RecoWire> output;
  
  //  Loop through the clusters and identify the hit pixels
  for( int iC = 0; iC < clusters.size(); ++iC ){
    std::vector<std::pair<double,std::pair<int,int> > > hitPix = clusters[iC].GetHitPixels();
    
    //Take the weighted average
    double averageRho = 0;
    double averageTheta = 0;
    double counter = 0;

    //Loop over pixels
    for( int iP = 0; iP < hitPix.size(); ++iP ){

      //For each dimension, convert bin to value
      double rho = houghSpace->GetYaxis()->GetBinCenter(hitPix[iP].second.second);
      double theta = houghSpace->GetXaxis()->GetBinCenter(hitPix[iP].second.first);

      averageRho += rho*hitPix[iP].first;
      averageTheta += theta*hitPix[iP].first;
      counter += hitPix[iP].first;
    }
    
    averageRho /= counter;
    averageTheta /= counter;
    
    //Create a wire
    RecoWire thisWire;
    thisWire.nPix = hitPix.size();
    
    //May need to swap?. If average theta is less than -1, it's when y is constant.
    if( averageTheta < -1 ){ 
      thisWire.XorY=1;
    }
    else{
      thisWire.XorY=0;
    }
    thisWire.theta = averageTheta;
    thisWire.rho = averageRho;
    output.push_back(thisWire);
  }

  return output;

}


/*
//Merge wires if they accidentally got split, or if there are multiple wires
//too close to each other in rho.
x std::vector<RecoWire> mergeWires( std::vector<RecoWire> wires )
{
  std::vector<RecoWire> output;
  
  //Brute forcing this is fine because there are only like 1200 wires in the largest grid
  for( int iWire1 = 0; iWire1 < wires.size(); ++iWire1 ){
    for( int iWire2 = 0; iWire2 < wires.size(); ++iWire2 ){
      
      //If they're on opposite sides of -1, then just return. We don't care about comparing them.
      if( wires[iWire1].theta < -1 && wires[iWire2].theta > -1 ) continue;

      //Now compare
      if( fabs(wires[iWire1].rho-wires[iWire2].rho) < 1 ) 
      
    }
  }



}
*/






//Run wire metrology using the hough transform method.
void RunMetrology()
{
  std::vector<Cluster> clusterVect = theGlobalGrid.GetClusters();
  ParameterSet pSet = theGlobalGrid.GetParameterSet();

  //Loop over clusters to identify the extreme positions of clusters
  double epsilon = 10;
  double minX = 999999;
  double maxX = -999999;
  double minY = 999999;
  double maxY = -999999;
  for( int iClust = 0; iClust < clusterVect.size(); ++iClust ){
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    if( clustXY.first < minX ) minX = clustXY.first;
    if( clustXY.first > maxX ) maxX = clustXY.first;
    if( clustXY.second < minY ) minY = clustXY.second;
    if( clustXY.second > maxY ) maxY = clustXY.second;
  }



  //Get parameter set info
  //ParameterSet pSet = theGlobalGrid.GetParameterSet();
  //double pixPerMm = pSet.GetPixelsPerMm()/1000.; //Really now pixels per micron


  //****Hough Line Finder****
  
  //Loop Through The Clusters to Identify the Boundaries
  int totalClusters = clusterVect.size();
  double epsilon2 = 1/1000;
  double minp = 999999;
  double maxp = -999999;
  double p;

  
  int thetaBins = 3000;
  int rhoBins = 9000;
  int XYBins = 1000;
  double lowerBound = -2;
  double upperBound = 0.5;
  double increment = (upperBound - lowerBound)/thetaBins;

  
  //To Loop Through ALL Clusters                                                                       
  std::cout << "First round of looping over " << clusterVect.size() << " clusters in hough transform" << std::endl;
  for(int iClust = 0; iClust < totalClusters; iClust++) {
    
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    double positionX = clustXY.first;
    double positionY = clustXY.second;
    
    //To Loop Through ONE Cluster                 
    for (double i = lowerBound; i < upperBound; i += increment) {
      p = positionX * cos(i) + positionY * sin(i);
      if (p < minp) minp = p;
      if (p > maxp) maxp = p;
    }
  }

  
  
  //Draw Histogram
  TH2F * houghLineFinder =  new TH2F("houghLineFinder","Rho-theta space for this grid", thetaBins, lowerBound-epsilon2, upperBound+epsilon2, rhoBins ,minp-epsilon2, maxp+epsilon2 );
  
  //To Loop Through ALL Clusters                                                                           
  std::cout << "Second round of looping over " << clusterVect.size() << " clusters in hough transform." << std::endl;
  double peakHeight = 0;
  for(int iClust = 0; iClust < totalClusters; iClust++) {
    std::pair<double,double> clustXY = clusterVect[iClust].GetGlobalCenterXY();
    double positionX = clustXY.first;
    double positionY = clustXY.second;
    
    //To Loop Through ONE Cluster                                                                          
    for (double i = lowerBound; i < upperBound; i += increment) {
      p = positionX * cos(i) + positionY * sin(i);
      houghLineFinder->Fill(i,p); 
      int binTheta = houghLineFinder->GetXaxis()->FindBin(i);
      int binRho = houghLineFinder->GetYaxis()->FindBin(p);
      if( houghLineFinder->GetBinContent(binTheta,binRho) > peakHeight ) peakHeight = houghLineFinder->GetBinContent(binTheta,binRho);
    }
  }

  houghLineFinder->GetXaxis()->SetTitle("Theta (radians)");
  houghLineFinder->GetYaxis()->SetTitle("Rho (mm)");
  houghLineFinder->Write();


  


  //Okay. Now we have hough space populated. Labib's method of finding the points is to
  //use weighted means. I'm going to try some clustering. I think if I format this right,
  //I can just use the DBCluster algorithm that I wrote a while back. First, we need
  //to identify a vector of hit pixels, which in this case is a vector of all of the
  //spots in the hough histogram that are above some threshold. So we start by thresholding
  //the histogram.
  double threshold = peakHeight*0.2;
  std::cout << "Sanity check. Hough threshold: " << threshold << std::endl;
  TH2F * houghSpaceThresholded = new TH2F("houghSpaceThresholded","Rho-theta space for this grid, thresholded; Theta [radians]; Rho [mm]",
					  thetaBins,
					  lowerBound-epsilon2,
					  upperBound+epsilon2,
					  rhoBins,
					  minp-epsilon2,
					  maxp+epsilon2 );
  
  //Loop through all bins in the houghlinefinder histogram and cut on threshold. We'll both fill a histogram and create a hitpixel
  //for this above-threshold point.
  //So as it turns out, a straight threshold cut isn't super useful, since the grid wires are of variable length. So I'll have to
  //do a threshold relative to the few bins above and below
  std::vector<std::pair<double,std::pair<int,int> > > houghPixelVect;
  std::cout << "Loop over hough line finder histogram." << std::endl;
  for( int iBinTheta = 0; iBinTheta < thetaBins; ++iBinTheta ){
    for( int iBinRho = 0; iBinRho < rhoBins; ++iBinRho ){

      /*
      //Take average of 2 bins above and below this bin. 3 for now, but hardcoded value is a test. Will need to be variable.
      double above = 0;
      double below = 0;
      int nBinsExtraOneSided = 6;
      for( int iAdj = 1; iAdj <= nBinsExtraOneSided; ++iAdj ){
	above+=houghLineFinder->GetBinContent(iBinTheta,iBinRho+iAdj);
	below+=houghLineFinder->GetBinContent(iBinTheta,iBinRho-iAdj);
      }
      above /= nBinsExtraOneSided;
      below /= nBinsExtraOneSided;
      double aboveBelowAvg = above + below/2;
      //Find average of these bins
      if( aboveBelowAvg == 0 ) continue;
      */
      //      if( iBinTheta == 1593 && iBinRho == 830 ){
      //	for( int iAdj = 0; iAdj < nBinsExtraOneSided; ++iAdj ){
      //	  std::cout << "aboveAndBelow is having " << houghLineFinder->GetBinContent(iBinTheta,iBinRho+iAdj
      //      std::cout << "ratio: " << houghLineFinder->GetBinContent(iBinTheta,iBinRho)/aboveBelowAvg << std::endl;
      
      
      //Base the cut on the ratio between the central bin and the average of the ones above and below
      //if( houghLineFinder->GetBinContent(iBinTheta,iBinRho)/aboveBelowAvg > 5 ){// &&
	  //	  (above/below) < 1.6666 &&
	  // (above/below ) > 0.6 ){
      if( houghLineFinder->GetBinContent(iBinTheta,iBinRho) > threshold ){
	std::cout << "Haha! Ratio good. Bin: " << iBinTheta << ", " << iBinRho << std::endl;
	houghSpaceThresholded->SetBinContent(iBinTheta,iBinRho,houghLineFinder->GetBinContent(iBinTheta,iBinRho));
	double val = houghSpaceThresholded->GetBinContent(iBinTheta,iBinRho);
	std::pair<int,int> houghPixelXY(iBinTheta,iBinRho);
	std::pair<double,std::pair<int,int> > houghPixel(val,houghPixelXY);
	houghPixelVect.push_back(houghPixel);
      }
    }
  }
  houghSpaceThresholded->Write();
  
  //Now we have a vector of hit pixels (hough pixels). Pass into the db cluster algorithm.
  //THESE CLUSTERS ARE NOT IN REAL SPACE. THEY ARE IN HOUGH SPACE.
  std::cout << "Clustering the hough space hits." << std::endl;
  std::vector<Cluster> clusters = ClusterTheHits(houghPixelVect,pSet);

  //First, go through and show the clusters
  TH2F * wireHoughClusters = new TH2F("wireHoughClusters","Clusters representing wires in hough space",
				      thetaBins,
				      lowerBound-epsilon2,
				      upperBound+epsilon2,
				      rhoBins,
				      minp-epsilon2,
				      maxp+epsilon2 );
  
  //Now loop through all of the clusters and fill this histo
  for( int iC = 0; iC < clusters.size(); ++iC ){
    std::vector<std::pair<double,std::pair<int,int> > > clust = clusters[iC].GetHitPixels();
    for( int iP = 0; iP < clust.size(); ++iP ){
      double x = wireHoughClusters->GetXaxis()->GetBinCenter(clust[iP].second.first);
      double y = wireHoughClusters->GetYaxis()->GetBinCenter(clust[iP].second.second);
      wireHoughClusters->Fill(x,y,iC+1);
    }
  }
  wireHoughClusters->Write();
  
  
  //Now we have a vector of very barebones clusters in hough space. All that is in them is a vector of hit pixels.
  //Now what we want to do is loop through those hit pixels and find the max? Weighted mean? Not sure yet. I guess we'll see.
  std::vector<RecoWire> wires = reconstructWires( clusters, wireHoughClusters );
  
  //Plot sizes of wires and their thetas
  TH1F * wireSizes = new TH1F("wireSizes","Wire Sizes, in bins; Wire cluster size [bins]; Counts",100,0,200 );
  TH1F * wireThetasX = new TH1F("wireThetasX","Wire Theta, X-constant wires; Theta [radians]; Counts",100,-0.05,0.05);
  TH1F * wireThetasY = new TH1F("wireThetasY","Wire Theta, Y-constant wires; Theta [radians]; Counts",100,-1.62,-1.52);
  for( int iWire = 0; iWire < wires.size(); ++iWire ){
    wireSizes->Fill(wires[iWire].nPix);
    if( wires[iWire].XorY==0) wireThetasX->Fill(wires[iWire].theta);
    if( wires[iWire].XorY==1) wireThetasY->Fill(wires[iWire].theta);
  }
  wireSizes->Write();
  wireThetasX->Write();
  wireThetasY->Write();


  //Some wires get split on accident. Merge them if necessary.
  //  std::vector<RecoWire> correctedWires = mergeWires( wires );
  
  //Now plot these wires overtop of the clusters
  TH2F * clusterPositions = new TH2F("clusterPositions","ClusterPositions in Real Space",XYBins,minX,maxX,XYBins,minY,maxY);
  for( int iC = 0; iC < clusterVect.size(); ++iC ){
    std::pair<double,double> clustXY = clusterVect[iC].GetGlobalCenterXY();
    clusterPositions->Fill(clustXY.first,clustXY.second,1);
  }

  //Draw
  TCanvas * c1 = new TCanvas();
  clusterPositions->Draw();
  
  //Now make a shitload of TF1s to represent the wires
  std::vector<TLine*> lineVect;
  for( int iWire = 0; iWire < wires.size(); ++iWire ){
    double a = 1;
    double b = 2;
    double c = 3;
    double d = 4;
    if( wires[iWire].XorY == 0 ){
      a = wires[iWire].rho;
      b = wires[iWire].rho;
      c = minY;
      d = maxY;
    }
    else if( wires[iWire].XorY == 1 ){
      a = minX;
      b = maxX;

      double y0 = wires[iWire].rho/cos(-1*wires[iWire].theta-TMath::Pi()/2.);
      double y1 = y0+tan(wires[iWire].theta+TMath::Pi()/2.)*minX;
      double y2 = y0+tan(wires[iWire].theta+TMath::Pi()/2.)*maxX;
      

      c = y1;
      d = y2;
    }
    else{ std::cout << "Shouldn't get here... Wire line drawing." << std::endl; }
    std::cout << "minX: " << minX << ", maxX: " << maxX << ", minY: " << minY << ", maxY: " << maxY << ", rho: " << wires[iWire].rho << ", theta: " << wires[iWire].theta << ", a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << std::endl;
    TLine * line = new TLine(a,c,b,d);
    line->SetLineColor(kRed);
    line->Draw("same");
    lineVect.push_back(line);

  }
  
  c1->Update();
  c1->Write();
  
}


//Compute the maximum pixel value from the hitpix
double computeMaxPixelVal( Cluster clu )
{
  double output = 0;
  for( int iHP = 0; iHP < clu.GetHitPixels().size(); ++iHP ){
    double thePixelVal = clu.GetHitPixels()[iHP].first;
    if( thePixelVal > output ) output = thePixelVal;
  }
  return output;
}

//Compute maximum original pixel val (i.e. non-IFT)
double computeOGMaxPixelVal( Cluster clu )
{
  double output = 0;
  for( int iHP = 0; iHP < clu.GetOGHitPixels().size(); ++iHP ){
    double thePixelVal = clu.GetOGHitPixels()[iHP].first;
    if( thePixelVal > output ) output = thePixelVal;
  }
  return output;
}

//Compute the sigma in the pixel values from the hitpix
double computeSigmaPixelVal( Cluster clu )
{
  double avg = 0;
  double avg2 = 0;
  for( int iHP = 0; iHP < clu.GetHitPixels().size(); ++iHP ){
    double thePixelVal = clu.GetHitPixels()[iHP].first;
    avg += thePixelVal;
    avg2 += thePixelVal*thePixelVal;
  }
  avg/=clu.GetHitPixels().size();
  avg2/=clu.GetHitPixels().size();
  double sigma = pow(avg2 - avg*avg,0.5);
  return sigma;
}

//Compute the original (i.e. non-IFT) sigma in the pixel values from the hitpix
double computeOGSigmaPixelVal( Cluster clu )
{
  double avg = 0;
  double avg2 = 0;
  for( int iHP = 0; iHP < clu.GetOGHitPixels().size(); ++iHP ){
    double thePixelVal = clu.GetOGHitPixels()[iHP].first;
    avg += thePixelVal;
    avg2 += thePixelVal*thePixelVal;
  }
  avg/=clu.GetOGHitPixels().size();
  avg2/=clu.GetOGHitPixels().size();
  double sigma = pow(avg2 - avg*avg,0.5);
  return sigma;
}



//This cuts clusters based on whether they are known to be from bad pixels
bool CXBadPixelClusters( const Cluster clu )
{
  //Get the OgHitPixels
  std::vector<std::pair<double,std::pair<int,int> > > hp = clu.GetOGHitPixels();
  
  //Loop over them and identify if they are bad (definition of "bad" comes from inspection)
  bool isBad1 = true;
  for( int iP = 0; iP < hp.size(); ++iP ){
    
    int x = hp[iP].second.first;
    int y = hp[iP].second.second;
    if( x != 0 && x != 1 ) isBad1 = false;
    if( y != 109 && y != 110 ) isBad1 = false;
    
  }

  bool isBad2 = true;
  for( int iP = 0; iP < hp.size(); ++iP ){
    int x = hp[iP].second.first;
    int y = hp[iP].second.second;
    if( x != 1008 && x != 1009 ) isBad2 = false;
    if( y != 71 && y != 72 ) isBad2 = false;
  }
  
  if( isBad1 || isBad2 ) return true;
  return false;
}	  
  
					  
  










