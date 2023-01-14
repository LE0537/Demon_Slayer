#pragma once

namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum OBJDIR { DIR_STRAIGHT, DIR_LEFT, DIR_RIGHT, DIR_BACK, DIR_LF, DIR_RF, DIR_LB, DIR_RB, DIR_STOP, DIR_END };
	enum OBJKEYSTATE { KEY_LEFT, KEY_RIGHT, KEY_LF, KEY_LB, KEY_RF, KEY_RB, KEY_END };
}

#include <process.h>
#include <string>
#include <time.h>
// Client_Defines.h

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern bool			g_bDebug;


using namespace Client;