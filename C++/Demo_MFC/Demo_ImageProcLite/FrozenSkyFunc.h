#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <math.h>

///=================================================
/// Astronomy Constant
///=================================================

// �����
#define PI 3.14159265

// ����Ƕ�ת���ȳ������� �� / 180 ������ = �Ƕ� * ToRadian��
#define ToRadian 0.01745329

// ���廡��ת�Ƕȳ������� 180 / �� ���Ƕ� = ���� * ToAngle��
#define ToAngle 57.29577951

// MJD0 �������� 0��
#define MJD0 2415020.0

// J2000 �������� 2000 January 1 at 12h
#define J2000 (2451545.0 - MJD0)

// 1�ع��� = 365.2422ƽ̫���� = 366.2422������
// 1������ = (1 - 1 / 366.2422)ƽ̫����
// 1ƽ̫���� = (1 + 1 / 365.2422)������
// SIDERATE ����ת����������ƽ̫����
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
// _IN_ dDec (��γ���Ƕȱ�ʾ)
// _IN_ dHourAngle (ʱ�ǣ�ʱ���ʾ����ȡ��������ȡ��)
// _IN_ dLatitude (����γ�ȣ��Ƕȱ�ʾ)
// _OUT_ Azimuth (��ƽ��λ�ǣ����ȱ�ʾ)
// _OUT_ altitude (�߶Ƚǣ����ȱ�ʾ)
//=================================================
void EquCoordToHorCoord(const double & dDec, const double & dHourAngle, const double & dLatitude, double & Azimuth, double & altitude);

//=================================================
// Func: Horizontal Coordinates To Equatorial Coordinates
// Refe: https://en.wikipedia.org/wiki/Celestial_coordinate_system
// Para:
// _IN_ Azimuth (��ƽ��λ�ǣ����ȱ�ʾ)
// _IN_ altitude (�߶Ƚǣ����ȱ�ʾ)
// _IN_ dLatitude (����γ�ȣ��Ƕȱ�ʾ)
// _OUT_ dDec (��γ���Ƕȱ�ʾ)
// _OUT_ dHourAngle (ʱ�ǣ�ʱ���ʾ����ȡ��������ȡ��)
//=================================================
void HorCoordToEquCoord(const double & Azimuth, const double & altitude, const double & dLatitude, double & dDec, double & dHourAngle);