////////////////////////////////////////////////////////
//
// ScanP2CorrelationAnalysis.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 8/16/2018
//
// This set of functions is meant to deal with
// correlating hotspots on the grid and hotspots of
// emission in Phase 2.
// 
//
/////////////////////////////////////////////////////////


//Standard includes
#include <iostream>

//MineSweeper includes
#include "Grid.hh"
#include "Cluster.hh"
#include "Picture.hh"
#include "ParameterSet.hh"
#include "Geometry.hh"

//ROOT includes
#include "TH2F.h"
#include "TFile.h"
#include "TMath.h"

//Global parameter set for shits and gigs
ParameterSet pSetGlobal;


//Struct for transformed clusters (clusters whose positions have
//been modified to reset the origin at the center of the coord system 
//and whose positions have been rotated into the P2 array's coord system)
struct transfCluster
{
  Cluster baseCluster;     //Original cluster (in loom coord sys)
  std::pair<double,double> newCoordSysXY; //Position in P2's coord system
};

//Struct for transformed pictures (pictures whose positions have been
//modified to reset the origin at the center of the coordinate system
//and whose positions have been rotated into the P2 array's coordinate system)
struct transfPicture
{
  Picture basePicture;                    //Original picture
  std::pair<double,double> newCoordSysXY; //Position in P2's coord system
};





//////////////////// CUT DEFINITIONS //////////////////////
bool CXClusterArea( const Cluster & clust )
{
  if( clust.GetArea()/pow(pSetGlobal.GetPixelsPerMm(),2)*1000*1000 > 15000 ) return true;
  return false;
}

bool CXClusterIntegral( const Cluster & clust )
{
  if( clust.GetIntegral() > 0 ) return true;
  return false;
}

bool CXClusterRectangularity( const Cluster & clust )
{
  std::pair<double,double> clustHWs = clust.GetHalfWidthsXY();
  double area = clustHWs.first*clustHWs.second*4.0;
  if( clust.GetArea()/area < 0.3 ) return true;
  return false;
}

bool CXClusterAvgBrightness( const Cluster & clust )
{
  if( clust.GetIntegral()/((double)clust.GetArea()) > 13000000 ) return true;
  return false;
}






//This transforms cluster positions into the P2 coordinate system. It first shifts the global origin at 
//the center of the grid, then rotates the clusters into their place in the P2 coordinate system
std::vector<transfCluster> createTransformedClusters( Grid grid, double phi )
{
  //Create output clusters
  std::vector<transfCluster> outputVect;

  //Get grid geometry information
  Geometry geo = grid.GetGeometry();
  double centerX = geo.GetCenterXY().first;
  double centerY = geo.GetCenterXY().second;
  std::cout << "Grid Center X: " << centerX << ", grid center y: " << centerY << std::endl;
  std::cout << "Grid diameter: " << geo.GetGridDiameter() << std::endl;
  
  //Loop over clusters in the grid
  std::vector<Cluster> baseClustVect = grid.GetClusters();
  for( int iC = 0; iC < baseClustVect.size(); ++iC ){
    
    //Get global position and shift it so that origin is at center of grid
    std::pair<double,double> basePosition = baseClustVect[iC].GetGlobalCenterXY();
    double shiftedX = basePosition.first - centerX;
    double shiftedY = basePosition.second - centerY;
    std::pair<double,double> tempShiftedXY;
    tempShiftedXY.first = shiftedX;
    tempShiftedXY.second = shiftedY;
    
    //Now rotate the position by phi, the number of degrees you have to rotate the coordinate system to take
    //the x axis of the loom into the X axis of the P2 array. This is an active transformation (alibi),
    //which acts on the points themselves, not the coordinate axes.
    double phiRad = phi*TMath::Pi()/180.;
    double rotatedX = cos(phiRad)*shiftedX - sin(phiRad)*shiftedY;
    double rotatedY = sin(phiRad)*shiftedX + cos(phiRad)*shiftedY;
    std::pair<double,double> rotatedXY;
    rotatedXY.first = rotatedX;
    rotatedXY.second = rotatedY;



    //Now make a transformed cluster with this info.
    transfCluster theTransfClust;
    theTransfClust.baseCluster = baseClustVect[iC];
    theTransfClust.newCoordSysXY = rotatedXY;

    outputVect.push_back(theTransfClust);
  }
  
  return outputVect;
      

}



