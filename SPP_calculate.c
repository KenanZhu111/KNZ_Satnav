#include<stdio.h>
#include<wchar.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<Commdlg.h>

#include"headers/public.h"
#include"headers/read.h"
#include"headers/cal_mod.h" 

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//FILE SELECT DIALOGS
#define FILEEXOP  TEXT("Obs Files(*.o*)\0*.20o;*.21o;*.22o;*.23o;*.24o\0"\
					 "All Files(*.*)\0*.*\0\0");

#define FILEEXNP  TEXT("Nav Files(*.p*)\0*.20p;*.21p;*.22p;*.23p;*.24p\0"\
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
#define NAVCLEAR	3307
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

HWND hwndb[13];//BUTTON HANDLE
HWND hwndopt;//OPTIONS HANDLE
HWND hwndopt_def_ion;//DEFAULT ION COR
HWND hwndopt_def_tro;//DEFAULT TRO COR
HWND hwndopt_confirm;//OPTION CONFIRM


FILE * fp_nav      = NULL;     
FILE * fp_obs      = NULL;     
FILE * result_file = NULL;

pnav_head nav_h    = NULL;
pnav_body nav_b    = NULL;
pobs_head obs_h    = NULL;
pobs_epoch obs_e   = NULL;
pobs_body obs_b    = NULL;

int o_epochnum = 0;
int gps_satnum = 0;
int bds_satnum = 0;


/*INITIAL SETTINGS*/
static int ionoption = 0;
static int trooption = 0;
/* ------------- */


















static int FileSelobsDialog(const wchar_t* path)//FILEDIALOG FOR OBS
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = FILEEXOP;
	ofn.lpstrTitle = TEXT("Open From");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn);
}

static int FileSelnavDialog(const wchar_t* path)//FILEDIALOG FOR NAV
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = FILEEXNP;
	ofn.lpstrTitle = TEXT("Open From");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn);
}

static int FileSaveDialog(const wchar_t* path)//FILEDIALOG FOR SAVE
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrDefExt = TEXT(".sp");
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = FILEEXSV;
	ofn.lpstrTitle = TEXT("Save To");
	ofn.Flags = OFN_OVERWRITEPROMPT;
	return GetSaveFileName(&ofn);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//MAIN WINDOW
