#ifndef _READ_H_
#define _READ_H_
#include<stdio.h>
#include<string.h>

#include"public.h"
#include"type2code.h"
/* ------------------------ 将字符串转换为浮点数,i起始位置，n输入多少个字符 ----------------------- */
static double strtonum(const char* buff, int i, int n)
{
	double value = 0.0;
	char str[256] = { 0 };
	char* p = str;
	/************************************
	* 当出现以下三种情况报错，返回0.0
	* 1.起始位置<0
	* 2.读取字符串个数<i
	* 3.str里面存放的字节数<n
	*************************************/
	if (i < 0 || (int)strlen(buff) < i || (int)sizeof(str) - 1 < n)
	{
		return 0.0;
	}
	for (buff += i; *buff && --n >= 0; buff++)
	{
		//三目操作符：D和d为文件中科学计数法部分，将其转换成二进制能读懂的e
		*p++ = ((*buff == 'D' || *buff == 'd') ? 'e' : *buff);
	}
	*p = '\0';
	//三目操作符，将str中存放的数以格式化读取到value中。
	return sscanf(str, "%lf", &value) == 1 ? value : 0.0;
}

/* -------------------------------------------------------------------------- */
/*                                    读取O文件                                  
/* -------------------------------------------------------------------------- */

/* -------------------------------- 获取O文件历元数 -------------------------------- */
extern int get_epochnum(FILE* fp_obs)
{
	int n = 0;//记录历元数
	int satnum = 0;//记录每个历元的卫星数
	char flag;//存放标志符号'>'
	char buff[MAXRINEX];//存放读取的字符串
	while (fgets(buff, MAXRINEX, fp_obs))
	{
		satnum = (int)strtonum(buff, 33, 2);
		strncpy(&flag, buff + 0, 1);
		if (flag == '>')
		{
			n++;
		}
	}
	return n;
}

/* -------------------------------- 读取O文件数据头 -------------------------------- */
extern void read_o_h(FILE* fp_obs, pobs_head obs_h)
{
	memset(obs_h->obscode_gps, 0, sizeof(obs_h->obscode_gps));
	memset(obs_h->obscode_bds, 0, sizeof(obs_h->obscode_bds));
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };
	char* lable = buff + 60;
	int i = 0;
	int j = 0;
	while (fgets(buff, MAXRINEX, fp_obs))
	{
		if (strstr(lable, "RINEX VERSION / TYPE"))
		{
			obs_h->ver = strtonum(buff, 0, 9);
			strncpy(obs_h->type, buff + 20, 30);
			continue;
		}
		else if (strstr(lable, "APPROX POSITION XYZ"))
		{
			obs_h->apX = strtonum(buff, 0, 14);
			obs_h->apY = strtonum(buff, 0 + 14, 14);
			obs_h->apZ = strtonum(buff, 0 + 14 + 14, 14);
			continue;
		}
		else if (strstr(lable, "ANTENNA: DELTA H/E/N"))
		{
			obs_h->ANTH = strtonum(buff, 0, 14);
			obs_h->ANTdeltaE = strtonum(buff, 14, 14);
			obs_h->ANTdeltaN = strtonum(buff, 14 + 14, 14);
			continue;
		}
		else if (strstr(lable, "SYS / # / OBS TYPES"))
		{
			char flag = {0};
			strncpy(&flag, buff + 0, 1);
			if (flag == 'G')
			{
				obs_h->obstypenum_gps = (int)strtonum(buff, 4, 2);
				if (obs_h->obstypenum_gps <= 13)
				{
					for (i = 0; i < obs_h->obstypenum_gps; i++)
					{
						obs_h->obscode_gps[i + 1] = Type2Code(i, buff);
					}
				}
				else if (obs_h->obstypenum_gps > 13)
				{
					for (i = 0; i < 13; i++)
					{
						obs_h->obscode_gps[i] = Type2Code(i, buff);
					}
					fgets(buff, MAXRINEX, fp_obs);
					for (i = 0; i < obs_h->obstypenum_gps - 13; i++)
					{
						obs_h->obscode_gps[i + 13] = Type2Code(i, buff);
					}
				}
				continue;
			}
			if (flag == 'C')
			{
				obs_h->obstypenum_bds = (int)strtonum(buff, 4, 2);
				if (obs_h->obstypenum_bds <= 13)
				{
					for (i = 0; i < obs_h->obstypenum_bds; i++)
					{
						obs_h->obscode_bds[i] = Type2Code(i, buff);
					}
				}
				else if (obs_h->obstypenum_bds > 13)
				{
					for (i = 0; i < 13; i++)
					{
						obs_h->obscode_bds[i] = Type2Code(i, buff);
					}
					fgets(buff, MAXRINEX, fp_obs);
					for (i = 0; i < obs_h->obstypenum_bds - 13; i++)
					{
						obs_h->obscode_bds[i + 13] = Type2Code(i, buff);
					}
				}
				continue;
			}
		}
		else if (strstr(lable, "INTERVAL"))
		{
			obs_h->interval = strtonum(buff, 0, 10);
			continue;
		}
		else if (strstr(lable, "TIME OF FIRST OBS"))
		{
			obs_h->f_y = (int)strtonum(buff, 2, 4);
			obs_h->f_m = (int)strtonum(buff, 2 + 6, 4);
			obs_h->f_d = (int)strtonum(buff, 2 + 6 + 6, 4);
			obs_h->f_h = (int)strtonum(buff, 2 + 6 + 6 + 6, 4);
			obs_h->f_min = (int)strtonum(buff, 2 + 6 + 6 + 6 + 6, 4);
			obs_h->f_sec = strtonum(buff, 2 + 6 + 6 + 6 + 6 + 6, 9);
			strncpy(obs_h->tsys, buff + 6 + 6 + 6 + 6 + 6 + 18, 3);
			continue;
		}
		else if (strstr(lable, "END OF HEADER"))
			break;
	}
}

