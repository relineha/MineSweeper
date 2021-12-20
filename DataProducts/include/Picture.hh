///////////////////////////////////////////////////////////////
//
// Picture.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the Picture object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

#ifndef PICTURE_HH
#define PICTURE_HH

//Standard includes
#include <string>
#include <vector>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"


//MineSweeper includes
#include "Particle.hh"
#include "Cluster.hh"

class Picture : public TObject
{

public:
  Picture();
  ~Picture();

  //Set functions
  void SetCenterXY( double x, double y );
  void SetPicName( std::string name );
  void AddParticle( Particle theParticle );
  void AddCluster( Cluster theCluster );
  
  //Get functions
  std::pair<double,double> GetCenterXY() const;
  std::string GetPicName() const;
  std::vector<Particle> GetParticles() const;
  std::vector<Cluster> GetClusters() const;

  
  
private:
  std::string f_Name;
  std::pair<double,double> f_CenterXY;
  std::vector<Particle> f_Particles;
  std::vector<Cluster> f_Clusters;

  ClassDef(Picture,1)

};


#endif
