#pragma once

#define _CurrentRaceMode 0x777CC8

int __fastcall QROptionsScreen_DrawNumRacers(DWORD* QROptionsScreen, void* EDX_Unused)
{
    const char* FEPkg;
    int PtrNumRacers;
    char buf[32];

    int CurrentRaceMode = *(int*)_CurrentRaceMode;
    int OptionNumber = QROptionsScreen[33];

    MakeUserCall(FEngSNPrintf, 4, esi, buf, esp, 32, esp, "Option%1dTextString", esp, OptionNumber);
    FEPkg = (const char*)*((DWORD*)QROptionsScreen + 3);
    MakeUserCall(FEngPrintf, 3, eax, FEPkg, edx, buf, esp, "Racers:");
    MakeUserCall(FEngSNPrintf, 4, esi, buf, esp, 32, esp, "Option%1dData", esp, OptionNumber);
    PtrNumRacers = cFrontendDatabase_GetOptions((DWORD*)_FEDatabase, CurrentRaceMode);
    return (int)MakeUserCall(FEngPrintf, 4, eax, FEPkg, edx, buf, esp, "%d", esp, *(DWORD*)(PtrNumRacers));
}

int __fastcall QROptionsScreen_ScrollNumRacers(DWORD* QROptionsScreen, void* EDX_Unused, int Action)
{
    int* PtrNumRacers; // ecx
    int NumRacers; // eax

    int CurrentRaceMode = *(int*)_CurrentRaceMode;

    PtrNumRacers = (int*)(cFrontendDatabase_GetOptions((DWORD*)_FEDatabase, CurrentRaceMode));
    NumRacers = *PtrNumRacers;
    if (Action == 0x9120409E) // PAD_LEFT
    {
        if (--NumRacers < MinRacers)
        {
            *PtrNumRacers = MaxRacers;
            return QROptionsScreen_DrawNumRacers(QROptionsScreen, EDX_Unused);
        }
    }
    else if (Action == 0xB5971BF1 && ++NumRacers > MaxRacers) // PAD_RIGHT
    {
        NumRacers = MinRacers;
    }
    *PtrNumRacers = NumRacers;
    return QROptionsScreen_DrawNumRacers(QROptionsScreen, EDX_Unused);
}