/* -------------------------------- 读取O文件数据块 -------------------------------- */
extern void read_o_b(FILE* fp_obs, pobs_epoch obs_e, pobs_body obs_b, int type_gps, int type_bds)
{
	int n = 0;//历元数
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };//判断符号
	while (fgets(buff, MAXRINEX, fp_obs))
	{
		int i = 0;//第i颗卫星
	    int j_g = 0;//第j颗GPS卫星
		int k_g = 0;//第j颗GPS卫星的第k个观测值
		int j_c = 0;//第j颗BDS卫星
		int k_c = 0;//第j颗BDS卫星的第k个观测值
		obs_e[n].gps_num = 0;
		obs_e[n].bds_num = 0;

		//按照格式将历元参考时间依次存入
		//除秒以外，均转换成整型
		obs_e[n].y = (int)strtonum(buff, 2, 4);
		obs_e[n].m = (int)strtonum(buff, 7, 2);
		obs_e[n].d = (int)strtonum(buff, 10, 2);
		obs_e[n].h = (int)strtonum(buff, 13, 2);
		obs_e[n].min = (int)strtonum(buff, 16, 2);
		obs_e[n].sec = strtonum(buff, 19, 10);
		obs_e[n].p_flag = (int)strtonum(buff, 31, 1);
		//输入卫星数
		obs_e[n].sat_num = strtonum(buff, 33, 3);
		strncpy(&flag, buff + 0, 1);

		memset(obs_e[n].sPRN, 0, sizeof(obs_e[n].sPRN));
		memset(obs_e[n].sPRN_GPS, 0, sizeof(obs_e[n].sPRN_GPS));
		memset(obs_e[n].sPRN_BDS, 0, sizeof(obs_e[n].sPRN_BDS));
		memset(obs_b[n].obs_gps, 0, sizeof(obs_b[n].obs_gps));
		memset(obs_b[n].obs_bds, 0, sizeof(obs_b[n].obs_bds));
		
		for (i = 0; i < obs_e[n].sat_num; i++)
		{									
			//判断是否为GPS卫星
			if (flag == 'G')//获取flag标识，判断GPS卫星为真，读取该卫星PRN号
			{		
				obs_e[n].sPRN[i] = strtonum(buff, 1, 2);//将卫星编号存储到第n个历元中，第i个PRN位置
				obs_e[n].sPRN_GPS[j_g] = obs_e[n].sPRN[i];//将GPS卫星编号与总观测卫星号关联
				for (k_g = 0; k_g < type_gps; k_g++)
				{
					obs_b[n].obs_gps[j_g][k_g] = strtonum(buff, 3 + 16 * k_g, 14);
				}				
				obs_e[n].gps_num++; j_g++;
				fgets(buff, MAXRINEX, fp_obs);
				strncpy(&flag, buff + 0, 1);				
			}
						
			else if (flag != 'G')//获取flag标识，判断GPS卫星为否
			{	 
				//判断是否为BDS卫星
				if (flag == 'C')//获取flag标识，判断BDS卫星为真，读取该卫星PRN号
				{
					obs_e[n].sPRN[i] = strtonum(buff, 1, 2);//将卫星编号存储到第n个历元中，第i个PRN位置
				 	obs_e[n].sPRN_BDS[j_c] = obs_e[n].sPRN[i];//将BDS卫星编号与总观测卫星号关联
					for (k_c = 0; k_c < type_bds; k_c++)
					{
						obs_b[n].obs_bds[j_c][k_c] = strtonum(buff, 3 + 16 * k_c, 14);
					}				 
				 	obs_e[n].bds_num++; j_c++;
				 	fgets(buff, MAXRINEX, fp_obs);
				 	strncpy(&flag, buff + 0, 1);
				}				
				 
				else if (flag != 'C')//获取flag标识，判断BDS卫星为真，读取该卫星PRN号
				{
				 	fgets(buff, MAXRINEX, fp_obs);
					strncpy(&flag, buff + 0, 1);
				}
			}
		}		
	n++;
	}	
}