LRESULT CALLBACK OptWndProc(HWND, UINT, WPARAM, LPARAM);//OPTION WINDOW

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int iCmdShow) {
	
	WNDCLASS wndclass;
	wndclass.style = CS_PARENTDC;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("SPP");

	RegisterClass(&wndclass);

	wndclass.lpfnWndProc = OptWndProc;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("OPT");

	RegisterClass(&wndclass);

	HWND hwnd;
	hwnd = CreateWindow(
		TEXT("SPP"),
		TEXT("SPP Calculate"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		650, 
		300, 
		0, 
		0,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	HFONT hbFont;
	hbFont = CreateFont(
		15,
		0,
		0,
		0,
		FW_LIGHT,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		TEXT("Segoe UI"));

	SendMessage(hwnd, WM_SETFONT, (WPARAM)hbFont, 1);

	for (int i = 0; i < 12; i++)
	{
		SendMessage(hwndb[i], WM_SETFONT, (WPARAM)hbFont, 1);
	}

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
 
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	const wchar_t wobs_file[MAX_PATH] = { 0 };
	const wchar_t wnav_file[MAX_PATH] = { 0 };
	const wchar_t wres_file[MAX_PATH] = { 0 };
	char obs_file[MAX_PATH] = { 0 };
	char nav_file[MAX_PATH] = { 0 };
	char res_file[MAX_PATH] = { 0 };//BE COMPATIBLE WITH MSVC

	RECT rect;
	HBITMAP Delico;
	TCHAR* SATSYS[5] = { TEXT("GPS"),TEXT("GLO"),TEXT("BeiDou"),TEXT("Galileo"),TEXT("SBAS") };
	static int cxchar, cychar;

	switch (msg) {
	
	case WM_CREATE:
		cxchar = LOWORD(GetDialogBaseUnits());
		cychar = HIWORD(GetDialogBaseUnits());//GET THE SYS FONT SIZE

		GetWindowRect(hwnd, &rect);
		MoveWindow(hwnd, rect.left, rect.bottom, 50 * cxchar, 15 * cychar, TRUE);//REPAINT THE MAIN WINDOW

		//CREATE THE BUTTONS
		hwndb[0] = CreateWindow(TEXT("Button"), TEXT("Open Obs Data"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		     cxchar,  5 * cychar,
		14 * cxchar,  2 * cychar,
		hwnd, (HMENU)OBSBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndb[1] = CreateWindow(TEXT("Button"), TEXT("Open Nav Data"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		17 * cxchar,  5 * cychar,
		14 * cxchar,  2 * cychar,
		hwnd, (HMENU)NAVBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndb[2] = CreateWindow(TEXT("Button"), TEXT("Sat-Position Calculate"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  
			 cxchar,  7 * cychar,
		32 * cxchar,  2 * cychar,
		hwnd, (HMENU)CALBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndb[3] = CreateWindow(TEXT("Button"), TEXT("Options..."), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		36 * cxchar,  7 * cychar,
		10 * cxchar,  2 * cychar,
		hwnd, (HMENU)OPTBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndb[4] = CreateWindow(TEXT("Button"), TEXT("Plot"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		36 * cxchar,  5 * cychar,
		10 * cxchar,  2 * cychar,
		hwnd, (HMENU)PLOBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		for (int i = 0; i < 5; i++)
		{
			hwndb[i + 5] = CreateWindow(TEXT("Button"), SATSYS[i], WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
							 (2 + 7 * i) * cxchar,  21 * cychar/2,
							           7 * cxchar,   5 * cychar/3,
							  hwnd, (HMENU)(5500 + i), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}
		hwndb[10] = CreateWindow(TEXT("Button"), TEXT("Satellite System"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		   1 * cxchar/2, 19 * cychar/2,
		    47 * cxchar,  6 * cychar/2,
			hwnd, (HMENU)SYSGROUP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);	
		hwndb[11] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			15 * cxchar, 5 * cychar,
			 2 * cxchar, 2 * cychar,
			hwnd, (HMENU)OBSCLEAR, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndb[12] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			31 * cxchar, 5 * cychar,
			 2 * cxchar, 2 * cychar,
			hwnd, (HMENU)NAVCLEAR, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
/* -------------------------- LOAD OBS DATA BUTTON -------------------------- */
			case OBSBUTTON:
				FileSelobsDialog(wobs_file);
				wcstombs(obs_file, wobs_file, MAX_PATH);
				fp_obs = fopen(obs_file, TEXT("r"));
				if (fp_obs != NULL)
				{
					o_epochnum = get_epochnum(fp_obs);
					rewind(fp_obs);
					obs_h = (pobs_head)malloc(sizeof(obs_head));
					obs_e = (pobs_epoch)malloc(sizeof(obs_epoch) * o_epochnum);
					obs_b = (pobs_body)malloc(sizeof(obs_body) * o_epochnum);
					if (obs_h && obs_e && obs_b)
					{
						read_o_h(fp_obs, obs_h);
						read_o_b(fp_obs, obs_e, obs_b, obs_h->obstypenum_gps, obs_h->obstypenum_bds);
					}
					fclose(fp_obs);
					if (o_epochnum > 0)
					{
						MessageBox(hwnd, TEXT("File of Obs load complete!"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
						break;
					}
				}
				break;
				return 0;
/* -------------------------- LOAD NAV DATA BUTTON -------------------------- */
			case NAVBUTTON:
				FileSelnavDialog(wnav_file);
				wcstombs(nav_file, wnav_file, MAX_PATH);
				fp_nav = fopen(nav_file, TEXT("r"));
				if (fp_nav != NULL)
				{
					gps_satnum = getgpssatnum(fp_nav);
					rewind(fp_nav);
					bds_satnum = getbdssatnum(fp_nav);
					rewind(fp_nav);
					nav_h = (pnav_head)malloc(sizeof(nav_head));
					nav_b = (pnav_body)malloc(sizeof(nav_body) * (gps_satnum + bds_satnum));
					if (nav_h && nav_b)
					{
						read_n_h(fp_nav, nav_h);
						read_n_b(fp_nav, nav_b);
					}
					fclose(fp_nav);
					if (gps_satnum > 0)
					{
						MessageBox(hwnd, TEXT("File of Nav load complete!"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
						break;
					}
				}
				break;
				return 0;
/* ---------------------------- CALCAULATE BUTTON --------------------------- */
			case CALBUTTON:
				if (fp_obs == NULL && fp_nav == NULL)
				{
					MessageBox(hwnd, TEXT("No Data File!"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs == NULL && fp_nav != NULL)
				{
					MessageBox(hwnd, TEXT("No Obs Data File!"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs != NULL && fp_nav == NULL)
				{
					MessageBox(hwnd, TEXT("No Nav Data File!"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs != NULL && fp_nav != NULL)
				{
					int ALLSTATE = 0x0000;
					for (int i = 0; i < 5; i++){ALLSTATE += SendMessage(hwndb[i + 5], BM_GETCHECK, wParam, lParam);}
					if (ALLSTATE == 0x0000)
					{
						while (1)
						{
							if (MessageBox(hwnd, TEXT("No GNSS is choosen\nPlease Retry"), TEXT("NOTICE"), MB_ICONERROR | MB_RETRYCANCEL) == IDRETRY)
							{
								break;
							}
						}
					}
					else
					{
						FileSaveDialog(wres_file);
						wcstombs(res_file, wres_file, MAX_PATH);
						if (res_file[0] == '\0')
						{
							while (MessageBox(hwnd, TEXT("Failed to select a save path.\nThe data has been cleared!"), TEXT("WARNING"), MB_ICONWARNING | MB_OK) == IDOK)
							{
								break;
							}
						}
						else
						{
							if (SendMessage(hwndb[5], BM_GETCHECK, wParam, lParam) == BST_CHECKED)
							{
								sat_gps_pos_clac(result_file, nav_b, obs_e, obs_b, obs_h, o_epochnum, gps_satnum, res_file);
							}
						}
						free(obs_h); free(obs_e); free(obs_b); free(nav_h); free(nav_b);
						fp_nav = NULL; fp_obs = NULL; result_file = NULL;
						nav_h = NULL; nav_b = NULL; obs_h = NULL; obs_e = NULL; obs_b = NULL;
					}
				}
				break;
				return 0;
/* ----------------------------- OPTIONS BUTTON ----------------------------- */
			case OPTBUTTON:
				DestroyWindow(hwndopt);
				hwndopt = CreateWindow(TEXT("OPT"), TEXT("Options"), WS_CAPTION | WS_SYSMENU,
					650, 300,
					0, 0,
				NULL, NULL, GetWindowLongPtr(hwndopt, GWLP_HINSTANCE), NULL);
				HFONT hbFont;
				hbFont = CreateFont(
					15,0,0,0,
					FW_LIGHT,FALSE,FALSE,FALSE,
					DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
					CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
					VARIABLE_PITCH,TEXT("Segoe UI"));
				
					SendMessage(hwndopt_def_ion, WM_SETFONT, (WPARAM)hbFont, 1);
					SendMessage(hwndopt_def_tro, WM_SETFONT, (WPARAM)hbFont, 1);
					SendMessage(hwndopt_confirm, WM_SETFONT, (WPARAM)hbFont, 1);//SEND FONT SETTING MESSAGE
					while (ionoption == 1) { SendMessage(hwndopt_def_ion, BM_SETCHECK, wParam, lParam); break; };
					while (trooption == 1) { SendMessage(hwndopt_def_tro, BM_SETCHECK, wParam, lParam); break; };//SEND SETTING STATE MESSAGE

				break;
				return 0;
/* ------------------------------- PLOT BUTTON ------------------------------ */
			case PLOBUTTON:

				break;
				return 0;
/* ------------------------------- DEFAULT MSG ------------------------------ */
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		} 
		return 0;
	case WM_CLOSE:
		if(MessageBox(hwnd, TEXT("You're shutting the program"), TEXT("NOTICE"), MB_OKCANCEL | MB_ICONASTERISK) == IDOK )
			DestroyWindow(hwnd);
		else
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK OptWndProc(HWND hwndopt, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int cxchar, cychar;
	RECT rect;
	switch (msg)
	{
	case WM_CREATE:
		cxchar = LOWORD(GetDialogBaseUnits());
		cychar = HIWORD(GetDialogBaseUnits());
		GetWindowRect(hwndopt, &rect);
		MoveWindow(hwndopt, rect.left, rect.bottom, 30 * cxchar, 7 * cychar, TRUE);
		ShowWindow(hwndopt, SW_SHOWDEFAULT);
		UpdateWindow(hwndopt);
		hwndopt_def_ion = CreateWindow(TEXT("BUTTON"), TEXT("Use Default ION_COR"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			2 * cxchar, 0, 18 * cxchar, 2 * cychar, hwndopt, (HMENU)OPTDFION, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndopt_def_tro = CreateWindow(TEXT("BUTTON"), TEXT("Use Default TRO_COR"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			2 * cxchar, 2 * cychar, 18 * cxchar, 2 * cychar, hwndopt, (HMENU)OPTDFTRO, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndopt_confirm = CreateWindow(TEXT("BUTTON"), TEXT("Confirm"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		   20 * cxchar, 5 * cychar/2,  7 * cxchar, 3 * cychar/2, hwndopt, (HMENU)OPTCONFI, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		

		break;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case OPTDFION:

			break;
			return 0;
		case OPTDFTRO:

			break;
			return 0;
		case OPTCONFI:
			if (SendMessage(hwndopt_def_ion, BM_GETCHECK, wParam, lParam) == BST_CHECKED){ionoption = 1;}
			else{ionoption = 0;}
			if (SendMessage(hwndopt_def_tro, BM_GETCHECK, wParam, lParam) == BST_CHECKED){trooption = 1;}
			else{trooption = 0;}//GET CHECKBOX'S SETTINGS

			DestroyWindow(hwndopt);
			break;
			return 0;
		}
		break;
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwndopt);
		break;
		return 0;
	case WM_DESTROY:
		break;
		return 0;
	default:
		return DefWindowProc(hwndopt, msg, wParam, lParam);
	}
	return 0;
}