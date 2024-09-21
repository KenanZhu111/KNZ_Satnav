#ifndef _RAHCAL_H_
#define _RAHCAL_H_
#define PI 3.141592653589793

#include<stdlib.h>
#include<math.h>

typedef struct 
{
    double R;
    double A;
    double H;
}rahcal, *prahcal;

/* -------------------------------------------------------------------------- */
/// @brief 计算站心坐标系下的卫星高度方位角
/// @param rahcal 自定义结构体变量用于传参
/// @param E 站心坐标东
/// @param N 站心坐标北
/// @param U 站心坐标天
/// @return 返回自定义结构体变量包含R, A, H成员                        
/* -------------------------------------------------------------------------- */
prahcal RAHCAL(prahcal rahcal, 
               double E, double N, double U)
{
    rahcal = (prahcal)malloc(sizeof(prahcal));
    rahcal->H = atan2(U, sqrt(E * E + N * N));
    rahcal->A = atan2(E, U);
    if (rahcal->A < 0)
        rahcal->A += 2 * PI;
    if (rahcal->A > 2 * PI)
        rahcal->A -= 2 * PI;
    rahcal->R = sqrt(E * E + N * N + U * U);

    return rahcal;
}

#endif