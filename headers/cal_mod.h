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
   
/* -------------------- GPS Satellite position calculate -------------------- */
pos_ts gps_pos(int sPRN, int best_epoch, double GPSsec, 
                pnav_body nav_b, pobs_head obs_h, pos_ts pos_t)
{   double T_S = GPSsec - pos_t.delta_t;
    double n_0 = sqrt(GM) / pow(nav_b[best_epoch].sqrtA,3);
    double n = n_0 + nav_b[best_epoch].deltan;
    double tk = T_S - nav_b[best_epoch].TOE;

    if (tk > 32400)tk -= 604800;
	else if (tk < -32400)tk += 604800;

    double Ms = nav_b[best_epoch].M0 + n * tk;

	double Es = Ms, E0;
    do
	{
	E0 = Es;
	Es = Ms + nav_b[best_epoch].e * sin(Es);
	} while (fabs(Es - E0) > 1.0e-10);

    double cosfs = (cos(Es) - nav_b[best_epoch].e) / (1 - nav_b[best_epoch].e * cos(Es));
    double sinfs = (sqrt(1 - pow(nav_b[best_epoch].e, 2)) * sin(Es)) / (1 - nav_b[best_epoch].e * cos(Es));
    double fs = atan2(sinfs, cosfs);
    double u0 = fs + nav_b[best_epoch].omega;

    double epsilon_u = nav_b[best_epoch].Cus * sin(2 * u0) + nav_b[best_epoch].Cuc * cos(2 * u0);
    double epsilon_r = nav_b[best_epoch].Crs * sin(2 * u0) + nav_b[best_epoch].Crc * cos(2 * u0);
    double epsilon_i = nav_b[best_epoch].Cis * sin(2 * u0) + nav_b[best_epoch].Cic * cos(2 * u0);

    double u = u0 + epsilon_u;
    double r = pow(nav_b[best_epoch].sqrtA, 2) * (1 - nav_b[best_epoch].e * cos(Es)) + epsilon_r;
    double i = nav_b[best_epoch].i0 + epsilon_i + nav_b[best_epoch].IDOT * tk;

    double x = r * cos(u);
    double y = r * sin(u);

    double l = nav_b[best_epoch].OMEGA + (nav_b[best_epoch].deltaomega - Earth_e) * tk - Earth_e * nav_b[best_epoch].TOE;
    double X = x * cos(l) - y * cos(i) * sin(l);
    double Y = x * sin(l) + y * cos(i) * cos(l);
    double Z = y * sin(i);

    pos_t.X =  cos(Earth_e * pos_t.delta_t) * X + sin(Earth_e * pos_t.delta_t) * Y;
    pos_t.Y = -sin(Earth_e * pos_t.delta_t) * X + cos(Earth_e * pos_t.delta_t) * Y;
    pos_t.Z = Z;//Correction of earth rotation

    double R = sqrt(pow(pos_t.X - obs_h->apX, 2) + pow(pos_t.Y - obs_h->apY, 2) + pow(pos_t.Z - obs_h->apZ, 2));
    pos_t.deltat = pos_t.delta_t;
    double rela = 2*sqrt(GM) * nav_b[best_epoch].e * nav_b[best_epoch].sqrtA * sin(Es)/pow(C_V,2);//Relativistic effect
    pos_t.delta_clk = nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * tk + nav_b[best_epoch].sa2 * pow(tk,2) - rela - nav_b[best_epoch].TGD;
    pos_t.delta_t = R / C_V + pos_t.delta_clk;

    return pos_t;}


