/////////////////////////////////////////////////////////
//
// Minesweeper.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the main file for running the Minesweeper
// software for dust and fiber identification on the LZ
// grids. 
//
/////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

//MineSweeper Includes

//Data Products
#include "Picture.hh"
#include "Particle.hh"
#include "Cluster.hh"
#include "ParameterSet.hh"
#include "Grid.hh"
#include "Geometry.hh"

//Algorithms
#include "RunSetup.hh"
#include "RunSweep.hh"
#include "RunCombination.hh"


//Analysis
#include "AnalysisFunctions.hh"
#include "ScanP2CorrelationAnalysis.hh"

//ROOT
#include "TFile.h"



int main()
{
  std::cout << "Test" << std::endl;


  //Run setup:
  // - parse config file with parameters, create/fill ParameterSet object
  // - parse the fileList of the pictures created
  // - Create a vector of Picture objects
  //   + Push back the picture objects with names
  //   + Extract the central position of the pictures from the names and save in the Picture objects
  ParameterSet pSet;
  Geometry geo;
  std::vector<Picture> pictureVect;
  runMineSweeperSetup( pSet, geo, pictureVect );



  ///////////////////////////////////////////////////////////////////////////////
  //This setting is for doing the cluster finding
  if( pSet.GetRunMode() == "PRODUCTION" ){
    //Run Particle finding
    // - Create vector of particles
    // - Loop over pictures
    //   + Run whatever particle finding algorithm we create on the picture
    //   + Save found particle position, size, etc. to the vector of particles. Also, save to the vector 
    //     of particles within the current picture (for good measure)
    //

    std::cout << "\n\n////////////////////////////////////////////" << std::endl;
    std::cout << "    RUNNING IN PRODUCTION MODE                " << std::endl;
    std::cout << "////////////////////////////////////////////" << std::endl;
    

    std::vector<Particle> particleVect;
    std::vector<Cluster> clusterVect;
    RunParticleFinding( pSet, pictureVect, particleVect, clusterVect);
    
    //Combining information
    // - Save ParameterSet object, vector of Pictures, and vector of Particles to the grid object.
    Grid theGrid = RunCombination( pSet, geo, pictureVect, particleVect, clusterVect );


    //Now I need to write my data to file. This is aa little more complicated now, because the
    //anode showed me that writing to a single root file may be intractible. So we may have to
    //split the grid up so that it writes across multiple root files, depending on cluster vect size.
    std::cout << "Writing grid object to file." << std::endl;
    if( pSet.GetWriteMode() == "MULTIFILE" ){
      std::vector<Grid> theSubGrids = SplitTheGrid(pSet,theGrid);
      for( int iSubGrid = 0; iSubGrid < theSubGrids.size(); ++iSubGrid ){
	char name[100];
	sprintf(name,"MineSweeperProductionOutputMultifile_%d.root",iSubGrid);
	TFile * f1 = new TFile(name,"RECREATE");
	theSubGrids[iSubGrid].Write();
	f1->Close();
      }
    }
    if( pSet.GetWriteMode() == "SINGLEFILE" ){
      TFile * f1 = new TFile("MineSweeperProductionOutput.root","RECREATE");
      theGrid.Write();
      f1->Close();
    }
  }



  ///////////////////////////////////////////////////////////////////////////////
  //This setting is for analyzing one grid scan that has already been run
  else if( pSet.GetRunMode() == "ANALYSIS" ){

    std::cout << "\n\n////////////////////////////////////////////" << std::endl;
    std::cout << "    RUNNING IN ANALYSIS MODE                " << std::endl;
    std::cout << "////////////////////////////////////////////" << std::endl;

    //The final grid object to be passed to analysis
    Grid * theFinalGrid = nullptr;
    TFile * f2;

    //Because we have two possible write modes, we're going to use different approaches 
    if( pSet.GetWriteMode() == "MULTIFILE" ){
      int nFiles = pSet.GetReadNumber(); //# of files to read in
      for( int iN = 0; iN < nFiles; ++iN ){
	char name[100];
	sprintf(name,"MineSweeperProductionOutputMultifile_%d.root",iN);
	f2 = new TFile(name,"READ");
	Grid * theSubGrid = (Grid*)f2->Get("Grid");

	//For the first sub-grid, pass the final grid everything.
	if( iN == 0 ){
	  theFinalGrid->SetGeometry(theSubGrid->GetGeometry());
	  theFinalGrid->SetParameterSet(theSubGrid->GetParameterSet());
	  for( int iP = 0; iP < theSubGrid->GetPictures().size(); ++iP ){
	    theFinalGrid->AddPicture(theSubGrid->GetPictures()[iP]);
	  }
	}

	//For all grids, add the clusters to the final grid
	for( int iC = 0; iC < theSubGrid->GetClusters().size(); ++iC ){
	  theFinalGrid->AddCluster(theSubGrid->GetClusters()[iC]);
	}
      }
    }
    else if( pSet.GetWriteMode() == "SINGLEFILE" ){
      //Open the root file
      f2 = new TFile("MineSweeperProductionOutput.root");
      theFinalGrid = (Grid*)f2->Get("Grid");
    }
    else{
      std::cout << "Write mode not recognized. No analysis being done." << std::endl;
      return -1;
    }



    //Run analysis scripts
    Analyze( *theFinalGrid );
    
    //Close the file
    f2->Close();
      
  }



  ///////////////////////////////////////////////////////////////////////////////
  //This setting is for looking at correlations between the scan data and Phase 2 emission data.
  //Because I don't know if only one grid scan is enough for this task, I leave open the potential
  //for looking at multiple grids.
  else if( pSet.GetRunMode() == "CORRELATION" ){

    std::cout << "\n\n////////////////////////////////////////////" << std::endl;
    std::cout << "    RUNNING IN CORRELATION MODE                " << std::endl;
    std::cout << "////////////////////////////////////////////" << std::endl;


    ////////// GRID SCANS /////////
    //Input1: Set of root files with grid scans in them
    const int nScans = 1;
    std::string gridFiles[nScans] = {"MineSweeperProductionOutput.root"};
    
    //Get the grids in a vector
    std::vector<Grid> gridVect;
    for( int iFile = 0; iFile < nScans; ++iFile ){
      TFile * theFile = new TFile((gridFiles[iFile]).c_str());
      Grid * theGrid = (Grid*)theFile->Get("Grid");
      gridVect.push_back(*theGrid);
      theFile->Close();
    }


    ////////// Phase 2 XY Emission Heatmaps /////////
    //Input2: Set of root files with XY emission heatmaps in them
    const int nHeatMaps = 1;
    std::string heatMapFiles[nHeatMaps] = {"/Users/ryanlinehan/LZ_Local_Work/Phase2Analysis/Phase2ScanP2CorrelationOutput.root"};
    //    std::string heatMapFiles[nHeatMaps] = {"/Users/ryanlinehan/LZ_Local_Work/Phase2Analysis/ScanP2CorrelationOutput.root"};
    
    //Get the files in a vector
    std::vector<TFile*> p2FileVect;
    for( int iFile = 0; iFile < nHeatMaps; ++iFile ){
      TFile * theFile = new TFile((heatMapFiles[iFile]).c_str());
      p2FileVect.push_back(theFile);
    }
    
    //Now plug these two vectors into the analysis function that handles
    //assessment of correlations
    AnalyzeCorrelations( gridVect, p2FileVect );

  }







  return 0;
}
