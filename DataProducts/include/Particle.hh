///////////////////////////////////////////////////////////////
//
// Particle.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the Particle object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <cstdlib>
#include <utility>
#include <vector>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"


class Particle : public TObject
{

public:
  Particle();
  ~Particle();

  //Set Functions
  void SetHalfWidthsXY( int hx, int hy);
  void SetGlobalCenterXY( double x, double y);
  void SetLocalCenterXY( int x, int y );
  
  //Get Functions
  std::pair<int,int> GetHalfWidthsXY();
  std::pair<double,double> GetGlobalCenterXY();
  std::pair<int,int> GetLocalCenterXY();
  
  
private:
  std::pair<int,int> f_HalfWidthsXY;                     //In pixels
  std::pair<double,double> f_GlobalCenterXY; //In mm, in grid coordinate system
  std::pair<double,double> f_LocalCenterXY;  //In pixels, relative to its own picture.


  ClassDef(Particle,1)
  

  

  


};


#endif
