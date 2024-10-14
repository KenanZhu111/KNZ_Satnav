#ifndef _TYPE2CODE_H_
#define _TYPE2CODE_H_

#include<string.h>
#include<stdio.h>

#include"public.h"
/* -------------------------------------------------------------------------- */
/// @brief converts the observation type to the numeric code defined in PUBLIC.H
/// @param i reads the number of incoming loops in the loop
/// @param buff type of incoming observation
/// @return Returns the obs code
/* -------------------------------------------------------------------------- */
int Type2Code(int i, char buff[1000])
{
    int  code_return = 0;
    char code_contrast[4];
    char code_original[4];
    
    const char L1codeindex[] = "C1CC1SC1LC1XC1PC1WC1YC1MC1AC1BC1Z";//obs type of different frequency 
    const char L2codeindex[] = "C2CC2SC2LC2XC2PC2WC2YC2MC2IC2QC2X";
    const char L5codeindex[] = "C5IC5QC5X";
    const char L6codeindex[] = "C6AC6BC6CC6XC6Z";
    const char L7codeindex[] = "C7IC7QC7XC7DC7PC7Z";
    const char L8codeindex[] = "C8JC8QC8X";

    for(int j = 0; j < (strlen(L1codeindex)/3); j++)
    {   memset(code_contrast,0,sizeof(code_contrast));
        memset(code_original,0,sizeof(code_original));
        strncpy(code_contrast, L1codeindex + 0 + j * 3 , 3);
        strncpy(code_original, buff + 7 + 4 * i      , 3);
        if(strcmp(code_contrast,code_original) == 0)
        {   code_return = j + 100;
            return code_return;
            break;}}

    for (int j = 0; j < (strlen(L2codeindex) / 3); j++)
    {   memset(code_contrast, 0, sizeof(code_contrast));
        memset(code_original, 0, sizeof(code_original));
        strncpy(code_contrast, L2codeindex + 0 + j * 3, 3);
        strncpy(code_original, buff + 7 + 4 * i, 3);
        if (strcmp(code_contrast, code_original) == 0)
        {   code_return = j + 200;
            return code_return;
            break;}}

    for (int j = 0; j < (strlen(L5codeindex) / 3); j++)
    {   memset(code_contrast, 0, sizeof(code_contrast));
        memset(code_original, 0, sizeof(code_original));
        strncpy(code_contrast, L5codeindex + 0 + j * 3, 3);
        strncpy(code_original, buff + 7 + 4 * i, 3);
        if (strcmp(code_contrast, code_original) == 0)
        {   code_return = j + 500;
            return code_return;
            break;}}

    for (int j = 0; j < (strlen(L6codeindex) / 3); j++)
    {   memset(code_contrast, 0, sizeof(code_contrast));
        memset(code_original, 0, sizeof(code_original));
        strncpy(code_contrast, L6codeindex + 0 + j * 3, 3);
        strncpy(code_original, buff + 7 + 4 * i, 3);
        if (strcmp(code_contrast, code_original) == 0)
        {   code_return = j + 600;
            return code_return;
            break;}}

    for (int j = 0; j < (strlen(L7codeindex) / 3); j++)
    {   memset(code_contrast, 0, sizeof(code_contrast));
        memset(code_original, 0, sizeof(code_original));
        strncpy(code_contrast, L7codeindex + 0 + j * 3, 3);
        strncpy(code_original, buff + 7 + 4 * i, 3);
        if (strcmp(code_contrast, code_original) == 0)
        {   code_return = j + 700;
            return code_return;
            break;}}

    for (int j = 0; j < (strlen(L8codeindex) / 3); j++)
    {   memset(code_contrast, 0, sizeof(code_contrast));
        memset(code_original, 0, sizeof(code_original));
        strncpy(code_contrast, L8codeindex + 0 + j * 3, 3);
        strncpy(code_original, buff + 7 + 4 * i, 3);
        if (strcmp(code_contrast, code_original) == 0)
        {   code_return = j + 800;
            return code_return;
            break;}}

    return code_return;
}

/* -------------------------------------------------------------------------- */
/// @brief the type of the obs value read matches the type code stored in an array
/// @param typecode obs type that you want to match
/// @param typenum the total number of obs types
/// @param typearr type codes store arrays
/// @return returns the array position of the corresponding obs type code
/* -------------------------------------------------------------------------- */
int Code2Type(int typecode, int typenum, int typearr[36])
{
    int pos;
    for (pos = 0; pos < typenum; pos++)
    {   if (typearr[pos] == typecode)
        {   break; 
            return pos;}}

    return pos;
}

#endif