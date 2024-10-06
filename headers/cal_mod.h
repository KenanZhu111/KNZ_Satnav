#ifndef _CAL_MOD_H_
#define _CAL_MOD_H_
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<math.h>
#include<time.h>

#include"public.h"

#include"2gpst.h"
#include"rahcal.h"
#include"xyz2blh.h"
#include"degRrad.h"
#include"deg2dms.h"
#include"blh2enu.h"
#include"type2code.h"
#include"select_epo.h"
   
/* ------------------------------- GPS卫星位置计算函数 ------------------------------ */
pos_ts gps_pos(int sPRN, int best_epoch, double GPSsec, 
             pnav_body nav_b, pobs_head obs_h, pos_ts pos_t)
    {    
        double T_S = GPSsec - pos_t.delta_t[sPRN];//计算信号发射时刻
        double n_0 = sqrt(GM) / pow(nav_b[best_epoch].sqrtA,3);
        double n = n_0 + nav_b[best_epoch].deltan; //计算卫星运行平均角速度
        double tk = T_S - nav_b[best_epoch].TOE;//计算归化时间
        if (tk > 32400)tk -= 604800;
	    else if (tk < -32400)tk += 604800;//归化时间改正
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
        pos_t.X[sPRN] =  cos(Earth_e * pos_t.delta_t[sPRN]) * X + sin(Earth_e * pos_t.delta_t[sPRN]) * Y;
        pos_t.Y[sPRN] = -sin(Earth_e * pos_t.delta_t[sPRN]) * X + cos(Earth_e * pos_t.delta_t[sPRN]) * Y;
        pos_t.Z[sPRN] = Z;
        double R = sqrt(pow(pos_t.X[sPRN] - obs_h->apX, 2) + pow(pos_t.Y[sPRN] - obs_h->apY, 2) + pow(pos_t.Z[sPRN] - obs_h->apZ, 2));
        pos_t.deltat[sPRN] = pos_t.delta_t[sPRN];
        double rela = 2*sqrt(GM) * nav_b[best_epoch].e * nav_b[best_epoch].sqrtA * sin(Es)/pow(C_V,2);
        pos_t.delta_clk[sPRN] = nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * tk + nav_b[best_epoch].sa2 * pow(tk,2) - rela;
        pos_t.delta_t[sPRN] = R / C_V + pos_t.delta_clk[sPRN];
        return pos_t;
    }
/* -------------------------------------------------------------------------- */