//This runs pertinent cuts on the clusters and produces a vector of transformed clusters who pass
//that slew of cuts 
std::vector<transfCluster> selectClusters( std::vector<transfCluster> cVect )
{
  //Output vector
  std::vector<transfCluster> outVect;

  //loop over clusters
  for( int iC = 0; iC < cVect.size(); ++iC ){
    
    //Define cuts
    bool cxClusterArea = CXClusterArea( cVect[iC].baseCluster );
    bool cxClusterIntegral = CXClusterIntegral( cVect[iC].baseCluster );
    bool cxClusterRectangularity = CXClusterRectangularity( cVect[iC].baseCluster );
    bool cxClusterAvgBrightness = CXClusterAvgBrightness( cVect[iC].baseCluster );
    
    //Make cut
    if( cxClusterAvgBrightness ) outVect.push_back(cVect[iC]);
    
  }
  
  //Return passed clusters
  return outVect;

}

//This selects a number of pictures
std::vector<Picture> selectPictures( Grid theGrid )
{
  //The way we select now is by just making a list of picture names and
  //comparing them to the pics in the grid. Inelegant, but right now, it's fast.
  const int nPix = 15;
  std::string nameArr[nPix] = {"/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-24.8_204.6_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-161.2_390.6_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-545.6_-613.8_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-582.8_-102.3_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-582.8_213.9_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-768.8_-427.8_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-905.2_-46.5_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-905.2_418.5_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-1116_213.9_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-458.8_213.9_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-719.2_381.3_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-533.2_-93_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-967.2_18.6_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-719.2_381.3_.bmp",
			       "/Users/ryanlinehan/LZ_Local_Work/Grids/TestMicroscopePix/UV/CathodePrototypePostPhase2/ZZZGridPoint_-1128.4_46.5_.bmp" };


			       
			      
  
  //Loop over all pictures
  std::vector<Picture> outVect;
  std::vector<Picture> pV = theGrid.GetPictures();
  for( int iSp = 0; iSp < nPix; ++iSp ){
    for( int iP = 0; iP < pV.size(); ++iP ){
      if( pV[iP].GetPicName() == nameArr[iSp] ){
	outVect.push_back(pV[iP]);
	break;
      }
    }
  }
  
  //Return the selected pictures
  return outVect;
}

//This transforms the selected pictures into the coordinate system of the P2 vessel. It creates objects of type
//transfPicture
std::vector<transfPicture> transformPictures( std::vector<Picture> pix, Grid grid, double phi )
{
  //Create output clusters
  std::vector<transfPicture> outputVect;
  
  //Get grid geometry information
  Geometry geo = grid.GetGeometry();
  double centerX = geo.GetCenterXY().first;
  double centerY = geo.GetCenterXY().second;
  
  //Loop over pictures in the selected picture vect
  for( int iP = 0; iP < pix.size(); ++iP ){

    //Get global position and shift it so that origin is at center of grid
    std::pair<double,double> basePosition = pix[iP].GetCenterXY();
    double shiftedX = basePosition.first - centerX;
    double shiftedY = basePosition.second - centerY;
    std::pair<double,double> tempShiftedXY;
    tempShiftedXY.first = shiftedX;
    tempShiftedXY.second = shiftedY;
    
    //Now rotate the position by phi, the number of degrees you have to rotate the coordinate system to take
    //the x axis of the loom into the X axis of the P2 array. This is an active transformation (alibi),
    //which acts on the points themselves, not the coordinate axes.
    double phiRad = phi*TMath::Pi()/180.;
    double rotatedX = cos(phiRad)*shiftedX - sin(phiRad)*shiftedY;
    double rotatedY = sin(phiRad)*shiftedX + cos(phiRad)*shiftedY;
    std::pair<double,double> rotatedXY;
    rotatedXY.first = rotatedX;
    rotatedXY.second = rotatedY;



    //Now make a transformed picture with this info.
    transfPicture theTransfPic;
    theTransfPic.basePicture = pix[iP];
    theTransfPic.newCoordSysXY = rotatedXY;

    outputVect.push_back(theTransfPic);
  }
  return outputVect;
}

