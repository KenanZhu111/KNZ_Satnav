//仅支持{ Ver 3.04 }版本RENIX观测值文件与广播星历文件读取

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"read.h"
#include"cal_mod.h"

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

	//O文件读取
	fp_obs = fopen("alic0600.22o", "r");//以只读的方式打开O文件
	int o_epochnum = get_epochnum(fp_obs);//O文件历元数
	rewind(fp_obs);
	obs_h = (pobs_head)malloc(sizeof(obs_head));//给O文件头开辟空间
	obs_e = (pobs_epoch)malloc(sizeof(obs_epoch) * o_epochnum);
	obs_b = (pobs_body)malloc(sizeof(obs_body) * o_epochnum);
		if (obs_h && obs_e && obs_b)
		{
			read_o_h(fp_obs, obs_h);//读取O文件头
			read_o_b(fp_obs, obs_e, obs_b, obs_h->obstypenum_gps, obs_h->obstypenum_bds);//读取O数据块
		}
		fclose(fp_obs);//关闭O文件

	//N文件读取
	fp_nav = fopen("brdm0600.22p", "r");//以只读的方式打开N文件
	int gps_satnum = getgpssatnum(fp_nav);//获取导航文件GPS卫星数据块数
	rewind(fp_nav);//将文件指针返回值起始位置
	nav_h = (pnav_head)malloc(sizeof(nav_head));//给N文件头开辟空间
	nav_b = (pnav_body)malloc(sizeof(nav_body) * gps_satnum * 20);
		if (nav_h && nav_b)
		{
			read_n_h(fp_nav, nav_h);//读取N头文件
			read_n_b(fp_nav, nav_b);//读取N数据块
		}
		fclose(fp_nav);//关闭N文件

	blh = (pblh)malloc(sizeof(pblh) * o_epochnum *256);
	enu = (penu)malloc(sizeof(penu) * o_epochnum *256);
	rah = (prah)malloc(sizeof(prah) * o_epochnum *256);
	pos_t = (ppos_t)malloc(sizeof(ppos_t) * o_epochnum * 256);
	station = (pstation)malloc(sizeof(pstation) * o_epochnum * 256);
	
	sat_gps_pos_clac(result_file, nav_b, obs_e, obs_b, obs_h, station, pos_t, blh, enu, rah, o_epochnum, gps_satnum);

	
	return 0;
}