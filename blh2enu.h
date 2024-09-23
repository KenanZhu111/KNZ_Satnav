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
/// @brief 将大地高坐标转换为站心坐标
/// @param blh2enu 自定义结构体类型用来传参
/// @param stationB 站心纬度
/// @param stationL 站心经度
/// @param deltax 卫星到站心的x坐标差值
/// @param deltay 卫星到站心的y坐标差值
/// @param deltaz 卫星到站心的z坐标差值
/// @return 返回自定义结构体类型包含E, N, U成员
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