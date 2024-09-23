#ifndef _RAHCAL_H_
#define _RAHCAL_H_

#include<stdlib.h>
#include<math.h>

#include"public.h"

typedef struct 
{
    double R;
    double A;
    double H;
}rahcal;

/* -------------------------------------------------------------------------- */
/// @brief 计算站心坐标系下的卫星高度方位角
/// @param rahcal 自定义结构体变量用于传参
/// @param E 站心坐标东
/// @param N 站心坐标北
/// @param U 站心坐标天
/// @return 返回自定义结构体变量包含R, A, H成员                        
/* -------------------------------------------------------------------------- */
rahcal RAHCAL(rahcal rahcal, 
              double E, double N, double U)
{
    rahcal.H = atan2(U, sqrt(E * E + N * N));
    rahcal.A = atan2(E, U);
    if (rahcal.A < 0)
        rahcal.A += 2 * PI;
    if (rahcal.A > 2 * PI)
        rahcal.A -= 2 * PI;
    rahcal.R = sqrt(E * E + N * N + U * U);

    return rahcal;
}

#endif