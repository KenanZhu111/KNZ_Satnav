# **MAIN FUNCTION INTRODUCTION**

* Read the RINEX of Observation & Navigation of version 3.xx.
* The reading of RINEX of Observation & Navigation of version 4.xx will support in the KNZ_Calculate.[OINP]
* The reading of RINEX of Observation & Navigation of version 2.xx will turn into a convert software with GUI.[OINP]
* Calculate the coordinates of each epoch of the satellite in the ECEF coordinate system (.sp file).[ADIN 2024/10/5]
* With the help of KNZ_Plot, the coordinates of the station can be calculated. And draw the image.[ADIN 2024/10/5]
* Until October 14, 2024, it is possible to calculate and output the satellite coordinates of the GPS & BeiDou & Galileo satellite system with KNZ_Calculate. With KNZ_Plot, pseudo-range single point positioning of GPS & Galileo satellite systems can be performed.[ADIN 2024/10/14]  

# MAIN NOTICE

## Program notice
  ### About main calculate
  * After a few updates, the observation file and navigation ephemeris file can be selected through the file dialog, and the SPP_calculate program can generate satellite position information (.sp file) in a folder specified by the user.
  * The format of the (.sp file) is described in detail in the help documentation.
  * Support the GNSS of GPS, BeiDou, Galileo.[ADIN 2024/10/14]
  ### About PyGMT(STOP)
  * Until October 5, 2024, project for PyGMT drawing will no longer be considered.[ADIN 2024/10/5]    
  ### About Matlab(STOP)(REPLACEED BY KNZ_Plot)
  * Until October 5, 2024, project for Matlab calculating will no longer be considered.[ADIN 2024/10/5] 
  ### About KNZ_Plot
  * With the help of python's tkinter and matplotlib libraries, it is realized to read and process the.sp file generated by SPP_calculate and generate the (.po file) in the (.sp file) sibling directory. The generated.po file can be processed by KNZ_Plot to obtain line plots of ENU components.
  * At present, the algorithm of station position calculation is integrated in the plot, and only two default tropospheric and ionospheric correction algorithms are supported at present, and the accuracy is still being optimized.
  * Until October 14, 2024, with KNZ_Plot, pseudo-range single point positioning of the GPS & Galileo satellite system can be performed.[ADIN 2024/10/14]
  * The format of the (.po file) is described in detail in the help documentation.
## Project Update

### Update: 2024/10/14
1. KNZ_Calculate: be able to calculate the sat pos of GPS & BDS & Galileo.
2. KNZ_Plot: be able to Single postiting by GPS & Galileo.

### Update: 2024/10/09
1. KNZ_Calculate: modified some mistake and optimize the use experience.
2. KNZ_Plot: more selective options and more united style of GUI.

### Rename the Project: 2024/10/08
1. Update KNZ_Plot.
2. Update more GUI parts.

### Update: 2024/10/06
1. SPP_Plot: Realize the file view function.
2. SPP_Plot: Realize the color select Optimized operation experience.

### Big Update: 2024/10/05
1. Realize the full gui program.
2. Complete the plot function.

### Update: 2024/09/28
1. Implement GUI by WIN32 API.
2. Optimize code structure.

### Update: 2024/09/26
1. Fix some bug.
2. Optimize code structure.

### Update: 2024/09/23
1. Modify some crash.
2. Fix some bug.

### Update: 2024/09/22
1. Modify UI.
2. Fix some bug.

### Big Update: 2024/09/21
1. Correct some mistake.
2. Optimize code structure.
3. Improve the function and operation logic of software input and output.
4. Adding and modify many useful function in their header file:
-blh2enu.h
-xyz2blh.h
-rahcal.h
-deg2dms.h
-degRrad.h

### Update: 2024/09/20
1. Correct some mistake.
2. Optimize code structure.


## Project notice
* When writing the code, with the help of the existing open source code results, and different degrees of reference, here one by one mark and express thanks:  

| *https://blog.csdn.net/why1472587?type=blog*   
| *https://zhuanlan.zhihu.com/p/416072448*                   
| *https://www.pygmt.org/latest/index.html*                  
| *https://blog.csdn.net/FrankXCR/article/details/135438701*

###  Document label specification
* [ADIN TIME]: new added in "TIME"(e.g. [ADIN 2024/9/22]: new added in 2024/9/22).
* (OINP): Operation in progress, or in urgent need of improvement.
* (STOP): No longer update maintenance or consider implementing features.
  
#### *This program code is solely for study and communication purposes* ####
