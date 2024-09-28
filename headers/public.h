#ifndef _PUBLIC_H_
#define _PUBLIC_H_

/* ------------------------------ WGS-84 CONST ------------------------------ */
#define a       6378137.0
#define e2      (0.0033528106647475*(2 - 0.0033528106647475))
#define C_V     299792458
#define GM      398600500000000
#define math_e  2.718281828459
#define PI      3.141592653589793
#define Earth_e 7.2921151467e-5
/* ----------------------------- GPS OBSERV TYPE ---------------------------- */
#define C1C      1
#define C1S      2
#define C1L      3
#define C1X      4
#define C1P      5
#define C1W      6
#define C1Y      7
#define C1M      8

#define C2C      9
#define C2S     10
#define C2L     11
#define C2X     12
#define C2P     13
#define C2W     14
#define C2Y     15
#define C2M     16

#define C5I     17
#define C5Q     18
#define C5X     19
/* ------------------------------ FOR CALMOD.H ------------------------------ */
#define MAXRINEX 1000
    //卫星位置结构体
    typedef struct
    {
        double X[36];
        double Y[36];
        double Z[36];
        double deltat[36];//改正前的信号传播时间
        double delta_t[36];//改正后的信号传播时间
        double delta_clk[36];
    }pos_ts;

    //测站位置结构体
    typedef struct
    {
        double X;
        double Y;
        double Z;
        double B;
        double L;
        double H;
        double delta_TR;
    }stations;
	//经纬高结构体
    typedef struct
    {
        double B;
        int B_d;
        int B_m;
        int B_s;
        double L;
        int L_d;
        int L_m;
        int L_s;
        double H;
    }blhs;
    //东北天结构体
    typedef struct
    {
	    double E;
	    double N;
	    double U;
    }enus;
	//高度方位角结构体
    typedef struct
    {
        double R;
        double A;
        double H;
    }rahs;
/* ------------------------------- FOR READ.H ------------------------------- */
//导航头文件结构体
typedef struct nav_head
{
	double ver;//rinex 版本号
	char type[20];//读取的数据类型
	double ION_GPSA[4];//8个电离层参数
	double ION_GPSB[4];
	double ION_BDSA[4];//8个电离层参数
	double ION_BDSB[4];
	double ION_QZSA[4];//8个电离层参数
	double ION_QZSB[4];
	double ION_IRNA[4];//8个电离层参数
	double ION_IRNB[4];
	double GPUT[4];
	double GLUT[4];
	double GAUT[4];
	double BDUT[4];
	double QZUT[4];
	double IRUT[4];
	double SBUT[4];
	int leap;
}nav_head, *pnav_head;

//导航数据结构体
typedef struct nav_body
{
	//数据块第一行内容：
	int sPRN;//卫星PRN号
	//历元：TOC中卫星钟的参考时刻
	int TOC_Y;//年
	int TOC_M;//月
	int TOC_D;//日
	int TOC_H;//时
	int TOC_Min;//分
	int TOC_Sec;//秒
	double sa0;//卫星钟差
	double sa1;//卫星钟偏
	double sa2;//卫星钟漂
 
	//数据块第二行内容：
	double IODE;//数据、星历发布时间(数据期龄)
	double Crs;//轨道半径的正弦调和改正项的振幅（单位：m）
	double deltan;//卫星平均运动速率与计算值之差(rad/s)
	double M0;//参考时间的平近点角(rad)
 
	//数据块第三行内容：
	double Cuc;//维度幅角的余弦调和改正项的振幅(rad)
	double e;//轨道偏心率
	double Cus;//轨道幅角的正弦调和改正项的振幅(rad)
	double sqrtA;//长半轴平方根
 
	//数据块第四行内容：
	double TOE;//星历的参考时刻(GPS周内秒)
	double Cic;//轨道倾角的余弦调和改正项的振幅(rad)
	double OMEGA;//参考时刻的升交点赤经
	double Cis;//维度倾角的正弦调和改正项的振幅(rad)
 
    //数据块第五行内容：
	double i0;//参考时间的轨道倾角(rad)
	double Crc;//轨道平径的余弦调和改正项的振幅(m)
	double omega;//近地点角距
	double deltaomega;//升交点赤经变化率(rad)
 
	//数据块第六行内容：
	double IDOT;//近地点角距(rad/s)
	double L2code;//L2上的码
	double GPSweek;//GPS周,于TOE一同表示
	double L2Pflag;//L2,p码数据标记
 
	//数据块第七行内容
	double sACC;//卫星精度
	double sHEA;//卫星健康状态
	double TGD;//sec
	double IODC;//钟的数据龄期
 
	//数据块第八行内容
	double TTN;//电文发送时间
	double fit;//拟合区间
	double spare1;//空
	double spare2;//空
 
}nav_body, *pnav_body;

//观测值头文件结构体
typedef struct obs_head
{
	double ver;//RINEX文件版本号
	char type[30];//文件类型
	double apX;//测站近似位置XYZ（WGS84）
	double apY;
	double apZ;
	double ANTH;//天线高
	double ANTdeltaE;//天线中心对于测站标志在东方向上的偏移量
	double ANTdeltaN;//天线中心对于测站标志在北方向上的偏移量
	int obstypenum_gps;//GPS观测值类型数量
	int obstypenum_bds;
	int obscode_gps[36];//rinex3.04GPS观测值类型列表
	int obscode_bds[36];
	double interval;//观测值的历元间隔
	int f_y;//第一个观测记录的时刻
	int f_m;
	int f_d;
	int f_h;
	int f_min;
	double f_sec;
	char tsys[5];//时间系统
}obs_head,*pobs_head;


//观测值历元数据结构体
typedef struct obs_epoch
{
	//观测历元时刻
	int y;
	int m;
	int d;
	int h;
	int min;
	double sec;
	int p_flag;//历元标志
	int sat_num;//当前历元所观测到的卫星数量
	int gps_num;//当前历元所观测到的GPS卫星数量
	int bds_num;//当前历元所观测到的BDS卫星数量
	int sPRN[84];//当前历元所能观测到的卫星的PRN列表
	int sPRN_GPS[25];
	int sPRN_BDS[36];
}obs_epoch,*pobs_epoch;


//观测值数据结构体
typedef struct obs_body
{
	double obs_gps[100][24];//GPS观测值
	double obs_bds[100][24];//BDS观测值
}obs_body,*pobs_body;
/* -------------------------------------------------------------------------- */

#endif