/* -------------------------------------------------------------------------- */
/*                                    读取N文件                                   
/* -------------------------------------------------------------------------- */

/* ---------------------- 获取文件数据块行数，从END OF HEADER后开始起算 --------------------- */
int getsatnum(FILE* fp_nav)
{
	int satnum = 0;
	int flag = 0;
	char buff[MAXRINEX];//用来存放读取到的字符串
	char satvar;
	char* lable = buff + 60;
	//fgets函数，读取一行，当读取结束后返回NULL指针，格式如下：
	//wchar_t * fgets ( wchar_t * str, int num, FILE * stream );
	while (fgets(buff, MAXRINEX, fp_nav))
	{
		if (strstr(lable, "END OF HEADER"))
		{
			flag = 1;
		}
		if (flag == 1)
		{
			while (fgets(buff, MAXRINEX, fp_nav))
			{
				strncpy(&satvar, buff + 0, 1);
				if (satvar == 'G' || satvar == 'E' || satvar == 'R' || satvar == 'S' || satvar == 'C' || satvar == 'I' || satvar == 'J')
				{
					satnum++;
				}
				else
				{
					continue;
				}
			}
		}
	}
	return satnum;
}

/* -------------------------------- 读取N文件数据头 -------------------------------- */
void read_n_h(FILE* fp_nav, pnav_head nav_h)
{
	char buff[MAXRINEX] = { 0 };
	char* lable = buff;
	int i = 0;
	int j = 0;
	while (fgets(buff, MAXRINEX, fp_nav))
	{
		if (strstr(lable, "RINEX VERSION / TYPE"))
		{
			nav_h->ver = strtonum(buff, 0, 9);
			strncpy((nav_h->type), buff + 20, 15);
			continue;
		}
		else if (strstr(lable, "GPSA"))
		{
			nav_h->ION_GPSA[0] = strtonum(buff, 6, 12);
			nav_h->ION_GPSA[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_GPSA[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_GPSA[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;
		}
		else if (strstr(lable, "GPSB"))
		{
			nav_h->ION_GPSB[0] = strtonum(buff, 6, 12);
			nav_h->ION_GPSB[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_GPSB[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_GPSB[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;
		}
		else if (strstr(lable, "BDSA"))
		{
			nav_h->ION_BDSA[0] = strtonum(buff, 6, 12);
			nav_h->ION_BDSA[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_BDSA[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_BDSA[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;
		}
		else if (strstr(lable, "BDSB"))
		{
			nav_h->ION_BDSB[0] = strtonum(buff, 6, 12);
			nav_h->ION_BDSB[1] = strtonum(buff, 6 + 12, 12);
			nav_h->ION_BDSB[2] = strtonum(buff, 6 + 12 + 12, 12);
			nav_h->ION_BDSB[3] = strtonum(buff, 6 + 12 + 12 + 12, 12);
			continue;
		}
		else if (strstr(lable, "BDUT"))
		{
			nav_h->BDUT[0] = strtonum(buff, 5, 17);
			nav_h->BDUT[1] = strtonum(buff, 5 + 17, 16);
			nav_h->BDUT[2] = strtonum(buff, 5 + 17 + 16, 7);
			nav_h->BDUT[3] = strtonum(buff, 5 + 17 + 16 + 7, 5);
			continue;
		}
		else if (strstr(lable, "GPUT"))
		{
			nav_h->GPUT[0] = strtonum(buff, 5, 17);
			nav_h->GPUT[1] = strtonum(buff, 5 + 17, 16);
			nav_h->GPUT[2] = strtonum(buff, 5 + 17 + 16, 7);
			nav_h->GPUT[3] = strtonum(buff, 5 + 17 + 16 + 7, 5);
			continue;
		}

		else if (strstr(lable, "LEAP SECONDS"))
		{
			nav_h->leap = (int)strtonum(buff, 4, 2);
		}
		else if (strstr(lable, "END OF HEADER"))//头文件读取结束
		break;
	}
}

/* -------------------------------- 读取N文件数据块 -------------------------------- */
void read_n_b(FILE* fp_nav, pnav_body nav_b)
{
	int i_g = 0;//第i_g个卫星数据
	char buff[MAXRINEX] = { 0 };
	char flag = { 0 };//判断符号
	while (fgets(buff, MAXRINEX, fp_nav))
	{
		int j_g = 0;//数据块的第j_g行数据
		int j_c = 0;//数据块的第j_c行数据
		strncpy(&flag, buff + 0, 1);
		if(flag == 'G')
		{
			for (j_g = 0; j_g < 8; j_g++)//读取总计第i_g颗 GPS卫星广播星历("总共包含8行数据") 数据块的第j_g行数据
			{
				switch (j_g)
				{
				case 0:
					nav_b[i_g].sPRN_BDS = 0;
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
					nav_b[i_g].spare1 = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].spare2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					break;
				}				
			}
			i_g++;
			continue;
		}
		else if(flag == 'C')
		{
			for (j_c = 0; j_c < 8; j_c++)//读取总计第i_g颗 BDS卫星广播星历("总共包含8行数据") 数据块的第j_c行数据
			{
				switch (j_c)
				{
				case 0:
					nav_b[i_g].sPRN_GPS = 0;
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
					nav_b[i_g].GPSweek = strtonum(buff, 4 + 19 + 19, 19);
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
					nav_b[i_g].spare1 = strtonum(buff, 4 + 19 + 19, 19);
					nav_b[i_g].spare2 = strtonum(buff, 4 + 19 + 19 + 19, 19);
					break;
				}
			}
			i_g++;
			continue;
		}		
	}
}
/* -------------------------------------------------------------------------- */

#endif