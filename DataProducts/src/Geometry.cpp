///////////////////////////////////////////////////////////////
//
// Geometry.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 8/16/2018
//
// This is the class implementation of the Geometry object
// used in Minesweeper. 
//
///////////////////////////////////////////////////////////////

//General includes
#include <iostream>

//MineSweeper includes
#include "Geometry.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(Geometry)

//Constructor
Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

//Set function for center of the grid in the global coordinate system (mm)
void Geometry::SetCenterXY( double x, double y )
{
  f_CenterXY.first = x;
  f_CenterXY.second = y;
}

//Set function for diameter of the grid
void Geometry::SetGridDiameter( double diam )
{
  f_GridDiameter = diam;
}

//Set function for the clocking mark's phi
void Geometry::SetClockPhi( double phi )
{
  f_ClockPhi = phi;
}

//Get functions
std::pair<double,double> Geometry::GetCenterXY() const
{
  return f_CenterXY;
}

double Geometry::GetGridDiameter() const
{
  return f_GridDiameter;
}

double Geometry::GetClockPhi() const
{
  return f_ClockPhi;
}
