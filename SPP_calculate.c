#include<stdio.h>
#include<wchar.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<Commdlg.h>
#include<commctrl.h>
#pragma comment(lib, "comctl32")

#include"resource.h"
#include"headers/public.h"
#include"headers/read.h"
#include"headers/cal_mod.h" 

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HWND hwndb[15];//BUTTON HANDLE
HWND hwndopt;//OPTIONS HANDLE
HWND hwndopt_def_ion;//DEFAULT ION COR
HWND hwndopt_def_tro;//DEFAULT TRO COR
HWND hwndopt_confirm;//OPTION CONFIRM

FILE* fp_nav = NULL;
FILE* fp_obs = NULL;
FILE* result_file = NULL;

pnav_head nav_h = NULL;
pnav_body nav_b = NULL;
pobs_head obs_h = NULL;
pobs_epoch obs_e = NULL;
pobs_body obs_b = NULL;

/*INITIAL CONST*/
int o_epochnum = 0;
int satnum = 0;

/*INITIAL SETTINGS*/
static int ionoption = 0;
static int trooption = 0;
/* ------------- */
char obs_f[MAX_PATH] = { 0 };
char nav_f[MAX_PATH] = { 0 };

char  path[MAX_PATH];
const wchar_t wpath[MAX_PATH];//PROGRAM'S FOLDER

static void ProgressBar(HWND hwnd, int BEG, int DES)
{
	PBRANGE range;
	SendMessage(hwnd, PBM_SETRANGE, (WPARAM)FALSE, (LPARAM)(MAKELPARAM(0, 100)));
	SendMessage(hwnd, PBM_GETRANGE, (WPARAM)TRUE, (LPARAM)&range);
	SendMessage(hwnd, PBM_SETPOS, (WPARAM)BEG, (LPARAM)0);
	for (int i = BEG; i < DES; i++) {
		SendMessage(hwnd, PBM_SETPOS, (WPARAM)(int)( i / 10), (LPARAM)0);
		if (SendMessage(hwnd, PBM_GETPOS, (WPARAM)0, (LPARAM)0) == range.iHigh * DES/PBSTATEFULL)
		{
			break;
		}
	}
	SendMessage(hwnd, PBM_SETPOS, (WPARAM)DES, (LPARAM)0);
}

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

