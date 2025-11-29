#include <stdio.h>
#include <windows.h>

#include "VoicemeeterRemote.h"
#include "voicemeeter.h"

/*******************************************************************************/
/**                           GET VOICEMEETER DIRECTORY                       **/
/*******************************************************************************/

static char uninstDirKey[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

#define INSTALLER_UNINST_KEY	"VB:Voicemeeter {17359A74-1236-5467}"


void RemoveNameInPath(char* szPath)
{
	long ll;
	ll = (long)strlen(szPath);
	while ((ll > 0) && (szPath[ll] != '\\')) ll--;
	if (szPath[ll] == '\\') szPath[ll] = 0;
}

#ifndef KEY_WOW64_32KEY
#define KEY_WOW64_32KEY 0x0200
#endif

BOOL __cdecl RegistryGetVoicemeeterFolder(char* szDir)
{
	char szKey[256];
	char sss[1024];
	DWORD nnsize = 1024;
	HKEY hkResult;
	LONG rep;
	DWORD pptype = REG_SZ;
	sss[0] = 0;

	// build Voicemeeter uninstallation key
	strcpy(szKey, uninstDirKey);
	strcat(szKey, "\\");
	strcat(szKey, INSTALLER_UNINST_KEY);

	// open key
	rep = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_READ, &hkResult);
	if (rep != ERROR_SUCCESS)
	{
		// if not present we consider running in 64bit mode and force to read 32bit registry
		rep = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_READ | KEY_WOW64_32KEY, &hkResult);
	}
	if (rep != ERROR_SUCCESS) return FALSE;
	// read uninstall profram path
	rep = RegQueryValueEx(hkResult, "UninstallString", 0, &pptype, (unsigned char*)sss, &nnsize);
	RegCloseKey(hkResult);

	if (pptype != REG_SZ) return FALSE;
	if (rep != ERROR_SUCCESS) return FALSE;
	// remove name to get the path only
	RemoveNameInPath(sss);
	if (nnsize > 512) nnsize = 512;
	strncpy(szDir, sss, nnsize);

	return TRUE;
}

/*******************************************************************************/
/**                                GET DLL INTERFACE                          **/
/*******************************************************************************/

static HMODULE G_H_Module = NULL;
static T_VBVMR_INTERFACE iVMR;


T_VBVMR_INTERFACE InitializeDLLInterfaces()
{
    char szDllName[1024];
    memset(&iVMR, 0, sizeof(T_VBVMR_INTERFACE));

    //get folder where is installed Voicemeeter
    if (RegistryGetVoicemeeterFolder(szDllName) == FALSE)
    {
        // voicemeeter not installed
        OutputDebugStringA("Voicemeeter not installed");
        return;
    }
    //use right dll according O/S type
    if (sizeof(void*) == 8)
        strcat(szDllName, "\\VoicemeeterRemote64.dll");
    else
        strcat(szDllName, "\\VoicemeeterRemote.dll");

        
    OutputDebugStringA("DLL path");
    OutputDebugStringA(szDllName);
    // Load Dll
    G_H_Module = LoadLibrary(szDllName);
    if (G_H_Module == NULL)
    {
    DWORD err = GetLastError();
    char buf[128];
    sprintf(buf, "Failed to load library, error code: %lu", err);
    OutputDebugStringA(buf);
        OutputDebugStringA("Failed to load library");
    }
    else
    {
        OutputDebugStringA("library loaded");
    }
    
    iVMR.VBVMR_Login = (T_VBVMR_Login) GetProcAddress(G_H_Module, "VBVMR_Login");
    iVMR.VBVMR_Logout = (T_VBVMR_Logout) GetProcAddress(G_H_Module, "VBVMR_Logout");

    iVMR.VBVMR_IsParametersDirty = (T_VBVMR_IsParametersDirty) GetProcAddress(G_H_Module, "VBVMR_IsParametersDirty");
    iVMR.VBVMR_GetParameterFloat = (T_VBVMR_GetParameterFloat) GetProcAddress(G_H_Module, "VBVMR_GetParameterFloat");

    OutputDebugStringA("function pointers set");
    OutputDebugStringA(iVMR.VBVMR_Login ? "Login function pointer valid" : "Login function pointer NULL");
    OutputDebugStringA(iVMR.VBVMR_Logout ? "Logout function pointer valid" : "Logout function pointer NULL");
    OutputDebugStringA(iVMR.VBVMR_IsParametersDirty ? "IsParametersDirty function pointer valid" : "IsParametersDirty function pointer NULL");
    OutputDebugStringA(iVMR.VBVMR_GetParameterFloat ? "GetParameterFloat function pointer valid" : "GetParameterFloat function pointer NULL");
    return iVMR;
}