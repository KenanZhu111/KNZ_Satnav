# **MAIN FUNCTION INTRODUCTION**

* Reads the specified observation data and navigation data.
* Calculate and document the longitude and latitude coordinates and height of the satellite at each epoch time.
* With the help of PyGMT, the output document information is processed and the satellite ground track map is drawn.
* With the help of MatLab tool, the satellite position is calculated and the station position is obtained by simultaneous solution, and the precision is evaluated by comparing with the coordinates of the precise station.  

# MAIN NOTICE

## Program notice
  ### About main calculate
  * Due to the lack of program writing experience, the ease of file reading is not perfect, if you need to read other observation files and navigation files, you can make changes in the ***main.c*** file.  
  ### About PyGMT
  * I have improved the drawing part of PyGMT to ensure that the output data can be used independently.    
  ### About Matlab
  * When the new station data is needed for calculation, two parts of parameters in the Matlab source file need to be corrected, the first is the exact coordinates of the station in the file header, and the second is the longitude, latitude and elevation information of the station in the file header  

## Project notice
* Due to my limited programming level, PyGMT drawing used too miscellaneous judgment statements, resulting in slow drawing or debugging errors, please forgive me.
* When writing the code, with the help of the existing open source code results, and different degrees of reference, here one by one mark and express thanks:  
  
  | *https://blog.csdn.net/why1472587?type=blog*               
  | *https://zhuanlan.zhihu.com/p/416072448*                   
  | *https://www.pygmt.org/latest/index.html*                  
  | *https://blog.csdn.net/FrankXCR/article/details/135438701*

  
  
### *This program code is solely for study and communication purposes* ###