/* -------------------- GAL Satellite position calculate -------------------- */
pos_ts gal_pos(int sPRN, int best_epoch, double GALsec,
    pnav_body nav_b, pobs_head obs_h, pos_ts pos_t)
{   double T_S = GALsec - pos_t.delta_t;
    double n_0 = sqrt(GM) / pow(nav_b[best_epoch].sqrtA, 3);
    double n = n_0 + nav_b[best_epoch].deltan;
    double tk = T_S - nav_b[best_epoch].TOE;

    if (tk > 32400)tk -= 604800;
    else if (tk < -32400)tk += 604800;

    double Ms = nav_b[best_epoch].M0 + n * tk;

    double Es = Ms, E0;
    do
    {
        E0 = Es;
        Es = Ms + nav_b[best_epoch].e * sin(Es);
    } while (fabs(Es - E0) > 1.0e-10);

    double cosfs = (cos(Es) - nav_b[best_epoch].e) / (1 - nav_b[best_epoch].e * cos(Es));
    double sinfs = (sqrt(1 - pow(nav_b[best_epoch].e, 2)) * sin(Es)) / (1 - nav_b[best_epoch].e * cos(Es));
    double fs = atan2(sinfs, cosfs);
    double u0 = fs + nav_b[best_epoch].omega;

    double epsilon_u = nav_b[best_epoch].Cus * sin(2 * u0) + nav_b[best_epoch].Cuc * cos(2 * u0);
    double epsilon_r = nav_b[best_epoch].Crs * sin(2 * u0) + nav_b[best_epoch].Crc * cos(2 * u0);
    double epsilon_i = nav_b[best_epoch].Cis * sin(2 * u0) + nav_b[best_epoch].Cic * cos(2 * u0);

    double u = u0 + epsilon_u;
    double r = pow(nav_b[best_epoch].sqrtA, 2) * (1 - nav_b[best_epoch].e * cos(Es)) + epsilon_r;
    double i = nav_b[best_epoch].i0 + epsilon_i + nav_b[best_epoch].IDOT * tk;

    double x = r * cos(u);
    double y = r * sin(u);

    double l = nav_b[best_epoch].OMEGA + (nav_b[best_epoch].deltaomega - Earth_e) * tk - Earth_e * nav_b[best_epoch].TOE;
    double X = x * cos(l) - y * cos(i) * sin(l);
    double Y = x * sin(l) + y * cos(i) * cos(l);
    double Z = y * sin(i);

    pos_t.X = cos(Earth_e * pos_t.delta_t) * X + sin(Earth_e * pos_t.delta_t) * Y;
    pos_t.Y = -sin(Earth_e * pos_t.delta_t) * X + cos(Earth_e * pos_t.delta_t) * Y;
    pos_t.Z = Z;//Correction of earth rotation

    double R = sqrt(pow(pos_t.X - obs_h->apX, 2) + pow(pos_t.Y - obs_h->apY, 2) + pow(pos_t.Z - obs_h->apZ, 2));
    pos_t.deltat = pos_t.delta_t;
    double rela = 2 * sqrt(GM) * nav_b[best_epoch].e * nav_b[best_epoch].sqrtA * sin(Es) / pow(C_V, 2);//Relativistic effect
    pos_t.delta_clk = nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * tk + nav_b[best_epoch].sa2 * pow(tk, 2) - rela - nav_b[best_epoch].BGDa;
    pos_t.delta_t = R / C_V + pos_t.delta_clk;

    return pos_t;}


