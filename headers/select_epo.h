#ifndef _SELECT_EPO_H_
#define _SELECT_EPO_H_

#include<math.h>

#include"public.h"
/* -------------------------------------------------------------------------- */
/// @brief find the nearest obs epoch with boardcast
/// @param SecofWeek second of week of obs epoch time
/// @param sPRN the satellite PRN which need to match
/// @param nav_b nav data file structure
/// @param satnum total number of boardcast satellite 
/// @param syscode GNSS code, more on PPUBLIC.H
/// @return Return the absolute epoch number of the best epoch 
///			When there is no matching satellite number, the function returns -1
/* -------------------------------------------------------------------------- */
extern int select_epoch(double SecofWeek, int sPRN, pnav_body nav_b, int satnum, int syscode)
{
	int best_epoch = -1;
	double min = 10000;//Initialize the minimum value
	double Min;
	if (syscode == GPS)//Be judged as GPS
	{	for (int i = 0; i < satnum; i++)
		{	if (sPRN == nav_b[i].sPRN_GPS)
			{	Min = fabs(SecofWeek - nav_b[i].TOE);
				if (Min <= min)
				{
					best_epoch = i;
					min = Min;
				}}}
		return best_epoch;}

	if (syscode == BDS)//Be judged as BeiDou
	{	for (int i = 0; i < satnum; i++)
		{	if (sPRN == nav_b[i].sPRN_BDS)
			{	Min = fabs(SecofWeek - nav_b[i].TOE);
				if (Min <= min)
				{
					best_epoch = i;
					min = Min;
				}}}
		return best_epoch;}

	if (syscode == GAL)//Be judged as Galileo
	{for (int i = 0; i < satnum; i++)
		{	if (sPRN == nav_b[i].sPRN_GAL)
			{	Min = fabs(SecofWeek - nav_b[i].TOE);
				if (Min <= min)
				{
					best_epoch = i;
					min = Min;
				}}}
		return best_epoch;}

	return best_epoch;
}
/* --------------------------- END OF SELECT_EPO_H -------------------------- */
#endif