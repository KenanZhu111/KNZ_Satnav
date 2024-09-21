#ifndef _CAL_MOD_H_
#define _CAL_MOD_H_
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"read.h"
#include"2gpst.h"
#include"select_epo.h"
#include"xyz2blh.h"
#include"degRrad.h"
#include"deg2dms.h"
#include"blh2enu.h"
#include"rahcal.h"

/* -------------------------------------------------------------------------- */
#define a       6378137.0//长半轴
#define f       (1 / 298.257223563)//扁率
#define e2      (f*(2-f))//第一偏心率平方
#define C_V     299792458//光速（m）
#define GM      398600500000000//地心引力常数
#define math_e  2.718281828459 //e值
#define PI      3.141592653589793
#define Earth_e 7.2921151467e-5 //地球自转角速度
#define C1      0
/* -------------------------------------------------------------------------- */

    //卫星位置结构体
    typedef struct
    {
        double X[36];
        double Y[36];
        double Z[36];
        double deltat[36];//改正前的信号传播时间
        double delta_t[36];//改正后的信号传播时间
        double delta_clk[36];
    }pos_t, *ppos_t;

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
    }station, *pstation;

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
    }blh, *pblh;
   
    typedef struct enu
    {
	    double E;
	    double N;
	    double U;
    }enu, *penu;

    typedef struct rah
    {
        double R;
        double A;
        double H;
    }rah, *prah;

    /* ------------------------------- GPS卫星位置计算函数 ------------------------------ */
    int gps_pos(int eponum, int sPRN, int best_epoch, double GPSsec, 
                pnav_body nav_b, pobs_head obs_h, ppos_t pos_t)
    {    
        double T_S = GPSsec - pos_t[eponum].deltat[sPRN];//计算信号发射时刻
        double n_0 = sqrt(GM) / pow(nav_b[best_epoch].sqrtA,3);
        double n = n_0 + nav_b[best_epoch].deltan; //计算卫星运行平均角速度
        double tk = T_S - nav_b[best_epoch].TOE;//计算归化时间
        if (tk > 32400)tk -= 604800;
	    else if (tk < -32400)tk += 604800;//规划时间改正
        double Ms = nav_b[best_epoch].M0 + n * tk;//计算卫星发射时卫星的平近角M
	    double Es = Ms, E0;//迭代计算偏近点角
        do
	    {
		E0 = Es;
		Es = Ms + nav_b[best_epoch].e * sin(Es);
	    } while (fabs(Es - E0) > 1.0e-5);//迭代计算偏近点角
        //计算真近点角
        double cosfs = (cos(Es) - nav_b[best_epoch].e) / (1 - nav_b[best_epoch].e * cos(Es));
        double sinfs = (sqrt(1 - pow(nav_b[best_epoch].e, 2)) * sin(Es)) / (1 - nav_b[best_epoch].e * cos(Es));
        double fs = atan2(sinfs, cosfs);
        double u0 = fs + nav_b[best_epoch].omega;//计算升交角距
	    //计算摄动改正项
        double epsilon_u = nav_b[best_epoch].Cus * sin(2 * u0) + nav_b[best_epoch].Cuc * cos(2 * u0);
        double epsilon_r = nav_b[best_epoch].Crs * sin(2 * u0) + nav_b[best_epoch].Crc * cos(2 * u0);
        double epsilon_i = nav_b[best_epoch].Cis * sin(2 * u0) + nav_b[best_epoch].Cic * cos(2 * u0);
        double u = u0 + epsilon_u;//改正后的升交角距
        double r = pow(nav_b[best_epoch].sqrtA, 2) * (1 - nav_b[best_epoch].e * cos(Es)) + epsilon_r;//改正后的轨道向径
        double i = nav_b[best_epoch].i0 + epsilon_i + nav_b[best_epoch].IDOT * tk;//改正后的轨道倾角
        double x = r * cos(u);
        double y = r * sin(u);//卫星在轨道平面坐标系中的坐标
        double l = nav_b[best_epoch].OMEGA + (nav_b[best_epoch].deltaomega - Earth_e) * tk - Earth_e * nav_b[best_epoch].TOE;
        //计算升交点经度
        double X = x * cos(l) - y * cos(i) * sin(l);
        double Y = x * sin(l) + y * cos(i) * cos(l);
        double Z = y * sin(i);//卫星在地固系下的直角坐标
        pos_t[eponum].X[sPRN] = cos(Earth_e * pos_t[eponum].delta_t[sPRN]) * X + sin(Earth_e * pos_t[eponum].delta_t[sPRN]) * Y;
        pos_t[eponum].Y[sPRN] = -sin(Earth_e * pos_t[eponum].delta_t[sPRN]) * X + cos(Earth_e * pos_t[eponum].delta_t[sPRN]) * Y;
        pos_t[eponum].Z[sPRN] = Z;
        double R = sqrt(pow(pos_t[eponum].X[sPRN] - obs_h->apX, 2) + pow(pos_t[eponum].Y[sPRN] - obs_h->apY, 2) + pow(pos_t[eponum].Z[sPRN] - obs_h->apY, 2));
        pos_t[eponum].deltat[sPRN] = pos_t[eponum].delta_t[sPRN];
        pos_t[eponum].delta_t[sPRN] = R / C_V;
        double rela = 2*sqrt(GM) * nav_b[best_epoch].e * nav_b[best_epoch].sqrtA * sin(Es)/pow(C_V,2);
        pos_t[eponum].delta_clk[sPRN] = nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * tk + nav_b[best_epoch].sa2 * pow(tk,2) - rela;
    }
    /* -------------------------------------------------------------------------- */


    /* ------------------------------- GPS卫星位置计算 ------------------------------ */
    int sat_gps_pos_clac(FILE * result_file, 
                         pnav_body nav_b, pobs_epoch obs_e, pobs_body obs_b,
                         pobs_head obs_h, pstation station, ppos_t pos_t, 
                         pblh blh, penu enu, prah rah, 
                         int o_epochnum, int gps_satnum)
    {
        for (int i = 0; i < o_epochnum; i++)//第i个历元
        {
            result_file = fopen(".\\Pos_out\\LLA_result_for_read.txt", "a+");
            fprintf(result_file, "\n>%04d %04d %02d %02d %02d %02d %07.04f",i + 1 ,obs_e[i].y ,obs_e[i].m ,obs_e[i].d ,obs_e[i].h ,obs_e[i].min ,obs_e[i].sec);
            fclose(result_file);
            result_file = fopen(".\\Draw the Track[by PyGMT]\\LLA_result_for_PyGMT.txt", "a+");
            fprintf(result_file, "\n>%04d %04d %02d %02d %02d %02d %07.04f",i + 1 ,obs_e[i].y ,obs_e[i].m ,obs_e[i].d ,obs_e[i].h ,obs_e[i].min ,obs_e[i].sec);
            fclose(result_file);
            result_file = fopen(".\\Observation_Station_Site-Solving_by_Matlab\\LLA_result_for_Matlab.txt", "a+");
            fprintf(result_file, "\n>%04d %04d %02d %02d %02d %02d %07.04f",i + 1 ,obs_e[i].y ,obs_e[i].m ,obs_e[i].d ,obs_e[i].h ,obs_e[i].min ,obs_e[i].sec);
            fclose(result_file);

            for (int j = 0; j < obs_e[i].gps_num; j++)//第j颗GPS卫星
            {
                int y = obs_e[i].y; int m = obs_e[i].m; int d = obs_e[i].d; 
                double h = obs_e[i].h; int min = obs_e[i].min; double sec = obs_e[i].sec;
                //转换为GPS周内秒
                double GPSsec = Time2GPSsec(y, m, d, h, min, sec);
                int sPRN = obs_e[i].sPRN_GPS[j];
                //遍历N文件GPS卫星数据块，寻找最佳历元
                int best_epoch = select_epoch(GPSsec, sPRN, nav_b, gps_satnum);
                //观测时刻 - 参考时刻
                double detat_toc = GPSsec - nav_b[best_epoch].TOE;
                station[i].delta_TR == 0.0;
                
                //计算近似的信号传播时间,接收机钟差已初始化为0(伪距/光速-接收机钟差+卫星钟差)
                pos_t[i].delta_t[sPRN] = (obs_b[i].obs_gps[j][C1] / C_V) - station[i].delta_TR + nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * detat_toc + nav_b[best_epoch].sa2 * pow(detat_toc, 2);
                
                //判断收敛
                pos_t[i].deltat[sPRN] = 0.0;
                while (fabs(pos_t[i].delta_t[sPRN] - pos_t[i].deltat[sPRN]) > 0.00001){
                    
                    gps_pos( i, sPRN, best_epoch, GPSsec, nav_b, obs_h, pos_t);
                }

                /* ----------------------------- 地固坐标系转经纬大地高坐标系 ----------------------------- */
                pxyz2blh tem1 = NULL;
                tem1 = (pxyz2blh)malloc(sizeof(pxyz2blh));
                tem1 = XYZ2BLH( tem1, pos_t[i].X[sPRN], pos_t[i].Y[sPRN], pos_t[i].Z[sPRN], a, e2);
                blh->B = rad2deg(tem1->B);
                blh->L = rad2deg(tem1->L);
                blh->H = rad2deg(tem1->H);

                pxyz2blh tem2 = NULL;
                tem2 = (pxyz2blh)malloc(sizeof(pxyz2blh));
                tem2 = XYZ2BLH( tem2, obs_h->apX, obs_h->apY, obs_h->apZ, a, e2);
                station->B = tem2->B;
                station->L = tem2->L;
                station->H = tem2->H;
                /* -------------------------------------------------------------------------- */
                
                /* ----------------------------- 经纬大地高坐标系转站心坐标系 ----------------------------- */
                double deltax = pos_t[i].X[sPRN] - obs_h->apX;
                double deltay = pos_t[i].Y[sPRN] - obs_h->apY;
                double deltaz = pos_t[i].Z[sPRN] - obs_h->apZ;
                pblh2enu tem3 = NULL;
                tem3 = (pblh2enu)malloc(sizeof(pblh2enu));
                tem3 = BLH2ENU(tem3, station->B, station->L, deltax, deltay, deltaz);
                enu->E = tem3->E;
                enu->N = tem3->N;
                enu->U = tem3->U;
                /* -------------------------------------------------------------------------- */

                /* ---------------------------- 卫星方位角a，高度角h，向径r计算 --------------------------- */
                prahcal tem4 = NULL;
                tem4 = (prahcal)malloc(sizeof(prahcal));
                tem4 = RAHCAL(tem4, enu->E, enu->N, enu->U);
                rah->R = tem4->R;
                rah->A = rad2deg(tem4->A);
                rah->H = rad2deg(tem4->H);
                /* -------------------------------------------------------------------------- */

                if (blh->H < 0 || nav_b[best_epoch].sHEA != 0)
                { 
                    break;
                }
                else
                {   
                    pdeg2dms temb = NULL;
                    temb = (pdeg2dms)malloc(sizeof(pdeg2dms));
                    temb = DEG2DMS(temb, blh->B);
                    blh->B_d = temb->D;
                    blh->B_m = temb->M;
                    blh->B_s = temb->S;

                    pdeg2dms teml = NULL;
                    teml = (pdeg2dms)malloc(sizeof(pdeg2dms));
                    teml = DEG2DMS(teml, blh->L);
                    blh->L_d = teml->D;
                    blh->L_m = teml->M;
                    blh->L_s = teml->S;

                    result_file = fopen(".\\Pos_out\\LLA_result_for_read.txt", "a+");
                    fprintf(result_file, "\nG%02d[sPRN] %4d° %02d′ %02d″ %4d° %02d′ %02d″ %15.05f",sPRN, blh->B_d, blh->B_m, blh->B_s, blh->L_d, blh->L_m, blh->L_s, blh->H);
                    fclose(result_file);
                    result_file = fopen(".\\Draw the Track[by PyGMT]\\LLA_result_for_PyGMT.txt", "a+");
                    fprintf(result_file, "\nG%02d[sPRN] %10.05f %10.05f %15.05f",sPRN, blh->B, blh->L, blh->H);
                    fclose(result_file);
                    result_file = fopen(".\\Observation_Station_Site-Solving_by_Matlab\\LLA_result_for_Matlab.txt", "a+");
                    fprintf(result_file, "\nG%02d[sPRN] %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %15.12f %16.13f"
                                            ,sPRN, pos_t[i].X[sPRN], pos_t[i].Y[sPRN], pos_t[i].Z[sPRN], obs_b[i].obs_gps[j][C1], obs_b[i].obs_gps[j][10], rah->H, nav_b[best_epoch].TGD, pos_t[i].delta_clk[sPRN]);
                    fclose(result_file);
                }
            }       
        
            result_file = fopen(".\\Observation_Station_Site-Solving_by_Matlab\\LLA_result_for_Matlab.txt", "a+");
            fprintf(result_file, "\n");
            fclose(result_file);
        }
        
        result_file = fopen(".\\Observation_Station_Site-Solving_by_Matlab\\LLA_result_for_Matlab.txt", "a+");
        fprintf(result_file, "\nEND");
        fclose(result_file);

        return 1;
    }

#endif