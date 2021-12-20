///////////////////////////////////////////////////////////////
//
// DBCluster.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 4/9/2018
//
// This header file contains the declarations for the
// algorithms that run the density-based clustering of
// hit pixels into a cluster object.
// 
//
///////////////////////////////////////////////////////////////

#ifndef DBCLUSTER_HH
#define DBCLUSTER_HH

//Standard includes
#include <vector>
#include <utility>

//MineSweeper includes
#include "Cluster.hh"
#include "ParameterSet.hh"


std::vector<Cluster> ClusterTheHits(std::vector<std::pair<double,std::pair<int,int> > > hitPix, const ParameterSet pSet );
std::vector<int> FindNeighbors(std::pair<double,std::pair<int,int> > theHitPix, std::vector<std::pair<double,std::pair<int,int> > > hitPix );








#endif 
