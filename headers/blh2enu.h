#ifndef _BLH2ENU_H_
#define _BLH2ENU_H_

#include<stdlib.h>
#include<math.h>

typedef struct
{
   double E;
   double N;
   double U;
}blh2enu;
/* -------------------------------------------------------------------------- */
/// @brief convert BLH coordinates to ENU coordinates
/// @param blh2enu parameter transfer structure
/// @param stationB latitude of receiver station
/// @param stationL longitude of receiver station
/// @param deltax X-coordinate difference of satellite arrival center
/// @param deltay Y-coordinate difference of satellite arrival center
/// @param deltaz Z-coordinate difference of satellite arrival center
/// @return return type of structure contain E, N, U members
/* -------------------------------------------------------------------------- */
blh2enu BLH2ENU(blh2enu blh2enu, 
                double stationB, double stationL,
                double deltax, double deltay, double deltaz)
{
    double sinB = sin(stationB);
    double cosB = cos(stationB);
    double sinL = sin(stationL);
    double cosL = cos(stationL);
    blh2enu.E = -     sinL*(deltax) +      cosL*(deltay);
    blh2enu.N = -sinB*cosL*(deltax) - sinB*sinL*(deltay) + cosB*(deltaz);
    blh2enu.U =  cosB*cosL*(deltax) + cosB*sinL*(deltay) + sinB*(deltaz);

    return blh2enu;
}

#endif