/* -------------------- BDS Satellite position calculate -------------------- */
pos_ts bds_pos(int sPRN, int best_epoch, double BDSsec,
                pnav_body nav_b, pobs_head obs_h, pos_ts pos_t)
{   double T_S = BDSsec - pos_t.delta_t;
    double n_0 = sqrt(GM) / pow(nav_b[best_epoch].sqrtA, 3);
    double n = n_0 + nav_b[best_epoch].deltan;
    double tk = T_S - nav_b[best_epoch].TOE - 14;

    if (tk > 32400)tk -= 604800;
    else if (tk < -32400)tk += 604800;
    double Ms = nav_b[best_epoch].M0 + n * tk;

    double Es = Ms, E0;
    do
    {
        E0 = Es;
        Es = Ms + nav_b[best_epoch].e * sin(Es);
    } while (fabs(Es - E0) > 1.0e-10);

    double cosfs = (cos(Es) - nav_b[best_epoch].e) / (1 - nav_b[best_epoch].e * cos(Es));
    double sinfs = (sqrt(1 - pow(nav_b[best_epoch].e, 2)) * sin(Es)) / (1 - nav_b[best_epoch].e * cos(Es));
    double fs = atan2(sinfs, cosfs);
    double u0 = fs + nav_b[best_epoch].omega;

    double epsilon_u = nav_b[best_epoch].Cus * sin(2 * u0) + nav_b[best_epoch].Cuc * cos(2 * u0);
    double epsilon_r = nav_b[best_epoch].Crs * sin(2 * u0) + nav_b[best_epoch].Crc * cos(2 * u0);
    double epsilon_i = nav_b[best_epoch].Cis * sin(2 * u0) + nav_b[best_epoch].Cic * cos(2 * u0);

    double u = u0 + epsilon_u;
    double r = pow(nav_b[best_epoch].sqrtA, 2) * (1 - nav_b[best_epoch].e * cos(Es)) + epsilon_r;
    double i = nav_b[best_epoch].i0 + epsilon_i + nav_b[best_epoch].IDOT * tk;

    double x = r * cos(u);
    double y = r * sin(u);

    double l, X, Y, Z;
    if (sPRN > 5 && sPRN < 59) //BDS IGSO & MEO Satellites
    {
        l = nav_b[best_epoch].OMEGA + (nav_b[best_epoch].deltaomega - Earth_e) * tk - Earth_e * nav_b[best_epoch].TOE;
        X = x * cos(l) - y * cos(i) * sin(l);
        Y = x * sin(l) + y * cos(i) * cos(l);
        Z =              y * sin(i);}

    else // BDS GEO Satellites
    {
        l = nav_b[best_epoch].OMEGA + nav_b[best_epoch].deltaomega * tk - Earth_e * nav_b[best_epoch].TOE;
        X = x * cos(l) - y * cos(i) * sin(l);
        Y = x * sin(l) + y * cos(i) * cos(l);
        Z = y * sin(i);

        double f = deg2rad(-5);
        double p = Earth_e * tk;

        X = X *  cos(p) + Y * sin(p) * cos(f) + Z * sin(p) * sin(f);
        Y = X * -sin(p) + Y * cos(p) * cos(f) + Z * cos(p) * sin(f);
        Z = Y * -sin(f) + Z * cos(f);}
        
    pos_t.X =  cos(Earth_e * pos_t.delta_t) * X + sin(Earth_e * pos_t.delta_t) * Y;
    pos_t.Y = -sin(Earth_e * pos_t.delta_t) * X + cos(Earth_e * pos_t.delta_t) * Y;
    pos_t.Z = Z;//Correction of earth rotation

    double rela = 2 * sqrt(GM) * nav_b[best_epoch].e * nav_b[best_epoch].sqrtA * sin(Es) / pow(C_V, 2);//Relativistic effect
    pos_t.delta_clk = nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * tk + nav_b[best_epoch].sa2 * pow(tk, 2) - rela;
    double R = sqrt(pow(pos_t.X - obs_h->apX, 2) + pow(pos_t.Y - obs_h->apY, 2) + pow(pos_t.Z - obs_h->apZ, 2));
    pos_t.deltat = pos_t.delta_t;
    pos_t.delta_t = R / C_V + pos_t.delta_clk;
    return pos_t;}


