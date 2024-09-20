#ifndef _SELECT_EPO_H_
#define _SELECT_EPO_H_

#include<stdio.h>
#include<math.h>

#include"read.h"

//挑选合适的星历
extern int select_epoch(double SecofWeek, int sPRN, pnav_body nav_b, int gps_satnum)
{
	int best_epoch = 0;
	double min=10000;//假设最小值是1万秒
	double Min;
	int i;
	for (i = 0; i < gps_satnum; i++)
	{
		if (sPRN == nav_b[i].sPRN)
		{
			Min = fabs(SecofWeek - nav_b[i].TOE);
			if (Min <= min)
			{
				best_epoch = i;
				min = Min;
			}
		}
	}
	return best_epoch;
}

#endif