/* ------------------------------- GPS卫星位置计算&输出 ------------------------------ */
void sat_gps_pos_clac(FILE* result_file,HWND hwndPB,
    pnav_body nav_b, pobs_epoch obs_e, pobs_body obs_b,pobs_head obs_h,
    int o_epochnum, int gps_satnum,
    char res_file[MAX_PATH], char obs_file[MAX_PATH], char nav_file[MAX_PATH],
    int ionoption, int trooption)
{
        PBRANGE range;
        SendMessage(hwndPB, PBM_SETRANGE, (WPARAM)FALSE, (LPARAM)(MAKELPARAM(0, 100)));
        SendMessage(hwndPB, PBM_GETRANGE, (WPARAM)TRUE, (LPARAM)&range);

        time_t gen_time;
        time(&gen_time);
        stations station = { 0 };
        xyz2blh tem2 = { 0 };
        tem2 = XYZ2BLH(tem2, obs_h->apX, obs_h->apY, obs_h->apZ);
        station.B = tem2.B;
        station.L = tem2.L;
        station.H = tem2.H;
        result_file = fopen(res_file, "w");
        fclose(result_file);
        result_file = fopen(res_file, "a+");
        fprintf(result_file, "@ GENERATE PROGRAM   : SPP Calculate v0.4.7\n");
        fprintf(result_file, "@ GENERATE TYPE      : Satellite  Position\n");
        fprintf(result_file, "@ GENERATE TIME      : %s", ctime(&gen_time));
        fprintf(result_file, "@ OBS FILE PATH      : %s\n", obs_file);
        fprintf(result_file, "@ NAV FILE PATH      : %s\n", nav_file);
        fprintf(result_file, "@ APPROX POSITION XYZ: %13.04f%13.04f%13.04f\n@ APPROX POSITION BLH:  %12.07f %12.07f %12.07f\n"
            , obs_h->apX, obs_h->apY, obs_h->apZ
            , station.B, station.L, station.H);
        if (ionoption == 0) { fprintf(result_file, "@ IONOS OPT          : NULL\n"); }
        if (ionoption == 1) { fprintf(result_file, "@ IONOS OPT          : DEFAULT\n"); }
        if (trooption == 0) { fprintf(result_file, "@ TROPO OPT          : NULL\n@\n\n"); }
        if (trooption == 1) { fprintf(result_file, "@ TROPO OPT          : DEFAULT\n@\n\n"); }
        fclose(result_file);

        for (int i = 0; i < o_epochnum; i++)
        {
            result_file = fopen(res_file, "a+");
            fprintf(result_file, "\n>%04d %04d %02d %02d %02d %02d %07.04f", i + 1, obs_e[i].y, obs_e[i].m, obs_e[i].d, obs_e[i].h, obs_e[i].min, obs_e[i].sec);
            fclose(result_file);

            int y = obs_e[i].y;
            int m = obs_e[i].m;
            int d = obs_e[i].d;
            double h = obs_e[i].h;
            int min = obs_e[i].min;
            double sec = obs_e[i].sec;

            xyz2blh tem1 = { 0 };
            blh2enu tem3 = { 0 };
            rahcal  tem4 = { 0 };
            deg2dms temb = { 0 };
            deg2dms teml = { 0 };

            blhs blh = { 0 };
            enus enu = { 0 };
            rahs rah = { 0 };
            pos_ts pos_t = { 0 };
            for (int j = 0; j < obs_e[i].gps_num; j++)//第j颗GPS卫星
            {
                double GPSsec = Time2GPSsec(y, m, d, h, min, sec);//转换为GPS周内秒
                int sPRN = obs_e[i].sPRN_GPS[j];
                int best_epoch = select_epoch(GPSsec, sPRN, nav_b, gps_satnum); //遍历N文件GPS卫星数据块，寻找最佳历元
                double detat_toc = GPSsec - nav_b[best_epoch].TOE;//观测时刻 - 参考时刻
                //计算近似的信号传播时间,接收机钟差已初始化为0(伪距/光速-接收机钟差+卫星钟差)
                pos_t.delta_t[sPRN] = (obs_b[i].obs_gps[j][Code2Type(C1C, obs_h->obstypenum_gps, obs_h->obscode_gps)] / C_V) - station.delta_TR + nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * detat_toc + nav_b[best_epoch].sa2 * pow(detat_toc, 2);
                pos_t.deltat[sPRN] = 0.0;//判断收敛
                while (fabs(pos_t.delta_t[sPRN] - pos_t.deltat[sPRN]) > 1.0e-9) {

                    pos_t = gps_pos(sPRN, best_epoch, GPSsec, nav_b, obs_h, pos_t);
                }

                /* ----------------------------- 地固坐标系转经纬大地高坐标系 ----------------------------- */
                tem1 = XYZ2BLH(tem1, pos_t.X[sPRN], pos_t.Y[sPRN], pos_t.Z[sPRN]);
                blh.B = rad2deg(tem1.B);
                blh.L = rad2deg(tem1.L);
                blh.H = tem1.H;
                /* -------------------------------------------------------------------------- */

                /* ----------------------------- 经纬大地高坐标系转站心坐标系 ----------------------------- */
                double deltax = pos_t.X[sPRN] - obs_h->apX;
                double deltay = pos_t.Y[sPRN] - obs_h->apY;
                double deltaz = pos_t.Z[sPRN] - obs_h->apZ;

                tem3 = BLH2ENU(tem3, station.B, station.L, deltax, deltay, deltaz);
                enu.E = tem3.E;
                enu.N = tem3.N;
                enu.U = tem3.U;
                /* -------------------------------------------------------------------------- */

                /* ---------------------------- 卫星方位角a，高度角h，向径r计算 --------------------------- */
                tem4 = RAHCAL(tem4, enu.E, enu.N, enu.U);
                rah.R = tem4.R;
                rah.A = rad2deg(tem4.A);
                rah.H = rad2deg(tem4.H);
                /* -------------------------------------------------------------------------- */
                if (blh.H < 0 || rah.H < 10 || nav_b[best_epoch].sHEA != 0)
                {
                    continue;
                }
                else
                {
                    temb = DEG2DMS(temb, blh.B);
                    blh.B_d = temb.D;
                    blh.B_m = temb.M;
                    blh.B_s = temb.S;

                    teml = DEG2DMS(teml, blh.L);
                    blh.L_d = teml.D;
                    blh.L_m = teml.M;
                    blh.L_s = teml.S;

                    result_file = fopen(res_file, "a+");
                    fprintf(result_file, "\nG%02d | %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %15.12f %16.13f"
                        , sPRN, pos_t.X[sPRN], pos_t.Y[sPRN], pos_t.Z[sPRN]
                        , obs_b[i].obs_gps[j][Code2Type(C1C, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                        , obs_b[i].obs_gps[j][Code2Type(C2L, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                        , rah.H, nav_b[best_epoch].TGD, pos_t.delta_clk[sPRN]);
                    fclose(result_file);
                }
            }
            result_file = fopen(res_file, "a+");
            fprintf(result_file, "\n");
            fclose(result_file);
            SendMessage(hwndPB, PBM_SETPOS, (WPARAM)((int)(i * 100 / o_epochnum)), (LPARAM)0);

        }
        result_file = fopen(res_file, "a+");
        fprintf(result_file, "\nEND");
        fclose(result_file);
        SendMessage(hwndPB, PBM_SETPOS, (WPARAM)range.iHigh, (LPARAM)0);
}
#endif