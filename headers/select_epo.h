#ifndef _SELECT_EPO_H_
#define _SELECT_EPO_H_

#include<math.h>

#include"public.h"

/* -------------------------------------------------------------------------- */
/// @brief 寻找与观测历元最近的卫星广播历元
/// @param SecofWeek 计算得到的观测历元的周内秒
/// @param sPRN 需要匹配的该历元卫星号
/// @param nav_b 导航文件数据块结构体，与卫星号匹配
/// @param satnum 广播星历卫星数量
/// @param syscode 卫星系统代码，详情见PPUBLIC.H
/// @return 返回最佳历元的绝对历元序号
/* -------------------------------------------------------------------------- */
extern int select_epoch(double SecofWeek, int sPRN, pnav_body nav_b, int satnum, int syscode)
{
	int best_epoch = 0;
	double min = 10000;//初始化最小值
	double Min;
	if (syscode == GPS)
	{
		for (int i = 0; i < satnum; i++)
		{
			if (sPRN == nav_b[i].sPRN_GPS)
			{
				Min = fabs(SecofWeek - nav_b[i].TOE);
				if (Min <= min)
				{
					best_epoch = i;
					min = Min;
				}
			}
		}
	}
	return best_epoch;
}

#endif