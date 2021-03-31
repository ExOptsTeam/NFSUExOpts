void __fastcall PreRaceStatsScreen_SetInitScript(DWORD* _PreRaceStatsScreen, void* EDX_Unused)
{
    int NumPlayers = _PreRaceStatsScreen[52];

    for (int i = 0; i < NumPlayers; i++)
    {
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 16], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 20], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 24], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 28], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 32], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 36], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 40], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 44], esp, "Init", esp, 1);
        MakeUserCall(FEngSetScript_obj, 3, edi, _PreRaceStatsScreen[i + 48], esp, "Init", esp, 1);
    }
}

void __fastcall PreRaceStatsScreen_DrawCars(DWORD* _PreRaceStatsScreen, void* EDX_Unused)
{
    int NumPlayers = _PreRaceStatsScreen[52];
    DWORD* PCurrentRace = *(DWORD**)_pCurrentRace;
    DWORD* CarPtr;
    char* CarName;
    
    for (int i = 0; i < NumPlayers; i++)
    {
        if (i >= PCurrentRace[9]) CarPtr = 0;
        else CarPtr = (DWORD*)PCurrentRace[328 + i];

        if (CarPtr)
        {
            CarPtr = (DWORD*)CarPtr[0];
            if (CarPtr)
            {
                CarName = (char*)CarPtr[3];
                if (CarName) MakeUserCall(FEngPrintf_obj, 2, eax, _PreRaceStatsScreen[i + 16], esp, CarName);
            }
        }
    }
}

void __fastcall PreRaceStatsScreen_DrawDrivers(DWORD* _PreRaceStatsScreen, void* EDX_Unused)
{
    int NumPlayers = _PreRaceStatsScreen[52];
    DWORD* PCurrentRace = *(DWORD**)_pCurrentRace;
    DWORD* Player = *(DWORD**)_PlayersByNumber;
    DWORD* Player2 = *(DWORD**)(_PlayersByNumber + 4);
    DWORD* CarPtr, *DriverInfo;
    char* DriverName;

    for (int i = 0; i < NumPlayers; i++)
    {
        if (i >= PCurrentRace[9]) CarPtr = 0;
        else CarPtr = (DWORD*)PCurrentRace[328 + i];

        if (CarPtr)
        {
            CarPtr = (DWORD*)CarPtr[0];
            if (CarPtr)
            {
                DriverInfo = (DWORD*)CarPtr[5];
                if (DriverInfo)
                {
                    // Check if Player's car
                    if (Player && (CarPtr == *(DWORD**)Player[1])) DriverName = Player_GetName(Player);
                    else if (Player2 && (CarPtr == *(DWORD**)Player2[1])) DriverName = Player_GetName(Player2);
                    else DriverName = (char*)MakeUserCall(DriverInfo_GetName, 1, eax, DriverInfo);

                    MakeUserCall(FEngPrintf_obj, 4, eax, _PreRaceStatsScreen[i + 20], esp, "0%d: %s", esp, i + 1, esp, DriverName);
                }
            }
        }
    }
}

void __fastcall PreRaceStatsScreen_NotificationMessage(DWORD* _PreRaceStatsScreen, void* EDX_Unused, DWORD msg, DWORD* FEObj, int arg1, int arg2)
{
    DWORD* PreRaceScreenManager; // eax
    char* RaceHUDPkg;

    switch (msg)
    {
    case 0xB5AF2461: // PAD_START
        RaceCoordinator_RCPostMessage(*(DWORD**)_pRaceCoordinator, 4, 0);
        break;
    case 0xC98356BA:
        PreRaceScreenManager = *(DWORD**)_pPreRaceScreenManager;

        if (PreRaceScreenManager[1] && !_PreRaceStatsScreen[53])
        {
            _PreRaceStatsScreen[53] = 1;

            PreRaceStatsScreen_SetInitScript(_PreRaceStatsScreen, EDX_Unused);
            MakeUserCall(PreRaceStatsScreen_SetVisibility, 2, esp, _PreRaceStatsScreen, eax, 1);
            PreRaceStatsScreen_DrawCars(_PreRaceStatsScreen, EDX_Unused);
            PreRaceStatsScreen_DrawDrivers(_PreRaceStatsScreen, EDX_Unused);

            if (*(bool*)_unk_78A344 || *(bool*)_bIsDragRace)
            {
                RaceHUDPkg = "drag_HUD_test.fng";
            }
            else if (*(int*)_NumberOfPlayers == 2)
            {
                RaceHUDPkg = "2PHudBot.fng";
            }
            else
            {
                RaceHUDPkg = "HUD_SingleRace.fng";
            }
            MakeUserCall(FEngSetScript, 4, eax, RaceHUDPkg, edx, "GaugeCluster", esp, "Fade", esp, 1);
        }
        
        if (_PreRaceStatsScreen[49])
        {
            if (_PreRaceStatsScreen[50])
            {
                if (!(bool)MakeUserCall(FEngIsScriptRunning, 3, eax, _PreRaceStatsScreen[3], edi, 0x798C7EFF, esp, "FadeOut"))
                {
                    if (_PreRaceStatsScreen[51]) RaceCoordinator_RCPostMessage(*(DWORD**)_pRaceCoordinator, 4, 0);
                    else PreRaceScreenManager[0] = 1;
                }
            }
            else
            {
                _PreRaceStatsScreen[50] = 1;
                MakeUserCall(FEngSetScript_H, 4, esi, _PreRaceStatsScreen[3], eax, 0x798C7EFF, esp, "FadeOut", esp, 1);
            }
        }
        
        if (PreRaceScreenManager[0]) feDisableStartRaceScreens(); // ReadyToSkip
        break;
    case 0xD8692111:
        _PreRaceStatsScreen[49] = 1;
        break;
    }
}