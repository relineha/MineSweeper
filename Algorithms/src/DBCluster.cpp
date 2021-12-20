///////////////////////////////////////////////////////////////
//
// DBCluster.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 4/9/2018
//
// This source file contains the algorithms for running a
// density-based clustering of hit pixels into a cluster
// object.
// 
//
///////////////////////////////////////////////////////////////


//Standard includes
#include <iostream>
#include <cmath>

//MineSweeper includes
#include "Cluster.hh"
#include "DBCluster.hh"
#include "ParameterSet.hh"

static ParameterSet pSetGlobal;

//The main function. This gets called to do clustering
std::vector<Cluster> ClusterTheHits(std::vector<std::pair<double,std::pair<int,int> > > hitPix, const ParameterSet pSet )
{
  //Set global pSet
  pSetGlobal = pSet;


  //Output vector of clusters
  std::vector<Cluster> output;
  
  //For kicks and gigs
  if( pSet.GetVerbosity() == 3 || pSet.GetVerbosity() == 5 ){
    std::cout << "Beginning DBCluster." << std::endl;
    std::cout << "--> We have to scan and cluster " << hitPix.size() << " points." << std::endl;
  }
  
  //Hokay. This is gonna suck. The algorithm is taken from Wikipedia's DBSCAN page's pseudocode

  //First, create a classification vector for each pixel and fill it. Classifications are:
  // 1. NOISE
  // 2. CLUSTERED
  // 3. UNDEFINED
  //All pixels start out undefined.
  std::vector<std::string> hitPixClass;
  for( int iHp = 0; iHp < hitPix.size(); ++iHp ){
    hitPixClass.push_back("UNDEFINED");
  }
  
  //Now loop again over the hit pixels to begin the actual algorithm.
  int clusterID = 0;
  for( int iHp = 0; iHp < hitPix.size(); ++iHp ){

    //Get this hit pixel.
    std::pair<double,std::pair<int,int> > theHitPixel = hitPix[iHp];
    
    //Check first to see if this pixel has been classified or not. If it has, then
    //we skip it. It's either noise or clustered.
    if( hitPixClass[iHp] != "UNDEFINED" ) continue;

    //Find the indices of its epsilon neighbors.
    std::vector<int> epsNeighbors = FindNeighbors(theHitPixel,hitPix);
    
    //If the group of its neighbors is smaller than a threshold, call this noise.
    if( epsNeighbors.size() < pSetGlobal.GetDBClusterMinPts() ){
      hitPixClass[iHp] = "NOISE";
      continue;
    }
    
    //Otherwise, we've found a new seed for a cluster. Create a cluster and push this hit pixel back into it.
    Cluster thisCluster;
    thisCluster.AddHitPixel(hitPix[iHp]);
    hitPixClass[iHp] = "CLUSTERED";

    
    //Now loop through all of the neighbors and add them to this cluster
    int allNeighbors = epsNeighbors.size();
    int iN = 0;
    while( iN < allNeighbors ){
      if( hitPixClass[epsNeighbors[iN]] == "NOISE" ){
	hitPixClass[epsNeighbors[iN]] = "CLUSTERED";
	thisCluster.AddHitPixel(hitPix[epsNeighbors[iN]]); //Add the RGB values of the hit pix to this cluster
      }
      if( hitPixClass[epsNeighbors[iN]] != "UNDEFINED" ){
	iN++;
	continue;
      }
      hitPixClass[epsNeighbors[iN]] = "CLUSTERED";
      thisCluster.AddHitPixel(hitPix[epsNeighbors[iN]]);
      
      //Now find neighbors of this pixel and do the same thing.
      std::vector<int> subEpsNeighbors = FindNeighbors(hitPix[epsNeighbors[iN]],hitPix);
      
      //Add these to the epsilon neighbors for further searching if a density check is passed
      //      if( subEpsNeighbors.size() >= pSetGlobal.GetDBClusterMinPts() ){ //This ends up systematically splitting hough line regions... Effectively, this says that if there are fewer than 3 epsilon neighbors of this point (which has been added to the cluster), then ignore the neighbors? Doesn't make sense...
      for( int iNeigh = 0; iNeigh < subEpsNeighbors.size(); ++iNeigh ){
	epsNeighbors.push_back(subEpsNeighbors[iNeigh]);
	allNeighbors++;
      }
      //      }
      
      iN++;
    }

    //Push back this cluster into the cluster list
    thisCluster.SetID(clusterID);
    output.push_back(thisCluster);
    clusterID++;
  }

  //Print out all hit pixel classifications
  if( pSet.GetVerbosity() == 3 || pSet.GetVerbosity() == 5 ){
    std::cout << "--> Reading out the hit classifications of all hits:" << std::endl;
    for( int iClust = 0; iClust < output.size(); ++iClust ){
      std::vector<std::pair<double,std::pair<int,int> > > hitsInCluster = output[iClust].GetHitPixels();
      for( int iP = 0; iP < hitsInCluster.size(); ++iP ){
	std::cout << "------> Cluster: " << output[iClust].GetID() << ", Hit X: " << hitsInCluster[iP].second.first << ", Y: " << hitPix[iP].second.second << ", class: " << hitPixClass[iP] << std::endl;
      }
    }
  }
  
  
  //return output
  if( pSet.GetVerbosity() == 3 || pSet.GetVerbosity() == 5 ){
    std::cout << "--> We found " << output.size() << " clusters." << std::endl;
    std::cout << "Finished DBCluster." << std::endl;
  }
  
  return output;
}


//This loops through all hit pixels and compares them to the current one to see if they are neighbors.
std::vector<int> FindNeighbors(std::pair<double,std::pair<int,int> > theHitPix, std::vector<std::pair<double,std::pair<int,int> > > hitPix )
{
  //Output vector: holds indices of neighbors
  std::vector<int> neighs;

  //Current coords of the target hit pix
  int x0 = theHitPix.second.first;
  int y0 = theHitPix.second.second;

  //Loop over all hit pix - this is very slow, and should be sped up.
  for( int iP = 0; iP < hitPix.size(); ++iP ){
    int x = hitPix[iP].second.first;
    int y = hitPix[iP].second.second;
    
    //Compare to the target hit pix. If it's within epsilon of this, then it's a neighbor
    double dist = pow(pow(x-x0,2)+pow(y-y0,2),0.5);
    if( dist <= pSetGlobal.GetDBClusterEpsilon() && dist != 0 ){
      neighs.push_back(iP);
    }
  }
  return neighs;
}
