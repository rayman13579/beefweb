#include "voicemeeterClient.hpp"
#include "VoicemeeterRemote.h"
#include "voicemeeter.hpp"
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
    iVMR.VBVMR_IsParametersDirty();
    iVMR.VBVMR_GetParameterFloat("Strip[0].Mute", &value);
    return value;
}

void VoicemeeterClient::initialize()
{
    iVMR = InitializeDLLInterfaces();
    iVMR.VBVMR_Login();
    iVMR.VBVMR_IsParametersDirty();
}