///////////////////////////////////////////////////////////////
//
// Particle.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the Particle object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

//MineSweeper includes
#include "Particle.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(Particle)


//Constructor
Particle::Particle()
{
}

//Destructor
Particle::~Particle()
{
}

//Set Functions
void Particle::SetHalfWidthsXY( int hx, int hy )
{
  f_HalfWidthsXY.first = hx;
  f_HalfWidthsXY.second = hy;
}

void Particle::SetGlobalCenterXY( double x, double y )
{
  f_GlobalCenterXY.first = x;
  f_GlobalCenterXY.second = y;
}

//Get Functions
std::pair<int,int> Particle::GetHalfWidthsXY()
{
  return f_HalfWidthsXY;
}

std::pair<double,double> Particle::GetGlobalCenterXY()
{
  return f_GlobalCenterXY;
}

std::pair<int,int> Particle::GetLocalCenterXY()
{
  return f_LocalCenterXY;
}