//This is a comparison between the cathode prototype's first scan and its performance in Phase 2.
//There may not be as many correlations here because of the dust exposure during the deflection test.
void AnalyzeCorrelations_CathodePrototype_PrePhase2( Grid theGrid, TFile * theP2File )
{

  //Parameter set setting
  pSetGlobal = theGrid.GetParameterSet();


  ///////////////////////////////////////////
  //  COORDINATE SYSTEM SETTING: CLUSTERS  //
  ///////////////////////////////////////////
  
  //We have two coordinate systems. One is the grid on the loom and the other is the grid in P2.
  //We want to find the relationship between the two coordinate axes. To do this, we need
  //to find by how much the coordinate system on the loom is rotated relative to the coordinate
  //system in P2.

  //First step: find how much the X axis of the grid on the loom is rotated with respect to the clock point
  //Second step: find how much the clock point is rotated relative to the high voltage point
  //Third step: find how much the HV point is rotated relative to the trapdoor
  //Fourth step: find how much the trapdoor is rotated relative to the X axis of the P2 setup
  //Then rotate.

  //Convention: A_B = A minus B, 
  //  double dPhi_loomX_clockPoint = 0; REAL
  double dPhi_loomX_clockPoint = 0.01;
  double dPhi_clockPoint_hvPoint = -307.5; //Have to rotate 307.5 degrees counterclockwise to take clockpoint to hvpoint (to take x axis of clockpoint to x axis of hvpoint)
  double dPhi_hvPoint_trapdoorPoint = -150.; //Have to rotate 150 degrees counterclockwise ...
  double dPhi_trapdoorPoint_p2Xaxis = -264.4; //...
  
  double dPhi_loomX_p2Xaxis = -1*(dPhi_loomX_clockPoint + dPhi_clockPoint_hvPoint + dPhi_hvPoint_trapdoorPoint + dPhi_trapdoorPoint_p2Xaxis);
  
  //We have our rotation angle. Now we get the clusters from the grid and "modify" their positions so that the global origin is at
  //the center of the grid
  std::vector<transfCluster> transformedClusters = createTransformedClusters( theGrid, dPhi_loomX_p2Xaxis );

  //Now what we want to do is to make cuts on the transformed clusters based on a number of variables. 
  std::vector<transfCluster> selectedClusters = selectClusters( transformedClusters );



  ///////////////////////////////////////////
  //  COORDINATE SYSTEM SETTING: PICTURES  //
  ///////////////////////////////////////////
  
  //Select pictures in the original coordinate system that have features in them. Features arbitrary for now.
  std::vector<Picture> selectedPictures = selectPictures( theGrid );
  
  //Now transform those pictures so that their coordinates are now within the P2 cordinate system.
  std::vector<transfPicture> transformedSelectedPictures = transformPictures( selectedPictures, theGrid, dPhi_loomX_p2Xaxis );




 
  

  

  /////////////////////////////////
  // SCAN DATA HISTOGRAM MAKING  //
  /////////////////////////////////

  //Now we make a bunch of histograms using the clusters we've made
  //Identify bins and extrema
  int nBinsX = 100;
  int nBinsY = 100;
  double maxX = 800./10.;
  double minX = -800./10.;
  double maxY = 800./10.;
  double minY = -800./10.;
  double dX = (maxX-minX)/nBinsX;
  double dY = (maxY-minY)/nBinsY;
  double dA = dX*dY;
  double cX = theGrid.GetGeometry().GetCenterXY().first;
  double cY = theGrid.GetGeometry().GetCenterXY().second;
  

  char title[150];
  sprintf(title,"# of clusters per %f cm^2;X [cm];Y [cm]",dA);
  TH2F * h_clustDensity_all = new TH2F("h_clustDensity_cut",title,nBinsX,minX,maxX,nBinsY,minY,maxY);
  TH2F * h_clustDensity_all_noTrans = new TH2F("h_clustDensity_cut_noTrans",title,nBinsX,minX*10.+cX,maxX*10.+cX,nBinsY,minY*10+cY,maxY*10+cY);
  TH2F * h_XYPositions_hotPics_noTrans = new TH2F("h_XYPositions_hotPics_noTrans","Positions of pictures with fibers (loom coordsys);X [cm];Y [cm]",nBinsX,minX*10.+cX,maxX*10.+cX,nBinsY,minY*10+cY,maxY*10+cY);
  TH2F * h_XYPositions_hotPics = new TH2F("h_XYPositions_hotPics","Positions of pictures with fibers (P2 coordsys);X [cm];Y [cm]",nBinsX,minX,maxX,nBinsY,minY,maxY);

  for( int iC = 0; iC < selectedClusters.size(); ++iC ){
    double x = selectedClusters[iC].newCoordSysXY.first/10.;
    double y = selectedClusters[iC].newCoordSysXY.second/10.;
    h_clustDensity_all->Fill(x,y);
    h_clustDensity_all_noTrans->Fill(x*10.+cX,y*10.+cY);
  }

  for( int iP = 0; iP < transformedSelectedPictures.size(); ++iP ){
    double x = transformedSelectedPictures[iP].newCoordSysXY.first/10;
    double y = transformedSelectedPictures[iP].newCoordSysXY.second/10;
    h_XYPositions_hotPics->Fill(x,y);
    h_XYPositions_hotPics_noTrans->Fill(x*10.+cX,y*10.+cY);
  }    
  






  
  //////////////////////////////////
  // P2 DATA HISTOGRAM EXTRACTION //
  //////////////////////////////////
  
  //Read in the Phase 2 data file and extract the centroid histos  
  TH2F * p2RateXY_17kV = (TH2F*)theP2File->Get("P2C9P8kV/h_rate_all");
  TH2F * p2RateXY_16kV = (TH2F*)theP2File->Get("P2C8P8kV/h_rate_all");
  TH2F * p2RateXY_14kV = (TH2F*)theP2File->Get("P2C7P7kV/h_rate_all");
  TH2F * p2RateXY_12kV = (TH2F*)theP2File->Get("P2C6P6kV/h_rate_all");
  TH2F * p2RateXY_10kV = (TH2F*)theP2File->Get("P2C5P5kV/h_rate_all");
  TH2F * p2RateXY_8kV = (TH2F*)theP2File->Get("P2C4P4kV/h_rate_all");
  TH2F * p2RateXY_6kV = (TH2F*)theP2File->Get("P2C3P3kV/h_rate_all");
  TH2F * p2RateXY_4kV = (TH2F*)theP2File->Get("P2C2P2kV/h_rate_all");
  TH2F * p2RateXY_0kV = (TH2F*)theP2File->Get("0V/h_rate_all");
  TH2F * p2PMTPositions = (TH2F*)theP2File->Get("pmtPositions");
  p2PMTPositions->SetStats(false);
  p2RateXY_16kV->SetStats(false);
  p2RateXY_14kV->SetStats(false);


  p2RateXY_17kV->SetName("rate_17kV");
  p2RateXY_16kV->SetName("rate_16kV");
  p2RateXY_14kV->SetName("rate_14kV");
  p2RateXY_12kV->SetName("rate_12kV");
  p2RateXY_10kV->SetName("rate_10kV");
  p2RateXY_8kV->SetName("rate_8kV");
  p2RateXY_6kV->SetName("rate_6kV");
  p2RateXY_4kV->SetName("rate_4kV");
  p2RateXY_0kV->SetName("rate_0kV");

  
  //Now subtract the two plots
  TH2F * p2RateXY_16minus0kV = new TH2F("p2RateXY_16minus0kV","All Pulse Rate Difference, 16kV - 0kV;X [cm];Y [cm]",p2RateXY_16kV->GetNbinsX(),minX,maxX,p2RateXY_16kV->GetNbinsY(),minY,maxY);
  for( int iBinX = 1; iBinX <= p2RateXY_16kV->GetNbinsX(); ++iBinX ){
    for( int iBinY = 1; iBinY <= p2RateXY_16kV->GetNbinsY(); ++iBinY ){
      double rate1 = p2RateXY_16kV->GetBinContent(iBinX,iBinY);
      double rate2 = p2RateXY_0kV->GetBinContent(iBinX,iBinY);
      if( rate1 == 0 && rate2 == 0 ) continue;
      p2RateXY_16minus0kV->SetBinContent(iBinX,iBinY,rate1-rate2);
    }
  }






  
  
  
  //////////////////////////////////
  // IDENTIFYING CORRELATIONS     //
  //////////////////////////////////
  
  //Use the centroid histos and the scan histos to plot correlations
  
  //Make output file (for everything)
  TFile * fout = new TFile("CorrelationsFile_CathodePrototype_PrePhase2_ana.root","RECREATE");

  
  //Save scan histos to output file
  h_clustDensity_all->Write();
  h_clustDensity_all_noTrans->Write();
  h_XYPositions_hotPics_noTrans->Write();
  h_XYPositions_hotPics->Write();


  //Save P2 histos to output file
  p2RateXY_17kV->Write();
  p2RateXY_16kV->Write();
  p2RateXY_14kV->Write();
  p2RateXY_12kV->Write();
  p2RateXY_10kV->Write();
  p2RateXY_8kV->Write();
  p2RateXY_6kV->Write();
  p2RateXY_4kV->Write();
  p2RateXY_0kV->Write();
  p2RateXY_16minus0kV->Write();
  p2PMTPositions->Write();



  //Save correlation histos to output file



  //Close file and save info
  fout->Close();




}





void AnalyzeCorrelations( const std::vector<Grid> & gridVect, const std::vector<TFile*> p2FileVect )
{
  //We're going to have to break this up a bit. Here, we look at more specific pairings
  AnalyzeCorrelations_CathodePrototype_PrePhase2(gridVect[0],p2FileVect[0]);
  
}
