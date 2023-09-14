#include <Windows.h>

#include <fstream>
#include <ctime>

#include "kthook/kthook.hpp"


using CTimerUpdateSignature = void(__cdecl*)();
using URLDownloadToFileASignature = HRESULT(__stdcall*)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);


kthook::kthook_simple<CTimerUpdateSignature>(CTimerUpdateHook);
kthook::kthook_simple<URLDownloadToFileASignature>(URLDownloadToFileAHook); 


void CTimerUpdateHooked(const decltype(CTimerUpdateHook)& hook);
HRESULT __stdcall URLDownloadToFileHooked(const decltype(URLDownloadToFileAHook)& hook, LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB);