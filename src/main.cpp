#include "main.hpp" 


HRESULT __stdcall URLDownloadToFileHooked(const decltype(URLDownloadToFileAHook)& hook, LPUNKNOWN pCaller, LPCSTR szURL, LPCSTR szFileName, DWORD dwReserved, LPBINDSTATUSCALLBACK lpfnCB) {    
    std::ofstream outFile("MoonDownLoadLog.txt", std::ios::app);

    if (outFile.is_open()) {
        std::time_t currentTime = std::time(nullptr);
        char szCurrentTime[9];
        std::strftime(szCurrentTime, sizeof(szCurrentTime), "%H:%M:%S", std::localtime(&currentTime));

        outFile << "[" << szCurrentTime << "]" << " File URL: \"" << szURL << "\", File Path: \"" << szFileName << "\"\n";

        outFile.close();
    }
    
    return hook.get_trampoline()(pCaller, szURL, szFileName, dwReserved, lpfnCB);
}

HMODULE __stdcall LoadLibraryAHooked(const decltype(LoadLibraryAHook)& hook, LPCSTR lpLibFileName) {
    auto libraryHandle = hook.get_trampoline()(lpLibFileName);
    
    if (libraryHandle == NULL)
        return libraryHandle;
       
    if (strcmp(lpLibFileName, "MoonLoader.asi") == 0) {  
        MessageBoxA(NULL, lpLibFileName, "LOAD", MB_OK);      
        setURLDownloadToFileAHook(libraryHandle); 
        LoadLibraryAHook.remove();                        
    }

    return libraryHandle;   
}


void initializePlugin() {   
    HMODULE moonHandle = GetModuleHandleA("MoonLoader.asi");
    if (moonHandle != NULL) {
        setURLDownloadToFileAHook(moonHandle);
    } else {
        HMODULE kernelHandle = GetModuleHandleA("kernel32.dll");
        if (kernelHandle != NULL) {           
            LPVOID pvLoadLibrary = GetProcAddress(kernelHandle, "LoadLibraryA");
            if (pvLoadLibrary != NULL) {
                LoadLibraryAHook.set_dest(pvLoadLibrary);
                LoadLibraryAHook.set_cb(&LoadLibraryAHooked);
                LoadLibraryAHook.install();
            } 
        }   
    }       
}


void setURLDownloadToFileAHook(HMODULE moonHandle) {
    auto moonHandleAddr = reinterpret_cast<uintptr_t>(moonHandle);

    URLDownloadToFileAHook.set_dest(moonHandleAddr + 0x13D680);
    URLDownloadToFileAHook.set_cb(&URLDownloadToFileHooked);
    URLDownloadToFileAHook.install();  
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        initializePlugin();

        break;
    
    case DLL_PROCESS_DETACH:          
        LoadLibraryAHook.remove();
        URLDownloadToFileAHook.remove();
        
        break;
    }   
    return TRUE;
}