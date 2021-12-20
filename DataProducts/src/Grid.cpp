///////////////////////////////////////////////////////////////
//
// Grid.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the Grid object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

//MineSweeper includes
#include "Grid.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(Grid)


//Constructor
Grid::Grid()
{
}

//Destructor
Grid::~Grid()
{
}


//Set functions

//Add pictures to the associated picture list
void Grid::AddPicture( Picture pic )
{
  f_Pictures.push_back(pic);
}

//Add a cluster to the associated cluster list
void Grid::AddCluster( Cluster cluster )
{
  f_Clusters.push_back(cluster);
}

//Add particles to the associated particle list
void Grid::AddParticle( Particle particle )
{
  f_Particles.push_back(particle);
}

//Set the parameter set
void Grid::SetParameterSet( ParameterSet pSet )
{
  f_ParameterSet = pSet;
}

//Set the geometry
void Grid::SetGeometry( Geometry geo )
{
  f_Geometry = geo;
}

//Get functions
std::vector<Picture> Grid::GetPictures() const
{
  return f_Pictures;
}

std::vector<Cluster> Grid::GetClusters() const
{
  return f_Clusters;
}

std::vector<Particle> Grid::GetParticles() const
{
  return f_Particles;
}

ParameterSet Grid::GetParameterSet() const
{
  return f_ParameterSet;
}

Geometry Grid::GetGeometry() const
{
  return f_Geometry;
}
