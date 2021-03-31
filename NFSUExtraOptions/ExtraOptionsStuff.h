#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"

#include "GlobalVariables.h"
#include "InGameFunctions.h"
#include "UserCalls.h"

int hotkeyUnlockAllThings, hotkeyAnyTrackInAnyMode, hotkeyFreezeCamera, hotkeyAutoDrive, GameRegion, StartingCash, StartingStylePoints, MinLaps, MaxLaps, MinRacers, MaxRacers, EATraxSkin, WindowedMode, WindowedModeX, WindowedModeY;
bool ShowMoreRaceOptions, ShowLanguageSelectScreen, AnyTrackInAnyMode, MorePaintTypes, ShowSpecialVinyls, ExOptsTeamTakeOver, EnableHiddenCameras, UnlockAllThings, ShowTournament, GarageZoom, GarageRotate, ShowDebugOptionsMenu, SkipMovies, EnableSound, EnableMusic, AllowMultipleInstances, once1, once2, WheelFix, DamageFix, EnablePreRaceStats;
float SplashScreenTimeLimit, GameSpeed, MiniWorldCarSpeedCruising, MiniWorldCarSpeedFast, MiniWorldCarSpeedSlow;

#include "QROptionsScreen.h"
#include "PreRaceStatsScreen.h"

#include "CodeCaves.h"
#include "HotkeyStuff.h"

