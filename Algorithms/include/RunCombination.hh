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
// This is the function definitions for the stuff mentioned
// above.
// 
// Do not tamper with this file!
//
///////////////////////////////////////////////////////////////

#ifndef RUNCOMBINATION_HH
#define RUNCOMBINATION_HH

//Standard includes

//MineSweeper Includes
#include "ParameterSet.hh"
#include "Particle.hh"
#include "Cluster.hh"
#include "Picture.hh"
#include "Grid.hh"
#include "Geometry.hh"

Grid RunCombination( const ParameterSet& pSet,
		     const Geometry& geo,
		     const std::vector<Picture> & picVect,
		     const std::vector<Particle> & particleVect,
		     const std::vector<Cluster> & clusterVect );


std::vector<Grid> SplitTheGrid( const ParameterSet & pSet, 
				const Grid & theGrid);


#endif
