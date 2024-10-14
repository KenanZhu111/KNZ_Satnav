#ifndef _READ_H_
#define _READ_H_
#include<stdio.h>
#include<string.h>

#include"public.h"
#include"type2code.h"


static double strtonum(const char* buff, int i, int n)
{
	double value = 0.0;
	char str[256] = { 0 };
	char* p = str;
	/* ---------------------------------- */
	/// Function will return 0.0 as error code when three occasion are below:
	/// 1.Begining position < 0
	/// 2.The number of Byte of read < i
	/// 3.The Byte size of string < n
	/* ---------------------------------- */
	if (i < 0 || (int)strlen(buff) < i || (int)sizeof(str) - 1 < n)
	{
		return 0.0;
	}
	for (buff += i; *buff && --n >= 0; buff++)
	{
		*p++ = ((*buff == 'D' || *buff == 'd') ? 'e' : *buff);
	}
	*p = '\0';

	return sscanf(str, "%lf", &value) == 1 ? value : 0.0;
}

/* -------------------------- Read the file of obs -------------------------- */
/* -------------------------------------------------------------------------- */
/// @brief get the total number of epoch of obs
/// @param fp_obs the file pointer of obs
/// @return epoch number
/* -------------------------------------------------------------------------- */
extern int get_epochnum(FILE* fp_obs)
{
	int n = 0;
	int satnum = 0;
	char flag;
	char buff[MAXRINEX];
	while (fgets(buff, MAXRINEX, fp_obs))
	{	satnum = (int)strtonum(buff, 33, 2);
		strncpy(&flag, buff + 0, 1);
		if (flag == '>')
		{	n++;}
	}
	return n;
}
/* ------------------- Read the header data of file of obs ------------------ */
extern void read_o_h(FILE* fp_obs, pobs_head obs_h)
{
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };
	char* lable = buff + 60;
	int i = 0;
	int j = 0;

	obs_h->interval = 30;
	obs_h->f_y = obs_h->f_m = obs_h->f_d = obs_h->f_h = obs_h->f_min = obs_h->f_sec = 0;
	obs_h->l_y = obs_h->l_m = obs_h->l_d = obs_h->l_h = obs_h->l_min = obs_h->l_sec = 0;
	memset(obs_h->obscode_gps, 0, sizeof(obs_h->obscode_gps));
	memset(obs_h->obscode_bds, 0, sizeof(obs_h->obscode_bds));
	memset(obs_h->obscode_gal, 0, sizeof(obs_h->obscode_gal));

	while (fgets(buff, MAXRINEX, fp_obs))
	{	if (strstr(lable, "RINEX VERSION / TYPE"))
		{	obs_h->ver = strtonum(buff, 0, 9);
			strncpy(obs_h->type, buff + 20, 30);
			continue;}

		else if (strstr(lable, "APPROX POSITION XYZ"))
		{	obs_h->apX = strtonum(buff, 0, 14);
			obs_h->apY = strtonum(buff, 0 + 14, 14);
			obs_h->apZ = strtonum(buff, 0 + 14 + 14, 14);
			continue;}

		else if (strstr(lable, "ANTENNA: DELTA H/E/N"))
		{	obs_h->ANTH = strtonum(buff, 0, 14);
			obs_h->ANTdeltaE = strtonum(buff, 14, 14);
			obs_h->ANTdeltaN = strtonum(buff, 14 + 14, 14);
			continue;}

		else if (strstr(lable, "SYS / # / OBS TYPES"))
		{	char flag = {0};
			strncpy(&flag, buff + 0, 1);

			if (flag == 'G')
			{	obs_h->obstypenum_gps = (int)strtonum(buff, 4, 2);

				if (obs_h->obstypenum_gps <= 13)
				{   for (i = 0; i < obs_h->obstypenum_gps; i++)
					{	obs_h->obscode_gps[i + 1] = Type2Code(i, buff);}}

				else if (obs_h->obstypenum_gps > 13)
				{	for (i = 0; i < 13; i++)
					{	obs_h->obscode_gps[i] = Type2Code(i, buff);}

					fgets(buff, MAXRINEX, fp_obs);

					for (i = 0; i < obs_h->obstypenum_gps - 13; i++)
					{	obs_h->obscode_gps[i + 13] = Type2Code(i, buff);}}

				continue;}

			else if (flag == 'C')
			{	obs_h->obstypenum_bds = (int)strtonum(buff, 4, 2);

				if (obs_h->obstypenum_bds <= 13)
				{	for (i = 0; i < obs_h->obstypenum_bds; i++)
					{	obs_h->obscode_bds[i] = Type2Code(i, buff);}}

				else if (obs_h->obstypenum_bds > 13)
				{	for (i = 0; i < 13; i++)
					{	obs_h->obscode_bds[i] = Type2Code(i, buff);}

					fgets(buff, MAXRINEX, fp_obs);

					for (i = 0; i < obs_h->obstypenum_bds - 13; i++)
					{	obs_h->obscode_bds[i + 13] = Type2Code(i, buff);}}
				continue;}

			else if (flag == 'E')
			{	obs_h->obstypenum_gal = (int)strtonum(buff, 4, 2);

				if (obs_h->obstypenum_gal <= 13)
				{	for (i = 0; i < obs_h->obstypenum_gal; i++)
					{obs_h->obscode_gal[i] = Type2Code(i, buff);}}

				else if (obs_h->obstypenum_gal > 13)
				{	for (i = 0; i < 13; i++)
					{obs_h->obscode_gal[i] = Type2Code(i, buff);}

					fgets(buff, MAXRINEX, fp_obs);

					for (i = 0; i < obs_h->obstypenum_gal - 13; i++)
					{obs_h->obscode_gal[i + 13] = Type2Code(i, buff);}}
				continue;}

			continue;}

		else if (strstr(lable, "INTERVAL"))
		{	obs_h->interval = strtonum(buff, 0, 10);
			continue;}

		else if (strstr(lable, "TIME OF FIRST OBS"))
		{	obs_h->f_y = (int)strtonum(buff, 2, 4);
			obs_h->f_m = (int)strtonum(buff, 2 + 6, 4);
			obs_h->f_d = (int)strtonum(buff, 2 + 6 + 6, 4);
			obs_h->f_h = (int)strtonum(buff, 2 + 6 + 6 + 6, 4);
			obs_h->f_min = (int)strtonum(buff, 2 + 6 + 6 + 6 + 6, 4);
			obs_h->f_sec = strtonum(buff, 2 + 6 + 6 + 6 + 6 + 6, 9);
			strncpy(obs_h->tsys, buff + 6 + 6 + 6 + 6 + 6 + 18, 3);
			continue;}

		else if (strstr(lable, "TIME OF LAST OBS"))
		{	obs_h->l_y = (int)strtonum(buff, 2, 4);
			obs_h->l_m = (int)strtonum(buff, 2 + 6, 4);
			obs_h->l_d = (int)strtonum(buff, 2 + 6 + 6, 4);
			obs_h->l_h = (int)strtonum(buff, 2 + 6 + 6 + 6, 4);
			obs_h->l_min = (int)strtonum(buff, 2 + 6 + 6 + 6 + 6, 4);
			obs_h->l_sec = strtonum(buff, 2 + 6 + 6 + 6 + 6 + 6, 9);
			continue;}

		else if (strstr(lable, "END OF HEADER"))
		{	break;}
	}
}
/* -------------------- Read the body data of file of obs ------------------- */
extern void read_o_b(FILE* fp_obs, pobs_epoch obs_e, pobs_body obs_b, pobs_head obs_h)
{
	int n = 0;
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };

	while (fgets(buff, MAXRINEX, fp_obs))
	{
		int i = 0;
	    int j_g = 0;
		int k_g = 0;
		int j_c = 0;
		int k_c = 0;
		int j_ga = 0;
		int k_ga = 0;
		obs_e[n].gps_num = 0;
		obs_e[n].bds_num = 0;
		obs_e[n].gal_num = 0;

		obs_e[n].y = (int)strtonum(buff, 2, 4);
		obs_e[n].m = (int)strtonum(buff, 7, 2);
		obs_e[n].d = (int)strtonum(buff, 10, 2);
		obs_e[n].h = (int)strtonum(buff, 13, 2);
		obs_e[n].min = (int)strtonum(buff, 16, 2);
		obs_e[n].sec = strtonum(buff, 19, 10);
		obs_e[n].p_flag = (int)strtonum(buff, 31, 1);
		obs_e[n].sat_num = strtonum(buff, 33, 3);
		strncpy(&flag, buff + 0, 1);

		memset(obs_e[n].sPRN, 0, sizeof(obs_e[n].sPRN));
		memset(obs_e[n].sPRN_GPS, 0, sizeof(obs_e[n].sPRN_GPS));
		memset(obs_e[n].sPRN_BDS, 0, sizeof(obs_e[n].sPRN_BDS));
		memset(obs_e[n].sPRN_GAL, 0, sizeof(obs_e[n].sPRN_GAL));
		memset(obs_b[n].obs_gps, 0, sizeof(obs_b[n].obs_gps));
		memset(obs_b[n].obs_bds, 0, sizeof(obs_b[n].obs_bds));
		memset(obs_b[n].obs_gal, 0, sizeof(obs_b[n].obs_gal));
		
		for (i = 0; i < obs_e[n].sat_num; i++)
		{									
			if (flag == 'G')
			{	obs_e[n].sPRN[i] = strtonum(buff, 1, 2);
				obs_e[n].sPRN_GPS[j_g] = obs_e[n].sPRN[i];
				
				for (k_g = 0; k_g < obs_h->obstypenum_gps; k_g++)
				{	obs_b[n].obs_gps[j_g][k_g] = strtonum(buff, 3 + 16 * k_g, 14);}

				obs_e[n].gps_num++; j_g++;
				fgets(buff, MAXRINEX, fp_obs);
				strncpy(&flag, buff + 0, 1);}
						
			else if (flag == 'C')
			{	 obs_e[n].sPRN[i] = strtonum(buff, 1, 2);
				 obs_e[n].sPRN_BDS[j_c] = obs_e[n].sPRN[i];
				 
				 for (k_c = 0; k_c < obs_h->obstypenum_bds; k_c++)
				 {	obs_b[n].obs_bds[j_c][k_c] = strtonum(buff, 3 + 16 * k_c, 14);}				 
				 
				 obs_e[n].bds_num++; j_c++;
				 fgets(buff, MAXRINEX, fp_obs);
				 strncpy(&flag, buff + 0, 1);
			}

			else if (flag == 'E')
			{	obs_e[n].sPRN[i] = strtonum(buff, 1, 2);
				obs_e[n].sPRN_GAL[j_ga] = obs_e[n].sPRN[i];

				for (k_ga = 0; k_ga < obs_h->obstypenum_gal; k_ga++)
				{	obs_b[n].obs_gal[j_ga][k_ga] = strtonum(buff, 3 + 16 * k_ga, 14);}

				obs_e[n].gal_num++; j_ga++;
				fgets(buff, MAXRINEX, fp_obs);
				strncpy(&flag, buff + 0, 1);
			}
				 
			else if (flag != 'G'&& flag != 'C'&& flag != 'E')
			{
				 fgets(buff, MAXRINEX, fp_obs);
				 strncpy(&flag, buff + 0, 1);
			}
		}		
	n++;
	}	
}
/* -------------------------- Read the file of nav -------------------------- */
/* -------------------------------------------------------------------------- */
/// @brief get the total number of satellite boardcast data of nav file 
/// @param fp_nav the file pointer of nav file
/// @return total number of satellite boardcast data
/* -------------------------------------------------------------------------- */
static int getsatnum(FILE* fp_nav)
{
	int satnum = 0;
	int flag = 0;
	char buff[MAXRINEX];
	char satvar;
	char* lable = buff + 60;

	while (fgets(buff, MAXRINEX, fp_nav))
	{	if (strstr(lable, "END OF HEADER"))
		{	flag = 1;}

		if (flag == 1)
		{	while (fgets(buff, MAXRINEX, fp_nav))
			{	strncpy(&satvar, buff + 0, 1);
				if (satvar == 'G' || satvar == 'E' || satvar == 'R' || satvar == 'S' || satvar == 'C' || satvar == 'I' || satvar == 'J')
				{	satnum++;}

				else
				{	continue;}}}}

	return satnum;
}
/* ------------------- Read the header data of file of nav ------------------ */
void read_n_h(FILE* fp_nav, pnav_head nav_h)
{
	char buff[MAXRINEX] = { 0 };
	char* lable = buff;
	int i = 0;
	int j = 0;
	while (fgets(buff, MAXRINEX, fp_nav))
	{
		if (strstr(lable, "RINEX VERSION / TYPE"))
		{	nav_h->ver = strtonum(buff, 0, 9);
			strncpy((nav_h->type), buff + 20, 15);
			continue;}

		else if (strstr(lable, "GPSA"))
		{	nav_h->ION_GPSA[0] = strtonum(buff, 6, 12);
			nav_h->ION_GPSA[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_GPSA[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_GPSA[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;}

		else if (strstr(lable, "GPSB"))
		{	nav_h->ION_GPSB[0] = strtonum(buff, 6, 12);
			nav_h->ION_GPSB[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_GPSB[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_GPSB[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;}

		else if (strstr(lable, "BDSA"))
		{	nav_h->ION_BDSA[0] = strtonum(buff, 6, 12);
			nav_h->ION_BDSA[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_BDSA[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_BDSA[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;}

		else if (strstr(lable, "BDSB"))
		{	nav_h->ION_BDSB[0] = strtonum(buff, 6, 12);
			nav_h->ION_BDSB[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_BDSB[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_BDSB[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;}

		else if (strstr(lable, "BDUT"))
		{	nav_h->BDUT[0] = strtonum(buff, 5, 17);
			nav_h->BDUT[1] = strtonum(buff, 5 + 17, 16);
			nav_h->BDUT[2] = strtonum(buff, 5 + 17 + 16, 7);
			nav_h->BDUT[3] = strtonum(buff, 5 + 17 + 16 + 7, 5);
			continue;}

		else if (strstr(lable, "GPUT"))
		{	nav_h->GPUT[0] = strtonum(buff, 5, 17);
			nav_h->GPUT[1] = strtonum(buff, 5 + 17, 16);
			nav_h->GPUT[2] = strtonum(buff, 5 + 17 + 16, 7);
			nav_h->GPUT[3] = strtonum(buff, 5 + 17 + 16 + 7, 5);
			continue;}

		else if (strstr(lable, "LEAP SECONDS"))
		{	nav_h->leap = (int)strtonum(buff, 4, 2);}

		else if (strstr(lable, "END OF HEADER"))
		break;
	}
}
/* -------------------- Read the body data of file of nav ------------------- */
void read_n_b(FILE* fp_nav, pnav_body nav_b)
{
	int i_g = 0;
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };
	while (fgets(buff, MAXRINEX, fp_nav))
	{
		int j_g = 0;
		int j_c = 0;
		int j_ga = 0;
		int j_gl = 0;
		strncpy(&flag, buff + 0, 1);
		if(flag == 'G')
		{	for (j_g = 0; j_g < 8; j_g++)
			{	switch (j_g)
				{
				case 0:
					nav_b[i_g].sPRN_BDS = -1;
					nav_b[i_g].sPRN_GAL = -1;
					nav_b[i_g].sPRN_GPS = (int)strtonum(buff, 1, 2);
					nav_b[i_g].TOC_Y = (int)strtonum(buff, 4, 4);
					nav_b[i_g].TOC_M = (int)strtonum(buff, 9, 2);
					nav_b[i_g].TOC_D = (int)strtonum(buff, 12, 2);
					nav_b[i_g].TOC_H = (int)strtonum(buff, 15, 2);
					nav_b[i_g].TOC_Min = (int)strtonum(buff, 18, 2);
					nav_b[i_g].TOC_Sec = strtonum(buff, 21, 2);
					nav_b[i_g].sa0 = strtonum(buff, 23, 19);
					nav_b[i_g].sa1 = strtonum(buff, 23 + 19, 19);
					nav_b[i_g].sa2 = strtonum(buff, 23 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 1:
					nav_b[i_g].IODE = strtonum(buff, 4, 19);
					nav_b[i_g].Crs = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].deltan = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].M0 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 2:
					nav_b[i_g].Cuc = strtonum(buff, 4, 19);
					nav_b[i_g].e = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].Cus = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].sqrtA = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 3:
					nav_b[i_g].TOE = strtonum(buff, 4, 19);
					nav_b[i_g].Cic = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].OMEGA = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].Cis = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 4:
					nav_b[i_g].i0 = strtonum(buff, 4, 19);
					nav_b[i_g].Crc = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].omega = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].deltaomega = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 5:
					nav_b[i_g].IDOT = strtonum(buff, 4, 19);
					nav_b[i_g].L2code = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].GPSweek= strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].L2Pflag = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 6:
					nav_b[i_g].sACC = strtonum(buff, 4, 19);
					nav_b[i_g].sHEA = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].TGD = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].IODC = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 7:
					nav_b[i_g].TTN = strtonum(buff, 4, 19);
					nav_b[i_g].fit = strtonum(buff, 4 + 19, 19);
					//nav_b[i_g].spare1 = strtonum(buff, 4 + 19 + 19, 19);
					//nav_b[i_g].spare2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					break;
				}}
			i_g++;
			continue;}

		if(flag == 'C')
		{	for (j_c = 0; j_c < 8; j_c++)
			{	switch (j_c)
				{
				case 0:
					nav_b[i_g].sPRN_GPS = -1;
					nav_b[i_g].sPRN_GAL = -1;
					nav_b[i_g].sPRN_BDS = (int)strtonum(buff, 1, 2);
					nav_b[i_g].TOC_Y = (int)strtonum(buff, 4, 4);
					nav_b[i_g].TOC_M = (int)strtonum(buff, 9, 2);
					nav_b[i_g].TOC_D = (int)strtonum(buff, 12, 2);
					nav_b[i_g].TOC_H = (int)strtonum(buff, 15, 2);
					nav_b[i_g].TOC_Min = (int)strtonum(buff, 18, 2);
					nav_b[i_g].TOC_Sec = strtonum(buff, 21, 2);
					nav_b[i_g].sa0 = strtonum(buff, 23, 19);
					nav_b[i_g].sa1 = strtonum(buff, 23 + 19, 19);
					nav_b[i_g].sa2 = strtonum(buff, 23 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 1:
					nav_b[i_g].AODE = strtonum(buff, 4, 19);
					nav_b[i_g].Crs = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].deltan = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].M0 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 2:
					nav_b[i_g].Cuc = strtonum(buff, 4, 19);
					nav_b[i_g].e = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].Cus = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].sqrtA = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 3:
					nav_b[i_g].TOE = strtonum(buff, 4, 19);
					nav_b[i_g].Cic = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].OMEGA = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].Cis = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 4:
					nav_b[i_g].i0 = strtonum(buff, 4, 19);
					nav_b[i_g].Crc = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].omega = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].deltaomega = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 5:
					nav_b[i_g].IDOT = strtonum(buff, 4, 19);
					//nav_b[i_g].L2code = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].BDTweek = strtonum(buff, 4 + 19 + 19, 19);
					//nav_b[i_g].L2Pflag = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 6:
					nav_b[i_g].sACC = strtonum(buff, 4, 19);
					nav_b[i_g].sHEA = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].TGD1 = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].TGD2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 7:
					//nav_b[i_g].TTN = strtonum(buff, 4, 19);
					nav_b[i_g].AODC = strtonum(buff, 4 + 19, 19);
					//nav_b[i_g].spare1 = strtonum(buff, 4 + 19 + 19, 19);
					//nav_b[i_g].spare2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					break;
				}}
			i_g++;
			continue;}

		if (flag == 'E')
		{	for (j_ga = 0; j_ga < 8; j_ga++)
			{	switch (j_ga)
				{
				case 0:
					nav_b[i_g].sPRN_BDS = -1;
					nav_b[i_g].sPRN_GPS = -1;
					nav_b[i_g].sPRN_GAL = (int)strtonum(buff, 1, 2);
					nav_b[i_g].TOC_Y = (int)strtonum(buff, 4, 4);
					nav_b[i_g].TOC_M = (int)strtonum(buff, 9, 2);
					nav_b[i_g].TOC_D = (int)strtonum(buff, 12, 2);
					nav_b[i_g].TOC_H = (int)strtonum(buff, 15, 2);
					nav_b[i_g].TOC_Min = (int)strtonum(buff, 18, 2);
					nav_b[i_g].TOC_Sec = strtonum(buff, 21, 2);
					nav_b[i_g].sa0 = strtonum(buff, 23, 19);
					nav_b[i_g].sa1 = strtonum(buff, 23 + 19, 19);
					nav_b[i_g].sa2 = strtonum(buff, 23 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 1:
					nav_b[i_g].IODE = strtonum(buff, 4, 19);
					nav_b[i_g].Crs = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].deltan = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].M0 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 2:
					nav_b[i_g].Cuc = strtonum(buff, 4, 19);
					nav_b[i_g].e = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].Cus = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].sqrtA = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 3:
					nav_b[i_g].TOE = strtonum(buff, 4, 19);
					nav_b[i_g].Cic = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].OMEGA = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].Cis = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 4:
					nav_b[i_g].i0 = strtonum(buff, 4, 19);
					nav_b[i_g].Crc = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].omega = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].deltaomega = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 5:
					nav_b[i_g].IDOT = strtonum(buff, 4, 19);
					//nav_b[i_g].L2code = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].GALweek = strtonum(buff, 4 + 19 + 19, 19);
					//nav_b[i_g].L2Pflag = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 6:
					nav_b[i_g].sACC = strtonum(buff, 4, 19);
					nav_b[i_g].sHEA = strtonum(buff, 4 + 19, 19);
					nav_b[i_g].BGDa = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].BGDb = strtonum(buff, 4 + 19 + 19 + 19, 19);
					fgets(buff, MAXRINEX, fp_nav);
					break;
				case 7:
					//nav_b[i_g].TTN = strtonum(buff, 4, 19);
					//nav_b[i_g].fit = strtonum(buff, 4 + 19, 19);
					//nav_b[i_g].spare1 = strtonum(buff, 4 + 19 + 19, 19);
					//nav_b[i_g].spare2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					break;
				}}
			i_g++;
			continue;}
	}
}
/* ------------------------------ END OF READ_H ----------------------------- */
#endif