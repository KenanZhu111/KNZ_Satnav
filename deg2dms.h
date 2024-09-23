#ifndef _DEG2DMS_H_
#define _DEG2DMS_H_

#include<stdlib.h>
#include<math.h>

typedef struct
{
    double D;
    double M;
    double S;    
}deg2dms;

/* -------------------------------------------------------------------------- */
/// @brief 将十进制的度数转换为度分秒形式
/// @param deg2dms 结构体传递参数
/// @param D 要转换度数
/// @return 返回自定义结构体包含度，分，秒成员                      
/* -------------------------------------------------------------------------- */
deg2dms DEG2DMS(deg2dms deg2dms,
                double D)
{
    int sig = D / fabs(D);//符号判断
    deg2dms.D = (int)(D);//转换为度分秒
    deg2dms.M = (int)((D - deg2dms.D) * 60);//分秒去除符号格式
    deg2dms.S = (int)(((D - deg2dms.D) * 60 - deg2dms.M) * 60);
    deg2dms.M  = deg2dms.M * sig;
    deg2dms.S  = deg2dms.S * sig;

    return deg2dms;
}

#endif