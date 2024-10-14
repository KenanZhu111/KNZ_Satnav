#ifndef _PUBLIC_H_
#define _PUBLIC_H_

/* --------------------------------- FOR UI --------------------------------- */
//FILE SELECT DIALOGS
#define FILEEXOP  TEXT("RINEX OBS(*.o*.*.*obs.*.*d)\0*.*o;*.*obs;*.*d\0"\
					 "All Files(*.*)\0*.*\0\0");

#define FILEEXNP  TEXT("RINEX NAV(*.*nav.*.hnav.*.gnav.*.qnav.*.*n.*.*g.*.*h.*.*q.*.*p)\0*.*nav;*.hnav;*.gnav;*.qnav;*.*n;*.*g;*.*h;*.*q;*.*p\0"\
					 "All Files(*.*)\0*.*\0\0")

#define FILEEXSV  TEXT("Spos Files(*.sp*)\0*.sp\0"\
					 "All Files(*.*)\0*.*\0\0");

//DEF MAIN WINDOW BUTTONS
#define OBSBUTTON	3301
#define NAVBUTTON	3302
#define CALBUTTON	3303
#define OPTBUTTON	3304
#define PLOBUTTON	3305
#define OBSCLEAR	3306
#define OBSRELOD	3307
#define NAVCLEAR	3308
#define NAVRELOD	3309
#define PROGRESS	3310
//DEF MAIN WINDOW GNSS NUTTONS
#define GPSCHECK	5500
#define GLOCHECK	5501
#define BEICHECK	5502
#define GALCHECK	5503
#define SBACHECK	5504
#define SYSGROUP	1001
//DEF OPTIONS WINDOW CHECK+BOXS
#define OPTDFION	7001
#define OPTDFTRO	7002
#define OPTCONFI	7003

//DEFINE PROGRESSBAR STATE
#define PBSTATENONE	0
#define PBSTATEHALF	500
#define PBSTATEFULL	1000


/* ------------------------------ WGS-84 CONST ------------------------------ */
#define a       6378137.0
#define e2      (0.0033528106647475*(2 - 0.0033528106647475))
#define C_V     299792458
#define GM      398600500000000
#define math_e  2.718281828459
#define PI      3.141592653589793
#define Earth_e 7.2921151467e-5


/* ----------------------------- GNSS OBSERV TYPE --------------------------- */
#define C1C     100
#define C1S     101
#define C1L     102
#define C1X     103
#define C1P     104
#define C1W     105
#define C1Y     106
#define C1M     107
#define C1A		108
#define C1B		109
#define C1Z		110

#define C2C     200
#define C2S     201
#define C2L     202
#define C2X     203
#define C2P     204
#define C2W     205
#define C2Y     206
#define C2M     207
#define C2I		208
#define C2Q     209
#define C2X     210

#define C5I     500
#define C5Q     501
#define C5X     502

#define C6A		600
#define C6B		601
#define C6C		602
#define C6X		603
#define C6Z		604

#define C7I		700
#define C7Q		701
#define C7X		702
#define C7D		703
#define C7P		704
#define C7Z		705

#define C8I		800
#define C8Q		801
#define C8X		802

/* ------------------------------- GNSS CODE -------------------------------- */
#define GPS		 03
#define BDS		 07
#define GAL		 11
#define GLO		 13
#define SBAS	 17


/* ------------------------------ FOR CALMOD.H ------------------------------ */
#define MAXRINEX 1000
#define DAYSEC	 86400

//satellite position & clock bias structure
typedef struct
{
	double X;
	double Y;
	double Z;
	double deltat;
	double delta_t;
	double delta_clk;
}pos_ts;

//receiver station position & clock bias structure
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

//longitude & latitude structure
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

//local Cartesian coordinates coordinate system structure
typedef struct
{
	double E;
	double N;
	double U;
}enus;

//satellite azimuth angle structure
typedef struct
{
	double R;
	double A;
	double H;
}rahs;
/* ------------------------------- FOR READ.H ------------------------------- */

//header data of file of nav
typedef struct nav_head
{
	double ver;//rinex 
	char type[20];
	double ION_GPSA[4];
	double ION_GPSB[4];
	double ION_BDSA[4];
	double ION_BDSB[4];
	double ION_QZSA[4];
	double ION_QZSB[4];
	double ION_IRNA[4];
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

//body data of file of nav: GPS	BeiDou	Galileo
typedef struct nav_body
{
	int sPRN_GPS;	int sPRN_BDS;	int sPRN_GAL;
	int TOC_Y;
	int TOC_M;
	int TOC_D;
	int TOC_H;
	int TOC_Min;
	int TOC_Sec;
	double sa0;
	double sa1;
	double sa2;
 
	double IODE;	double AODE;
	double Crs;
	double deltan;
	double M0;
 
	double Cuc;
	double e;
	double Cus;
	double sqrtA;
 
	double TOE;
	double Cic;
	double OMEGA;
	double Cis;
 
	double i0;
	double Crc;
	double omega;
	double deltaomega;
 
	double IDOT;
	double L2code;				    int Datasource;
	double GPSweek; double BDTweek; double GALweek;
	double L2Pflag;
 
	double sACC;
	double sHEA;
	double TGD;		double TGD1;	double BGDa;
	double IODC;	double TGD2;	double BGDb;
 
	double TTN;
	double fit;		double AODC;
	double spare1;
	double spare2;
 
}nav_body, *pnav_body;

//header data of file of obs
typedef struct obs_head
{
	double ver;
	char type[30];
	double apX;
	double apY;
	double apZ;
	double ANTH;
	double ANTdeltaE;
	double ANTdeltaN;
	int obstypenum_gps;
	int obstypenum_bds;
	int obstypenum_gal;
	int obscode_gps[26];
	int obscode_bds[26];
	int obscode_gal[26];
	double interval;
	int f_y;
	int f_m;
	int f_d;
	int f_h;
	int f_min;
	double f_sec;
	int l_y;
	int l_m;
	int l_d;
	int l_h;
	int l_min;
	double l_sec;
	char tsys[5];//time system
}obs_head,*pobs_head;

//epoch data of file of obs
typedef struct obs_epoch
{
	int y;
	int m;
	int d;
	int h;
	int min;
	double sec;//epoch time
	int p_flag;//epoch flag
	int sat_num;//the total number of obsable sat
	int gps_num;//the total gps sat
	int bds_num;//the total bds sat
	int gal_num;//the total galileo sat
	int sPRN[84];//total sprn of obs sat
	int sPRN_GPS[30];
	int sPRN_BDS[60];
	int sPRN_GAL[30];
}obs_epoch,*pobs_epoch;

//body data of file of obs
typedef struct obs_body
{
	double obs_gps[30][26];//GPS obs data
	double obs_bds[60][26];//BDS obs data
	double obs_gal[30][26];//GALILEO obs data
}obs_body,*pobs_body;
/* -------------------------------------------------------------------------- */

#endif