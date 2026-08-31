#include <xtl.h>
#include <xboxmath.h>

#define ANALOG_DEADZONE 8000 

typedef DWORD(WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
XInputGetState_t OriginalXInputGetState;

DWORD WINAPI ModdedXInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    DWORD result = OriginalXInputGetState(dwUserIndex, pState);

    if (result == ERROR_SUCCESS && pState != NULL) {
        SHORT rightAnalogX = pState->Gamepad.sThumbRX;
        SHORT rightAnalogY = pState->Gamepad.sThumbRY;

        if (rightAnalogX > ANALOG_DEADZONE || rightAnalogX < -ANALOG_DEADZONE ||
            rightAnalogY > ANALOG_DEADZONE || rightAnalogY < -ANALOG_DEADZONE) {
            
            pState->Gamepad.bLeftTrigger = 255;
        }
    }

    return result;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        HMODULE hXam = GetModuleHandle("xam.xex");
        if (hXam) {
            OriginalXInputGetState = (XInputGetState_t)GetProcAddress(hXam, (LPCSTR)400);
        }
    }
    return TRUE;
}
