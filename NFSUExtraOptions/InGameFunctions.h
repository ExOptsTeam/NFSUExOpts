#pragma once

#include "stdio.h"
#include <windows.h>

// Function pointers
int(__stdcall* Player_AutoPilotOn)(DWORD* _Player) = (int(__stdcall*)(DWORD*))0x42E230;
char*(__stdcall* Player_GetName)(DWORD* _Player) = (char* (__stdcall*)(DWORD*))0x431D30;
void(*UpdateCameraMovers)() = (void(*)())0x48E8B0;
void(*MovieStop)() = (void(*)())0x5A4C80;
void(*feDisableStartRaceScreens)() = (void(*)())0x495E90;
int(__thiscall* QRLapPopupScreen_DrawLaps)(DWORD* QRLapPopupScreen) = (int(__thiscall*)(DWORD*))0x4B81F0;
int(__thiscall* QROptionsScreen_DrawNumRaces)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B8280;
int(__thiscall* QROptionsScreen_DrawCatchUp)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B8310;
int(__thiscall* QROptionsScreen_DrawTraffic)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B83C0;
int(__thiscall* QROptionsScreen_DrawAISkill)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B84A0;
int(__thiscall* QROptionsScreen_ScrollLaps)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B8560;
int(__thiscall* QROptionsScreen_ScrollNumRaces)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B85C0;
int(__thiscall* QROptionsScreen_ScrollCatchUp)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B8650;
int(__thiscall* QROptionsScreen_ScrollTraffic)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B8690;
int(__thiscall* QROptionsScreen_ScrollAISkill)(DWORD* QROptionsScreen) = (int(__thiscall*)(DWORD*))0x4B86F0;
int(__fastcall* cFrontendDatabase_GetOptions)(DWORD* cFrontendDatabase, int eRaceType) = (int(__fastcall*)(DWORD*, int))0x4AB510;
int(__cdecl* FEngSetVisible_obj)(void* FEObject) = (int(__cdecl*)(void*))0x4F6970;
int(__stdcall* RaceCoordinator_RCPostMessage)(DWORD* RaceCoordinator, int RCMessage, int unk) = (int(__stdcall*)(DWORD*, int, int))0x421C00;

// Functions which has odd calling conventions (using NFSUCalls.h to wrap them)
int(*Player_AutoPilotOff)(DWORD* EDI_Player) = (int(*)(DWORD*))0x42E400;
char*(*DriverInfo_GetName)(DWORD* EAX_DriverInfo) = (char*(*)(DWORD*))0x580AF0;
int(*QROptionsScreen_ShowSingleOption)(DWORD* QROptionsScreen, int a2) = (int(*)(DWORD*, int))0x4B8D50;
int(*FEHashUpper)(char const* EDX_StringToHash) = (int(*)(char const*))0x4FD230;
int(*FEngPrintf_obj)(DWORD* EAX_obj, const char* format, ...) = (int(*)(DWORD*, const char*, ...))0x4F68A0;
int(*FEngPrintf_H)(const char* EAX_pkg, int EDI_hash, const char* format, ...) = (int(*)(const char*, int, const char*, ...))0x4F6850;
int(*FEngPrintf)(const char* EAX_pkg, const char* EDX_obj_name, const char* format, ...) = (int(*)(const char*, const char*, const char*, ...))0x4F67F0;
int(*FEngSNPrintf)(const char* ESI_buf, int len, const char* format, ...) = (int(*)(const char*, int, const char*, ...))0x4F68C0;
int(*cFEng_SetButtonState)(DWORD* cFeng, char const* EAX_pkg, int EDI_hash, bool state) = (int(*)(DWORD*, char const*, int, bool))0x4F5F80;
int(*FEngQueuePackagePush)(char const* EAX_pkg, char const* EDI_pkg2, int state) = (int(*)(char const*, char const*, int))0x4F63E0;
int(*FEngSwitchPackages)(char const* pkg, char const* EDI_pkg2) = (int(*)(char const*, char const*))0x4F6360;
int(*FEngFindPackage)(char const* EAX_pkg) = (int(*)(char const*))0x4F65D0;
int(*FEPackage_FindObjectByHash)(DWORD* EDX_pkg, int ECX_hash) = (int(*)(DWORD*, int))0x4FFB70;
int(*FEPackageManager_BroadcastMessage)(DWORD* EDX_FEPackageManager, int msgHash) = (int(*)(DWORD*, int))0x4F3DC0;
int(*FEngine_FindObject)(DWORD* EAX_FEngine, unsigned int hash, const char* ECX_pkg) = (int(*)(DWORD*, unsigned int, char const*))0x4EF050;
int(*QROptionsScreen_SetupCircuit)(DWORD* EAX_QROptionsScreen) = (int(*)(DWORD*))0x4B8E60;
int(*FEngSetScript_obj)(DWORD* EDI_feObj, char const* script, bool unk) = (int(*)(DWORD*, char const*, bool))0x4F6B70;
int(*FEngSetScript_H)(char const* ESI_pkg, int EAX_hash, char const* script, bool unk) = (int(*)(char const*, int, char const*, bool))0x4F6C30;
int(*FEngSetScript)(char const* EAX_pkg, char const* EDX_obj_name, char const* script, bool unk) = (int(*)(char const*, char const*, char const*, bool))0x4F6BD0;
int(*PreRaceStatsScreen_SetVisibility)(DWORD* PreRaceStatsScreen, bool EAX_visible) = (int(*)(DWORD*, bool))0x4959D0;
bool(*FEngIsScriptRunning)(char const* EAX_pkg, int EDI_hash, char const* script) = (bool(*)(char const*, int, char const*))0x4F6C80;
int(*FEngSetVisible)(char const* EDI_pkg,char const* EDX_obj) = (int(*)(char const*, char const*))0x414A60;
DWORD QRModeSelectScreen_MaybeMakeGameModeAvailable = 0x4B6FE0;