#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"

bool once3, IsOnFocus, AutoDrive;
DWORD TheGameFlowManager;
HWND windowHandle;

void Thing()
{
	UpdateCameraMovers(); // Hijacked call from main loop

	_asm pushad;

	TheGameFlowManager = *(DWORD*)_TheGameFlowManager; // 3 = FE, 4&5 = Loading screen, 6 = Gameplay
	windowHandle = *(HWND*)_hWnd;
	IsOnFocus = !(*(bool*)_IsLostFocus);

	// Windowed Mode Related Fixes (Center and Resize)
	if (WindowedMode && windowHandle && !once3)
	{
		RECT o_cRect, n_cRect, n_wRect;
		GetClientRect(windowHandle, &o_cRect);
		
		DWORD wStyle = GetWindowLongPtr(windowHandle, GWL_STYLE);

		switch (WindowedMode)
		{
			case 1: wStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); break;
			case 2: default: wStyle |= (WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW); break;
		}

		SetWindowLongPtr(windowHandle, GWL_STYLE, wStyle);

		// make window change style
		SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);

		//GetWindowRect(windowHandle, &n_wRect);
		GetClientRect(windowHandle, &n_cRect);
		int n_wWidth = *(int*)_ResX;
		int n_wHeight = *(int*)_ResY;
		int dif_wWidth = o_cRect.right - n_cRect.right;
		int dif_wHeight = o_cRect.bottom - n_cRect.bottom;
		int newWidth = n_wWidth + dif_wWidth;
		int newHeight = n_wHeight + dif_wHeight;

		HWND hDesktop = GetDesktopWindow();
		RECT desktop;
		GetWindowRect(hDesktop, &desktop);

		int newXPos = ((desktop.right - desktop.left) - newWidth) / 2;
		int newYPos = ((desktop.bottom - desktop.top) - newHeight) / 2;

		SetWindowPos(windowHandle, NULL, newXPos, newYPos, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);
		
		once3 = 1;
	}

	// Lock EA Trax Skin
	if (EATraxSkin != -1)
	{
		injector::WriteMemory<int>(_ChyronSkin, EATraxSkin, true);
	}

	// Any Track in Any Mode
	if ((GetAsyncKeyState(hotkeyAnyTrackInAnyMode) & 1)) 
	{
		CIniReader iniReader("NFSUExtraOptionsSettings.ini");
		AnyTrackInAnyMode = !AnyTrackInAnyMode;
		iniReader.WriteInteger("Menu", "AnyTrackInAnyRaceMode", AnyTrackInAnyMode);

		if (AnyTrackInAnyMode)
		{
			injector::MakeNOP(0x4BA16C, 2, true);
			injector::MakeNOP(0x4BA202, 2, true);
		}

		else
		{
			injector::WriteMemory<WORD>(0x4BA16C, 0x1D74, true);
			injector::WriteMemory<WORD>(0x4BA202, 0x2C74, true);
		}
	}

	// Drunk Driver
	if ((GetAsyncKeyState(hotkeyAutoDrive) & 1) && (TheGameFlowManager == 6) && IsOnFocus)
	{
		AutoDrive = !AutoDrive;

		DWORD PlayerThing = *(DWORD*)_PlayersByIndex;


		if (PlayerThing)
		{
			if (AutoDrive)
			{
				Player_AutoPilotOn((DWORD*)PlayerThing);
			}
			else
			{
				MakeUserCall(Player_AutoPilotOff, 1, edi, PlayerThing);
			}
		}
	}

	// Freeze Camera
	if ((GetAsyncKeyState(hotkeyFreezeCamera) & 1) && IsOnFocus)
	{
		*(bool*)_Camera_StopUpdating = !(*(bool*)_Camera_StopUpdating);
	}

	// Unlock All Things
	if ((GetAsyncKeyState(hotkeyUnlockAllThings) & 1))
	{
		UnlockAllThings = !UnlockAllThings;
		CIniReader iniReader("NFSUExtraOptionsSettings.ini");
		iniReader.WriteInteger("Gameplay", "UnlockAllThings", UnlockAllThings);

		injector::WriteMemory<unsigned char>(_UnlockEverythingThatCanBeLocked, UnlockAllThings, true);
	}

	_asm popad;
}