///////////////////////////////////////////////////////////////
//
// Picture.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the Picture object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

//MineSweeper includes
#include "Picture.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(Picture)


//Constructor
Picture::Picture()
{
}

Picture::~Picture()
{
}

//Set function for center (crosshairs) point of the picture
//in the coordinate system of the loom used during that picture
//taking run.
void Picture::SetCenterXY(double x, double y)
{
  f_CenterXY.first = x;
  f_CenterXY.second = y;
}


//Sets the name of this picture.
void Picture::SetPicName( std::string name )
{
  f_Name = name;
}


//Takes a particle and pushes it back into the picture's particle vector
void Picture::AddParticle( Particle theParticle )
{
  f_Particles.push_back(theParticle);
}

//Takes a particle and pushes it back into the picture's particle vector
void Picture::AddCluster( Cluster theCluster )
{
  f_Clusters.push_back(theCluster);
}


//Returns the center
std::pair<double,double> Picture::GetCenterXY() const
{
  return f_CenterXY;
}

//Returns the name
std::string Picture::GetPicName() const
{
  return f_Name;
}


//Returns the particle list
std::vector<Particle> Picture::GetParticles() const
{
  return f_Particles;
}

//Returns the cluster list
std::vector<Cluster> Picture::GetClusters() const
{
  return f_Clusters;
}
