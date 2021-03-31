#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"

char* TakeoverString = "NFSU Extra Options - © 2021 ExOpts Team. No rights reserved.";
bool ShouldShowLapPopup;

// 0x4DF385
void __declspec(naked) SplashScreenCodeCave()
{
	_asm
	{
		call FEngSetVisible // edi = package, edx = object
		mov eax, [ebp + 0xC] // package name
		mov edx, 0x6C3A3C // "LicenseBlurb"
		push TakeoverString
		call FEngPrintf // eax = package, edx = object, push fmt
		add esp,4
		push 0x4DF38A
		retn
	}
}

void __declspec(naked) StartingCashCodeCave()
{
	_asm
	{
		push ebx
		mov ebx, StartingCash
		mov dword ptr ds : [esi + 0x1374] , ebx
		pop ebx
		push 0x5A0A45
		retn
	}
}

void __declspec(naked) TournamentCodeCave()
{
	_asm
	{
		call QRModeSelectScreen_MaybeMakeGameModeAvailable

		newcode :
			mov ebx, [esp + 0x38]
			mov al, byte ptr ds : [0x7589FE]
			test al, al
			jz caveexit
			cmp dword ptr ds : [0x777B4C] , 04
			jz caveexit

			mov edx, dword ptr ds : [esp + 0x0C]
			inc edx
			push edx
			push 0x6C5F64 //"Mode_%d"
			push 0x20
			lea esi, [esp + 0x20]
			mov dword ptr ds : [esp + 0x18] , edx
			call FEngSNPrintf
			mov esi, dword ptr ds : [ebx + 0x0C]
			add esp, 0x0C
			lea edx, [esp + 0x14]
			call FEHashUpper
			test esi, esi
			mov edi, eax
			je loc_4B7988
			mov eax, esi
			call FEngFindPackage
			test eax, eax
			je loc_4B7988
			mov ecx, edi
			mov edx, eax
			call FEPackage_FindObjectByHash
			test eax, eax
			jne loc_4B79A1

		loc_4B7988 :
			mov ebx, dword ptr ds : [0x73578C]
			test ebx, ebx
			je loc_4B79B7
			mov eax, [ebx + 8]
			push edi
			mov ecx, esi
			call FEngine_FindObject
			test eax, eax
			je loc_4B79B1

		loc_4B79A1 :
			cmp dword ptr ds : [eax + 0x18] , 2
			jne loc_4B79B1
			mov ebx, dword ptr ds : [0x73578C]
			mov esi, eax
			jmp loc_4B79B9

		loc_4B79B1 :
			mov ebx, dword ptr ds : [0x73578C]

		loc_4B79B7 :
			xor esi, esi

			loc_4B79B9 :
			push esi
			call FEngSetVisible_obj
			add esp, 4
			test ebx, ebx
			je loc_4B79E0
			push 1
			lea edx, [esp + 0x18]
			call FEHashUpper
			mov edi, eax
			mov eax, [esp + 0x3C]
			mov eax, [eax + 0x0C]
			push ebx
			call cFEng_SetButtonState

		loc_4B79E0 :
			mov edx, 0x6C5F04 //"GENERICPLACEHOLDER"
			or ecx, -1
			mov al, 0x47
			lea ebx, [ebx + 0]

		loc_4B79F0 :
			imul ecx, ecx, 0x21
			movzx eax, al
			add ecx, eax
			mov al, [edx + 01]
			inc edx
			test al, al
			jne loc_4B79F0
			push ecx
			push 0xCDA558A7 // "TOURNAMENT"
			push 0x80E29CC5 // desc
			mov edx, 0x6C5EDC
			call FEHashUpper
			mov ecx, [esp + 0x44]
			push eax
			push 0
			push ecx
			mov ebx, 4
			call QRModeSelectScreen_MaybeMakeGameModeAvailable


		caveexit :
			push 0x4B7927
			retn
	}
}

// 0x4BB3C4
void __declspec(naked) LapPopupCodeCave_QRTrackSelectScreen_NotificationMessage()
{
	_asm
	{
		mov al, ShouldShowLapPopup
		test al,al
		jz originalcode

		showlappopup:
			mov eax,[ebp+0x0C]
			push 255
			mov edi,0x6C210C // "MU_QuickRaceLapPopup_PC.fng"
			call FEngQueuePackagePush
			add esp,04
			mov ShouldShowLapPopup,0
			jmp caveexit

		originalcode:
			mov eax, [ebp + 0x0C]
			push 255
			mov edi, 0x6C2588 // "MU_QuickRaceOptions_PC.fng"
			call FEngQueuePackagePush
			add esp, 04

		caveexit:
		push 0x4BB3D9
		retn
	}
}

//0x4BB41A
void __declspec(naked) LapPopupCodeCave_QRTrackSelectScreen_NotificationMessage_2()
{
	_asm
	{
		mov ecx, dword ptr ds : [0x777CC8] // CurrentRaceMode
		cmp ecx, 0 // Circuit
		je ShowLapPopup
		cmp ecx, 2 // Lap KO
		je ShowLapPopup
		cmp ecx, 4 // Tournament
		je ShowLapPopup
		cmp ecx, 6 // Drift
		je ShowLapPopup

		DontShowLapPopup :
		mov ShouldShowLapPopup, 0
			jmp caveexit

			ShowLapPopup :
		mov ShouldShowLapPopup, 1

			caveexit :
			push 0x4BB420
			retn
	}
}

//0x4B7B88
void __declspec(naked) LapPopupCodeCave_QRModeSelectScreen_NotificationMessage()
{
	_asm
	{
		mov dword ptr ds:[0x777CC8], edx // CurrentRaceMode
		cmp edx, 0 // Circuit
		je ShowLapPopup
		cmp edx, 2 // Lap KO
		je ShowLapPopup
		cmp edx, 4 // Tournament
		je ShowLapPopup
		cmp edx, 6 // Drift
		je ShowLapPopup

		DontShowLapPopup:
		mov ShouldShowLapPopup, 0
		jmp caveexit

		ShowLapPopup:
		mov ShouldShowLapPopup, 1

		caveexit:
			push 0x4B7B8E
			retn
	}
}

//0x4BD748
void __declspec(naked) LapPopupCodeCave_QRLapPopupScreen_NotificationMessage()
{
	_asm
	{
		mov eax, [esi + 0x0C]
		push 255
		mov edi, 0x6C2588 // "MU_QuickRaceOptions_PC.fng"
		call FEngQueuePackagePush
		add esp, 4
		pop edi
		pop esi
		retn 0x10
	}
}

//0x4BD740
void __declspec(naked) LapPopupCodeCave_QRLapPopupScreen_NotificationMessage_2()
{
	_asm
	{
		mov ShouldShowLapPopup, 1
		add esp, 4
		pop edi
		pop esi
		retn 0x10
	}
}

void __declspec(naked) SkipMovieCodeCave()
{
	_asm
	{
		mov eax, [_SkipMovies]
		test eax, eax
		jz loc_5A4A5D
		push 0xC3960EB9
		mov edx, _gFEPackageManager
		call FEPackageManager_BroadcastMessage
		retn

		loc_5A4A5D:
			push 0x5A4A5D
			retn
	}
}
