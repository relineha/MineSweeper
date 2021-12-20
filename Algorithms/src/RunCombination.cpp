///////////////////////////////////////////////////////////////
//
// RunCombination.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/31/2018
//
// This file wraps up the information that MineSweeper collects
// and puts it into a grid object for later analysis.
// 
// Do not tamper with this file!
//
///////////////////////////////////////////////////////////////

//Standard includes
#include <iostream>

//MineSweeper Includes
#include "Picture.hh"
#include "Particle.hh"
#include "ParameterSet.hh"
#include "Grid.hh"
#include "Geometry.hh"
#include "Cluster.hh"

Grid RunCombination( const ParameterSet& pSet,
		     const Geometry & geo,
		     const std::vector<Picture> & picVect,
		     const std::vector<Particle> & particleVect,
		     const std::vector<Cluster> & clusterVect )

{
  //Define output grid
  Grid outputGrid;
  
  //Set the parameter set
  outputGrid.SetParameterSet(pSet);

  //Set the geometry
  outputGrid.SetGeometry(geo);

  //Set the picture vect
  for( size_t iC = 0; iC < picVect.size(); ++iC ){
    outputGrid.AddPicture(picVect[iC]);
  }

  //Set the particle vect
  for( size_t iC = 0; iC < particleVect.size(); ++iC ){
    outputGrid.AddParticle(particleVect[iC]);
  }

  //Set the cluster vect
  for( size_t iC = 0; iC < clusterVect.size(); ++iC ){
    outputGrid.AddCluster(clusterVect[iC]);
  }

  //Return the grid
  return outputGrid;
}

//If we get a big enough grid, it's hard to write all of the output to it. So we need to split the
//grid up into sub grids and export them as individual files. This is kind of kludgy, but meh.
std::vector<Grid> SplitTheGrid( const ParameterSet & pSet, 
				const Grid & theGrid)
{

  //Create output
  std::vector<Grid> output;

  //Now, get the max number of clusters per output file
  int maxClusters = pSet.GetMaxClustersPerProdFile();

  //Next, create a grid with all of the same properties as the argument grid, except for the
  //cluster vect. It will contain all pictures, particles, etc, for no reason other than that
  //it's easy. 
  Grid newGrid;

  //Loop over clusters in the grid
  int counter = 0;
  std::vector<Cluster> clusts = theGrid.GetClusters();
  for( int iClu = 0; iClu < clusts.size(); ++iClu ){
    counter++;
    newGrid.AddCluster(clusts[iClu]);
    
    
    //On the first cluster and every time that the counter passes the max number of clusters per sub grid,
    //create a new grid and set its common values (the pictures, geo, etc.)
    if( counter == maxClusters ){
      newGrid.SetParameterSet(pSet);
      newGrid.SetGeometry(theGrid.GetGeometry());
      for( size_t iC = 0; iC < theGrid.GetPictures().size(); ++iC ) newGrid.AddPicture(theGrid.GetPictures()[iC]);
      for( size_t iC = 0; iC < theGrid.GetParticles().size(); ++iC ) newGrid.AddParticle(theGrid.GetParticles()[iC]);
      
      //Push the grid back into the output vector and reset both it and the counter.      
      output.push_back(newGrid);
      Grid newTempGrid;
      newGrid = newTempGrid;
      counter = 0;
    }
  }
  
  //Return the split grid
  return output;

}
