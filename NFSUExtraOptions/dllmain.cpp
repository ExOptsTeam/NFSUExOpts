#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"

DWORD Strings;
int hotkeyUnlockAllTracks, hotkeyAnyTrackInAnyMode;
unsigned char MinLaps, MaxLaps;
bool ShowOpponents, AnyTrackInAnyMode, MorePaintTypes, ShowSpecialVinyls, nlgzrgnTakeover, EnableHiddenCameras, UnlockAllTracks, ReplaceTournamentWithFreeRun;
bool once1 = 0, once2 = 0;
float SplashScreenTimeLimit;

DWORD WINAPI Thing(LPVOID);

DWORD AddOptionToTrackOptsMenu = 0x004B8D50;
DWORD TrackOptionsLaps = 0x4B81F0;
DWORD TrackOptionsOpponents = 0x4B8280;
DWORD TrackOptionsCatchUp = 0x4B8310;
DWORD TrackOptionsTraffic = 0x4B83C0;
DWORD TrackOptionsDifficulty = 0x4B84A0;

void __declspec(naked) OpponentsCodeCaveSprint()
{
	_asm
	{
			xor eax, eax
			push esi
			lea esp, dword ptr ds : [esp + 0x00]

		menuelementloop:
			lea esi, [eax + 0x01]
			push esi
			call AddOptionToTrackOptsMenu
			mov eax, esi
			cmp eax, 4
			jnge menuelementloop

			//mov ecx,ebx
			//mov [ebx+00000084],00000001
			//mov [ebx+60],SpeedVer14Org.exe+B8560 // Laps
			//call TrackOptionsLaps
			mov ecx, ebx
			mov dword ptr ds : [ebx + 0x90], 1
			mov dword ptr ds : [ebx + 0x60], 0x4B85C0 // Opponents
			call TrackOptionsOpponents

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0x98], 2
			mov dword ptr ds : [ebx + 0x64], 0x4B8650 // Catch Up
			call TrackOptionsCatchUp

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0x9C], 3
			mov dword ptr ds : [ebx + 0x68], 0x4B8690 // Traffic
			call TrackOptionsTraffic

			pop esi

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0xA0], 4
			mov dword ptr ds : [ebx + 0x6C], 0x4B86F0 // Difficulty

			pop ebx
			jmp TrackOptionsDifficulty

	}
}

void __declspec(naked) OpponentsCodeCaveDrag()
{
	_asm
	{
			
			push 1
			call AddOptionToTrackOptsMenu
			mov ecx, ebx
			mov dword ptr ds: [ebx + 0x90], 1
			mov dword ptr ds : [ebx + 0x60], 0x4B85C0 // Opponents
			call TrackOptionsOpponents

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0x98], 2
			mov dword ptr ds : [ebx + 0x64], 0x4B8650 // Catch Up
			call TrackOptionsCatchUp
			push 2
			call AddOptionToTrackOptsMenu

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0x9C], 3
			mov dword ptr ds : [ebx + 0x68], 0x4B8690 // Traffic
			call TrackOptionsTraffic
			push 3
			call AddOptionToTrackOptsMenu

			mov ecx, ebx
			mov dword ptr ds : [ebx + 0xA0], 4
			mov dword ptr ds : [ebx + 0x6C], 0x4B86F0 // Difficulty
			call TrackOptionsDifficulty
			push 4
			call AddOptionToTrackOptsMenu

			pop ebx
			ret
	}
}