void Init()
{
	CIniReader iniReader("NFSUExtraOptionsSettings.ini");

	// Hotkeys
	hotkeyAnyTrackInAnyMode = iniReader.ReadInteger("Hotkeys", "AnyTrackInAnyMode", 36); // HOME
	hotkeyFreezeCamera = iniReader.ReadInteger("Hotkeys", "FreezeCamera", 19); // Pause/Break
	hotkeyUnlockAllThings = iniReader.ReadInteger("Hotkeys", "UnlockAllThings", 116); //F5
	hotkeyAutoDrive = iniReader.ReadInteger("Hotkeys", "AutoDrive", 117); //F6

	// LapControllers
	MinLaps = iniReader.ReadInteger("LapControllers", "Minimum", 0);
	MaxLaps = iniReader.ReadInteger("LapControllers", "Maximum", 127);

	// RacerControllers
	MinRacers = iniReader.ReadInteger("RacerControllers", "Minimum", 1);
	MaxRacers = iniReader.ReadInteger("RacerControllers", "Maximum", 4);

	// Menu
	ShowMoreRaceOptions = iniReader.ReadInteger("Menu", "ShowMoreRaceOptions", 1) != 0;
	ShowLanguageSelectScreen = iniReader.ReadInteger("Menu", "ShowLanguageSelectScreen", 0) != 0;
	ShowSpecialVinyls = iniReader.ReadInteger("Menu", "ShowSpecialVinyls", 1) != 0;
	ShowDebugOptionsMenu = iniReader.ReadInteger("Menu", "ShowDebugOptionsMenu", 0) != 0;
	ShowTournament = iniReader.ReadInteger("Menu", "ShowTournament", 0) != 0;
	EATraxSkin = iniReader.ReadInteger("Menu", "EATraxSkin", -1);
	AnyTrackInAnyMode = iniReader.ReadInteger("Menu", "AnyTrackInAnyRaceMode", 0) != 0;
	SplashScreenTimeLimit = iniReader.ReadFloat("Menu", "SplashScreenTimeLimit", 30.0f);
	MiniWorldCarSpeedFast = iniReader.ReadFloat("Menu", "MiniWorldCarSpeedFast", 50.0f);
	MiniWorldCarSpeedCruising = iniReader.ReadFloat("Menu", "MiniWorldCarSpeedCruising", 20.0f);
	MiniWorldCarSpeedSlow = iniReader.ReadFloat("Menu", "MiniWorldCarSpeedSlow", 1.0f);
	GarageZoom = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteZoom", 0) != 0;
	GarageRotate = iniReader.ReadInteger("Menu", "ShowcaseCamInfiniteRotation", 0) != 0;
	ExOptsTeamTakeOver = iniReader.ReadInteger("Menu", "DisableTakeover", 0) == 0;

	// Gameplay
	EnableHiddenCameras = iniReader.ReadInteger("Gameplay", "EnableHiddenCameraModes", 0) != 0;
	EnablePreRaceStats = iniReader.ReadInteger("Gameplay", "EnablePreRaceStats", 0) != 0;
	GameSpeed = iniReader.ReadFloat("Gameplay", "GameSpeed", 1.0f);
	GameRegion = iniReader.ReadInteger("Gameplay", "GameRegion", 0);
	StartingCash = iniReader.ReadInteger("Gameplay", "StartingCash", 0);
	StartingStylePoints = iniReader.ReadInteger("Gameplay", "StartingStylePoints", 0);
	UnlockAllThings = iniReader.ReadInteger("Gameplay", "UnlockAllThings", 0) != 0;

	// Fixes
	WheelFix = iniReader.ReadInteger("Fixes", "DisappearingWheelsFix", 1) != 0;
	DamageFix = iniReader.ReadInteger("Fixes", "DamageFix", 0) != 0;

	// Misc
	WindowedMode = iniReader.ReadInteger("Misc", "WindowedMode", 0);
	SkipMovies = iniReader.ReadInteger("Misc", "SkipMovies", 0) != 0;
	EnableSound = iniReader.ReadInteger("Misc", "EnableSound", 1) != 0;
	EnableMusic = iniReader.ReadInteger("Misc", "EnableMusic", 1) != 0;
	AllowMultipleInstances = iniReader.ReadInteger("Misc", "AllowMultipleInstances", 0) == 1;

	// Restrictions
	MinLaps %= 128;
	MaxLaps %= 128;
	MinRacers = (MinRacers - 1) % 4 + 1;
	MaxRacers = (MaxRacers - 1) % 4 + 1;

	// Allow Multiple Instances
	if (AllowMultipleInstances)
	{
		injector::WriteMemory<unsigned char>(0x4472F1, 0xEB, true);
	}

	// Set Lap Controllers
	// QROptionsScreen::ScrollLaps
	injector::WriteMemory<BYTE>(0x4B85AB, MinLaps, true); // Minimum Lap Controller for Increment
	injector::WriteMemory<BYTE>(0x4B8587, MinLaps, true); // Minimum Lap Controller for Decrement
	injector::WriteMemory<BYTE>(0x4B85A7, MaxLaps, true); // Maximum Lap Controller for Increment
	injector::WriteMemory<BYTE>(0x4B858B, MaxLaps, true); // Maximum Lap Controller for Decrement
	// QRLapPopupScreen::ScrollLaps
	injector::WriteMemory<BYTE>(0x4BD4B3, MinLaps, true); // Minimum Lap Controller for Decrement
	injector::WriteMemory<BYTE>(0x4BD4D9, MinLaps, true); // Minimum Lap Controller for Increment
	injector::WriteMemory<BYTE>(0x4BD4BB, MaxLaps, true); // Maximum Lap Controller for Decrement
	injector::WriteMemory<BYTE>(0x4BD4D5, MaxLaps, true); // Maximum Lap Controller for Increment

	// Enable Drift Camera Mode For All
	if (EnableHiddenCameras)
	{
		injector::WriteMemory<BYTE>(0x42E091, 0xEB, true); // Player::CycleDriveCameraPovType
		injector::MakeNOP(0x4BE78B, 2, true);
		injector::MakeNOP(0x4BE749, 2, true);
	}

	// Enable Pre-Race Stats from PS2 Demo
	if (EnablePreRaceStats)
	{
		//injector::MakeNOP(0x495905, 2, true); // PreRaceStatsScreen::PreRaceStatsScreen
		injector::WriteMemory(0x6C7A08, &PreRaceStatsScreen_NotificationMessage, true); // PreRaceStatsScreen::vtable
	}

	// Splash Screen Time Limit
	injector::WriteMemory<float>(0x6ccac0, SplashScreenTimeLimit, true);

	// Any Track In Any Mode
	if (AnyTrackInAnyMode)
	{
		injector::MakeNOP(0x4BA16C, 2, true);
		injector::MakeNOP(0x4BA202, 2, true);
	}

	// Show Language Select Screen
	if (ShowLanguageSelectScreen)
	{
		injector::WriteMemory<DWORD>(0x6FA1A4, 0x6C2368, true); // BootFlowNTSC - "LS_LangSelect.fng"
		injector::WriteMemory<DWORD>(0x6FA1BC, 0x6C2368, true); // BootFlowPAL - "LS_LangSelect.fng"
		injector::MakeJMP(0x4DEEDA, 0x4DEF81, true); // Always show more languages
	}

	// Show Tournament in Quick Race Menu
	if (ShowTournament)
	{
		injector::MakeJMP(0x4B7922, TournamentCodeCave, true); // QRModeSelectScreen::BuildModeList
	}

	// Fix racer counts (Old ExOpts fix)
	//injector::WriteMemory<BYTE>(0x758980, 4, true); // Circuit
	//injector::WriteMemory<BYTE>(0x7589A0, 4, true); // Sprint
	//injector::WriteMemory<BYTE>(0x7589C0, 4, true); // LapKnockout
	//injector::WriteMemory<BYTE>(0x7589E0, 1, true); // FreeRun
	//injector::WriteMemory<BYTE>(0x758A04, 4, true); // Tournament
	//injector::WriteMemory<BYTE>(0x758A88, 4, true); // Drag
	//injector::WriteMemory<BYTE>(0x758AB4, 1, true); // Drift

	// Add Opponents option to Race Options menu
	if (ShowMoreRaceOptions)
	{
		// Show lap popup
		injector::MakeJMP(0x4BB3C4, LapPopupCodeCave_QRTrackSelectScreen_NotificationMessage, true);
		injector::MakeJMP(0x4BB41A, LapPopupCodeCave_QRTrackSelectScreen_NotificationMessage_2, true);
		injector::MakeJMP(0x4B7B88, LapPopupCodeCave_QRModeSelectScreen_NotificationMessage, true);
		injector::MakeJMP(0x4BD748, LapPopupCodeCave_QRLapPopupScreen_NotificationMessage, true); // Switch from lap popup to race options when pressed enter
		injector::MakeJMP(0x4BD740, LapPopupCodeCave_QRLapPopupScreen_NotificationMessage_2, true); // Reactivate lap popup when pressed back

		// Fix Lap Controller for Lap KO - RaceStarter::SetupLapKO
		injector::WriteMemory<BYTE>(0x4B4eb5, 0x10, true); // Read normal lap count instead of KO lap
		injector::MakeNOP(0x4B4ebc, 5, true); // Don't fix it to 3

		// TEMP - Overlap options 4 and 5 for Tournament Menu (to fix invisible 5th option text and data)
		injector::WriteMemory<int>(0x4B9108, 4, true);

		// Make Circuit, Sprint, Drag and KO use the same options screen
		injector::MakeCALL(0x4B89BE, QROptionsScreen_SetupCircuit, true); // Sprint
		injector::MakeCALL(0x4b89c9, QROptionsScreen_SetupCircuit, true); // KO
		injector::MakeCALL(0x4B8A17, QROptionsScreen_SetupCircuit, true); // Drag

		// Show Racers Option
		injector::WriteMemory(0x4B8EDF, QROptionsScreen_ScrollNumRacers, true); // QROptionsScreen::SetupCircuit
		injector::MakeCALL(0x4B8EE3, QROptionsScreen_DrawNumRacers, true); // QROptionsScreen::SetupCircuit
		injector::WriteMemory(0x4B90A5, QROptionsScreen_ScrollNumRacers, true); // QROptionsScreen::SetupTournament
		injector::MakeCALL(0x4B90A9, QROptionsScreen_DrawNumRacers, true); // QROptionsScreen::SetupTournament
	}

	// Show Special Vinyls
	if (ShowSpecialVinyls)
	{
		// Unlock all vinyls
		injector::WriteMemory<BYTE>(0x4E27D7, 0, true);
		injector::WriteMemory<BYTE>(0x4AAA6D, 0xEB, true);
		injector::MakeNOP(0x5A3577, 2, true);

		// No Restrictions
		injector::WriteMemory<WORD>(0x4E4791, 0x3AEB, true); // EB 3A - jmp 0x4E47CD
		injector::MakeRangedNOP(0x4E46C9, 0x4E46E0, true);
		injector::WriteMemory<BYTE>(0x4E46C9, 0xB8, true); // mov eax,0x00000000
		injector::WriteMemory<int>(0x4E46CA, 0, true);
		injector::WriteMemory<BYTE>(0x4E46CE, 0xB9, true);  // mov ecx,0x00000000
		injector::WriteMemory<int>(0x4E46CF, 0, true);
		injector::WriteMemory<BYTE>(0x4E46D4, 0xBE, true);  // mov esi,0x00000000
		injector::WriteMemory<int>(0x4E46D5, 0, true);
		injector::WriteMemory<BYTE>(0x4E46DA, 0xBB, true);  // mov ebx,0x00000000
		injector::WriteMemory<int>(0x4E46DB, 0, true);
	}

	// Copyright text
	if (ExOptsTeamTakeOver)
	{
		injector::MakeJMP(0x4DF385, SplashScreenCodeCave, true);
	}
	
	// Fix Invisible Wheels
	if (WheelFix)
	{
		injector::WriteMemory<BYTE>(0x5696E4, 0x01, true); // CarPartCuller::CullParts
	}

	// Damage Fix
	if (DamageFix)
	{
		injector::WriteMemory<BYTE>(0x465DA4, 0xEB, true); // Collision::CheckForWildCollision
	}
	
	// Garage Hacks
	if (GarageZoom)
	{
		injector::WriteMemory<BYTE>(0x48721D, 0xEB, true);
		injector::WriteMemory<BYTE>(0x487236, 0xEB, true);
	}
	if (GarageRotate)
	{
		injector::WriteMemory<BYTE>(0x4871F2, 0xEB, true);
		injector::WriteMemory<BYTE>(0x487209, 0xEB, true);
	}

	// Game Speed
	injector::WriteMemory<float>(_Tweak_GameSpeed, GameSpeed, true);

	// Debug Menu
	if (ShowDebugOptionsMenu)
	{
		injector::WriteMemory<int>(0x006C56E4, 0x00000000, true);
	}

	// EA Trax Skin
	if (EATraxSkin != -1)
	{
		EATraxSkin %= 4;
		injector::MakeNOP(0x4AA212, 3, true); // Don't zero it at boot
		injector::WriteMemory<int>(_ChyronSkin, EATraxSkin, true);
	}

	// Game Region
	injector::WriteMemory<int>(_BuildRegion, GameRegion, true);

	// Starting Cash
	injector::MakeRangedNOP(0x5A0A3F, 0x5A0A45, true);
	injector::MakeJMP(0x5A0A3F, StartingCashCodeCave, true);

	// Starting Style Points
	injector::WriteMemory<int>(0x41CF8E, StartingStylePoints, true);

	// Unlock all things (Load preference)
	if (UnlockAllThings)
	{
		injector::WriteMemory<BYTE>(_UnlockEverythingThatCanBeLocked, 1, true);
	}

	// Garage Car Speed
	injector::WriteMemory<float>(0x6B6B28, MiniWorldCarSpeedFast, true); // While changing cars
	injector::WriteMemory<float>(0x6B6B2C, MiniWorldCarSpeedCruising, true); // Normal car speed
	injector::WriteMemory<float>(0x6B6B30, MiniWorldCarSpeedSlow, true); // Used on rim tuning and paint

	// Windowed Mode
	if (WindowedMode > 0)
	{
		injector::WriteMemory<BYTE>(_WindowedMode, 1, true); // Vanilla Windowed Mode
	}

	// Skip movies
	if (SkipMovies)
	{
		injector::WriteMemory<BYTE>(_SkipMovies, 1, true); // Vanilla Skip Movies
		injector::MakeJMP(0x5A4A50, SkipMovieCodeCave, true); // MovieStart (fix blank screen when skipped a movie)
	}
	
	if (!EnableSound) injector::WriteMemory<BYTE>(_IsSoundEnabled, 0, true); // Enable sound
	if (!EnableMusic) injector::WriteMemory<BYTE>(_IsAudioStreamingEnabled, 0, true); // Enable music

	// Other Things
	injector::MakeCALL(0x4479BE, Thing, true);
}