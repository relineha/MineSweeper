///////////////////////////////////////////////////////////////
//
// DateAndTime.cpp
//
// Author: Ryan Linehan
// rlinehan@stanford.edu
// 3/30/2018
//
// This is the class implementation of the DateAndTime object
// used in Minesweeper.
//
///////////////////////////////////////////////////////////////

//MineSweeper includes
#include "DateAndTime.hh"

//ROOT includes
#include "TObject.h"
#include "TClass.h"
#include "TBuffer.h"

ClassImp(DateAndTime)


//Constructor
DateAndTime::DateAndTime()
{
}

//Destructor
DateAndTime::~DateAndTime()
{
}

//Set Function
void DateAndTime::SetTime( int day, int month, int year, int second, int minute, int hour )
{
  f_Day = day;
  f_Month = month;
  f_Year = year;
  f_Second = second;
  f_Minute = minute;
  f_Hour = hour;
}

//Get Functions
int DateAndTime::GetDay()
{
  return f_Day;
}

int DateAndTime::GetMonth()
{
  return f_Month;
}

int DateAndTime::GetYear()
{
  return f_Year;
}

int DateAndTime::GetSecond()
{
  return f_Second;
}

int DateAndTime::GetMinute()
{
  return f_Minute;
}

int DateAndTime::GetHour()
{
  return f_Hour;
}


