///////////////////////////////////////////////////////////////
//
// Cluster.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the Cluster object
// used in Minesweeper. A cluster is an object associated with
// a bunch of neighbor pixels above threshold in the IFFT of the
// modified FFT of the picture. It is associated with some kind
// of physical importance, and has a parameter to describe
// that. For example, is a cluster associated with a cross
// nonuniformity, a reflection, or an actual piece of garbage
// on the wire? Clusters are defined by a center in XY and
// a half width in X and in Y.
//
///////////////////////////////////////////////////////////////

#ifndef CLUSTER_HH
#define CLUSTER_HH

//Standard includes
#include <string>
#include <vector>
#include <utility>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"


class Cluster : public TObject
{

public:
  Cluster();
  ~Cluster();

  //Set functions

  void SetLocalCenterXY( int x, int y );
  void SetGlobalCenterXY( double x, double y );
  void SetHalfWidthsXY( double hx, double hy );
  void SetType( std::string typeName );
  void SetID( int id );
  void AddHitPixel( std::pair<double,std::pair<int,int> > pixel );
  void SetOwnerPicture( std::string picName );
  void SetIntegral( double val );
  void SetArea( int val );
  void SetAvgBrightness( double val );
  void AddHitPixelRGB( std::vector<int> pixel );
  void SetOGHitPixels( std::vector<std::pair<double,std::pair<int,int> > > ogHitPix );
  void SetOGIntegral( double val );
  void SetOGAvgBrightness( double val );

  //Get functions
  std::pair<int,int> GetLocalCenterXY() const;
  std::pair<double,double> GetGlobalCenterXY() const;
  std::pair<double,double> GetHalfWidthsXY() const;
  std::string GetType() const;
  int GetID() const;
  std::vector<std::pair<double,std::pair<int,int> > > GetHitPixels() const;
  std::string GetOwnerPicture() const;
  double GetIntegral() const;
  int GetArea() const;
  double GetAvgBrightness() const;
  std::vector<std::vector<int> > GetHitPixelsRGB() const;
  std::vector<std::pair<double,std::pair<int,int> > > GetOGHitPixels() const;
  double GetOGIntegral() const;
  double GetOGAvgBrightness() const;
  
  
private:
  std::pair<int,int> f_LocalCenterXY;                                // In pixels, this is the center of the box in the picture's coordinate system.
  std::pair<double,double> f_GlobalCenterXY;                         // In mm, this is the center relative to the grid coordinate system's origin.
  std::pair<double,double> f_HalfWidthsXY;                               // Self explanatory, in pixels
  std::string f_Type;                                                // What is this cluster most like? Dust? A cross? A reflection? etc.
  int f_Id;                                                          // Cluster id (within this picture)
  std::string f_OwnerPicture;                                        // Picture on which this cluster is located
  std::vector<std::pair<double,std::pair<int,int> > > f_HitPixels;   // The values and locations of the pixels contained within this (IFT).
  std::vector<std::vector<int> > f_HitPixelsRGB;                     // The RGB values for the hit pixels in this cluster

  //Variables that may help with large feature identification
  double f_Integral;                                                 //Integral over the pixels (in IFT brightness)
  int f_Area;                                                        //Area in number of hit pixels
  double f_AvgBrightness;                                            //Integral divided by area
  std::pair<double,double> f_OffsetFromWire;                         //How far away from a wire this is. This seems hard. I'll come back to it later.

  std::vector<std::pair<double,std::pair<int,int> > > f_OgHitPixels; // The values and locations of the pixels contained within this (original image). (Locations should be the same as f_HitPixels, but values shouldn't)
  double f_OgIntegral;                                               //Integral over the pixels (in original, BW pixel brightness)
  double f_OgAvgBrightness;

  
  ClassDef(Cluster,1)
  
};


#endif
