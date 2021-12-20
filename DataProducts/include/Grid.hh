///////////////////////////////////////////////////////////////
//
// Grid.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the Grid object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

#ifndef GRID_HH
#define GRID_HH

//Standard includes
#include <cstdlib>
#include <utility>
#include <vector>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"

//MineSweeper includes
#include "Particle.hh"
#include "Cluster.hh"
#include "Picture.hh"
#include "ParameterSet.hh"
#include "Geometry.hh"

class Grid : public TObject
{

public:
  Grid();
  ~Grid();

  //Set functions
  void AddPicture( Picture pic );
  void AddCluster( Cluster cluster );
  void AddParticle( Particle particle );
  void SetParameterSet( ParameterSet pSet );
  void SetGeometry( Geometry geo );
  

  //Get functions
  std::vector<Picture> GetPictures() const;
  std::vector<Cluster> GetClusters() const;
  std::vector<Particle> GetParticles() const;
  ParameterSet GetParameterSet() const;
  Geometry GetGeometry() const;

  
private:
  std::vector<Picture> f_Pictures;
  std::vector<Cluster> f_Clusters;      //Non-periodic pixel groups
  std::vector<Particle> f_Particles;    //Non-periodic pixel groups classified as blemishes/fuzz
  ParameterSet f_ParameterSet;
  Geometry f_Geometry;
  
  
  
  ClassDef(Grid,1)

};


#endif
