#include "voicemeeterClient.hpp"
#include "VoicemeeterRemote.h"
#include "voicemeeter.h"
#include <windows.h>


VoicemeeterClient::VoicemeeterClient()
{
    initialize();
}

VoicemeeterClient::~VoicemeeterClient()
{
    iVMR.VBVMR_Logout();
}

float VoicemeeterClient::getMuted()
{
    float value = -1;
    if (iVMR.VBVMR_GetParameterFloat) {
        iVMR.VBVMR_GetParameterFloat("Strip[0].Mute", &value);
    }
    return value;
}

void VoicemeeterClient::initialize()
{
    iVMR = InitializeDLLInterfaces();
    OutputDebugStringA("VoicemeeterClient initialized");
    iVMR.VBVMR_Login();
    OutputDebugStringA("VoicemeeterClient logged in");
    iVMR.VBVMR_IsParametersDirty();
    OutputDebugStringA("VoicemeeterClient parameters dirty checked");
}