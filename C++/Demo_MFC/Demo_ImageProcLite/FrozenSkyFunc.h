#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <math.h>

///=================================================
/// Astronomy Constant
///=================================================

// 定义π
#define PI 3.14159265

// 定义角度转弧度乘数，即 π / 180 （弧度 = 角度 * ToRadian）
#define ToRadian 0.01745329

// 定义弧度转角度乘数，即 180 / π （角度 = 弧度 * ToAngle）
#define ToAngle 57.29577951

// MJD0 简化儒略日 0日
#define MJD0 2415020.0

// J2000 简化儒略日 2000 January 1 at 12h
#define J2000 (2451545.0 - MJD0)

// 1回归年 = 365.2422平太阳日 = 366.2422恒星日
// 1恒星日 = (1 - 1 / 366.2422)平太阳日
// 1平太阳日 = (1 + 1 / 365.2422)恒星日
// SIDERATE 用于转换恒星日与平太阳日
#define SIDERATE 0.9972695677


///=================================================
/// Project Function
///=================================================


///=================================================
/// Astronomy Function
///=================================================

//=================================================
// Func: Insure 0 <= value < range
// Para:
// _IN_ v (&value)
// _IN_ range (range to make 0 <= value < range)
//=================================================
void YXRange(double *v, double range);

//=================================================
// Func: Date to Modified Julian Day (MJD)
//=================================================
void DateToMJD(int year, int month, double day, double *mjd);

//=================================================
// Func: Get Local Sidereal Time
//=================================================
void GetLocalSiderealTime(double longtitude, double * LST);
void GetLocalSiderealTime(double longtitude, SYSTEMTIME curTime, double * LST);

//=================================================
// Func: Equatorial Coordinates To Horizontal Coordinates
// Refe: https://en.wikipedia.org/wiki/Celestial_coordinate_system
// Para:
// _IN_ dDec (赤纬，角度表示)
// _IN_ dHourAngle (时角，时间表示，向东取负，向西取正)
// _IN_ dLatitude (地理纬度，角度表示)
// _OUT_ Azimuth (地平方位角，弧度表示)
// _OUT_ altitude (高度角，弧度表示)
//=================================================
void EquCoordToHorCoord(const double & dDec, const double & dHourAngle, const double & dLatitude, double & Azimuth, double & altitude);

//=================================================
// Func: Horizontal Coordinates To Equatorial Coordinates
// Refe: https://en.wikipedia.org/wiki/Celestial_coordinate_system
// Para:
// _IN_ Azimuth (地平方位角，弧度表示)
// _IN_ altitude (高度角，弧度表示)
// _IN_ dLatitude (地理纬度，角度表示)
// _OUT_ dDec (赤纬，角度表示)
// _OUT_ dHourAngle (时角，时间表示，向东取负，向西取正)
//=================================================
void HorCoordToEquCoord(const double & Azimuth, const double & altitude, const double & dLatitude, double & dDec, double & dHourAngle);