/* -------------------------------------------------------------------------- */
/* 仅支持{ Ver 3.04 }版本RENIX观测值文件与广播星历文件读取                    
/* -------------------------------------------------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include"read.h"
#include"cal_mod.h"
#include"terminal.h"

int main(void)
{
	//数据读取
	FILE * fp_nav = NULL;     //导航星历文件指针
	FILE * fp_obs = NULL;     //观测值文件指针
	FILE * result_file = NULL;//结果输出文件指针

	pstation station = NULL;
	ppos_t pos_t = NULL;
	pblh blh = NULL;
	penu enu = NULL;
	prah rah = NULL;

	pnav_head nav_h = NULL;
	pnav_body nav_b = NULL;
	pobs_head obs_h = NULL;
	pobs_epoch obs_e = NULL;
	pobs_body obs_b = NULL;//创建结构体指针，并将其初始化为空

	int over = 0;

while(over == 0)
	{
		int     langop;
		pfilename filetype = NULL;

/* ---------------------------------- 终端显示 ---------------------------------- */
		langop = Begin_inTermianl();
		filetype = (pfilename)malloc(sizeof(pfilename) * 512);
		filetype = InO_inTermianl(langop, filetype);
/* -------------------------------------------------------------------------- */
//O文件读取
		fp_obs = fopen(filetype->obs_file, "r");

		ReadO_inTerminal(fp_obs, langop, filetype->obs_file);//显示函数

		int o_epochnum = get_epochnum(fp_obs);
		rewind(fp_obs);
		obs_h = (pobs_head)malloc(sizeof(obs_head));
		obs_e = (pobs_epoch)malloc(sizeof(obs_epoch) * o_epochnum);
		obs_b = (pobs_body)malloc(sizeof(obs_body) * o_epochnum);
			if (obs_h && obs_e && obs_b)
			{
				read_o_h(fp_obs, obs_h);
				read_o_b(fp_obs, obs_e, obs_b, obs_h->obstypenum_gps, obs_h->obstypenum_bds);
			}
			fclose(fp_obs);

		filetype = OutO_InN_inTermianl(fp_obs, langop, filetype);//显示函数			
/* -------------------------------------------------------------------------- */
//N文件读取
		fp_nav = fopen(filetype->nav_file, "r");

		ReadN_inTerminal(fp_nav, fp_obs, langop, filetype->nav_file);

		int gps_satnum = getgpssatnum(fp_nav);
		rewind(fp_nav);
		nav_h = (pnav_head)malloc(sizeof(nav_head));
		nav_b = (pnav_body)malloc(sizeof(nav_body) * gps_satnum * 20);
			if (nav_h && nav_b)
			{
				read_n_h(fp_nav, nav_h);
				read_n_b(fp_nav, nav_b);
			}
			fclose(fp_nav);
		
		OutN_inTerminal(fp_nav, fp_obs, langop);//显示函数

		if (fp_obs != NULL && fp_nav != NULL)
		{			
			over = sat_gps_pos_clac(result_file, nav_b, obs_e, obs_b, obs_h, station, pos_t, blh, enu, rah, o_epochnum, gps_satnum);			
		}

		End_inTerminal(fp_nav, fp_obs, langop, over);//显示函数
	}
	return 0;
}