#ifndef _TYPE2CODE_H_
#define _TYPE2CODE_H_

#include<string.h>
#include<stdio.h>

#include"public.h"

/* -------------------------------------------------------------------------- */
/// @brief 将观测值类型转换为PUBLIC.H中定义的数字代码
/// @param i 读取循环中传入的循环次数
/// @param buff 传入的观测值类型
/// @return 返回观测值代码
/* -------------------------------------------------------------------------- */
int Type2Code(int i, char buff[1000])
{
    int  code_return = 0;
    char code_contrast[4];
    char code_original[4];
    
    for(int j = 0; j < 19; j++)
    {
        memset(code_contrast,0,sizeof(code_contrast));
        memset(code_original,0,sizeof(code_original));
        const char codeindex[]="C1CC1SC1LC1XC1PC1WC1YC1M"
                               "C2CC2SC2LC2XC2PC2WC2YC2M"
                               "C5IC5QC5X";
        strncpy(code_contrast, codeindex + 0 + j * 3 , 3);
        strncpy(code_original, buff + 7 + 4 * i      , 3);
        if(strcmp(code_contrast,code_original) == 0)
        {
            code_return = j + 1;
            break;
        }
    }
    return code_return;
}

/* -------------------------------------------------------------------------- */
/// @brief 将读取的观测值类型匹配类型代码存储数组
/// @param typecode 希望匹配的观测值
/// @param typenum 观测值类型总数量
/// @param typearr 类型代码存储数组
/// @return 返回对应的观测值类型代码的数组位置
/* -------------------------------------------------------------------------- */
int Code2Type(int typecode, int typenum, int typearr[36])
{
    int pos;
    for (pos = 0; pos < typenum; pos++)
    {
        if (typearr[pos] == typecode)
        {
            break;
        }
    }
    return pos;
}

#endif