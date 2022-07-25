#include <Windows.h>
#include <detours.h>

static HMODULE ntdll, userenvdll;
typedef NTSTATUS (*tRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
static tRtlGetVersion pRtlGetVersion;
typedef BOOL (*tGetUserProfileDirectoryA)(HANDLE hToken, LPSTR lpProfileDir, LPDWORD lpcchSize);
static tGetUserProfileDirectoryA pGetUserProfileDirectoryA;

NTSTATUS RtlGetVersion(PRTL_OSVERSIONINFOW lpVersionInformation)
{
    pRtlGetVersion(lpVersionInformation);
    lpVersionInformation->dwMajorVersion = 10;
    return ERROR_SUCCESS;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
        return TRUE;

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        userenvdll = LoadLibraryExA("userenv.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        pGetUserProfileDirectoryA = (tGetUserProfileDirectoryA)GetProcAddress(userenvdll, "GetUserProfileDirectoryA");
        ntdll = LoadLibraryExA("ntdll.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        pRtlGetVersion = (tRtlGetVersion)GetProcAddress(ntdll, "RtlGetVersion");
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach((PVOID*)&pRtlGetVersion, RtlGetVersion);
        DetourTransactionCommit();
        break;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach((PVOID*)&pRtlGetVersion, RtlGetVersion);
        DetourTransactionCommit();
        break;
    }

    return TRUE;
}

BOOL GetUserProfileDirectoryA(HANDLE hToken, LPSTR lpProfileDir, LPDWORD lpcchSize)
{
    return pGetUserProfileDirectoryA(hToken, lpProfileDir, lpcchSize);
}