//DWORD WINAPI PBThreadProc(LPVOID lpParameter);//PROGRESS BAR WINDOW
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
	wndclass.lpszClassName = TEXT("CAL");

	RegisterClass(&wndclass);

	wndclass.lpfnWndProc = OptWndProc;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("OPT");

	RegisterClass(&wndclass);

	HWND hwnd;
	hwnd = CreateWindow(
		TEXT("CAL"),
		TEXT("KNZ_Calculate"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 
		0, 
		0, 
		0,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	HICON Delico,Relico;
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
	Delico = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
	Relico = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON4));
	SendMessage(hwndb[11], BM_SETIMAGE, IMAGE_ICON, (LPARAM)Delico);
	SendMessage(hwndb[12], BM_SETIMAGE, IMAGE_ICON, (LPARAM)Relico);
	SendMessage(hwndb[13], BM_SETIMAGE, IMAGE_ICON, (LPARAM)Delico);
	SendMessage(hwndb[14], BM_SETIMAGE, IMAGE_ICON, (LPARAM)Relico);

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
	TCHAR* SATSYS[5] = { TEXT("GPS"),TEXT("GLO"),TEXT("BeiDou"),TEXT("Galileo"),TEXT("SBAS") };
	static HWND hwndPB;
	static int cxchar, cychar;

	switch (msg) {
	
	case WM_CREATE:
	{
		getcwd(path, sizeof(path));//GET THE PROGRAM'S FOLDER
		const char plot[] = "KNZ_Plot.exe";
		sprintf(path, "%s\\%s", path, plot);
		
		cxchar = LOWORD(GetDialogBaseUnits());
		cychar = HIWORD(GetDialogBaseUnits());//GET THE SYS FONT SIZE

		MoveWindow(hwnd, (GetSystemMetrics(SM_CXSCREEN) - 50 * cxchar) / 2, (GetSystemMetrics(SM_CYSCREEN) - 16 * cychar) / 2, 50 * cxchar, 16 * cychar, TRUE);//REPAINT THE MAIN WINDOW

		InitCommonControls();
		hwndPB = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
			0.5 * cxchar, 12.25 * cychar,
			47 * cxchar, 1 * cychar,
			hwnd, (HMENU)PROGRESS, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		//CREATE THE BUTTONS
		hwndb[0] = CreateWindow(TEXT("Button"), TEXT("Open Obs Data"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			cxchar, 5 * cychar,
			14 * cxchar, 2 * cychar,
			hwnd, (HMENU)OBSBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[1] = CreateWindow(TEXT("Button"), TEXT("Open Nav Data"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			17 * cxchar, 5 * cychar,
			14 * cxchar, 2 * cychar,
			hwnd, (HMENU)NAVBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[2] = CreateWindow(TEXT("Button"), TEXT("Sat-Position Calculate"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			cxchar, 7 * cychar,
			32 * cxchar, 2 * cychar,
			hwnd, (HMENU)CALBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[3] = CreateWindow(TEXT("Button"), TEXT("Options..."), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			36 * cxchar, 7 * cychar,
			10 * cxchar, 2 * cychar,
			hwnd, (HMENU)OPTBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[4] = CreateWindow(TEXT("Button"), TEXT("Plot"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			36 * cxchar, 5 * cychar,
			10 * cxchar, 2 * cychar,
			hwnd, (HMENU)PLOBUTTON, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		for (int i = 0; i < 5; i++)
		{
			hwndb[i + 5] = CreateWindow(TEXT("Button"), SATSYS[i], WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
				(2 + 7 * i) * cxchar, 21 * cychar / 2,
				7 * cxchar, 5 * cychar / 3,
				hwnd, (HMENU)(5500 + i), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}
		hwndb[10] = CreateWindow(TEXT("Button"), TEXT("Satellite System"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
			1 * cxchar / 2, 19 * cychar / 2,
			47 * cxchar, 6 * cychar / 2,
			hwnd, (HMENU)SYSGROUP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		hwndb[11] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_ICON | BS_PUSHBUTTON,
			15 * cxchar, 5 * cychar,
			2 * cxchar, 1 * cychar,
			hwnd, (HMENU)OBSCLEAR, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[12] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_ICON | BS_PUSHBUTTON,
			15 * cxchar, 6 * cychar,
			2 * cxchar, 1 * cychar,
			hwnd, (HMENU)OBSRELOD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[13] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_ICON | BS_PUSHBUTTON,
			31 * cxchar, 5 * cychar,
			2 * cxchar, 1 * cychar,
			hwnd, (HMENU)NAVCLEAR, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndb[14] = CreateWindow(TEXT("Button"), NULL, WS_VISIBLE | WS_CHILD | BS_ICON | BS_PUSHBUTTON,
			31 * cxchar, 6 * cychar,
			2 * cxchar, 1 * cychar,
			hwnd, (HMENU)NAVRELOD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		return 0;
	}
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
/* -------------------------- LOAD OBS DATA BUTTON -------------------------- */
			case OBSBUTTON:
				if (fp_obs == NULL)
				{
					FileSelobsDialog(wobs_file);
					wcstombs(obs_file, wobs_file, MAX_PATH);
					fp_obs = fopen(obs_file, TEXT("r"));
					if (fp_obs != NULL)
					{
						strcpy(obs_f, obs_file);
						o_epochnum = get_epochnum(fp_obs);
						rewind(fp_obs);
						obs_h = (pobs_head)malloc(sizeof(obs_head));
						obs_e = (pobs_epoch)malloc(sizeof(obs_epoch) * o_epochnum);
						obs_b = (pobs_body)malloc(sizeof(obs_body) * o_epochnum);
						if (obs_h && obs_e && obs_b)
						{
							read_o_h(fp_obs, obs_h);
							ProgressBar(hwndPB, PBSTATENONE, PBSTATEHALF);
							read_o_b(fp_obs, obs_e, obs_b, obs_h->obstypenum_gps, obs_h->obstypenum_bds);
						}
						fclose(fp_obs);

						ProgressBar(hwndPB, PBSTATEHALF, PBSTATEFULL);
					}
				}
				else
				{
					MessageBox(hwnd, TEXT("File of Obs has already been loaded ! \nTo load new data file of Obs, \nPlease clear the existing data first"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
				}
				break;
				return 0;
/* -------------------------- LOAD NAV DATA BUTTON -------------------------- */
			case NAVBUTTON:
				if (fp_nav == NULL)
				{
					FileSelnavDialog(wnav_file);
					wcstombs(nav_file, wnav_file, MAX_PATH);
					fp_nav = fopen(nav_file, TEXT("r"));
					if (fp_nav != NULL)
					{
						strcpy(nav_f, nav_file);
						satnum = getsatnum(fp_nav);
						rewind(fp_nav);
						nav_h = (pnav_head)malloc(sizeof(nav_head));
						nav_b = (pnav_body)malloc(sizeof(nav_body) * (satnum));
						if (nav_h && nav_b)
						{
							read_n_h(fp_nav, nav_h);
							ProgressBar(hwndPB, PBSTATENONE, PBSTATEHALF);
							read_n_b(fp_nav, nav_b);
						}
						fclose(fp_nav);
						ProgressBar(hwndPB, PBSTATEHALF, PBSTATEFULL);
					}
				}
				else
				{
					MessageBox(hwnd, TEXT("File of Nav has already been loaded ! \nTo load new data file of Nav, \nPlease clear the existing data first"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
				}
				break;
				return 0;
/* -------------------------- CLEAR OBS DATA BUTTON ------------------------- */
			case OBSCLEAR:
				while (fp_obs != NULL)
				{
					free(obs_h); free(obs_e); free(obs_b);
					fp_obs = NULL; obs_e = NULL; obs_b = NULL;
					while (MessageBox(hwnd, TEXT("File of Obs has been cleared."), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK) == IDOK)
					{
						break;
					}
				}	
				break;
				return 0;

			case OBSRELOD:
				if (fp_obs != NULL)
				{
					if (o_epochnum > 0)
					{
						MessageBox(hwnd, TEXT("File of Obs Reload complete !"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
						break;
					}
				}
				break;
				return 0;
/* -------------------------- CLEAR NAV DATA BUTTON ------------------------- */
			case NAVCLEAR:
				while(fp_nav != NULL)
				{
					free(nav_h); free(nav_b);
					fp_nav = NULL;nav_h = NULL; nav_b = NULL;
					while (MessageBox(hwnd, TEXT("File of Nav has been cleared."), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK) == IDOK)
					{
						break;
					}
				}
				break;
				return 0;

			case NAVRELOD:
				if (fp_nav != NULL)
				{
					if (satnum > 100)
					{
						MessageBox(hwnd, TEXT("File of Nav Reload complete !"), TEXT("NOTICE"), MB_ICONASTERISK | MB_OK);
						break;
					}
				}
				break;
				return 0;
/* ---------------------------- CALCAULATE BUTTON --------------------------- */
			case CALBUTTON:
				if (fp_obs == NULL && fp_nav == NULL)
				{
					MessageBox(hwnd, TEXT("No Data File !"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs == NULL && fp_nav != NULL)
				{
					MessageBox(hwnd, TEXT("No Obs Data File !"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs != NULL && fp_nav == NULL)
				{
					MessageBox(hwnd, TEXT("No Nav Data File !"), TEXT("STOP"), MB_ICONERROR | MB_OKCANCEL | MB_DEFBUTTON1);
				}
				else if (fp_obs != NULL && fp_nav != NULL)
				{
					int ALLSTATE = 0x0000;
					for (int i = 0; i < 5; i++){ALLSTATE += SendMessage(hwndb[i + 5], BM_GETCHECK, wParam, lParam);}
					if (ALLSTATE == 0x0000)
					{
						while (MessageBox(hwnd, TEXT("No GNSS is choosen\nPlease Retry"), TEXT("STOP"), MB_ICONERROR | MB_RETRYCANCEL) == IDRETRY)
						{
								break;
						}
						
					}
					else
					{
						FileSaveDialog(wres_file);
						wcstombs(res_file, wres_file, MAX_PATH);
						if (res_file[0] == '\0')
						{
							while (MessageBox(hwnd, TEXT("Failed to select a save path.\nThe data has been cleared !"), TEXT("WARNING"), MB_ICONWARNING | MB_OK) == IDOK)
							{
								break;
							}
						}
						else
						{
							if (SendMessage(hwndb[5], BM_GETCHECK, wParam, lParam) == BST_CHECKED)
							{

								sat_gps_pos_clac(result_file, hwndPB, 
									nav_b, obs_e, obs_b, obs_h, 
									o_epochnum, satnum, 
									res_file, obs_f, nav_f, 
									ionoption, trooption);

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
					0, 0,
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
			case PLOBUTTON: {
				
				mbstowcs(wpath, path, MAX_PATH);

				if (ShellExecute(NULL, TEXT("open"), wpath, NULL, NULL, SW_SHOW) <= 32)
				{
					MessageBox(hwnd, TEXT("Failed to open Plot module !\nThe file may be removed or be damaged.\n "), TEXT("ERROR"), MB_ICONERROR | MB_OK);
				}

				break;
				return 0;
			}
/* ------------------------------- DEFAULT MSG ------------------------------ */
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		} 
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
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

		MoveWindow(hwndopt, (GetSystemMetrics(SM_CXSCREEN) - 30 * cxchar) / 2, (GetSystemMetrics(SM_CYSCREEN) - 7 * cychar) / 2, 30 * cxchar, 7 * cychar, TRUE);
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