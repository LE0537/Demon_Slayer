#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_SELECTCHAR, LEVEL_GAMERESULT, LEVEL_MENU, LEVEL_STORYMENU, LEVEL_ADVRUI, LEVEL_SELECTMAP,LEVEL_ADVAKAZA,LEVEL_BATTLEENMU,LEVEL_BATTLEAKAZA ,LEVEL_END };
	enum OBJDIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_LEFT_DASH , DIR_RIGHT_DASH ,DIR_STOP ,DIR_END };
	enum OBJKEYSTATE { KEY_LEFT, KEY_RIGHT, KEY_LF, KEY_LB, KEY_RF, KEY_RB, KEY_END };
}

#include <process.h>
#include <string>
#include <time.h>
#include <queue>
// Client_Defines.h
#define fBGM	0.7f
#define fEFFECT 1.0f
#define fVOICE 1.f
extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern bool			g_bDebug;
extern bool			g_bCollBox;
extern int			g_iLevel; //겜플 1 루이스토리 2 그외 0
extern bool         g_bThread;
extern bool			g_bDeathTime;
using namespace Client;