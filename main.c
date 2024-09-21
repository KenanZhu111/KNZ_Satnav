/* -------------------------------------------------------------------------- */
/* 仅支持{ Ver 3.04 }版本RENIX观测值文件与广播星历文件读取                    
/* -------------------------------------------------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

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

	int over = 0;

/* -------------------------------------------------------------------------- */	
while(over == 0)
	{
		int     langop;
		char obs_flie[20];
		char nav_file[20];

		printf("|**************SPP-pseudorange-positioning*****************************************|\n");
		printf("**@Author: KenanZhu\n**@Date: 2024/09/21\n**@Make by: C & Python & Matlab\n");
		printf("**@INTRO: \n**The main functions of the software include:\n");
		printf("**read RINEX3.04 files, and output three formats of document files,\n");
		printf("**please cooperate with the corresponding tools for processing.\n");
		printf("**Specific please see:\n  https://github.com/KenanZhu111/SPP_-pseudorange-positioning\n");
		printf("|**********************************************************************************|\n\n");

		printf("|>>> 请选择您的语言:/Please choose your language <<<|\n--------------------\n[1 | ENG][0 | 中文]\n--------------------\n");	
		scanf("%d",&langop);

		if (langop == 0)
			{	printf("|>>> 本程序只支持 ver3.04 RINEX文件的读取 <<<|\n\n");
			printf("|>>  请输入观测数据文件的文件名（包含拓展名）：\n");
			scanf("%s", &obs_flie);}
		if (langop == 1)
			{	printf("|>>> This program only supports reading ver3.04 RINEX files <<<|\n\n");
			printf("|>>  Please enter the file name (including the extension) of the observation data file.:\n");
			scanf("%s", &obs_flie);}	
/* -------------------------------------------------------------------------- */
//O文件读取
		fp_obs = fopen(obs_flie, "r");
		if (langop == 0 && fp_obs == NULL)
			{printf("|>>  观测文件%s读取失败,请检查文件名或者确认文件是否被移动\n", obs_flie);}
		else if (langop == 0 && fp_obs != NULL)
			{printf("|>>  观测文件%s读取成功,正在处理数据...\n\n", obs_flie);}
		
		if (langop == 1 && fp_obs == NULL)
			{printf("|>>  Observation file %s failed to be read. Please check the file name or confirm whether the file has been moved\n\n", obs_flie);}
		else if (langop == 1 && fp_obs != NULL)	
			{printf("|>>  The observation file %s has been read successfully and data is being processed...\n", obs_flie);}

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

		if (langop == 0 && fp_obs != NULL)
			{	printf("|>>> 观测文件读取完成! <<<|\n\n\n");
			printf("|>>  请输入导航文件的文件名（包含拓展名）：\n");
			scanf("%s", &nav_file);}
		if (langop == 1 && fp_obs != NULL)
			{	printf("|>>> Observation file read complete! <<<|\n\n\n");
			printf("|>>  Please enter the file name (including the extension) of the Navigation data file.:\n");
			scanf("%s", &nav_file);}			
/* -------------------------------------------------------------------------- */
//N文件读取
		fp_nav = fopen(nav_file, "r");
		fp_obs = fopen(obs_flie, "r");
		if (langop == 0 && fp_obs != NULL && fp_nav == NULL)
			{printf("|>>  导航文件%s读取失败,请检查文件名或者确认文件是否被移动\n", nav_file);}
		else if (langop == 0 && fp_obs != NULL && fp_nav != NULL)
			{printf("|>>  观测文件%s读取成功,正在处理数据...\n\n", nav_file);}
		
		if (langop == 1 && fp_obs != NULL && fp_nav == NULL)
			{printf("|>>  Navigation file %s failed to be read. Please check the file name or confirm whether the file has been moved\n", nav_file);}
		else if (langop == 1 && fp_obs != NULL && fp_nav != NULL)	
			{printf("|>>  The Navigation file %s has been read successfully and data is being processed...\n\n", nav_file);}

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
		
		if (langop == 0 && fp_obs != NULL && fp_nav != NULL)
			{printf("|>>> 导航文件读取完成! <<<|\n\n");printf("|>>  正在计算数据:[-");}
		if (langop == 1 && fp_obs != NULL && fp_nav != NULL)
			{printf("|>>> Navigation file read complete! <<<|\n\n");printf("|>>  Calculating data:[-");}

/* -------------------------------------------------------------------------- */
		if (fp_obs != NULL && fp_nav != NULL)
		{			
			over = sat_gps_pos_clac(result_file, nav_b, obs_e, obs_b, obs_h, station, pos_t, blh, enu, rah, o_epochnum, gps_satnum);			
		}
/* -------------------------------------------------------------------------- */
		if (langop == 0 && fp_nav != NULL && fp_obs != NULL)
		{	if (over == 1)
				{printf("\n数据生成完毕!");}
			if (over != 1)
				{printf("\n数据处理发生错误!");}}
		if (langop == 1 && fp_nav != NULL && fp_obs != NULL)
		{	if (over == 1)
				{printf("\nData generation completed!");}
			if (over != 1)
				{printf("\nData processing error!");}}
	}
	return 0;
}