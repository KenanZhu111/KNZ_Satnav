/*
 * @Author: KenanZhu111 3471685733@qq.com
 * @Date: 2024-09-27 19:05:41
 * @LastEditors: KenanZhu111 3471685733@qq.com
 * @LastEditTime: 2024-09-27 19:55:46
 * @FilePath: \Sat_Position_Solving_ver1.4.7\maingui.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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

#define FILEEXOP  TEXT("All Files(*.*)\0*.*\0"\
					 "Obs Files(*.o*)\0*.20o;*.21o;*.22o;*.23o;*.24o\0"\
					 "Nav Files(*.p*)\0*.20p;*.21p;*.22p;*.23p;*.24p\0\0");
#define FILEEXSV  TEXT("All Files(*.*)\0*.*\0"\
					 "Spos Files(*.sp*)\0*.sp\0\0");


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int iCmdShow) {
	
	
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("SPP");

	RegisterClass(&wndclass);

	HWND hwnd;
	hwnd = CreateWindow(
		TEXT("SPP"),
		TEXT("SPP Calculate"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		650, 
		300, 
		0, 
		0,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, 1);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
 
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

int FileSelectDialog(const wchar_t *path)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = FILEEXOP;
	ofn.lpstrTitle = TEXT("Open From");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_SHOWHELP;
	return GetOpenFileName(&ofn);
}

int FileSaveDialog(const wchar_t* path)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrDefExt = TEXT(".sp");
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = FILEEXSV;
	ofn.lpstrTitle = TEXT("Save To");
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_SHOWHELP;
	return GetSaveFileName(&ofn);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	const wchar_t wobs_file[MAX_PATH] = { 0 };
	const wchar_t wnav_file[MAX_PATH] = { 0 };
	const wchar_t wres_file[MAX_PATH] = { 0 };
	char obs_file[MAX_PATH] = { 0 };
	char nav_file[MAX_PATH] = { 0 };
	char res_file[MAX_PATH] = { 0 };

	HWND hwndopt;

	RECT rect;
	TCHAR* SATSYS[5] = { TEXT("GPS"), TEXT("GLO"),TEXT("SBAS"), TEXT("BeiDou"), TEXT("Galileo") };
	static int cxchar, cychar;
	switch (msg) {
	
	case WM_CREATE:
		cxchar = LOWORD(GetDialogBaseUnits());
		cychar = HIWORD(GetDialogBaseUnits());

		GetWindowRect(hwnd, &rect);
		MoveWindow(hwnd, rect.left, rect.bottom, 50 * cxchar, 15 * cychar, TRUE);

		CreateWindow(TEXT("Button"), TEXT("Load Obs file"), WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,  
		     cxchar,  5 * cychar,
		16 * cxchar,  2 * cychar,
		hwnd, (HMENU)3301, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("Button"), TEXT("Load Nav file"), WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,  
		17 * cxchar,  5 * cychar,
		16 * cxchar,  2 * cychar,
		hwnd, (HMENU)3302, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("Button"), TEXT("Sat-Position Calculate"), WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,  
			 cxchar,  7 * cychar,
		32 * cxchar,  2 * cychar,
		hwnd, (HMENU)3303, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("Button"), TEXT("Options..."), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		36 * cxchar,  7 * cychar,
		10 * cxchar,  2 * cychar,
		hwnd, (HMENU)3304, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("Button"), TEXT("Plot"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		36 * cxchar,  5 * cychar,
		10 * cxchar,  2 * cychar,
		hwnd, (HMENU)3304, ((LPCREATESTRUCT)lParam)->hInstance, NULL);


		for (int i = 0; i < 5; i++)
		{
			CreateWindow(TEXT("Button"), SATSYS[i], WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX,
							 (2 + 9 * i) * cxchar,  21 * cychar/2,
							           8 * cxchar,   5 * cychar/3,
							  hwnd, (HMENU)(5500 + i), ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}

		CreateWindow(TEXT("Button"), TEXT("GNSS"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
				 cxchar, 19 * cychar/2,
			47 * cxchar,  6 * cychar/2,
			hwnd, (HMENU)(1001), ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case 3301: 
				while(MessageBox(hwnd,TEXT("Please choose Obs file"),TEXT("NOTICE"),MB_ICONASTERISK|MB_YESNO) == IDYES)
				{
					FileSelectDialog(wobs_file);
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
							MessageBox(hwnd, TEXT("File of Obs load complete!"), TEXT("NOTICE"), MB_ICONASTERISK);
							break;
						}
					}
					else
						break;
				}
				break;
				return 0;
			case 3302:
				while(MessageBox(hwnd,TEXT("Please choose Nav file"),TEXT("NOTICE"),MB_ICONASTERISK|MB_YESNO) == IDYES)
				{
					FileSelectDialog(wnav_file);
					wcstombs(nav_file, wnav_file, MAX_PATH);
					fp_nav = fopen(nav_file, TEXT("r"));
					if (fp_nav != NULL)
					{
						gps_satnum = getgpssatnum(fp_nav);
						rewind(fp_nav);
						nav_h = (pnav_head)malloc(sizeof(nav_head));
						nav_b = (pnav_body)malloc(sizeof(nav_body) * gps_satnum * 2);
						if (nav_h && nav_b)
						{
							read_n_h(fp_nav, nav_h);
							read_n_b(fp_nav, nav_b);
						}
						fclose(fp_nav);
						if (gps_satnum > 0)
						{
							MessageBox(hwnd, TEXT("File of Nav load complete!"), TEXT("NOTICE"), MB_ICONASTERISK);
							break;
						}
					}
					else
						break;
				}
				break;
				return 0;
			case 3303:
				if (fp_obs == NULL || fp_nav == NULL)
				{
					MessageBox(hwnd, TEXT("No Data File!"), TEXT("WARNING"), MB_ICONERROR | MB_OK);
				}
				else if (fp_obs != NULL && fp_nav != NULL)
				{
					while(1)
					{
						FileSaveDialog(wres_file);
						if (wres_file == TEXT(""))
						{
							break;
						}
						else
						{
							wcstombs(res_file, wres_file, MAX_PATH);
							sat_gps_pos_clac(result_file, nav_b, obs_e, obs_b, obs_h, o_epochnum, gps_satnum, res_file);
							free(obs_h); free(obs_e); free(obs_b); free(nav_h); free(nav_b);
							fp_nav = NULL; fp_obs = NULL; result_file = NULL;
							break;
						}
						
					}
					
				}
				break;
				return 0;
			case 3304:

				break;
				return 0;
		} 
		return 0;
	case WM_CLOSE:
		if(MessageBox(hwnd, TEXT("You're shutting the program"), TEXT("Confirm"), MB_YESNO | MB_ICONASTERISK) == IDYES )
			DestroyWindow(hwnd);
		else
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}