void Init()
{
	CIniReader iniReader("NFSUExtraOptionsSettings.ini");

	// Hotkeys
	hotkeyAnyTrackInAnyMode = iniReader.ReadInteger("Hotkeys", "AnyTrackInAnyMode", 36); // HOME

	// LapControllers
	MinLaps = iniReader.ReadInteger("LapControllers", "Minimum", 0);
	MaxLaps = iniReader.ReadInteger("LapControllers", "Maximum", 127);

	// Menu
	ShowOpponents = iniReader.ReadInteger("Menu", "ShowOpponents", 1) == 1;
	ReplaceTournamentWithFreeRun = iniReader.ReadInteger("Menu", "ReplaceTournamentWithFreeRun", 0) == 1;
	AnyTrackInAnyMode = iniReader.ReadInteger("Menu", "AnyTrackInAnyRaceMode", 0) == 1;
	SplashScreenTimeLimit = iniReader.ReadFloat("Menu", "SplashScreenTimeLimit", 30.0f);
	nlgzrgnTakeover = iniReader.ReadInteger("Menu", "DisableTakeover", 0) == 0;

	// Gameplay
	EnableHiddenCameras = iniReader.ReadInteger("Gameplay", "EnableHiddenCameraModes", 0) == 1;

	// Restrictions
	if (MinLaps < 0 || MinLaps > 127) MinLaps = 1;
	if (MaxLaps < 0 || MaxLaps > 127) MaxLaps = 10;

	// Set Lap Controllers
	injector::WriteMemory<unsigned char>(0x4B85AB, MinLaps, true); // Minimum Lap Controller for Increment
	injector::WriteMemory<unsigned char>(0x4B8587, MinLaps, true); // Minimum Lap Controller for Decrement
	injector::WriteMemory<unsigned char>(0x4B85A7, MaxLaps, true); // Maximum Lap Controller for Increment
	injector::WriteMemory<unsigned char>(0x4B858B, MaxLaps, true); // Maximum Lap Controller for Decrement

	// Enable Drift Camera Mode For All
	if (EnableHiddenCameras)
	{
		injector::WriteMemory<unsigned char>(0x42E091, 0xEB, true);
		injector::MakeNOP(0x4BE78B, 2, true);
		injector::MakeNOP(0x4BE749, 2, true);
	}

	// Splash Screen Time Limit
	injector::WriteMemory<float>(0x6ccac0, SplashScreenTimeLimit, true);

	// Any Track In Any Mode
	if (AnyTrackInAnyMode)
	{
		injector::MakeNOP(0x4BA16C, 2, true);
		injector::MakeNOP(0x4BA202, 2, true);
	}

	// Replace Tournament With Free Run
	if (ReplaceTournamentWithFreeRun)
	{
		injector::WriteMemory<DWORD>(0x4b7a02, 0xCDA558A7, true);
		injector::WriteMemory<DWORD>(0x4B7a07, 0x80E29CC5, true);
		injector::WriteMemory<unsigned char>(0x4B7A1E, 0x04, true);
	}

	// Add Opponents option to Race Options menu
	if (ShowOpponents)
	{
		// Use unused option for opponents
		injector::WriteMemory<unsigned char>(0x4b85de, 0x08, true); // Change address to read??
		injector::WriteMemory<unsigned char>(0x4b85df, 0x90, true);

		injector::WriteMemory<unsigned char>(0x4b82f1, 0x10, true); // Change address to write??
		injector::WriteMemory<unsigned char>(0x4b82f2, 0x90, true);

		injector::MakeNOP(0x4b55f3, 1, true); // Disable decrement to use it as opponent count.
		injector::MakeNOP(0x4b4a12, 1, true);
		injector::MakeNOP(0x4b4e92, 1, true);
		injector::MakeNOP(0x4b4c62, 1, true);

		injector::WriteMemory<unsigned char>(0x4b85e7, 0, true); // Set controllers
		injector::WriteMemory<unsigned char>(0x4b8600, 0, true);
		injector::WriteMemory<unsigned char>(0x4b85eb, 3, true);
		injector::WriteMemory<unsigned char>(0x4b85fc, 3, true);

		injector::WriteMemory<unsigned char>(0x4b9108, 4, true); // Make 5th element overlap 4th (Otherwise it won't appear)

		// Fix Lap Controller for Lap KO
		injector::WriteMemory<unsigned char>(0x4B4eb5, 0x10, true); // Read normal lap count instead of KO lap
		injector::MakeNOP(0x4B4ebc, 5, true); // Don't fix it to 3

		injector::MakeCALL(0x4b89b3, 0x4B9070, true); // Circuit
		injector::MakeJMP(0x4B8F4A, OpponentsCodeCaveSprint, true); // Sprint
		injector::MakeJMP(0x4B913A, OpponentsCodeCaveDrag, true); // Drag
		injector::MakeCALL(0x4b89c9, 0x4B9070, true); // KO
	}
	

    // Other Things
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Thing, NULL, 0, NULL);
}

DWORD WINAPI Thing(LPVOID)
{
	while (true)
	{
		Sleep(1);
		Strings = *(DWORD*)0x734434;

		if ((GetAsyncKeyState(hotkeyAnyTrackInAnyMode) & 1)) // When pressed "Any Track in Any Mode" key
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

			while ((GetAsyncKeyState(hotkeyAnyTrackInAnyMode) & 0x8000) > 0) { Sleep(0); }
		}

		if (Strings && ShowOpponents && !once1) // Indicate if ExOpts is Loaded at splash screen (If nlgzrgnTakeOver enabled)
		{
			DWORD NumberOfRacesAddr = (Strings + 0x190c);
			char* OpponentsString = "Opponents:";
			injector::WriteMemory<DWORD>(NumberOfRacesAddr, (DWORD)OpponentsString, true);
			once1 = 1;
		}

		if (Strings && nlgzrgnTakeover && !once2) // Indicate if ExOpts is Loaded at splash screen (If nlgzrgnTakeOver enabled)
		{
			DWORD CopyrightAddr = (Strings + 0x58cc);
			char* Copyright = *(char**)CopyrightAddr;
			char* Append = "^NFSU Extra Options - © 2016 nlgzrgn. No rights reserved.";
			char* Tookover = strcat(Copyright, Append);
			injector::WriteMemory<DWORD>(CopyrightAddr, (DWORD)Tookover, true);
			once2 = 1;
		}
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x670CB5) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English speed.exe (3,03 MB (3.178.496 bytes)).", "NFSU Extra Options", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
	
}

