#include <Windows.h>

#include <fstream>
#include <ctime>

#include "kthook/kthook.hpp"


using URLDownloadToFileASignature = HRESULT(__stdcall*)(LPUNKNOWN, LPCSTR, LPCSTR, DWORD, LPBINDSTATUSCALLBACK);
using LoadLibraryASignature = HMODULE(__stdcall*)(LPCSTR lpLibFileName);


kthook::kthook_simple<URLDownloadToFileASignature>(URLDownloadToFileAHook); 
kthook::kthook_simple<LoadLibraryASignature>(LoadLibraryAHook); 


void initializePlugin();
void setURLDownloadToFileAHook(HMODULE moonHandle);


HRESULT __stdcall URLDownloadToFileHooked(const decltype(URLDownloadToFileAHook)& hook, LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB);
HMODULE __stdcall LoadLibraryAHooked(const decltype(LoadLibraryAHook)& hook, LPCSTR lpLibFileName);