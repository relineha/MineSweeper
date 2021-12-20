///////////////////////////////////////////////////////////////
//
// Geometry.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 
//
// This is the class definition of the Geometry object
// used in Minesweeper. This object holds information about
// the grid diameter, center in XY, and the location in phi
// of the clocking mark.
//
///////////////////////////////////////////////////////////////

#ifndef GEOMETRY_HH
#define GEOMETRY_HH

//Standard includes
#include <string>
#include <vector>
#include <utility>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"


class Geometry : public TObject
{

public:
  Geometry();
  ~Geometry();

  //Set functions
  void SetCenterXY( double X, double Y );
  void SetGridDiameter( double diam );
  void SetClockPhi( double phi );
  

  //Get functions
  std::pair<double,double> GetCenterXY() const;
  double GetGridDiameter() const;
  double GetClockPhi() const;
  
private:
  
  std::pair<double,double> f_CenterXY; // The X,Y of the center (in mm)
  double f_GridDiameter;                   // Diameter of the grid ring (mm)
  double f_ClockPhi;                   // Phi of the clocking mark on the ring (viewed from top of grid) (degrees)
  


  ClassDef(Geometry,1)
  
};


#endif
