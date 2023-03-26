#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_SELECTCHAR, LEVEL_GAMERESULT, LEVEL_MENU, LEVEL_STORYMENU, LEVEL_ADVRUI, LEVEL_SELECTMAP,LEVEL_ADVAKAZA,LEVEL_BATTLEENMU,LEVEL_BOSSENMU ,LEVEL_ENDING,LEVEL_END };
	enum OBJDIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_LEFT_DASH , DIR_RIGHT_DASH ,DIR_STOP ,DIR_END };
	enum OBJKEYSTATE { KEY_LEFT, KEY_RIGHT, KEY_LF, KEY_LB, KEY_RF, KEY_RB, KEY_END };
}

#include <process.h>
#include <string>
#include <time.h>
#include <queue>
// Client_Defines.h
//#define fBGM	0.7f
//#define g_fEffect 1.0f
//#define g_fVoice 0.7f
//#define g_fDialog 1.0f
extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern bool			g_bDebug;
extern bool			g_bCollBox;
extern int			g_iLevel; 
extern bool         g_bThread;
extern bool			g_bDeathTime;
extern float		g_fFar;
extern float		g_fBGM;
extern float		g_fEffect;
extern float		g_fVoice;
extern float		g_fDialog;
extern float        g_fLoading;
extern bool			g_bSpecialSkillHit;
extern bool			g_bMiniGame;
extern bool			g_bNaviRender;
extern bool			g_bDiffuseRender;
extern bool			g_bCursor;
using namespace Client;