/* -------------------------------- Satellite position calculate & Output -------------------------------- */
void sat_pos_clac(FILE* result_file,HWND hwndPB,
    pnav_body nav_b, pobs_epoch obs_e, pobs_body obs_b,pobs_head obs_h,
    int o_epochnum, int satnum, int gnssdecode,
    char res_file[MAX_PATH], char obs_file[MAX_PATH], char nav_file[MAX_PATH],
    int ionoption, int trooption)
{
    PBRANGE range;
    SendMessage(hwndPB, PBM_SETRANGE, (WPARAM)FALSE, (LPARAM)(MAKELPARAM(0, 100)));
    SendMessage(hwndPB, PBM_GETRANGE, (WPARAM)TRUE, (LPARAM)&range);

    time_t gen_time;time(&gen_time);stations station = { 0 };xyz2blh tem2 = { 0 };
    tem2 = XYZ2BLH(tem2, obs_h->apX, obs_h->apY, obs_h->apZ);
    station.B = tem2.B;
    station.L = tem2.L;
    station.H = tem2.H;

    result_file = fopen(res_file, "w");
    fclose(result_file);
    result_file = fopen(res_file, "a+");
    fprintf(result_file, "@ GENERATE PROGRAM   : KNZ_Calculate v1.2.2\n");
    fprintf(result_file, "@ GENERATE TYPE      : Satellite  Position\n");
    fprintf(result_file, "@ GENERATE TIME      : %s", ctime(&gen_time));
    fprintf(result_file, "@ OBS FILE PATH      : %s\n", obs_file);
    fprintf(result_file, "@ NAV FILE PATH      : %s\n", nav_file);
    fprintf(result_file, "@ TIME OF FIRST OBS  : %4d %02d %02d %02d %02d %07.4f\n"
            , obs_h->f_y, obs_h->f_m, obs_h->f_d, obs_h->f_h, obs_h->f_min, obs_h->f_sec);
    fprintf(result_file, "@ TIME OF LAST OBS   : %4d %02d %02d %02d %02d %07.4f\n"
            , obs_h->l_y, obs_h->l_m, obs_h->l_d, obs_h->l_h, obs_h->l_min, obs_h->l_sec);
    fprintf(result_file, "@ APPROX POSITION XYZ: %13.04f%13.04f%13.04f\n@ APPROX POSITION BLH:  %12.07f %12.07f %12.07f\n"
            , obs_h->apX, obs_h->apY, obs_h->apZ
            , station.B, station.L, station.H);

    if (ionoption == 0) { fprintf(result_file, "@ IONOS OPT          : NULL\n"); }
    if (ionoption == 1) { fprintf(result_file, "@ IONOS OPT          : DEFAULT\n"); }
    if (trooption == 0) { fprintf(result_file, "@ TROPO OPT          : NULL\n@\n\n"); }
    if (trooption == 1) { fprintf(result_file, "@ TROPO OPT          : DEFAULT\n@\n\n"); }
    fclose(result_file);

    //the beginning second of observation
    double beginsec = Time2Weeksec(obs_h->f_y, obs_h->f_m, obs_h->f_d, obs_h->f_h, obs_h->f_min, obs_h->f_sec);
    //the ending second of observation
    double endinsec = Time2Weeksec(obs_h->l_y, obs_h->l_m, obs_h->l_d, obs_h->l_h, obs_h->l_min, obs_h->l_sec);

    for (int i = 0; i < o_epochnum; i++)
    {   int y = obs_e[i].y;
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

        double Weeksec = Time2Weeksec(y, m, d, h, min, sec);
        int epochcount = round((Weeksec - beginsec) / obs_h->interval) + 1;

        result_file = fopen(res_file, "a+");
        fprintf(result_file, "\n>%04d %04d %02d %02d %02d %02d %07.04f"
                , epochcount, obs_e[i].y, obs_e[i].m, obs_e[i].d, obs_e[i].h, obs_e[i].min, obs_e[i].sec);
        fclose(result_file);

        if (gnssdecode % GPS == 0)//Be judged as GPS
        {
            for (int j = 0; j < obs_e[i].gps_num; j++)
            {
                int sPRN = obs_e[i].sPRN_GPS[j];
                int best_epoch = select_epoch(Weeksec, sPRN, nav_b, satnum, GPS);

                if (best_epoch == -1)//when the sPRN is not exist func will return -1
                    break;

                double detat_toc = Weeksec - nav_b[best_epoch].TOE;

                //Iterative approximate propagation time
                pos_t.delta_t = (obs_b[i].obs_gps[j][Code2Type(C1C, obs_h->obstypenum_gps, obs_h->obscode_gps)] / C_V)
                    - station.delta_TR + nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * detat_toc + nav_b[best_epoch].sa2 * pow(detat_toc, 2);
                pos_t.deltat = 0.0;
                while (fabs(pos_t.delta_t - pos_t.deltat) > 1.0e-9)
                {
                    pos_t = gps_pos(sPRN, best_epoch, Weeksec, nav_b, obs_h, pos_t);
                }

                tem1 = XYZ2BLH(tem1, pos_t.X, pos_t.Y, pos_t.Z);
                blh.B = rad2deg(tem1.B);
                blh.L = rad2deg(tem1.L);
                blh.H = tem1.H;

                double deltax = pos_t.X - obs_h->apX;
                double deltay = pos_t.Y - obs_h->apY;
                double deltaz = pos_t.Z - obs_h->apZ;

                tem3 = BLH2ENU(tem3, station.B, station.L, deltax, deltay, deltaz);
                enu.E = tem3.E;
                enu.N = tem3.N;
                enu.U = tem3.U;

                //Satellite altitude Angle calculation
                tem4 = RAHCAL(tem4, enu.E, enu.N, enu.U);
                rah.R = tem4.R;
                rah.A = rad2deg(tem4.A);
                rah.H = rad2deg(tem4.H);

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
                    fprintf(result_file, "\nG %02d| %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %16.13f"
                            , sPRN, pos_t.X, pos_t.Y, pos_t.Z
                            , obs_b[i].obs_gps[j][Code2Type(C1C, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                            , obs_b[i].obs_gps[j][Code2Type(C2L, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                            , rah.H, pos_t.delta_clk);//GPS satellite
                    fclose(result_file);
                }
            }
        }

        if (gnssdecode % BDS == 0)//Be judged as BeiDou
        {
            for (int j = 0; j < obs_e[i].bds_num; j++)
            {
                int sPRN = obs_e[i].sPRN_BDS[j];
                int best_epoch = select_epoch(Weeksec, sPRN, nav_b, satnum, BDS);

                if (best_epoch == -1)//when the sPRN is not exist func will return -1
                    break;

                double detat_toc = Weeksec - nav_b[best_epoch].TOE - 14;//BDST - GPST = 14s

                //Iterative approximate propagation time
                pos_t.delta_t = (obs_b[i].obs_bds[j][Code2Type(C2I, obs_h->obstypenum_bds, obs_h->obscode_bds)] / C_V)
                    - station.delta_TR + nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * detat_toc + nav_b[best_epoch].sa2 * pow(detat_toc, 2);
                pos_t.deltat = 0.0;
                while (fabs(pos_t.delta_t - pos_t.deltat) > 1.0e-9)
                {
                pos_t = bds_pos(sPRN, best_epoch, Weeksec, nav_b, obs_h, pos_t);
                }

                tem1 = XYZ2BLH(tem1, pos_t.X, pos_t.Y, pos_t.Z);
                blh.B = rad2deg(tem1.B);
                blh.L = rad2deg(tem1.L);
                blh.H = tem1.H;

                double deltax = pos_t.X - obs_h->apX;
                double deltay = pos_t.Y - obs_h->apY;
                double deltaz = pos_t.Z - obs_h->apZ;

                tem3 = BLH2ENU(tem3, station.B, station.L, deltax, deltay, deltaz);
                enu.E = tem3.E;
                enu.N = tem3.N;
                enu.U = tem3.U;

                //Satellite altitude Angle calculation
                tem4 = RAHCAL(tem4, enu.E, enu.N, enu.U);
                rah.R = tem4.R;
                rah.A = rad2deg(tem4.A);
                rah.H = rad2deg(tem4.H);

                if (blh.H < 0 || rah.H < 10 || nav_b[best_epoch].sHEA != 0){
                    continue;
                }

                else{
                    temb = DEG2DMS(temb, blh.B);
                    blh.B_d = temb.D;
                    blh.B_m = temb.M;
                    blh.B_s = temb.S;

                    teml = DEG2DMS(teml, blh.L);
                    blh.L_d = teml.D;
                    blh.L_m = teml.M;
                    blh.L_s = teml.S;

                    result_file = fopen(res_file, "a+");
                    if (sPRN >= 19) {
                        fprintf(result_file, "\nCC%02d| %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %16.13f"
                                , sPRN, pos_t.X, pos_t.Y, pos_t.Z
                                , obs_b[i].obs_bds[j][Code2Type(C2I, obs_h->obstypenum_bds, obs_h->obscode_bds)]
                                , obs_b[i].obs_bds[j][Code2Type(C7I, obs_h->obstypenum_bds, obs_h->obscode_bds)]
                                , rah.H, pos_t.delta_clk);//BDS-3 satellite
                    }
                    else {
                        fprintf(result_file, "\nCB%02d| %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %16.13f"
                                , sPRN, pos_t.X, pos_t.Y, pos_t.Z
                                , obs_b[i].obs_bds[j][Code2Type(C2I, obs_h->obstypenum_bds, obs_h->obscode_bds)]
                                , obs_b[i].obs_bds[j][Code2Type(C7I, obs_h->obstypenum_bds, obs_h->obscode_bds)]
                                , rah.H, pos_t.delta_clk);//BDS-2 satellite
                    }
                    fclose(result_file);
                }
            }
        }

        if (gnssdecode % GAL == 0)//Be judged as Galileo
        {
            for (int j = 0; j < obs_e[i].gal_num; j++)
            {
                int sPRN = obs_e[i].sPRN_GAL[j];
                int best_epoch = select_epoch(Weeksec, sPRN, nav_b, satnum, GAL);

                if (best_epoch == -1)//when the sPRN is not exist func will return -1
                    break;

                double detat_toc = Weeksec - nav_b[best_epoch].TOE;

                //Iterative approximate propagation time
                pos_t.delta_t = (obs_b[i].obs_gal[j][Code2Type(C1C, obs_h->obstypenum_gal, obs_h->obscode_gal)] / C_V)
                        - station.delta_TR + nav_b[best_epoch].sa0 + nav_b[best_epoch].sa1 * detat_toc + nav_b[best_epoch].sa2 * pow(detat_toc, 2);
                pos_t.deltat = 0.0;
                while (fabs(pos_t.delta_t - pos_t.deltat) > 1.0e-9)
                {
                    pos_t = gal_pos(sPRN, best_epoch, Weeksec, nav_b, obs_h, pos_t);
                }

                tem1 = XYZ2BLH(tem1, pos_t.X, pos_t.Y, pos_t.Z);
                blh.B = rad2deg(tem1.B);
                blh.L = rad2deg(tem1.L);
                blh.H = tem1.H;

                double deltax = pos_t.X - obs_h->apX;
                double deltay = pos_t.Y - obs_h->apY;
                double deltaz = pos_t.Z - obs_h->apZ;

                tem3 = BLH2ENU(tem3, station.B, station.L, deltax, deltay, deltaz);
                enu.E = tem3.E;
                enu.N = tem3.N;
                enu.U = tem3.U;

                //Satellite altitude Angle calculation
                tem4 = RAHCAL(tem4, enu.E, enu.N, enu.U);
                rah.R = tem4.R;
                rah.A = rad2deg(tem4.A);
                rah.H = rad2deg(tem4.H);

                if (blh.H < 0 || rah.H < 10 || nav_b[best_epoch].sHEA != 0)
                {   continue;}

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
                    fprintf(result_file, "\nE %02d| %15.05f %15.05f %15.05f %15.05f %15.05f %15.05f %16.13f"
                            , sPRN, pos_t.X, pos_t.Y, pos_t.Z
                            , obs_b[i].obs_gal[j][Code2Type(C1C, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                            , obs_b[i].obs_gal[j][Code2Type(C6C, obs_h->obstypenum_gps, obs_h->obscode_gps)]
                            , rah.H, pos_t.delta_clk);//Galileo satellite
                    fclose(result_file);
                }
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