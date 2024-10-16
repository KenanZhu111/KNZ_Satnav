#ifndef _2GPST_H_
#define _2GPST_H_

/* -------------------------------------------------------------------------- */
/// @brief convert UTC to GPS Second of week
/// @param y year
/// @param m month
/// @param d day
/// @param h hour
/// @param min minute
/// @param sec second
/// @return return GPS Second of week
/* -------------------------------------------------------------------------- */
double Time2Weeksec(int y, int m, int d, double h, int min, double sec)
{
	if (m > 2)
	{
		y = y;
		m = m;
	}
	if (m <= 2)
	{
		y = y - 1;
		m = m + 12;
	}	
	h = h + min/60.0 + sec/3600.0;
	double JD = (int)(365.25 * y) + (int)(30.6001 * (m + 1)) + d + h/24.0 + 1720981.5;
	double MJD = JD - 2400000.5; 
	int gpsweek = (int)((MJD - 44244) / 7);
	double secofweek = (MJD - 44244.0 - gpsweek * 7.0) * 86400.0;
	return secofweek;
}

#endif