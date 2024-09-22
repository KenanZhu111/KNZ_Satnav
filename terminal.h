#ifndef _TERMINAL_H_
#define _TREMINAL_H_

#include<stdio.h>
#include<stdlib.h>

typedef struct 
{
    char obs_file[20];
    char nav_file[20];
}filename, *pfilename;


int Begin_inTermianl()
{
    int langop;
        printf("|**************SPP-pseudorange-positioning*****************************************|\n");
	printf("**@Author: KenanZhu\n**@Date: 2024/09/21\n**@Make by: C & Python & Matlab\n");
	printf("**@INTRO: \n**The main functions of the software include:\n");
	printf("**read RINEX3.04 files, and output three formats of document files,\n");
	printf("**please cooperate with the corresponding tools for processing.\n");
	printf("**Specific please see:\n  https://github.com/KenanZhu111/SPP_-pseudorange-positioning\n");
	printf("|**********************************************************************************|\n\n");

	printf("|>>> 请选择您的语言:/Please choose your language <<<|\n--------------------\n[1 | ENG][0 | 中文]\n--------------------\n");	
	scanf("%d",&langop);

    return langop;
}

pfilename InO_inTermianl(int langop, pfilename filename)
{
    filename = (pfilename)malloc(sizeof(pfilename));
    if (langop == 0)
		{	
            printf("|>>> 本程序只支持 ver3.04 RINEX文件的读取 <<<|\n\n");
			printf("|>>  请输入观测数据文件的文件名（包含拓展名）：\n");
			scanf("%s", &filename->obs_file);
        }
	if (langop == 1)
		{	
            printf("|>>> This program only supports reading ver3.04 RINEX files <<<|\n\n");
			printf("|>>  Please enter the file name (including the extension) of the observation data file.:\n");
			scanf("%s", &filename->obs_file);
        }
    return filename;
}

void ReadO_inTerminal(FILE *fp_obs, int langop, char obs_file[20])
{
    if (langop == 0 && fp_obs == NULL)
			{
                printf("|>>  观测文件%s读取失败,请检查文件名或者确认文件是否被移动\n\n", obs_file);
            }
	else if (langop == 0 && fp_obs != NULL)
			{
                printf("|>>  观测文件%s读取成功,正在处理数据...\n\n", obs_file);
            }
		
	if (langop == 1 && fp_obs == NULL)
			{
                printf("|>>  Observation file %s failed to be read. Please check the file name or confirm whether the file has been moved\n\n", obs_file);
            }
	else if (langop == 1 && fp_obs != NULL)	
			{
                printf("|>>  The observation file %s has been read successfully and data is being processed...\n\n", obs_file);
            }
}

pfilename OutO_InN_inTermianl(FILE *fp_obs, int langop, pfilename filename)
{
    if (langop == 0 && fp_obs != NULL)
			{	
                printf("|>>> 观测文件读取完成! <<<|\n\n\n");
			    printf("|>>  请输入导航文件的文件名（包含拓展名）：\n\n");
			    scanf("%s", &filename->nav_file);
            }
	if (langop == 1 && fp_obs != NULL)
			{	
                printf("|>>> Observation file read complete! <<<|\n\n\n");
			    printf("|>>  Please enter the file name (including the extension) of the Navigation data file.:\n\n");
			    scanf("%s", &filename->nav_file);
            }
    return filename;
}

void ReadN_inTerminal(FILE *fp_nav, FILE *fp_obs, int langop, char nav_file[20])
{
    if (langop == 0 && fp_obs != NULL && fp_nav == NULL)
			{
                printf("|>>  导航文件%s读取失败,请检查文件名或者确认文件是否被移动\n\n", nav_file);
            }
	else if (langop == 0 && fp_obs != NULL && fp_nav != NULL)
			{
                printf("|>>  观测文件%s读取成功,正在处理数据...\n\n", nav_file);
            }
		
	if (langop == 1 && fp_obs != NULL && fp_nav == NULL)
			{
                printf("|>>  Navigation file %s failed to be read. Please check the file name or confirm whether the file has been moved\n\n", nav_file);
            }
	else if (langop == 1 && fp_obs != NULL && fp_nav != NULL)	
			{
                printf("|>>  The Navigation file %s has been read successfully and data is being processed...\n\n", nav_file);
            }
}

void OutN_inTerminal(FILE *fp_nav, FILE *fp_obs, int langop)
{
    if (langop == 0 && fp_obs != NULL && fp_nav != NULL)
			{
                printf("|>>> 导航文件读取完成! <<<|\n\n");printf("|>>  正在计算数据:[=");
            }
	if (langop == 1 && fp_obs != NULL && fp_nav != NULL)
			{
                printf("|>>> Navigation file read complete! <<<|\n\n");printf("|>>  Calculating data:[=");
            }
}

void End_inTerminal(FILE *fp_nav, FILE *fp_obs, int langop, int over)
{
    if (langop == 0 && fp_nav != NULL && fp_obs != NULL)
		{	if (over == 1)
				{printf("\n数据生成完毕!");}
			if (over != 1)
				{printf("\n数据处理发生错误!");}
        }
	if (langop == 1 && fp_nav != NULL && fp_obs != NULL)
		{	
            if (over == 1)
				{printf("\nData generation completed!");}
			if (over != 1)
				{printf("\nData processing error!");}
        }
}

#endif
