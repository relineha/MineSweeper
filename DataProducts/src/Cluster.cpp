///////////////////////////////////////////////////////////////
//
// Cluster.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the Cluster object
// used in Minesweeper. 
//
///////////////////////////////////////////////////////////////

//MineSweeper includes
#include "Cluster.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(Cluster)

//Constructor
Cluster::Cluster()
{
}

Cluster::~Cluster()
{
}

//Set function for center (crosshairs) point of the cluster
//in pixels (relative to the picture)
void Cluster::SetLocalCenterXY(int x, int y)
{
  f_LocalCenterXY.first = x;
  f_LocalCenterXY.second = y;
}

//Set function for the global center of the cluster in XY (relative to grid coordinate system)
void Cluster::SetGlobalCenterXY(double x, double y)
{
  f_GlobalCenterXY.first = x;
  f_GlobalCenterXY.second = y;
}

//Set function for the half widths of the clusters found (in pixels)
void Cluster::SetHalfWidthsXY(double hx, double hy )
{
  f_HalfWidthsXY.first = hx;
  f_HalfWidthsXY.second = hy;
}

//Set function for the type of the cluster
void Cluster::SetType( std::string typeName )
{
  f_Type = typeName;
}

//Set function for the cluster ID
void Cluster::SetID( int id )
{
  f_Id = id;
}

void Cluster::AddHitPixel( std::pair<double,std::pair<int,int> > pixel )
{
  f_HitPixels.push_back(pixel);
}

//Set the name of the picture within which this cluster was found
void Cluster::SetOwnerPicture( std::string picName )
{
  f_OwnerPicture = picName;
}

//Set the value of the cluster integral
void Cluster::SetIntegral( double val )
{
  f_Integral = val;
}

//Set the value of the cluster area
void Cluster::SetArea( int val )
{
  f_Area = val;
}

//Set the value of the cluster average brightness
void Cluster::SetAvgBrightness( double val )
{
  f_AvgBrightness = val;
}

//Add a hit pixel's RGB values
void Cluster::AddHitPixelRGB( std::vector<int> pixel )
{
  f_HitPixelsRGB.push_back(pixel);
}

//Set the OG hit pixels (original image hit pixels). This is done all at once because
//the pixels are basically copied from the normal hit pixels but with different pixel
//values, and I don't want any ability to set them separately/independently.
void Cluster::SetOGHitPixels( std::vector<std::pair<double,std::pair<int,int> > > ogHitPix )
{
  f_OgHitPixels = ogHitPix;
}

//Sets the integral of the cluster in units of the original image's pixel values. Differs from
//setIntegral because setIntegral is in units of the IFT's pixel values.
void Cluster::SetOGIntegral( double val )
{
  f_OgIntegral = val;
}

//Sets the average brightness of the cluster in units of the original image's pixel values. Differs
//from setAvgBrightness because avgBrightness is in terms of the IFT's pixel vals.
void Cluster::SetOGAvgBrightness( double val )
{
  f_OgAvgBrightness = val;
}






//Returns the local center (in pixels)
std::pair<int,int> Cluster::GetLocalCenterXY() const
{
  return f_LocalCenterXY;
}


//Returns the center in the global coordinate system (in mm)
std::pair<double,double> Cluster::GetGlobalCenterXY() const
{
  return f_GlobalCenterXY;
}

//Returns the half widths in pixels
std::pair<double,double> Cluster::GetHalfWidthsXY() const
{
  return f_HalfWidthsXY;
}

//Returns the type. Nominal types include:
// - PARTICLE - useful for dust finding
// - REFLECTION - not super useful.
// - CROSS - useful for metrology
std::string Cluster::GetType() const
{
  return f_Type;
}

//Returns a cluster ID
int Cluster::GetID() const
{
  return f_Id;
}

//Returns the vector of hit pixels associated with the cluster
std::vector<std::pair<double,std::pair<int,int> > > Cluster::GetHitPixels() const
{
  return f_HitPixels;
}

//Returns the picture name of the picture within which this cluster was found
std::string Cluster::GetOwnerPicture() const
{
  return f_OwnerPicture;
}


//Returns the integral
double Cluster::GetIntegral() const
{
  return f_Integral;
}

//Returns the area
int Cluster::GetArea() const
{
  return f_Area;
}

//Returns the avg brightness
double Cluster::GetAvgBrightness() const
{
  return f_AvgBrightness;
}

//Returns a hit pixel's RGB vals
std::vector<std::vector<int> > Cluster::GetHitPixelsRGB() const
{
  return f_HitPixelsRGB;
}

//Returns the og hit pixels
std::vector<std::pair<double,std::pair<int,int> > > Cluster::GetOGHitPixels() const
{
  return f_OgHitPixels;
}

//Returns the integral of the og hit pixels
double Cluster::GetOGIntegral() const
{
  return f_OgIntegral;
}

//Returns the avg brightness of the og hit pixels
double Cluster::GetOGAvgBrightness() const
{
  return f_OgAvgBrightness;
}
