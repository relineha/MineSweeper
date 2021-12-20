///////////////////////////////////////////////////////////////
//
// DateAndTime.hh
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class definition of the DateAndTime object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

#ifndef DATEANDTIME_HH
#define DATEANDTIME_HH

#include <cstdlib>
#include <utility>
#include <vector>

//ROOT includes
#include "TObject.h"
#include "TBuffer.h"


class DateAndTime : public TObject
{

public:
  DateAndTime();
  ~DateAndTime();

  void SetTime( int day, int month, int year, int second, int minute, int hour );
  int GetDay();
  int GetMonth();
  int GetYear();
  int GetSecond();
  int GetMinute();
  int GetHour();
		

private:
  int f_Day;
  int f_Month;
  int f_Year;
  int f_Second;
  int f_Minute;
  int f_Hour;
  
  ClassDef(DateAndTime,1)

};


#endif
