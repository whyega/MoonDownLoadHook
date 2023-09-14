#include "main.hpp" 


void CTimerUpdateHooked(const decltype(CTimerUpdateHook)& hook) {
    const uintptr_t moonHandle = reinterpret_cast<uintptr_t>(GetModuleHandleA("MoonLoader.asi"));

    URLDownloadToFileAHook.set_dest(moonHandle + 0x13D680);
    URLDownloadToFileAHook.set_cb(&URLDownloadToFileHooked);
    URLDownloadToFileAHook.install();   
    
    CTimerUpdateHook.remove();    
}

HRESULT __stdcall URLDownloadToFileHooked(const decltype(URLDownloadToFileAHook)& hook, LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB) {    
    std::ofstream outFile("MoonDownLoadLog.txt", std::ios::app);

    if (outFile.is_open()) {
        std::time_t currentTime = std::time(nullptr);
        char szCurrentTime[9];
        std::strftime(szCurrentTime, sizeof(szCurrentTime), "%H:%M:%S", std::localtime(&currentTime));

        outFile << "[" << szCurrentTime << "]" << " File URL: \"" << szURL << "\", File Path: \"" << szFileName << "\"\n";

        outFile.close();
    }
    
    return hook.call_trampoline(pCaller, szURL, szFileName, dwReserved, lpfnCB);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CTimerUpdateHook.set_dest(0x561B10);
        CTimerUpdateHook.set_cb(&CTimerUpdateHooked); 
        CTimerUpdateHook.install(); 

        break;
    
    case DLL_PROCESS_DETACH:
        CTimerUpdateHook.remove();         
        URLDownloadToFileAHook.remove();
        
        break;
    }   
    return TRUE;
}